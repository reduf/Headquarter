#ifdef PORTAL_SOCKET_H
#error "socket.h included more than once"
#endif
#define PORTAL_SOCKET_H

typedef array(struct sockaddr) array_sockaddr_t;

int send_full(SOCKET fd, const uint8_t *buffer, size_t buffer_len);
int recv_to_buffer(SOCKET fd, array_uint8_t *buffer);

int resolve_dns(array_sockaddr_t *addresses, int af, const char *hostname, uint16_t port);
int resolve_dns4(array_sockaddr_t *addresses, const char *hostname, uint16_t port);

int get_sockname(SOCKET fd, char *buffer, size_t buffer_len);
