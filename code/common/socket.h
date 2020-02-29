#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H

struct socket {
#ifdef _WIN32
    uintptr_t handle;
#else // _WIN32
    int handle;
#endif
};

#ifdef _WIN32
# define SHUT_RD     (0)
# define SHUT_WR     (1)
# define SHUT_RDWR   (2)
#else
# define closesocket close
# define SOCKET_ERROR   (-1)
# define INVALID_SOCKET (-1)
#endif

#endif // COMMON_SOCKET_H
