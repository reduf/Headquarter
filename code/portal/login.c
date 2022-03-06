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

    // @TODO:
    // We do the handshake process. After this handshake, we will have
    // a secured connection with initialized ciphers. We will then be
    // able to send encrypted application data.
    // if ((ret = ssl_sts_connection_handshake(&ssl)) != 0) {
    //     ssl_sts_connection_free(&ssl);
    //     return 1;
    // }

    for (;;) {
        if ((ret = ssl_sts_connection_step(&ssl)) != 0) {
            if (ret != ERR_SSL_CONTINUE_PROCESSING) {
                ssl_sts_connection_free(&ssl);
                return ret;
            }
        }

        if (!array_empty(ssl.write)) {
            if ((ret = send_full(ssl.fd, ssl.write.data, ssl.write.size)) != 0) {
                ssl_sts_connection_free(&ssl);
                return ret;
            }

            array_clear(ssl.write);
        }

        if (ssl.state == AWAIT_CLIENT_FINISHED)
            break;

        // This is a ugly, but only there for testing. We need to not block on
        // socket read when there is nothing to read, but this is a next step.
        if (array_empty(ssl.read))
        {
            switch (ssl.state)
            {
                case AWAIT_SERVER_HELLO:
                case AWAIT_SERVER_KEY_EXCHANGE:
                case AWAIT_SERVER_HELLO_DONE:
                    if ((ret = recv_to_buffer(ssl.fd, &ssl.read)) != 0) {
                        ssl_sts_connection_free(&ssl);
                        return ret;
                    }
                    break;

                default:
                    break;
            }
        }
    }

    fprintf(stderr, "We are ready to initiate secured connection");

    // Time to calculate the "premaster secret"
    // U, P = <read from user> (username, password)
    // N, g, s, B = <read from server>
    // a = random() (client private)
    // A = g^a % N  (client public)
    // u = SHA1(PAD(A) | PAD(B))
    // k = SHA1(N | PAD(g))
    // x = SHA1(s | SHA1(U | ":" | P))
    // <premaster secret> = (B - (k * g^x)) ^ (a + (u * x)) % N

    return 0;
}
