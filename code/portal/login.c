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

    fprintf(stderr, "We are ready to initiate secured connection");

    ssl_sts_connection_free(&ssl);
    return 0;
}
