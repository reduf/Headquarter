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

static int recv_sts_response(
    struct ssl_sts_connection *ssl,
    array_uint8_t *response,
    struct sts_header *header,
    size_t expected_seq_number)
{
    int ret;

    for (;;) {
        const size_t BUFFER_SIZE = 1024;
        uint8_t *ptr = array_push(*response, BUFFER_SIZE);

        size_t retlen;
        if ((ret = ssl_sts_connection_recv(ssl, ptr, BUFFER_SIZE, &retlen)) != 0)
            return 1;

        response->size = array_size(*response) - (BUFFER_SIZE - retlen);

        ret = sts_parse_header(header, response->data, response->size);
        if (ret != STSE_INCOMPLETE_CONTENT && ret != STSE_INCOMPLETE_HEADER) {
            break;
        }
    }

    if (ret != STSE_SUCCESS) {
        return 1;
    }

    if (header->status_code != 200) {
        fprintf(stderr, "Unexpected status code %u", header->status_code);
        return 1;
    }

    if (header->sequence_number != expected_seq_number) {
        fprintf(stderr, "Sequence number doesn't match expected sequence number");
        return 1;
    }

    return 0;
}

static int auth_login_finish(struct ssl_sts_connection *ssl)
{
    const char url[] = "/Auth/LoginFinish";
    const size_t url_len = sizeof(url) - 1;

    const uint32_t timeout = 29 * 1000;
    const size_t seq_number = 2;

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
        seq_number,
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

    struct sts_header header = {0};
    array_uint8_t response;
    array_init(response, 1024);

    if ((ret = recv_sts_response(ssl, &response, &header, seq_number)) != 0) {
        array_reset(response);
        return ret;
    }

    // Parse response

    return 0;
}

static int auth_list_game_accounts(struct ssl_sts_connection *ssl)
{
    const char url[] = "/Auth/ListMyGameAccounts";
    const size_t url_len = sizeof(url) - 1;

    const uint32_t timeout = 29 * 1000;
    const size_t seq_number = 3;

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
        seq_number,
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

    struct sts_header header = {0};
    array_uint8_t response;
    array_init(response, 1024);

    if ((ret = recv_sts_response(ssl, &response, &header, seq_number)) != 0) {
        array_reset(response);
        return ret;
    }

    return 0;
}

static int auth_request_game_token(struct ssl_sts_connection *ssl)
{
    const char url[] = "/Auth/RequestGameToken";
    const size_t url_len = sizeof(url) - 1;

    const uint32_t timeout = 29 * 1000;
    const size_t seq_number = 4;

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
        seq_number,
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

    struct sts_header header = {0};
    array_uint8_t response;
    array_init(response, 1024);

    if ((ret = recv_sts_response(ssl, &response, &header, seq_number)) != 0) {
        array_reset(response);
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
