#ifdef PORTAL_STS_C
#error "sts.c included more than once"
#endif
#define PORTAL_STS_C

#define SSL_HS_CLIENT_HELLO 1
#define SSL_HS_SERVER_HELLO 2

#define TLS_SRP_SHA_WITH_AES_256_CBC_SHA 0xC020
#define TLS_SRP_SHA_WITH_AES_128_CBC_SHA 0xC01D

#define TLS_CONTENT_TYPE_HANDSHAKE 0x16

static void array_add_be_uint16(array_uint8_t *buffer, uint16_t value)
{
    uint8_t *ptr = array_push(*buffer, sizeof(value));
    be16enc(ptr, value);
}

static void ssl_tls12_start_handshake_msg(array_uint8_t *buffer, uint8_t hs_type)
{
    /*
     * Reserve 4 bytes for hanshake header.
     *    ...
     *    HandshakeType msg_type;
     *    uint24 length;
     *    ...
     */

    array_add(*buffer, hs_type);

    array_add(*buffer, 0);
    array_add(*buffer, 0);
    array_add(*buffer, 0);
}

static int ssl_tls12_finish_handshake_msg(array_uint8_t *buffer, uint8_t hs_type)
{
    const size_t HANDSHAKE_HDR_LEN = 4;
    assert(HANDSHAKE_HDR_LEN <= array_size(*buffer));
    assert(array_at(*buffer, 0) == hs_type);

    uint8_t *content_begin = array_begin(*buffer) + HANDSHAKE_HDR_LEN;
    size_t content_length = array_end(*buffer) - content_begin;

    const size_t UINT24_MAX = 0xFFFFFF;
    if (UINT24_MAX <= content_length)
        return 1;

    uint8_t *hs_len_ptr = array_begin(*buffer) + 1;
    hs_len_ptr[0] = (content_length >> 16) & 0xFF;
    hs_len_ptr[1] = (content_length >> 8) & 0xFF;
    hs_len_ptr[2] = content_length & 0xFF;
    return 0;
}

static void ssl_tls12_write_srp(array_uint8_t *buffer,
    const char *username, size_t length)
{
    assert(length < (UINT8_MAX - 1));

    static const uint16_t EXTENSION_SRP = 12;
    array_add_be_uint16(buffer, EXTENSION_SRP);

    // The length of the extension which effectively encode the length of the
    // username on 1 byte and then the actual username. So, it's length + 1.
    array_add_be_uint16(buffer, (uint16_t)(length + 1));
    array_add(*buffer, (uint8_t)length);

    // Add the username.
    array_insert(*buffer, length, (uint8_t *)username);
}

static int ssl_tls12_write_extension(
    array_uint8_t *buffer, struct ssl_tls12_context *ctx)
{
    // We first need to write the length on a `uint16be_t`, but we don't know
    // it yet. Instead, we reserve those bytes and we will write it later.
    uint8_t *extensions_len_ptr = array_push(*buffer, 2);

    // The first extension is undocumented and doesn't contain any data.
    // So we simply hardcode it.
    array_add_be_uint16(buffer, 0xADAE);
    array_add_be_uint16(buffer, 0); // Length is 0

    // The second extension is SRP specifying the username. (i.e., email)
    ssl_tls12_write_srp(buffer, ctx->srp_username, ctx->srp_username_len);

    size_t extension_len = array_end(*buffer) - (extensions_len_ptr + 2);
    if ((size_t)UINT16_MAX < extension_len)
        return 1;

    be16enc(extensions_len_ptr, (uint16_t)extension_len);
    return 0;
}

static int ssl_tls12_write_client_hello_body(
    array_uint8_t *buffer, struct ssl_tls12_context *ctx)
{
    // The version is always "\x03\x03". (i.e., TLS v1.2)
    array_add(*buffer, 0x03);
    array_add(*buffer, 0x03);

    // Write random bytes.
    uint8_t *random_time_ptr = array_push(*buffer, 4);
    be32enc(random_time_ptr, ctx->random_time);
    array_insert(*buffer, sizeof(ctx->random_bytes), ctx->random_bytes);

    // The session id is always null
    array_add(*buffer, 0);

    // Write cipher suites
    static const uint16_t ciphers[] = {
        TLS_SRP_SHA_WITH_AES_256_CBC_SHA,
        TLS_SRP_SHA_WITH_AES_128_CBC_SHA,
        0xFF02, // No ideas what this is...
        0xFF01, // No ideas what this is...
        0xFF04, // No ideas what this is...
        0xFF03, // No ideas what this is...
    };

    assert(sizeof(ciphers) <= UINT16_MAX);
    array_add_be_uint16(buffer, (uint16_t)sizeof(ciphers));
    for (size_t i = 0; i < ARRAY_SIZE(ciphers); ++i) {
        array_add_be_uint16(buffer, ciphers[i]);
    }

    // Write compression methods.
    // This is hardcoded and means 1 compression methods named "null".
    // Not sure if required, but simply reproducing the behavior of the
    // official DLL.
    array_add(*buffer, 1);
    array_add(*buffer, 0);

    if (ssl_tls12_write_extension(buffer, ctx) != 0) {
        return 1;
    }

    return 0;
}

