#ifdef PORTAL_LOGIN_C
#error "login.c included more than once"
#endif
#define PORTAL_LOGIN_C

struct str {
    const char *ptr;
    size_t      len;
};

static const uint32_t TIMEOUT_MS = 29 * 1000;
static mbedtls_entropy_context g_entropy;

static struct str s_from_unsigned_utf8(const uint8_t *ptr, size_t len)
{
    return (struct str){(const char *)ptr, len};
}

static struct str s_from_signed_utf8(const char *ptr, size_t len)
{
    return (struct str){ptr, len};
}

static struct str s_from_c_str(const char *ptr)
{
    size_t len = strlen(ptr);
    return s_from_signed_utf8(ptr, len);
}

static int s_cmp(struct str *str1, struct str *str2)
{
    if (str1->len != str2->len)
        return str1->len - str2->len;
    return memcmp(str1->ptr, str2->ptr, str1->len);
}

// Find str1 in str2
static size_t s_find(struct str *str1, struct str *str2)
{
    if (str1->len < str2->len)
        return (size_t)-1;

    size_t count = str1->len - str2->len;
    for (size_t i = 0; i < count; ++i) {
        struct str temp = {
            .ptr = str1->ptr + i,
            .len = str2->len,
        };

        if (s_cmp(&temp, str2) == 0)
            return i;
    }

    return (size_t)-1;
}

static struct str s_substr(struct str *str, size_t pos, size_t len)
{
    if (str->len < pos)
        pos = str->len;
    if ((str->len - pos) < len)
        len = str->len - pos;
    return (struct str){ .ptr = str->ptr + pos, .len = len };
}

static bool s_parse_uuid(struct str *str, struct uuid *u)
{
    // This check means that the `sscanf` is safe.
    if (str->len != sizeof("AABBCCDD-AABB-AABB-AABB-AABBCCDDEEFF") - 1) {
        return false;
    }

    int ret = sscanf(
        str->ptr,
        "%08" SCNx32 "-%04" SCNx16 "-%04" SCNx16 "-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
        &u->time_low, &u->time_mid, &u->time_hi_and_version,
        &u->clock_seq_hi_and_reserved, &u->clock_seq_low, &u->node[0],
        &u->node[1], &u->node[2], &u->node[3], &u->node[4], &u->node[5]);

    if (ret != 11)
        return false;

    return true;
}

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
    struct sts_reply *reply,
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

        ret = sts_parse_reply(reply, response->data, response->size);
        if (ret != STSE_INCOMPLETE_CONTENT && ret != STSE_INCOMPLETE_HEADER) {
            break;
        }
    }

    if (ret != STSE_SUCCESS) {
        return 1;
    }

    if (reply->status_code != 200) {
        fprintf(stderr, "Unexpected status code %u", reply->status_code);
        return 1;
    }

    if (reply->sequence_number != expected_seq_number) {
        fprintf(stderr, "Sequence number doesn't match expected sequence number");
        return 1;
    }

    return 0;
}

static int auth_login_finish(struct sts_connection *sts, struct ssl_sts_connection *ssl)
{
    const char url[] = "/Auth/LoginFinish";
    const size_t url_len = sizeof(url) - 1;

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
        ++sts->seq_number,
        TIMEOUT_MS,
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

    struct sts_reply reply = {0};
    array_uint8_t response;
    array_init(response, 1024);

    if ((ret = recv_sts_response(ssl, &response, &reply, sts->seq_number)) != 0) {
        array_reset(response);
        return ret;
    }

    if (reply.status_code != 200) {
        fprintf(stderr, "Reply to '%s' failed with status %u\n", url, reply.status_code);
        array_reset(response);
        return 1;
    }

    // The reply has the following format:
    // <Reply>
    // <UserId>{GUID}</UserId>
    // <UserCenter>{Integer}</UserCenter>
    // <UserName>{Gw2 Username}</UserName>
    // <ResumeToken>{GUID}</ResumeToken>
    // <EmailVerified>{0|1}</EmailVerified>
    // </Reply>

    struct str reply_content = s_from_unsigned_utf8(reply.content, reply.content_length);
    struct str userid_open = s_from_c_str("<UserId>");
    struct str userid_close = s_from_c_str("</UserId>");

    size_t open_pos = s_find(&reply_content, &userid_open);
    if (open_pos == (size_t)-1) {
        fprintf(stderr, "Failed to find '<UserId>'\n");
        array_reset(response);
        return 1;
    }

    struct str rem = s_substr(&reply_content, (open_pos + userid_open.len), SIZE_MAX);
    size_t close_pos = s_find(&rem, &userid_close);
    if (close_pos == (size_t)-1) {
        fprintf(stderr, "Failed to find '</UserId>'\n");
        array_reset(response);
        return 1;
    }

    struct str user_id = s_substr(&rem, 0, close_pos);
    if (!s_parse_uuid(&user_id, &sts->user_id)) {
        fprintf(stderr, "Failed to parse the user_id uuid '%.*s'\n", (int)user_id.len, user_id.ptr);
        array_reset(response);
        return 1;
    }

    array_reset(response);
    return 0;
}

