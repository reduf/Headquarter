#ifdef PORTAL_SSL_C
#error "ssl.c included more than once"
#endif
#define PORTAL_SSL_C

static void sha1_swap_word(const void *data, size_t size, uint8_t *digest)
{
    const uint8_t *bytes = (const uint8_t *)data;
    // @Robustness: Type aliasing
    mbedtls_sha1(bytes, size, digest);

    le32enc(digest,      be32dec(digest));
    le32enc(digest + 4,  be32dec(digest + 4));
    le32enc(digest + 8,  be32dec(digest + 8));
    le32enc(digest + 12, be32dec(digest + 12));
    le32enc(digest + 16, be32dec(digest + 16));
}

#define SHA1_DIGEST_SIZE 20

static int compute_srp_hash(
    uint8_t *digest,
    const uint8_t *part1, size_t part1_len,
    const uint8_t *part2, size_t part2_len)
{
    mbedtls_sha1_context ctx;
    mbedtls_sha1_init(&ctx);

    int ret;
    if ((ret = mbedtls_sha1_starts(&ctx)) != 0) {
        goto cleanup;
    }

    if ((ret = mbedtls_sha1_update(&ctx, part1, part1_len)) != 0) {
        goto cleanup;
    }

    uint8_t seperator = ':';
    if ((ret = mbedtls_sha1_update(&ctx, &seperator, sizeof(seperator))) != 0) {
        goto cleanup;
    }

    if ((ret = mbedtls_sha1_update(&ctx, part2, part2_len)) != 0) {
        goto cleanup;
    }

    if ((ret = mbedtls_sha1_finish(&ctx, digest)) != 0) {
        goto cleanup;
    }

cleanup:
    mbedtls_sha1_free(&ctx);
    if (ret != 0)
        ret = 1;
    return 0;
}

static int sha1_of_two_values(
    uint8_t *digest,
    const uint8_t *part1, size_t part1_len,
    const uint8_t *part2, size_t part2_len)
{
    mbedtls_sha1_context ctx;
    mbedtls_sha1_init(&ctx);

    int ret;
    if ((ret = mbedtls_sha1_starts(&ctx)) != 0) {
        goto cleanup;
    }

    if ((ret = mbedtls_sha1_update(&ctx, part1, part1_len)) != 0) {
        goto cleanup;
    }

    if ((ret = mbedtls_sha1_update(&ctx, part2, part2_len)) != 0) {
        goto cleanup;
    }

    if ((ret = mbedtls_sha1_finish(&ctx, digest)) != 0) {
        goto cleanup;
    }

cleanup:
    mbedtls_sha1_free(&ctx);
    if (ret != 0)
        ret = 1;
    return 0;
}

static int sha1_of_two_values_with_padding(
    uint8_t *digest, size_t padding,
    const uint8_t *part1, size_t part1_len,
    const uint8_t *part2, size_t part2_len)
{
    uint8_t buffer1[256];
    uint8_t buffer2[256];

    if ((sizeof(buffer1) < padding) && (sizeof(buffer2) < padding))
        return 1;

    if ((padding < part1_len) || (padding < part2_len))
        return 1;

    size_t number_of_zeroes = padding - part1_len;
    memset(buffer1, 0, number_of_zeroes);
    memcpy(buffer1 + number_of_zeroes, part1, part1_len);

    number_of_zeroes = padding - part2_len;
    memset(buffer2, 0, number_of_zeroes);
    memcpy(buffer2 + number_of_zeroes, part2, part2_len);

    return sha1_of_two_values(digest, buffer1, padding, buffer2, padding);
}

static int compute_static_hash(
    struct ssl_sts_connection *ssl,
    const char *username, size_t username_len,
    const char *password, size_t password_len)
{
    STATIC_ASSERT(SHA1_DIGEST_SIZE == sizeof(ssl->static_verifier_hash));
    return compute_srp_hash(
        ssl->static_verifier_hash,
        (const uint8_t *)username, username_len,
        (const uint8_t *)password, password_len);
}

void ssl_sts_connection_init(struct ssl_sts_connection *ssl)
{
    memset(ssl, 0, sizeof(*ssl));

    ssl->fd = INVALID_SOCKET;
    ssl->state = AWAIT_CLIENT_HELLO;

    mbedtls_ctr_drbg_init(&ssl->prng);

    array_init(ssl->read, 1024);
    array_init(ssl->write, 1024);
}

