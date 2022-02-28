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

int parse_sts_header(struct sts_header *header, const uint8_t *raw, size_t length)
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
                 &separator, &header->status_code);

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
            if (sscanf(data, "s:%uR", &header->sequence_number) != 1) {
                return STSE_UNSUPPORTED_HEADER;
            }
        } else if (data[0] == 'l') {
            if (sscanf(data, "l:%u", &header->content_length) != 1) {
                return STSE_UNSUPPORTED_HEADER;
            }
        } else {
            return STSE_UNSUPPORTED_HEADER;
        }

        skip_line(&data, &length, line_len);
    }

    if (length < header->content_length) {
        return STSE_INCOMPLETE_CONTENT;
    }

    header->content = (const uint8_t *)data;
    return 0;
}

void sts_connection_init(struct sts_connection *sts)
{
    memset(sts, 0, sizeof(*sts));

    sts->fd = INVALID_SOCKET;
    array_init(sts->addresses, 1);
}

void sts_connection_free(struct sts_connection *sts)
{
    if (sts->fd != INVALID_SOCKET)
        closesocket(sts->fd);
    array_reset(sts->addresses);
}

static int resolve_dns(array_sockaddr_t *addresses, int af, const char *hostname, uint16_t port)
{
    int ret;
    struct addrinfo hints = {0};
    struct addrinfo *results = NULL;

    char port_as_str[sizeof("65535")];
    snprintf(port_as_str, sizeof(port_as_str), "%" PRIu16, port);

    hints.ai_family = af;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if ((ret = getaddrinfo(hostname, port_as_str, &hints, &results)) != 0) {
        fprintf(stderr, "Failed to resolved host '%s'\n", hostname);
        return 1;
    }

    for (struct addrinfo *it = results; it != NULL; it = it->ai_next) {
        array_add(*addresses, *it->ai_addr);
    }

    freeaddrinfo(results);
    return 0;
}

static int resolve_dns4(array_sockaddr_t *addresses, const char *hostname, uint16_t port)
{
    return resolve_dns(addresses, AF_INET, hostname, port);
}

static void appendv(array_uint8_t *buffer, const char *fmt, va_list args)
{
    int ret = vsnprintf(NULL, 0, fmt, args);
    if (ret < 0)
        abort();

    // We need to allocate one more bytes, because  of `vsnprintf`.
    // We will pop this "\0" byte later.
    uint8_t *write_ptr = array_push(*buffer, (size_t)ret + 1);
    vsnprintf((char *)write_ptr, (size_t)ret + 1, fmt, args);
    array_pop(*buffer);
}

static void appendf(array_uint8_t *buffer, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    appendv(buffer, fmt, args);
    va_end(args);
}

static uint64_t anet_epoch()
{
    // For some reason the server has an offset on the epoch of 31 years.
    // This value represent the exact offset
    static const time_t EPOCH_OFFSET = 978307200;

    time_t epoch = time(NULL);
    return (uint64_t)(epoch - EPOCH_OFFSET);
}

static int set_sockname(struct sts_connection *sts)
{
    struct sockaddr_storage sa;
    socklen_t len = sizeof(sa);
    int ret = getsockname(sts->fd, (struct sockaddr *)&sa, &len);
    if (ret != 0) {
        fprintf(stderr, "'getsockname' failed\n");
        return 1;
    }

    void *addr = NULL;
    switch (sa.ss_family)
    {
        case AF_INET:
            addr = &((struct sockaddr_in *)&sa)->sin_addr;
            break;
        case AF_INET6:
            addr = &((struct sockaddr_in6 *)&sa)->sin6_addr;
            break;
        default:
            return 1;
    }

    if (inet_ntop(sa.ss_family, addr, sts->sockname, sizeof(sts->sockname)) == NULL) {
        fprintf(stdout, "Couldn't parse ip address\n");
        return 1;
    }

    return 0;
}

static int sts_write_header(array_uint8_t *request,
    const char *url, size_t url_len, size_t content_len)
{
    const char version[] = " STS/1.0\r\n";

    if ((size_t)UINT32_MAX < content_len) {
        return 1;
    }

    uint32_t content_len_as_u32 = (uint32_t)content_len;

    // Every requests start with 'P ' (\x50\x20).
    array_insert(*request, 2, "P ");
    array_insert(*request, url_len, (const uint8_t *)url);
    // The request is followd by the version, on the same line.
    array_insert(*request, sizeof(version) - 1, version);

    // The content length is written as `l:%d`, we force use a `uint32_t`
    // ensuring that a buffer of 32 bytes is always enough.
    char content_length_buffer[32];
    int ret = snprintf(content_length_buffer, sizeof(content_length_buffer),
                       "l:%" PRIu32, (uint32_t)content_len_as_u32);
    if (ret < 0)
        return 1;

    array_insert(*request, (size_t)ret, content_length_buffer);
    return 0;
}

static void sts_finish_request(array_uint8_t *request,
    const uint8_t *content, size_t content_len)
{
    // We are done writing the header, so we append "\r\n\r\n" like in http.
    array_insert(*request, 4, "\r\n\r\n");
    array_insert(*request, content_len, content);
}