static int auth2f_upgrade_totp(
    struct sts_connection *sts,
    struct ssl_sts_connection *ssl,
    int otp,
    int remember_me)
{
    const char url[] = "/Auth2f/Upgrade";
    const size_t url_len = sizeof(url) - 1;

    array_uint8_t content;
    array_init(content, 1024);
    appendf(&content, "<Request>\n");
    appendf(&content, "<Otp>%d</Otp>\n", otp);
    if (remember_me != 0)
        appendf(&content, "<WhitelistIp/>\n");
    appendf(&content, "</Request>\n");

    array_uint8_t request;
    array_init(request, 1024);
    int ret = sts_write_request_with_sequence_number(
        &request,
        url, url_len,
        ++sts->seq_number,
        TIMEOUT_MS,
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

    struct sts_reply reply = {0};
    array_uint8_t response;
    array_init(response, 1024);

    if ((ret = recv_sts_response(ssl, &response, &reply, sts->seq_number)) != 0) {
        array_reset(response);
        return ret;
    }

    if (reply.status_code != 200) {
        fprintf(stderr, "Reply to '%s' failed with status %u\n", url, reply.status_code);
        array_reset(response);
        return 1;
    }

    array_reset(response);
    return 0;
}

static int auth_list_game_accounts(struct sts_connection *sts, struct ssl_sts_connection *ssl)
{
    const char url[] = "/Auth/ListMyGameAccounts";
    const size_t url_len = sizeof(url) - 1;

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
        ++sts->seq_number,
        TIMEOUT_MS,
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

    struct sts_reply reply = {0};
    array_uint8_t response;
    array_init(response, 1024);

    if ((ret = recv_sts_response(ssl, &response, &reply, sts->seq_number)) != 0) {
        array_reset(response);
        return ret;
    }

    if (reply.status_code != 200) {
        fprintf(stderr, "Reply to '%s' failed with status %u\n", url, reply.status_code);
        array_reset(response);
        return 1;
    }

    // The reply has the following format:
    // <Reply type="array">
    // <Row>
    // <GameCode>gw1</GameCode>
    // <Alias>gw1</Alias>
    // <Created>{ISO8601 time}</Created>
    // </Row>
    // </Reply>

    array_reset(response);
    return 0;
}

static int auth_request_game_token(struct sts_connection *sts, struct ssl_sts_connection *ssl)
{
    const char url[] = "/Auth/RequestGameToken";
    const size_t url_len = sizeof(url) - 1;

    // It seems that the 'GameCode' and 'AccountAlias' values are directly
    // taked from the previous packet. It returns an array of such information.

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
        ++sts->seq_number,
        TIMEOUT_MS,
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

    struct sts_reply reply = {0};
    array_uint8_t response;
    array_init(response, 1024);

    if ((ret = recv_sts_response(ssl, &response, &reply, sts->seq_number)) != 0) {
        array_reset(response);
        return ret;
    }

    if (reply.status_code != 200) {
        fprintf(stderr, "Reply to '%s' failed with status %u\n", url, reply.status_code);
        array_reset(response);
        return 1;
    }

    // The reply has the following format.
    // We need the game token to connect to Guild Wars 1.
    // <Reply>
    // <Token>{UUID}</Token>
    // </Reply>
    struct str reply_content = s_from_unsigned_utf8(reply.content, reply.content_length);
    struct str token_open = s_from_c_str("<Token>");
    struct str token_close = s_from_c_str("</Token>");

    size_t open_pos = s_find(&reply_content, &token_open);
    if (open_pos == (size_t)-1) {
        fprintf(stderr, "Failed to find '<Token>'\n");
        array_reset(response);
        return 1;
    }

    struct str rem = s_substr(&reply_content, (open_pos + token_open.len), SIZE_MAX);
    size_t close_pos = s_find(&rem, &token_close);
    if (close_pos == (size_t)-1) {
        fprintf(stderr, "Failed to find '</Token>'\n");
        array_reset(response);
        return 1;
    }

    struct str token = s_substr(&rem, 0, close_pos);
    if (!s_parse_uuid(&token, &sts->token)) {
        fprintf(stderr, "Failed to parse the token uuid '%.*s'\n", (int)token.len, token.ptr);
        array_reset(response);
        return 1;
    }

    array_reset(response);
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
        return ret;
    }

    struct ssl_sts_connection ssl;
    ssl_sts_connection_init(&ssl);

    if ((ret = sts_connection_start_tls(&sts, &ssl)) != 0) {
        goto cleanup;
    }

    // We "seed" the ssl structure effectively allowing it to generate
    // a random private key.
    if ((ret = ssl_sts_connection_seed(&ssl, &g_entropy)) != 0) {
        goto cleanup;
    }

    // We init the SRP necessary data. Note that the password isn't
    // kept in memory by the `struct ssl_sts_connection` structure.
    if ((ret = ssl_sts_connection_init_srp(&ssl, username, password)) != 0) {
        goto cleanup;
    }

    // We do the handshake process. After this handshake, we will have
    // a secured connection with initialized ciphers. We will then be
    // able to send encrypted application data.
    if ((ret = ssl_sts_connection_handshake(&ssl)) != 0) {
        goto cleanup;
    }

    // At this point, we have a secured connection and we are authenticathed.
    // We need to use the STS protocol, over TLS-SRP to retrieve a user id and
    // a game token.

    if ((ret = auth_login_finish(&sts, &ssl)) != 0) {
        goto cleanup;
    }

    if ((ret = auth_list_game_accounts(&sts, &ssl)) != 0) {
        goto cleanup;
    }

    if ((ret = auth_request_game_token(&sts, &ssl)) != 0) {
        goto cleanup;
    }

cleanup:
    ssl_sts_connection_free(&ssl);
    sts_connection_free(&sts);
    return ret;
}
