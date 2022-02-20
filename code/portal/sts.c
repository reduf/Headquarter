#ifdef PORTAL_STS_C
#error "sts.c included more than once"
#endif
#define PORTAL_STS_C

#define SSL_HS_CLIENT_HELLO 1
#define TLS_SRP_SHA_WITH_AES_256_CBC_SHA 0xC020
#define TLS_SRP_SHA_WITH_AES_128_CBC_SHA 0xC01D

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

    size_t extension_len = array_end(*buffer) - extensions_len_ptr;
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