static int send_full(SOCKET fd, const uint8_t *buffer, size_t buffer_len)
{
    int ret;
    size_t written = 0;

    while (written < buffer_len) {
        const char *p = (const char *)buffer + written;
        if ((ret = send(fd, p, buffer_len - written, 0)) <= 0)
            return ret;
        written += (size_t)ret;
    }

    return 0;
}

static int recv_to_buffer(SOCKET fd, array_uint8_t *buffer)
{
    int ret;

    const size_t BUFFER_SIZE = 1024;
    uint8_t *ptr = array_push(*buffer, BUFFER_SIZE);

    if ((ret = recv(fd, (char *)ptr, BUFFER_SIZE, 0)) <= 0) {
        size_t size = array_size(*buffer) - BUFFER_SIZE;
        array_resize(*buffer, size);
        return ret;
    }

    size_t size = array_size(*buffer) - (BUFFER_SIZE - (size_t)ret);
    array_resize(*buffer, size);
    return 0;
}

static int sts_write_request(
    struct sts_connection *sts,
    const char *url, size_t url_len,
    const uint8_t *content, size_t content_len)
{
    int ret;

    array_uint8_t request;
    array_init(request, 1024);

    if ((ret = sts_write_header(&request, url, url_len, content_len)) != 0) {
        array_reset(request);
        return 1;
    }

    sts_finish_request(&request, content, content_len);

    if ((ret = send_full(sts->fd, request.data, request.size)) != 0) {
        array_reset(request);
        return ret;
    }

    array_reset(request);
    return 0;
}

static int sts_write_request_with_sequence_number(
    struct sts_connection *sts, const char *url, size_t url_len,
    size_t seq_number, uint32_t timeout_ms, const uint8_t *content, size_t content_len)
{
    int ret;

    array_uint8_t request;
    array_init(request, 1024);

    if ((ret = sts_write_header(&request, url, url_len, content_len)) != 0) {
        return 1;
    }

    array_insert(request, 2, "\r\n");

    char seq_number_buffer[64];
    ret = snprintf(
        seq_number_buffer, sizeof(seq_number_buffer),
        "s:%" PRIu32 ";timeout=%" PRIu32,
        (uint32_t)seq_number, timeout_ms);

    if (ret < 0) {
        array_reset(request);
        return 1;
    }

    array_insert(request, (size_t)ret, seq_number_buffer);
    sts_finish_request(&request, content, content_len);

    if ((ret = send_full(sts->fd, request.data, request.size)) != 0) {
        array_reset(request);
        return ret;
    }

    array_reset(request);
    return 0;
}

int sts_connection_connect(struct sts_connection *sts, const char *hostname)
{
    int ret;

    if ((ret = resolve_dns4(&sts->addresses, hostname, 6112)) != 0)
        return ret;

    sts->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sts->fd == INVALID_SOCKET) {
        fprintf(stderr, "'socket' failed\n");
        return 1;
    }

    struct sockaddr *it;
    array_foreach(it, sts->addresses) {
        if ((ret = connect(sts->fd, it, sizeof(*it))) == 0) {
            break;
        }
    }

    if (it == array_end(sts->addresses)) {
        fprintf(stderr, "Couldn't connect to '%s:%hu\n", hostname, 6112);
        return 1;
    }

    if ((ret = set_sockname(sts)) != 0) {
        return ret;
    }

    array_uint8_t content;
    array_init(content, 1024);

    appendf(&content, "<Connect>\n");
    appendf(&content, "<ConnType>400</ConnType>\n");
    appendf(&content, "<Address>%s</Address>\n", sts->sockname);
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
    if ((ret = sts_write_request(sts, url, url_len, content.data, content.size)) != 0) {
        return ret;
    }

    return 0;
}

int sts_connection_start_tls(struct sts_connection *sts)
{
    int ret;

    const char url[] = "/Auth/StartTls";
    size_t url_len = ARRAY_SIZE(url) - 1;
    const uint8_t content[] = "";
    size_t content_len = ARRAY_SIZE(content) - 1;

    if ((ret = sts_write_request_with_sequence_number(
        sts, url, url_len, 1, 4000, content, content_len)) != 0) {

        return ret;
    }

    array_uint8_t buffer;
    array_init(buffer, 1024);

    struct sts_header header = {0};
    for (;;) {
        if ((ret = recv_to_buffer(sts->fd, &buffer)) != 0) {
            array_reset(buffer);
            return ret;
        }

        ret = parse_sts_header(&header, buffer.data, buffer.size);

        // We wait till we receive all the header and all the content.
        if (ret != STSE_INCOMPLETE_CONTENT && ret != STSE_INCOMPLETE_HEADER) {
            // We don't care about the content of the response, it's useless
            // and the header gives us all the necessary information.
            array_reset(buffer);
            break;
        }
    }

    if (ret != STSE_SUCCESS) {
        return ret;
    }

    if (header.status_code != 400) {
        fprintf(stderr, "Couldn't start a STS connection, status: %d\n", header.status_code);
        return 1;
    }

    return 0;
}
