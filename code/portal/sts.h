#ifdef PORTAL_STS_H
#error "sts.h included more than once"
#endif
#define PORTAL_STS_H

struct sts_connection {
    SOCKET           fd;
    array_sockaddr_t addresses;
    char             sockname[MAX(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)];
};

void sts_connection_init(struct sts_connection *sts);
void sts_connection_free(struct sts_connection *sts);

int sts_connection_connect(struct sts_connection *sts, const char *hostname);
int sts_connection_start_tls(struct sts_connection *sts);

#define STSE_SUCCESS                 0
#define STSE_INCOMPLETE_CONTENT      1
#define STSE_INCOMPLETE_HEADER       2
#define STSE_UNSUPPORTED_HEADER      3
#define STSE_UNSUPPORTED_PROTOCOL    4

struct sts_header {
    unsigned status_code;
    unsigned sequence_number;
    unsigned content_length;
    const uint8_t *content;
};

int parse_sts_header(struct sts_header *request, const uint8_t *raw, size_t length);
