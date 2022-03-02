#ifdef PORTAL_SOCKET_C
#error "socket.c included more than once"
#endif
#define PORTAL_SOCKET_C

int send_full(SOCKET fd, const uint8_t *buffer, size_t buffer_len)
{
    int ret;
    size_t written = 0;

    while (written < buffer_len) {
        const char *p = (const char *)buffer + written;
        if ((ret = send(fd, p, buffer_len - written, 0)) <= 0)
            return ret;
        written += (size_t)ret;
    }

    return 0;
}

int recv_to_buffer(SOCKET fd, array_uint8_t *buffer)
{
    int ret;

    const size_t BUFFER_SIZE = 1024;
    uint8_t *ptr = array_push(*buffer, BUFFER_SIZE);

    if ((ret = recv(fd, (char *)ptr, BUFFER_SIZE, 0)) <= 0) {
        size_t size = array_size(*buffer) - BUFFER_SIZE;
        array_resize(*buffer, size);
        return ret;
    }

    size_t size = array_size(*buffer) - (BUFFER_SIZE - (size_t)ret);
    array_resize(*buffer, size);
    return 0;
}
