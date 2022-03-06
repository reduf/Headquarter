#ifdef PORTAL_SSL_H
#error "ssl.h included more than once"
#endif
#define PORTAL_SSL_H

#define ERR_SSL_CONTINUE_PROCESSING    1
#define ERR_SSL_UNEXPECTED_MESSAGE     2
#define ERR_SSL_UNSUPPORTED_PROTOCOL   3
#define ERR_SSL_BAD_INPUT_DATA         4

enum ssl_sts_state {
    AWAIT_CLIENT_HELLO,
    AWAIT_SERVER_HELLO,
    AWAIT_SERVER_KEY_EXCHANGE,
    AWAIT_SERVER_HELLO_DONE,
    AWAIT_CLIENT_KEY_EXCHANGE,
    AWAIT_CLIENT_CHANGE_CIPHER_SPEC,
    AWAIT_CLIENT_FINISHED,
};

// The hardcoded size here do not reflect the size(s) that are supported by
// the protocol, but it's currently what is used by portal.  We could be more
// flexible, trading some simplicity.
struct server_key {
    uint8_t prime[128];
    uint8_t generator[1];
    uint8_t salt[8];
    uint8_t server_public[128];
};

struct client_key {
    uint8_t private[128];
    uint8_t public[128];
};

struct tls12_random {
    uint32_t time;
    uint8_t  bytes[28];
};

struct ssl_sts_connection {
    SOCKET fd;
    enum ssl_sts_state state;

    mbedtls_ctr_drbg_context prng;

    array_uint8_t read;
    array_uint8_t write;

    struct tls12_random client_random;
    struct tls12_random server_random;
    struct server_key   server_key;
    struct client_key   client_key;

    char srp_username[256];
    size_t srp_username_len;

    uint8_t static_verifier_hash[20];
    uint8_t verifier_hash[20];
};

void ssl_sts_connection_init(struct ssl_sts_connection *ssl);
void ssl_sts_connection_free(struct ssl_sts_connection *ssl);

int ssl_sts_connection_init_srp(struct ssl_sts_connection *ssl, const char *username, const char *password);
int ssl_sts_connection_seed(struct ssl_sts_connection *ssl, mbedtls_entropy_context *entropy);

int ssl_sts_connection_handshake(struct ssl_sts_connection *ssl);
