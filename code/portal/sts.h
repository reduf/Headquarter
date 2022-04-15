#ifdef PORTAL_STS_H
#error "sts.h included more than once"
#endif
#define PORTAL_STS_H

#define STSE_SUCCESS                 0
#define STSE_UNSUCCESSFUL            1
#define STSE_INCOMPLETE_CONTENT      2
#define STSE_INCOMPLETE_HEADER       3
#define STSE_UNSUPPORTED_HEADER      4
#define STSE_UNSUPPORTED_PROTOCOL    5

struct sts_connection {
    SOCKET fd;
    size_t seq_number;
    struct uuid user_id;
    struct uuid token;
};

void sts_connection_init(struct sts_connection *sts);
void sts_connection_free(struct sts_connection *sts);

int sts_connection_connect(struct sts_connection *sts, const char *hostname);
int sts_connection_start_tls(struct sts_connection *sts, struct ssl_sts_connection *ssl);

struct sts_reply {
    unsigned status_code;
    unsigned sequence_number;
    unsigned content_length;
    const uint8_t *content;
};

int sts_parse_reply(struct sts_reply *reply, const uint8_t *raw, size_t length);

int sts_write_request(
    array_uint8_t *request,
    const char *url, size_t url_len,
    const uint8_t *content, size_t content_len);

int sts_write_request_with_sequence_number(
    array_uint8_t *request,
    const char *url, size_t url_len,
    size_t seq_number,
    uint32_t timeout_ms,
    const uint8_t *content, size_t content_len);