void ssl_tls2_init(struct ssl_tls12_context *ctx)
{
    array_init(ctx->buffer, 1);
    ctx->srp_username_len = 0;
}

void ssl_tls2_free(struct ssl_tls12_context *ctx)
{
    array_reset(ctx->buffer);
}

void ssl_tls12_write_auth_packet(array_uint8_t *buffer, const uint8_t *content, size_t length)
{
    array_add(*buffer, TLS_CONTENT_TYPE_HANDSHAKE);

    // The version is always "\x03\x03". (i.e., TLS v1.2)
    array_add(*buffer, 0x03);
    array_add(*buffer, 0x03);

    assert(length <= UINT16_MAX);
    array_add_be_uint16(buffer, (uint16_t)length);
    array_insert(*buffer, length, content);
}

int ssl_tls12_write_client_hello(struct ssl_tls12_context *ctx)
{
    array_reserve(ctx->buffer, 1024);

    ssl_tls12_start_handshake_msg(&ctx->buffer, SSL_HS_CLIENT_HELLO);

    if (ssl_tls12_write_client_hello_body(&ctx->buffer, ctx) != 0) {
        array_reset(ctx->buffer);
        return 1;
    }

    if (ssl_tls12_finish_handshake_msg(&ctx->buffer, SSL_HS_CLIENT_HELLO) != 0) {
        array_reset(ctx->buffer);
        return 1;
    }

    // Do we need to calculate the checksum?

    return 0;
}

#define ERR_SSL_CONTINUE_PROCESSING 1
#define ERR_SSL_UNEXPECTED_MESSAGE  2
#define ERR_SSL_UNSUPPORTED_PROTOCOL 3
#define ERR_SSL_BAD_INPUT_DATA 4

static int parse_tls12_handshake(const uint8_t *data, size_t length,
    uint8_t content_type, const uint8_t **subdata, size_t *sublen)
{
    const size_t HEADER_LEN = 5;
    // The packet starts with 1 byte defining the content type, 2 bytes defining
    // the version and 2 bytes defining the length of the payload.
    if (length < HEADER_LEN)
        return ERR_SSL_CONTINUE_PROCESSING;

    if (data[0] != content_type)
        return ERR_SSL_UNEXPECTED_MESSAGE;

    if (data[1] != 3 && data[2] != 3)
        return ERR_SSL_UNSUPPORTED_PROTOCOL;

    uint16_t client_sublen = be16dec(&data[3]);

    // This can't overflow, we checked it before.
    if ((length - HEADER_LEN) < client_sublen)
        return ERR_SSL_CONTINUE_PROCESSING;

    *subdata = &data[HEADER_LEN];
    *sublen = client_sublen;
    return 0;
}

struct server_hello {
    uint32_t random_time;
    uint8_t  random_bytes[28];
};

static uint32_t be24dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;
    return (((unsigned)p[1] << 16) | (p[2] << 8) | p[3]);
}

static int chk_stream_read8(const uint8_t **data, size_t *length, uint8_t *out)
{
    if (*length < sizeof(*out))
        return ERR_SSL_BAD_INPUT_DATA;

    *out = **data;
    *data += sizeof(*out);
    *length -= sizeof(*out);
    return 0;
}

static int chk_stream_read16(const uint8_t **data, size_t *length, uint16_t *out)
{
    if (*length < sizeof(*out))
        return ERR_SSL_BAD_INPUT_DATA;

    *out = be16dec(*data);
    *data += sizeof(*out);
    *length -= sizeof(*out);
    return 0;
}

static int chk_stream_read32(const uint8_t **data, size_t *length, uint32_t *out)
{
    if (*length < sizeof(*out))
        return ERR_SSL_BAD_INPUT_DATA;

    *out = be32dec(*data);
    *data += sizeof(*out);
    *length -= sizeof(*out);
    return 0;
}

static int chk_stream_read(const uint8_t **data, size_t *length, uint8_t *out, size_t out_len)
{
    if (*length < out_len)
        return ERR_SSL_BAD_INPUT_DATA;

    memcpy(out, data, out_len);
    *data += out_len;
    *length -= out_len;
    return 0;
}

