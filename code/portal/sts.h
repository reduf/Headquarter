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
void ssl_tls12_write_auth_packet(array_uint8_t *buffer, const uint8_t *content, size_t length);

int ssl_tls12_write_client_key_exchange(array_uint8_t *buffer, const uint8_t *key, size_t key_len);
void ssl_tls12_write_change_cipher_spec(array_uint8_t *buffer);

struct server_hello {
    uint32_t random_time;
    uint8_t  random_bytes[28];
};

int sts_process_server_hello(struct server_hello *hello, const uint8_t *data, size_t length);

struct server_key {
    uint8_t prime[128];
    uint8_t generator[1];
    uint8_t salt[8];
    uint8_t server_public[128];
};

int sts_process_server_key_exchange(struct server_key *key, const uint8_t *data, size_t length);
int sts_process_server_done(const uint8_t *data, size_t length);

void sts_write_request(array_uint8_t *request,
    const char *url, size_t url_len,
    const uint8_t *content, size_t content_len);

void sts_write_sequenced_request(
    array_uint8_t *request, size_t seq_number, uint32_t timeout_ms,
    const char *url, size_t url_len, const uint8_t *content, size_t content_len);

#define STSE_SUCCESS                 0
#define STSE_INCOMPLETE_CONTENT      1
#define STSE_INCOMPLETE_HEADER       2
#define STSE_UNSUPPORTED_HEADER      3
#define STSE_UNSUPPORTED_PROTOCOL    4

struct sts_request {
    unsigned status_code;
    unsigned sequence_number;
    unsigned content_length;
    const uint8_t *content;
};

int parse_sts_request(struct sts_request *request, const uint8_t *raw, size_t length);
