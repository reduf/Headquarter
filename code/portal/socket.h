#ifdef PORTAL_SOCKET_H
#error "socket.h included more than once"
#endif
#define PORTAL_SOCKET_H

int send_full(SOCKET fd, const uint8_t *buffer, size_t buffer_len);
int recv_to_buffer(SOCKET fd, array_uint8_t *buffer);
