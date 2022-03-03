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

    if ((ret = sts_connection_connect(&sts, "cligate.1.ncplatform.net.")) != 0) {
        sts_connection_free(&sts);
        return 1;
    }

    if ((ret = sts_connection_start_tls(&sts)) != 0) {
        sts_connection_free(&sts);
        return 1;
    }

    struct ssl_sts_connection ssl;
    ssl_sts_connection_init(&ssl);
    ssl_sts_connection_upgrade_sts_connection(&ssl, &sts);
    sts_connection_free(&sts);

    if ((ret = ssl_sts_connection_seed(&ssl, &g_entropy)) != 0) {
        ssl_sts_connection_free(&ssl);
        return 1;
    }

    if ((ret = ssl_sts_connection_init_srp(&ssl, username, password)) != 0) {
        ssl_sts_connection_free(&ssl);
        return 1;
    }

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
    // I, P = <read from user>
    // N, g, s, B = <read from server>
    // a = random()
    // A = g^a % N
    // u = SHA1(PAD(A) | PAD(B))
    // k = SHA1(N | PAD(g))
    // x = SHA1(s | SHA1(I | ":" | P))
    // <premaster secret> = (B - (k * g^x)) ^ (a + (u * x)) % N

    return 0;
}
