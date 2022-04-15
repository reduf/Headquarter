#ifdef PORTAL_STS_C
#error "sts.c included more than once"
#endif
#define PORTAL_STS_C

static int parse_next_line(const char *data, size_t length, size_t *line_len)
{
    // Every line finish with a "\r\n", so we need at least two bytes to
    // have a complete line.
    if (length < 2) {
        return STSE_INCOMPLETE_HEADER;
    }

    size_t i;
    for (i = 0; i < (length - 1); ++i) {
        if (data[i] == '\r' && data[i + 1] == '\n')
            break;
    }

    // We didn't find a line ending;
    if (i == (length - 1))
        return STSE_INCOMPLETE_HEADER;

    *line_len = i;
    return 0;
}

static void skip_line(const char **data, size_t *length, size_t line_len)
{
    assert((line_len + 2) <= *length);
    assert((*data)[line_len] == '\r' && (*data)[line_len + 1] == '\n');

    *data += (line_len + 2);
    *length -= (line_len + 2);
}

int sts_parse_reply(struct sts_reply *reply, const uint8_t *raw, size_t length)
{
    int ret;
    size_t line_len;
    const char *data = (const char *)raw;

    if ((ret = parse_next_line(data, length, &line_len)) != 0) {
        return ret;
    }

    unsigned version_major;
    unsigned version_minor;
    char separator;
    ret = sscanf(data, "STS/%1u.%1u%c%3u", &version_major, &version_minor,
                 &separator, &reply->status_code);

    if (ret != 4) {
        return STSE_UNSUPPORTED_PROTOCOL;
    }

    if ((version_major != 1) || (version_minor != 0)) {
        fprintf(stderr, "Unsupported version %u.%u\n", version_major, version_minor);
        return STSE_UNSUPPORTED_PROTOCOL;
    }

    skip_line(&data, &length, line_len);

    while (length != 0) {
        if ((ret = parse_next_line(data, length, &line_len)) != 0) {
            return ret;
        }

        // Check if we are done parsing the header.
        if (line_len == 0) {
            // finish parsing the header.
            skip_line(&data, &length, line_len);
            break;
        }

        // We only care about the two "header" we saw. (i.e., 's' and 'l')
        if (data[0] == 's') {
            if (sscanf(data, "s:%uR", &reply->sequence_number) != 1) {
                return STSE_UNSUPPORTED_HEADER;
            }
        } else if (data[0] == 'l') {
            if (sscanf(data, "l:%u", &reply->content_length) != 1) {
                return STSE_UNSUPPORTED_HEADER;
            }
        } else {
            return STSE_UNSUPPORTED_HEADER;
        }

        skip_line(&data, &length, line_len);
    }

    if (length < reply->content_length) {
        return STSE_INCOMPLETE_CONTENT;
    }

    reply->content = (const uint8_t *)data;
    return 0;
}

void sts_connection_init(struct sts_connection *sts)
{
    memset(sts, 0, sizeof(*sts));
    sts->fd = INVALID_SOCKET;
}

void sts_connection_free(struct sts_connection *sts)
{
    if (sts->fd != INVALID_SOCKET)
        closesocket(sts->fd);
}

static uint64_t anet_epoch()
{
    // For some reason the server has an offset on the epoch of 31 years.
    // This value represent the exact offset
    static const time_t EPOCH_OFFSET = 978307200;

    time_t epoch = time(NULL);
    return (uint64_t)(epoch - EPOCH_OFFSET);
}

static int sts_write_header(array_uint8_t *request,
    const char *url, size_t url_len, size_t content_len)
{
    const char version[] = " STS/1.0\r\n";

    if ((size_t)UINT32_MAX < content_len) {
        return STSE_UNSUCCESSFUL;
    }

    uint32_t content_len_as_u32 = (uint32_t)content_len;

    // Every requests start with 'P ' (\x50\x20).
    array_insert(request, 2, "P ");
    array_insert(request, url_len, (const uint8_t *)url);
    // The request is followd by the version, on the same line.
    array_insert(request, sizeof(version) - 1, version);

    // The content length is written as `l:%d`, we force use a `uint32_t`
    // ensuring that a buffer of 32 bytes is always enough.
    char content_length_buffer[32];
    int ret = snprintf(content_length_buffer, sizeof(content_length_buffer),
                       "l:%" PRIu32, (uint32_t)content_len_as_u32);
    if (ret < 0)
        return STSE_UNSUCCESSFUL;

    array_insert(request, (size_t)ret, content_length_buffer);
    return 0;
}

static void sts_finish_request(array_uint8_t *request,
    const uint8_t *content, size_t content_len)
{
    // We are done writing the header, so we append "\r\n\r\n" like in http.
    array_insert(request, 4, "\r\n\r\n");
    array_insert(request, content_len, content);
}

int sts_write_request(
    array_uint8_t *request,
    const char *url, size_t url_len,
    const uint8_t *content, size_t content_len)
{
    int ret;

    if ((ret = sts_write_header(request, url, url_len, content_len)) != 0) {
        return STSE_UNSUCCESSFUL;
    }

    sts_finish_request(request, content, content_len);
    return 0;
}

static int sts_send_request(
    struct sts_connection *sts,
    const char *url, size_t url_len,
    const uint8_t *content, size_t content_len)
{
    int ret;

    array_uint8_t request;
    array_init(&request);

    if ((ret = sts_write_request(&request, url, url_len, content, content_len)) != 0) {
        array_reset(&request);
        return ret;
    }

    ret = send_full(sts->fd, request.data, request.size);
    array_reset(&request);

    if (ret != 0) {
        fprintf(stderr, "Failed to send the data\n");
        return ret;
    }

    return 0;
}

