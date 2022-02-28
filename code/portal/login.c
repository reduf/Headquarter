#ifdef PORTAL_LOGIN_C
#error "login.c included more than once"
#endif
#define PORTAL_LOGIN_C

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

    if ((ret = ssl_sts_connection_init_srp(&ssl, username, password)) != 0) {
        ssl_sts_connection_free(&ssl);
        return 1;
    }

    for (;;) {
        if ((ret = ssl_sts_connection_step(&ssl)) != 0) {
            ssl_sts_connection_free(&ssl);
            return ret;
        }
    }

    return 0;
}
