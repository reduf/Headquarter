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

int resolve_dns(array_sockaddr_t *addresses, int af, const char *hostname, uint16_t port)
{
    int ret;
    struct addrinfo hints = {0};
    struct addrinfo *results = NULL;

    char port_as_str[sizeof("65535")];
    snprintf(port_as_str, sizeof(port_as_str), "%" PRIu16, port);

    hints.ai_family = af;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if ((ret = getaddrinfo(hostname, port_as_str, &hints, &results)) != 0) {
        fprintf(stderr, "Failed to resolved host '%s'\n", hostname);
        return 1;
    }

    for (struct addrinfo *it = results; it != NULL; it = it->ai_next) {
        array_add(*addresses, *it->ai_addr);
    }

    freeaddrinfo(results);
    return 0;
}

int resolve_dns4(array_sockaddr_t *addresses, const char *hostname, uint16_t port)
{
    return resolve_dns(addresses, AF_INET, hostname, port);
}