int sts_write_request_with_sequence_number(
    array_uint8_t *request,
    const char *url, size_t url_len,
    size_t seq_number,
    uint32_t timeout_ms,
    const uint8_t *content, size_t content_len)
{
    int ret;

    if ((ret = sts_write_header(request, url, url_len, content_len)) != 0) {
        return STSE_UNSUCCESSFUL;
    }

    array_insert(request, 2, "\r\n");

    char seq_number_buffer[64];
    ret = snprintf(
        seq_number_buffer, sizeof(seq_number_buffer),
        "s:%" PRIu32 ";timeout=%" PRIu32,
        (uint32_t)seq_number, timeout_ms);

    if (ret < 0) {
        return STSE_UNSUCCESSFUL;
    }

    array_insert(request, (size_t)ret, seq_number_buffer);
    sts_finish_request(request, content, content_len);

    return 0;
}

int sts_send_request_with_sequence_number(
    struct sts_connection *sts,
    const char *url, size_t url_len,
    size_t seq_number,
    uint32_t timeout_ms,
    const uint8_t *content, size_t content_len)
{
    int ret;

    array_uint8_t request;
    array_init(&request);

    if ((ret = sts_write_request_with_sequence_number(
            &request,
            url, url_len,
            seq_number,
            timeout_ms,
            content, content_len)) != 0) {

        array_reset(&request);
        return ret;
    }

    ret = send_full(sts->fd, request.data, request.size);
    array_reset(&request);

    if (ret != 0) {
        fprintf(stderr, "Failed to send the data\n");
        return ret;
    }

    return 0;
}

int sts_connection_connect(struct sts_connection *sts, const char *hostname)
{
    int ret;

    sts->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sts->fd == INVALID_SOCKET) {
        fprintf(stderr, "'socket' failed\n");
        return STSE_UNSUCCESSFUL;
    }

    array_sockaddr_t addresses;
    array_init(&addresses);
    if ((ret = resolve_dns4(&addresses, hostname, 6112)) != 0) {
        fprintf(stderr, "Failed to resolve the DNS name\n");
        return ret;
    }

    struct sockaddr *it;
    array_foreach(it, &addresses) {
        if ((ret = connect(sts->fd, it, sizeof(*it))) == 0) {
            break;
        }
    }

    if (it == array_end(&addresses)) {
        fprintf(stderr, "Couldn't connect to '%s:%d\n", hostname, 6112);
        array_reset(&addresses);
        return STSE_UNSUCCESSFUL;
    }

    array_reset(&addresses);

    char sockname[MAX(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)];
    if ((ret = get_sockname(sts->fd, sockname, sizeof(sockname))) != 0) {
        return ret;
    }

    array_uint8_t content;
    array_init(&content);

    appendf(&content, "<Connect>\n");
    appendf(&content, "<ConnType>400</ConnType>\n");
    appendf(&content, "<Address>%s</Address>\n", sockname);
    appendf(&content, "<ProductType>0</ProductType>\n");
    appendf(&content, "<ProductName>Gw</ProductName>\n");
    appendf(&content, "<AppIndex>1</AppIndex>\n");
    appendf(&content, "<Epoch>%" PRIu64 "</Epoch>\n", anet_epoch());
    appendf(&content, "<Program>1</Program>\n");
    appendf(&content, "<Build>1002</Build>\n");
    appendf(&content, "<Process>%d</Process>\n", getpid());
    appendf(&content, "</Connect>\n");

    const char url[] = "/Sts/Connect";
    size_t url_len = ARRAY_SIZE(url) - 1;
    if ((ret = sts_send_request(sts, url, url_len, content.data, content.size)) != 0) {
        return ret;
    }

    return 0;
}

int sts_connection_start_tls(struct sts_connection *sts, struct ssl_sts_connection *ssl)
{
    int ret;

    const char url[] = "/Auth/StartTls";
    size_t url_len = ARRAY_SIZE(url) - 1;
    const uint8_t content[] = "";
    size_t content_len = ARRAY_SIZE(content) - 1;

    const uint32_t timeout = 4000;

    if ((ret = sts_send_request_with_sequence_number(
            sts,
            url, url_len,
            ++sts->seq_number,
            timeout,
            content, content_len)) != 0) {

        return ret;
    }

    array_uint8_t buffer;
    array_init(&buffer);

    struct sts_reply reply = {0};
    for (;;) {
        if ((ret = recv_to_buffer(sts->fd, &buffer)) != 0) {
            array_reset(&buffer);
            return ret;
        }

        ret = sts_parse_reply(&reply, buffer.data, buffer.size);

        // We wait till we receive all the header and all the content.
        if (ret != STSE_INCOMPLETE_CONTENT && ret != STSE_INCOMPLETE_HEADER) {
            // We don't care about the content of the response, it's useless
            // and the header gives us all the necessary information.
            array_reset(&buffer);
            break;
        }
    }

    if (ret != STSE_SUCCESS) {
        return ret;
    }

    if (reply.status_code != 400) {
        fprintf(stderr, "Couldn't start a STS connection, status: %d\n", reply.status_code);
        return STSE_UNSUCCESSFUL;
    }

    // If we were successful, we transfer the connection to the
    // `struct ssl_sts_connection` structure.
    ssl->fd = sts->fd;
    sts->fd = INVALID_SOCKET;

    return 0;
}
