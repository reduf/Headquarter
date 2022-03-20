#ifdef PORTAL_LOGIN_C
#error "login.c included more than once"
#endif
#define PORTAL_LOGIN_C

static mbedtls_entropy_context g_entropy;

int portal_init()
{
    mbedtls_entropy_init(&g_entropy);
    return 0;
}

void portal_free()
{
    mbedtls_entropy_free(&g_entropy);
}

static int auth_login_finish(struct ssl_sts_connection *ssl)
{
    const char url[] = "/Auth/LoginFinish";
    const size_t url_len = sizeof(url) - 1;

    const uint32_t timeout = 29 * 1000;

    array_uint8_t content;
    array_init(content, 1024);
    appendf(&content, "<Request>\n");
    appendf(&content, "<Language>en</Language>\n");
    appendf(&content, "</Request>\n");

    array_uint8_t request;
    array_init(request, 1024);
    int ret = sts_write_request_with_sequence_number(
        &request,
        url, url_len,
        2,
        timeout,
        content.data, content.size);

    array_reset(content);

    if (ret != 0) {
        return ret;
    }

    ret = ssl_sts_connection_send(ssl, request.data, request.size);
    array_reset(request);

    if (ret != 0) {
        return ret;
    }

    return 0;
}

static int auth_list_game_accounts(struct ssl_sts_connection *ssl)
{
    const char url[] = "/Auth/ListMyGameAccounts";
    const size_t url_len = sizeof(url) - 1;

    const uint32_t timeout = 29 * 1000;

    array_uint8_t content;
    array_init(content, 1024);
    appendf(&content, "<Request>\n");
    appendf(&content, "<GameCode>gw1</GameCode>\n");
    appendf(&content, "</Request>\n");

    array_uint8_t request;
    array_init(request, 1024);
    int ret = sts_write_request_with_sequence_number(
        &request,
        url, url_len,
        2,
        timeout,
        content.data, content.size);

    array_reset(content);

    if (ret != 0) {
        return ret;
    }

    ret = ssl_sts_connection_send(ssl, request.data, request.size);
    array_reset(request);

    if (ret != 0) {
        return ret;
    }

    return 0;
}

static int auth_request_game_token(struct ssl_sts_connection *ssl)
{
    const char url[] = "/Auth/RequestGameToken";
    const size_t url_len = sizeof(url) - 1;

    const uint32_t timeout = 29 * 1000;

    array_uint8_t content;
    array_init(content, 1024);
    appendf(&content, "<Request>\n");
    appendf(&content, "<GameCode>gw1</GameCode>\n");
    appendf(&content, "<AccountAlias>gw1</AccountAlias>\n");
    appendf(&content, "</Request>\n");

    array_uint8_t request;
    array_init(request, 1024);
    int ret = sts_write_request_with_sequence_number(
        &request,
        url, url_len,
        2,
        timeout,
        content.data, content.size);

    array_reset(content);

    if (ret != 0) {
        return ret;
    }

    ret = ssl_sts_connection_send(ssl, request.data, request.size);
    array_reset(request);

    if (ret != 0) {
        return ret;
    }

    return 0;
}

int portal_login(const char *username, const char *password)
{
    int ret;

    struct sts_connection sts;
    sts_connection_init(&sts);

    // Open socket and send the initial message, following ANet proprietary
    // "sts" protocol.
    if ((ret = sts_connection_connect(&sts, "cligate.1.ncplatform.net.")) != 0) {
        sts_connection_free(&sts);
        return 1;
    }

    struct ssl_sts_connection ssl;
    ssl_sts_connection_init(&ssl);

    if ((ret = sts_connection_start_tls(&sts, &ssl)) != 0) {
        sts_connection_free(&sts);
        ssl_sts_connection_free(&ssl);
        return 1;
    }

    // We are done with the sts object, all relevant information are
    // transferred to the ssl struct.
    sts_connection_free(&sts);

    // We "seed" the ssl structure effectively allowing it to generate
    // a random private key.
    if ((ret = ssl_sts_connection_seed(&ssl, &g_entropy)) != 0) {
        ssl_sts_connection_free(&ssl);
        return 1;
    }

    // We init the SRP necessary data. Note that the password isn't
    // kept in memory by the `struct ssl_sts_connection` structure.
    if ((ret = ssl_sts_connection_init_srp(&ssl, username, password)) != 0) {
        ssl_sts_connection_free(&ssl);
        return 1;
    }

    // We do the handshake process. After this handshake, we will have
    // a secured connection with initialized ciphers. We will then be
    // able to send encrypted application data.
    if ((ret = ssl_sts_connection_handshake(&ssl)) != 0) {
        ssl_sts_connection_free(&ssl);
        return 1;
    }

    // At this point, we have a secured connection and we are authenticathed.
    // We need to use the STS protocol, over TLS-SRP to retrieve a user id and
    // a game token.

    if ((ret = auth_login_finish(&ssl)) != 0) {
        ssl_sts_connection_free(&ssl);
        return 1;
    }

    if ((ret = auth_list_game_accounts(&ssl)) != 0) {
        ssl_sts_connection_free(&ssl);
        return 1;
    }

    if ((ret = auth_request_game_token(&ssl)) != 0) {
        ssl_sts_connection_free(&ssl);
        return 1;
    }

    ssl_sts_connection_free(&ssl);
    return 0;
}