void ssl_sts_connection_free(struct ssl_sts_connection *ssl)
{
    if (ssl->fd != INVALID_SOCKET) {
        closesocket(ssl->fd);
        ssl->fd = INVALID_SOCKET;
    }

    mbedtls_ctr_drbg_free(&ssl->prng);

    array_reset(ssl->read);
    array_reset(ssl->write);
}

int ssl_sts_connection_init_srp(struct ssl_sts_connection *ssl, const char *username, const char *password)
{
    int ret;

    size_t username_len = strlen(username);
    size_t password_len = strlen(password);
    if ((ret = compute_static_hash(ssl, username, username_len, password, password_len)) != 0) {
        return ret;
    }

    if (sizeof(ssl->srp_username) < username_len) {
        return 1;
    }

    memcpy(ssl->srp_username, username, username_len);
    ssl->srp_username_len = username_len;

    return 0;
}

int ssl_sts_connection_seed(struct ssl_sts_connection *ssl, mbedtls_entropy_context *entropy)
{
    const uint8_t custom[] = "Master Togo";

    int ret = mbedtls_ctr_drbg_seed(
        &ssl->prng,
        mbedtls_entropy_func,
        entropy,
        custom,
        sizeof(custom) - 1);

    if (ret != 0) {
        fprintf(stderr, "Failed to see the prng\n");
        return 1;
    }

    ret = mbedtls_ctr_drbg_random(
        &ssl->prng,
        ssl->client_key.private,
        sizeof(ssl->client_key.private));

    if (ret != 0) {
        return 1;
    }

    return 0;
}

#define SSL_HS_CLIENT_HELLO            1
#define SSL_HS_SERVER_HELLO            2
#define SSL_HS_SERVER_KEY_EXCHANGE     12
#define SSL_HS_CLIENT_KEY_EXCHANGE     16

#define TLS_SRP_SHA_WITH_AES_256_CBC_SHA 0xC020
#define TLS_SRP_SHA_WITH_AES_128_CBC_SHA 0xC01D

#define SSL_MSG_CHANGE_CIPHER_SPEC     20
#define SSL_MSG_ALERT                  21
#define SSL_MSG_HANDSHAKE              22
#define SSL_MSG_APPLICATION_DATA       23
#define SSL_MSG_CID                    25

static void ssl_srp_start_protocol_msg(struct ssl_sts_connection *ssl, size_t *header_pos)
{
    *header_pos = array_size(ssl->write);

    array_add(ssl->write, SSL_MSG_HANDSHAKE);

    // The version is always "\x03\x03". (i.e., TLS v1.2)
    array_add(ssl->write, 0x03);
    array_add(ssl->write, 0x03);

    // Reserve two bytes to later write the length.
    (void)array_push(ssl->write, 2);
}

static int ssl_srp_finish_protocol_msg(struct ssl_sts_connection *ssl, size_t header_pos)
{
    const size_t MSG_HDR_LEN = 5;
    assert((header_pos + MSG_HDR_LEN) <= array_size(ssl->write));
    assert(array_at(ssl->write, header_pos) == SSL_MSG_HANDSHAKE);

    size_t msg_size = array_size(ssl->write) - (header_pos + MSG_HDR_LEN);
    if ((size_t)UINT16_MAX < msg_size)
        return 1;

    uint8_t *p = &array_at(ssl->write, header_pos + 3);
    be16enc(p, (uint16_t)msg_size);
    return 0;
}

static void ssl_srp_start_handshake_msg(struct ssl_sts_connection *ssl, size_t *pos, uint8_t hs_type)
{
    size_t header_pos;
    ssl_srp_start_protocol_msg(ssl, &header_pos);

    // We assume this offset when we finish the handhsake message.
    // There should be more elegant solution, but that's what we
    // have for now.
    assert((array_size(ssl->write) - header_pos) == 5);

    //
    // Reserve 4 bytes for hanshake header.
    //    ...
    //    HandshakeType msg_type;
    //    uint24 length;
    //    ...
    //

    // Save the position of the header to later write the appropriate size.
    *pos = array_size(ssl->write);

    array_add(ssl->write, hs_type);
    (void)array_push(ssl->write, 3);
}