static int parse_server_hello(struct server_hello *hello, const uint8_t *data, size_t length)
{
    int ret;
    // The type is encoded on 1 byte and the length on 3 bytes.
    const size_t MIN_LEN = 4;

    if (length < MIN_LEN)
        return ERR_SSL_BAD_INPUT_DATA;

    if (data[0] != SSL_HS_SERVER_HELLO)
        return ERR_SSL_UNEXPECTED_MESSAGE;

    uint32_t content_len = be24dec(&data[1]);
    if ((length - MIN_LEN) != content_len) {
        // At this point, we should already know we have the complete data.
        return ERR_SSL_BAD_INPUT_DATA;
    }

    const uint8_t *content = &data[MIN_LEN];

    uint16_t version;
    if ((ret = chk_stream_read16(&content, &content_len, &version)) != 0)
        return ret;
    // @Cleanup: Maybe not the best error code to return...
    if (version != 0x0303)
        return ERR_SSL_BAD_INPUT_DATA;

    if ((ret = chk_stream_read32(&content, &content_len, &hello->random_time)) != 0)
        return ret;
    if ((ret = chk_stream_read(&content, &content_len, hello->random_bytes, sizeof(hello->random_bytes))) != 0)
        return ret;

    uint8_t session_id_len;
    if ((ret = chk_stream_read8(&content, &content_len, &session_id_len)) != 0)
        return ret;

    // @Cleanup: Should we distinguish with unsupported input data?!?
    if (session_id_len != 0)
        return ERR_SSL_BAD_INPUT_DATA;

    uint16_t cipher_suite;
    if ((ret = chk_stream_read16(&content, &content_len, &cipher_suite)) != 0)
        return ret;

    // @Cleanup:
    // In theory any of the ciphers we told the cipher should work...
    if (cipher_suite != TLS_SRP_SHA_WITH_AES_256_CBC_SHA)
        return ERR_SSL_BAD_INPUT_DATA;

    uint8_t compression_method;
    if ((ret = chk_stream_read8(&content, &content_len, &compression_method)) != 0)
        return ret;
    if (compression_method != 0)
        return ERR_SSL_BAD_INPUT_DATA;

    uint16_t extension_len;
    if ((ret = chk_stream_read16(&content, &content_len, &extension_len)) != 0)
        return ret;
    if (content_len != extension_len)
        return ERR_SSL_BAD_INPUT_DATA;

    // @Cleanup: Should we read the extension and ensure we get what we expect?
    return 0;
}

int sts_process_server_hello(const uint8_t *data, size_t length)
{
    int ret;
    const uint8_t *server_hello_data;
    size_t server_hello_len;

    ret = parse_tls12_handshake(data, length, TLS_CONTENT_TYPE_HANDSHAKE,
        &server_hello_data, &server_hello_len);
    if (ret != 0) {
        return ret;
    }

    struct server_hello hello = {0};
    if ((ret = parse_server_hello(&hello, server_hello_data, server_hello_len)) != 0)
        return ret;

    return 0;
}

static sts_write_header(array_uint8_t *request,
    const char *url, size_t url_len, size_t content_len)
{
    const char version[] = " STS/1.0\r\n";

    // Every requests start with 'P ' (\x50\x20).
    array_insert(*request, 2, "P ");
    array_insert(*request, url_len, (const uint8_t *)url);
    // The request is followd by the version, on the same line.
    array_insert(*request, sizeof(version) - 1, version);

    // The content length is written as `l:%d`, we force use a `uint32_t`
    // ensuring that a buffer of 32 bytes is always enough.
    char content_length_buffer[32];
    int ret = snprintf(content_length_buffer, sizeof(content_length_buffer),
                       "l:%" PRIu32, (uint32_t)content_len);
    if (ret < 0)
        abort();
    array_insert(*request, (size_t)ret, content_length_buffer);
}

static sts_finish_request(array_uint8_t *request, const uint8_t *content, size_t content_len)
{
    // We are done writing the header, so we append "\r\n\r\n" like in http.
    array_insert(*request, 4, "\r\n\r\n");
    array_insert(*request, content_len, content);
}

void sts_write_request(array_uint8_t *request,
    const char *url, size_t url_len,
    const uint8_t *content, size_t content_len)
{
    sts_write_header(request, url, url_len, content_len);
    sts_finish_request(request, content, content_len);
}

void sts_write_sequenced_request(
    array_uint8_t *request, size_t seq_number, uint32_t timeout_ms,
    const char *url, size_t url_len, const uint8_t *content, size_t content_len)
{
    sts_write_header(request, url, url_len, content_len);
    array_insert(*request, 2, "\r\n");

    char seq_number_buffer[64];
    int ret = snprintf(
        seq_number_buffer, sizeof(seq_number_buffer),
        "s:%" PRIu32 ";timeout=%" PRIu32,
        (uint32_t)seq_number, timeout_ms);
    if (ret < 0)
        abort();

    array_insert(*request, (size_t)ret, seq_number_buffer);
    sts_finish_request(request, content, content_len);
}

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

int parse_sts_request(struct sts_request *request, const uint8_t *raw, size_t length)
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
    ret = sscanf(raw, "STS/%1u.%1u%c%3u", &version_major, &version_minor,
                 &separator, &request->status_code);

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
            if (sscanf(data, "s:%uR", &request->sequence_number) != 1) {
                return STSE_UNSUPPORTED_HEADER;
            }
        } else if (data[0] == 'l') {
            if (sscanf(data, "l:%u", &request->content_length) != 1) {
                return STSE_UNSUPPORTED_HEADER;
            }
        } else {
            return STSE_UNSUPPORTED_HEADER;
        }

        skip_line(&data, &length, line_len);
    }

    if (length < request->content_length) {
        return STSE_INCOMPLETE_CONTENT;
    }

    request->content = data;
    return 0;
}
