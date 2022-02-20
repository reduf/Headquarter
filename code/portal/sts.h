#ifdef PORTAL_STS_H
#error "sts.h included more than once"
#endif
#define PORTAL_STS_H

typedef array(uint8_t) array_uint8_t;
struct ssl_tls12_context {
    array_uint8_t buffer;

    uint32_t random_time;
    uint8_t  random_bytes[28];

    // The length 256 is actually the limit, because the SRP extension
    // length (containing the data) needs to be encoded in a `uint8_t`.
    char     srp_username[256];
    size_t   srp_username_len;
};

void ssl_tls2_init(struct ssl_tls12_context *ctx);
void ssl_tls2_free(struct ssl_tls12_context *ctx);

int ssl_tls12_write_client_hello(struct ssl_tls12_context *ctx);

void sts_write_request(array_uint8_t *request,
    const char *url, size_t url_len,
    const uint8_t *body, size_t body_len);