static int ssl_srp_finish_handshake_msg(struct ssl_sts_connection *ssl, size_t header_pos, uint8_t hs_type)
{
    int ret;
    const size_t HANDSHAKE_HDR_LEN = 4;
    assert((header_pos + HANDSHAKE_HDR_LEN) <= array_size(ssl->write));
    assert(array_at(ssl->write, header_pos) == hs_type);

    uint8_t *content_begin = array_begin(ssl->write) + header_pos + HANDSHAKE_HDR_LEN;
    size_t content_length = array_end(ssl->write) - content_begin;

    const size_t UINT24_MAX = 0xFFFFFF;
    if (UINT24_MAX <= content_length)
        return 1;

    uint8_t *hs_len_ptr = array_begin(ssl->write) + 1;
    hs_len_ptr[header_pos + 0] = (content_length >> 16) & 0xFF;
    hs_len_ptr[header_pos + 1] = (content_length >> 8) & 0xFF;
    hs_len_ptr[header_pos + 2] = content_length & 0xFF;

    if ((ret = ssl_srp_finish_protocol_msg(ssl, (header_pos - 5))) != 0) {
        return ret;
    }

    return 0;
}

static void ssl_srp_write_srp(array_uint8_t *buffer,
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

static int ssl_srp_write_extension(struct ssl_sts_connection *ssl)
{
    // We first need to write the length on a `uint16be_t`, but we don't know
    // it yet. Instead, we reserve those bytes and we will write it later.
    size_t extensions_len_pos = array_size(ssl->write);
    (void)array_push(ssl->write, 2);

    // The first extension is undocumented and doesn't contain any data.
    // So we simply hardcode it.
    array_add_be_uint16(&ssl->write, 0xADAE);
    array_add_be_uint16(&ssl->write, 0); // Length is 0

    // The second extension is SRP specifying the username. (i.e., email)
    ssl_srp_write_srp(&ssl->write, ssl->srp_username, ssl->srp_username_len);

    size_t extension_len = array_size(ssl->write) - (extensions_len_pos + 2);
    if ((size_t)UINT16_MAX < extension_len)
        return 1;

    be16enc(&array_at(ssl->write, extensions_len_pos), (uint16_t)extension_len);
    return 0;
}

static int ssl_srp_write_client_hello_body(struct ssl_sts_connection *ssl)
{
    // The version is always "\x03\x03". (i.e., TLS v1.2)
    array_add(ssl->write, 0x03);
    array_add(ssl->write, 0x03);

    // Write random bytes.
    {
        // This pointer gets invalidated whenever we do anything with the
        // array.
        uint8_t *random_time_ptr = array_push(ssl->write, 4);
        be32enc(random_time_ptr, ssl->client_random.time);
    }

    array_insert(ssl->write, sizeof(ssl->client_random.bytes), ssl->client_random.bytes);

    // The session id is always null
    array_add(ssl->write, 0);

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
    array_add_be_uint16(&ssl->write, (uint16_t)sizeof(ciphers));
    for (size_t i = 0; i < ARRAY_SIZE(ciphers); ++i) {
        array_add_be_uint16(&ssl->write, ciphers[i]);
    }

    // Write compression methods.
    // This is hardcoded and means 1 compression methods named "null".
    // Not sure if required, but simply reproducing the behavior of the
    // official DLL.
    array_add(ssl->write, 1);
    array_add(ssl->write, 0);

    if (ssl_srp_write_extension(ssl) != 0) {
        return 1;
    }

    return 0;
}

static int ssl_srp_write_client_hello(struct ssl_sts_connection *ssl)
{
    assert(ssl->state == AWAIT_CLIENT_HELLO);

    size_t header_pos;
    ssl_srp_start_handshake_msg(ssl, &header_pos, SSL_HS_CLIENT_HELLO);

    if (ssl_srp_write_client_hello_body(ssl) != 0) {
        return 1;
    }

    if (ssl_srp_finish_handshake_msg(ssl, header_pos, SSL_HS_CLIENT_HELLO) != 0) {
        return 1;
    }

    // @Cleanup: We need to calculate the checksum for the hashmac.

    ssl->state = AWAIT_SERVER_HELLO;
    return 0;
}

static int compute_client_public(struct ssl_sts_connection *ssl)
{
    int ret;
    mbedtls_mpi generator;
    mbedtls_mpi prime_modulus;
    mbedtls_mpi client_private;
    mbedtls_mpi client_public;

    mbedtls_mpi_init(&generator);
    mbedtls_mpi_init(&prime_modulus);
    mbedtls_mpi_init(&client_private);
    mbedtls_mpi_init(&client_public);

    if ((ret = mbedtls_mpi_read_binary(&prime_modulus, ssl->server_key.prime,
                                       sizeof(ssl->server_key.prime))) != 0) {
        ret = 1;
        goto cleanup;
    }
    if ((ret = mbedtls_mpi_read_binary(&generator, ssl->server_key.generator,
                                       sizeof(ssl->server_key.generator))) != 0) {
        ret = 1;
        goto cleanup;
    }
    if ((ret = mbedtls_mpi_read_binary(&client_private, ssl->client_key.private,
                                       sizeof(ssl->client_key.private))) != 0) {
        ret = 1;
        goto cleanup;
    }

    if ((ret = mbedtls_mpi_exp_mod(&client_public, &generator, &client_private,
                                   &prime_modulus, NULL)) != 0) {
        ret = 1;
        goto cleanup;
    }

    if ((ret = mbedtls_mpi_write_binary(&client_public, ssl->client_key.public,
                                        sizeof(ssl->client_key.public))) != 0) {
        ret = 1;
        goto cleanup;
    }

cleanup:
    mbedtls_mpi_free(&generator);
    mbedtls_mpi_free(&prime_modulus);
    mbedtls_mpi_free(&client_private);
    mbedtls_mpi_free(&client_public);
    return ret;
}

static int ssl_srp_write_client_key_exchange(struct ssl_sts_connection *ssl)
{
    assert(ssl->state == AWAIT_CLIENT_KEY_EXCHANGE);
    STATIC_ASSERT(sizeof(ssl->client_key.public) <= UINT16_MAX);

    int ret;
    if ((ret = compute_client_public(ssl)) != 0) {
        return ret;
    }

    size_t header_pos;
    ssl_srp_start_handshake_msg(ssl, &header_pos, SSL_HS_CLIENT_KEY_EXCHANGE);

    const uint16_t keylen = sizeof(ssl->client_key.public);
    array_add_be_uint16(&ssl->write, keylen);
    array_insert(ssl->write, keylen, ssl->client_key.public);

    if (ssl_srp_finish_handshake_msg(ssl, header_pos, SSL_HS_CLIENT_KEY_EXCHANGE) != 0) {
        return 1;
    }

    ssl->state = AWAIT_CLIENT_CHANGE_CIPHER_SPEC;
    return 0;
}

static int ssl_srp_write_change_cipher_spec(struct ssl_sts_connection *ssl)
{
    array_add(ssl->write, SSL_MSG_CHANGE_CIPHER_SPEC);

    // The version is always "\x03\x03". (i.e., TLS v1.2)
    array_add(ssl->write, 0x03);
    array_add(ssl->write, 0x03);

    // We hardcode the length of the message, because we only send one.
    array_add_be_uint16(&ssl->write, 1);

    array_add(ssl->write, 1);

    ssl->state = AWAIT_CLIENT_FINISHED;
    return 0;
}

static int ssl_srp_change_cipher_spec(struct ssl_sts_connection *ssl)
{
    assert(ssl->state == AWAIT_CLIENT_CHANGE_CIPHER_SPEC);

    int ret;

    uint8_t verifier_digest[SHA1_DIGEST_SIZE];
    ret = sha1_of_two_values(
        verifier_digest,
        ssl->server_key.salt, sizeof(ssl->server_key.salt),
        ssl->static_verifier_hash, sizeof(ssl->static_verifier_hash));
    if (ret != 0)
        return ret;

    const size_t padding = sizeof(ssl->server_key.prime);

    uint8_t u_buffer[SHA1_DIGEST_SIZE];
    ret = sha1_of_two_values_with_padding(
        u_buffer, padding,
        ssl->client_key.public, sizeof(ssl->client_key.public),
        ssl->server_key.server_public, sizeof(ssl->server_key.server_public));
    if (ret != 0)
        return ret;

    uint8_t k_buffer[SHA1_DIGEST_SIZE];
    ret = sha1_of_two_values_with_padding(
        k_buffer, padding,
        ssl->server_key.prime, sizeof(ssl->server_key.prime),
        ssl->server_key.generator, sizeof(ssl->server_key.generator));
    if (ret != 0)
        return ret;

    // Calculating the pre-master secret.
    mbedtls_mpi generator;
    mbedtls_mpi prime;
    mbedtls_mpi server_public;
    mbedtls_mpi client_public;
    mbedtls_mpi client_private;
    mbedtls_mpi verifier;
    mbedtls_mpi u;
    mbedtls_mpi k;

    mbedtls_mpi_init(&generator); // Also "g"
    mbedtls_mpi_init(&prime); // Also "N"
    mbedtls_mpi_init(&server_public); // Also "B"
    mbedtls_mpi_init(&client_public); // Also "A"
    mbedtls_mpi_init(&client_private); // Also "a"
    mbedtls_mpi_init(&verifier); // Also "x"
    mbedtls_mpi_init(&u);
    mbedtls_mpi_init(&k);

    // Those values are the outputs or the temporary values.
    mbedtls_mpi shared_secret;
    mbedtls_mpi base;
    mbedtls_mpi base1;
    mbedtls_mpi base2;
    mbedtls_mpi exponent;
    mbedtls_mpi exponent1;

    mbedtls_mpi_init(&shared_secret);
    mbedtls_mpi_init(&base);
    mbedtls_mpi_init(&base1);
    mbedtls_mpi_init(&base2);
    mbedtls_mpi_init(&exponent);
    mbedtls_mpi_init(&exponent1);

    if ((ret = mbedtls_mpi_read_binary(&generator, ssl->server_key.generator, sizeof(ssl->server_key.generator))) != 0) {
        goto cleanup;
    }
    if ((ret = mbedtls_mpi_read_binary(&prime, ssl->server_key.prime, sizeof(ssl->server_key.prime))) != 0) {
        goto cleanup;
    }
    if ((ret = mbedtls_mpi_read_binary(&server_public, ssl->server_key.server_public, sizeof(ssl->server_key.server_public))) != 0) {
        goto cleanup;
    }
    if ((ret = mbedtls_mpi_read_binary(&client_public, ssl->client_key.public, sizeof(ssl->client_key.public))) != 0) {
        goto cleanup;
    }
    if ((ret = mbedtls_mpi_read_binary(&client_private, ssl->client_key.private, sizeof(ssl->client_key.private))) != 0) {
        goto cleanup;
    }
    if ((ret = mbedtls_mpi_read_binary(&verifier, verifier_digest, sizeof(verifier_digest))) != 0) {
        goto cleanup;
    }
    if ((ret = mbedtls_mpi_read_binary(&u, u_buffer, sizeof(u_buffer))) != 0) {
        goto cleanup;
    }
    if ((ret = mbedtls_mpi_read_binary(&k, k_buffer, sizeof(k_buffer))) != 0) {
        goto cleanup;
    }

    // The formula is:
    // <premaster secret> = (B - (k * g^x)) ^ (a + (u * x)) % N
    // base = (B - (k * g^x))
    //  base1 = g^x
    //  base2 = k * g^x
    if ((ret = mbedtls_mpi_exp_mod(&base1, &generator, &verifier, &prime, NULL)) != 0)
        goto cleanup;
    if ((ret = mbedtls_mpi_mul_mpi(&base2, &k, &base1)) != 0)
        goto cleanup;
    if ((ret = mbedtls_mpi_sub_mpi(&base, &server_public, &base2)) != 0)
        goto cleanup;

    // exponent = (a + (u * x))
    //  exponent1 = (u * x)
    if ((ret = mbedtls_mpi_mul_mpi(&exponent1, &u, &verifier)) != 0)
        goto cleanup;
    if ((ret = mbedtls_mpi_add_mpi(&exponent, &client_private, &exponent1)) != 0)
        goto cleanup;

    // Calculate the premastered secret
    if ((ret = mbedtls_mpi_exp_mod(&shared_secret, &base, &exponent, &prime, NULL)) != 0)
        goto cleanup;

    if ((ret = ssl_srp_write_change_cipher_spec(ssl)) != 0) {
        return 1;
    }

    ssl->state = AWAIT_CLIENT_FINISHED;
    return 0;

cleanup:;
    mbedtls_mpi_free(&generator);
    mbedtls_mpi_free(&prime);
    mbedtls_mpi_free(&server_public);
    mbedtls_mpi_free(&client_public);
    mbedtls_mpi_free(&client_private);
    mbedtls_mpi_free(&verifier);
    mbedtls_mpi_free(&u);
    mbedtls_mpi_free(&k);

    mbedtls_mpi_free(&shared_secret);
    mbedtls_mpi_free(&base);
    mbedtls_mpi_free(&base1);
    mbedtls_mpi_free(&base2);
    mbedtls_mpi_free(&exponent);
    mbedtls_mpi_free(&exponent1);

    if (ret != 0)
        ret = 1;
    return ret;
}

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

static int parse_server_hello(struct ssl_sts_connection *ssl, const uint8_t *data, size_t length)
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

    if ((ret = chk_stream_read32(&content, &content_len, &ssl->server_random.time)) != 0)
        return ret;
    if ((ret = chk_stream_read(&content, &content_len, ssl->server_random.bytes,
                               sizeof(ssl->server_random.bytes))) != 0) {
        return ret;
    }

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

static int ssl_srp_process_server_hello(struct ssl_sts_connection *ssl)
{
    assert(ssl->state == AWAIT_SERVER_HELLO);

    int ret;
    const uint8_t *inner_data;
    size_t inner_len;

    ret = parse_tls12_handshake(
        ssl->read.data, ssl->read.size, SSL_MSG_HANDSHAKE,
        &inner_data, &inner_len);
    if (ret != 0) {
        return ret;
    }

    if ((ret = parse_server_hello(ssl, inner_data, inner_len)) != 0)
        return ret;

    size_t processed = (inner_data - ssl->read.data) + inner_len;
    array_remove_range_ordered(ssl->read, 0, processed);
    ssl->state = AWAIT_SERVER_KEY_EXCHANGE;
    return 0;
}

static int parse_server_key_exchange(struct server_key *key, const uint8_t *data, size_t length)
{
    int ret;
    // The type is encoded on 1 byte and the length on 3 bytes.
    const size_t MIN_LEN = 4;

    if (length < MIN_LEN)
        return ERR_SSL_BAD_INPUT_DATA;

    if (data[0] != SSL_HS_SERVER_KEY_EXCHANGE)
        return ERR_SSL_UNEXPECTED_MESSAGE;

    uint32_t content_len = be24dec(&data[1]);
    if ((length - MIN_LEN) != content_len) {
        // At this point, we should already know we have the complete data.
        return ERR_SSL_BAD_INPUT_DATA;
    }

    const uint8_t *content = &data[MIN_LEN];

    // Currently, we support only an hardcoded format, with specified length
    // for every integers. This isn't a requirement of the protocol, but it's
    // simpler for us.

    uint16_t prime_len;
    if ((ret = chk_stream_read16(&content, &content_len, &prime_len)) != 0)
        return ret;
    if (prime_len != sizeof(key->prime)) {
        // @Cleanup: Add trace prints.
        return ERR_SSL_UNSUPPORTED_PROTOCOL;
    }
    if ((ret = chk_stream_read(&content, &content_len, key->prime, sizeof(key->prime))) != 0)
        return ret;

    uint16_t generator_len;
    if ((ret = chk_stream_read16(&content, &content_len, &generator_len)) != 0)
        return ret;
    if (generator_len != sizeof(key->generator)) {
        // @Cleanup: Add trace prints.
        return ERR_SSL_UNSUPPORTED_PROTOCOL;
    }
    if ((ret = chk_stream_read(&content, &content_len, key->generator, sizeof(key->generator))) != 0)
        return ret;

    uint8_t salt_len;
    if ((ret = chk_stream_read8(&content, &content_len, &salt_len)) != 0)
        return ret;
    if (salt_len != sizeof(key->salt)) {
        // @Cleanup: Add trace prints.
        return ERR_SSL_UNSUPPORTED_PROTOCOL;
    }
    if ((ret = chk_stream_read(&content, &content_len, key->salt, sizeof(key->salt))) != 0)
        return ret;

    uint16_t server_public_len;
    if ((ret = chk_stream_read16(&content, &content_len, &server_public_len)) != 0)
        return ret;
    if (server_public_len != sizeof(key->server_public)) {
        // @Cleanup: Add trace prints.
        return ERR_SSL_UNSUPPORTED_PROTOCOL;
    }
    if ((ret = chk_stream_read(&content, &content_len, key->server_public, sizeof(key->server_public))) != 0)
        return ret;

    if (content_len != 0)
        return ERR_SSL_BAD_INPUT_DATA;

    return 0;
}

static int ssl_srp_process_server_key_exchange(struct ssl_sts_connection *ssl)
{
    assert(ssl->state == AWAIT_SERVER_KEY_EXCHANGE);

    int ret;
    const uint8_t *inner_data;
    size_t inner_len;

    if ((ret = parse_tls12_handshake(ssl->read.data, ssl->read.size, SSL_MSG_HANDSHAKE,
                                     &inner_data, &inner_len)) != 0) {
        return ret;
    }

    if ((ret = parse_server_key_exchange(&ssl->server_key, inner_data, inner_len)) != 0)
        return ret;

    size_t processed = (inner_data - ssl->read.data) + inner_len;
    array_remove_range_ordered(ssl->read, 0, processed);
    ssl->state = AWAIT_SERVER_HELLO_DONE;
    return 0;
}

static int sts_process_server_done(struct ssl_sts_connection *ssl)
{
    assert(ssl->state == AWAIT_SERVER_HELLO_DONE);

    int ret;
    const uint8_t *inner_data;
    size_t inner_len;

    if ((ret = parse_tls12_handshake(ssl->read.data, ssl->read.size, SSL_MSG_HANDSHAKE,
                                     &inner_data, &inner_len)) != 0) {
        return ret;
    }

    char expected[] = "\x0e\x00\x00\x00";
    if (inner_len != (sizeof(expected) - 1))
        return ERR_SSL_BAD_INPUT_DATA;

    if (memcmp(inner_data, expected, inner_len) != 0)
        return ERR_SSL_BAD_INPUT_DATA;

    size_t processed = (inner_data - ssl->read.data) + inner_len;
    array_remove_range_ordered(ssl->read, 0, processed);
    ssl->state = AWAIT_CLIENT_KEY_EXCHANGE;
    return 0;
}

static int wait_for_server_step(int (*stepf)(struct ssl_sts_connection *ssl), struct ssl_sts_connection *ssl)
{
    int ret;
    for (;;) {
        ret = stepf(ssl);

        // `ERR_SSL_CONTINUE_PROCESSING` means we don't have enough data,
        // so we read more from the network.
        if (ret != ERR_SSL_CONTINUE_PROCESSING)
            break;

        if ((ret = recv_to_buffer(ssl->fd, &ssl->read)) != 0) {
            return ERR_SSL_UNEXPECTED_MESSAGE;
        }
    }

    return ret;
}

static int send_client_step(int (*stepf)(struct ssl_sts_connection *ssl), struct ssl_sts_connection *ssl)
{
    int ret;
    if ((ret = stepf(ssl)) != 0)
        return ret;
    if ((ret = send_full(ssl->fd, ssl->write.data, ssl->write.size)) != 0)
        return ret;
    return 0;
}

int ssl_sts_connection_handshake(struct ssl_sts_connection *ssl)
{
    if (ssl->state != AWAIT_CLIENT_HELLO) {
        fprintf(stderr, "Invalid state (%d) the proceed to the handhsake\n", (int)ssl->state);
        return 1;
    }

    int ret;
    if ((ret = send_client_step(ssl_srp_write_client_hello, ssl)) != 0) {
        fprintf(stderr, "'ssl_srp_write_client_hello' failed: %d\n", ret);
        return 1;
    }

    if ((ret = wait_for_server_step(ssl_srp_process_server_hello, ssl)) != 0) {
        fprintf(stderr, "'ssl_srp_process_server_hello' failed: %d\n", ret);
        return 1;
    }

    if ((ret = wait_for_server_step(ssl_srp_process_server_key_exchange, ssl)) != 0) {
        fprintf(stderr, "'ssl_srp_process_server_key_exchange' failed: %d\n", ret);
        return 1;
    }

    if ((ret = wait_for_server_step(sts_process_server_done, ssl)) != 0) {
        fprintf(stderr, "'sts_process_server_done' failed: %d\n", ret);
        return 1;
    }

    if ((ret = send_client_step(ssl_srp_write_client_key_exchange, ssl)) != 0) {
        fprintf(stderr, "'ssl_srp_write_client_key_exchange' failed: %d\n", ret);
        return 1;
    }

    if ((ret = send_client_step(ssl_srp_change_cipher_spec, ssl)) != 0) {
        fprintf(stderr, "'ssl_srp_change_cipher_spec' failed: %d\n", ret);
        return 1;
    }

    assert(ssl->state == AWAIT_CLIENT_FINISHED);
    return 0;
}
