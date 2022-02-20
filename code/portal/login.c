#ifdef PORTAL_LOGIN_C
#error "login.c included more than once"
#endif
#define PORTAL_LOGIN_C

// For some reason the server has an offset on the epoch of 31 years.
// This value represent the exact offset
static const time_t EPOCH_OFFSET = 978307200;

static void appendv(array_uint8_t *buffer, const char *fmt, va_list args)
{
    int ret = vsnprintf(NULL, 0, fmt, args);
    if (ret < 0)
        abort();

    // We need to allocate one more bytes, because  of `vsnprintf`.
    // We will pop this "\0" byte later.
    uint8_t *write_ptr = array_push(*buffer, (size_t)ret + 1);
    vsnprintf((char *)write_ptr, (size_t)ret + 1, fmt, args);
    array_pop(*buffer);
}

static void appendf(array_uint8_t *buffer, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    appendv(buffer, fmt, args);
    va_end(args);
}

static uint64_t anet_epoch()
{
    time_t epoch = time(NULL);
    return (uint64_t)(epoch - EPOCH_OFFSET);
}

struct sockname {
    char name[MAX(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)];
};

typedef array(struct sockaddr) array_sockaddr_t;
static int resolve_dns(array_sockaddr_t *addresses, int af, const char *hostname, const char *port)
{
    int ret;
    struct addrinfo hints = {0};
    struct addrinfo *results = NULL;

    hints.ai_family = af;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if ((ret = getaddrinfo(hostname, port, &hints, &results)) != 0) {
        fprintf(stderr, "Failed to resolved host '%s'\n", hostname);
        return ret;
    }

    for (struct addrinfo *it = results; it != NULL; it = it->ai_next) {
        array_add(*addresses, *it->ai_addr);
    }

    freeaddrinfo(results);
    return 0;
}

static int resolve_dns4(array_sockaddr_t *addresses, const char *hostname, const char *port)
{
    return resolve_dns(addresses, AF_INET, hostname, port);
}

static struct sockname get_local_addr(SOCKET fd)
{
    struct sockname name;
    name.name[0] = 0;

    struct sockaddr_storage sa;
    socklen_t len = sizeof(sa);
    int ret = getsockname(fd, (struct sockaddr *)&sa, &len);
    if (ret != 0) {
        fprintf(stderr, "'getsockname' failed\n");
        return name;
    }

    void *src = NULL;
    switch (sa.ss_family)
    {
        case AF_INET:
            src = &((struct sockaddr_in *)&sa)->sin_addr;
            break;
        case AF_INET6:
            src = &((struct sockaddr_in6 *)&sa)->sin6_addr;
            break;
        default:
            return name;
    }

    if (inet_ntop(sa.ss_family, src, name.name, sizeof(name.name)) == NULL) {
        fprintf(stdout, "Couldn't parse ip address\n");
        return name;
    }

    return name;
}

bool portal_login(const char *username, const char *password)
{
    int ret;

    array_sockaddr_t addresses = {0};
    if ((ret = resolve_dns4(&addresses, "cligate.1.ncplatform.net.", "6112")) != 0) {
        array_reset(addresses);
        return false;
    }

    struct sockaddr *it;
    array_foreach(it, addresses) {
        if (it->sa_family == AF_INET) {
            break;
        }
    }

    if (it == array_end(addresses)) {
        array_reset(addresses);
        fprintf(stderr, "Resolved DNS, but couldn't get any addresses");
        return false;
    }

    struct sockaddr target;
    memcpy(&target, it, sizeof(*it));
    array_reset(addresses);

    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == INVALID_SOCKET) {
        fprintf(stderr, "'socket' failed\n");
        return false;
    }

    if ((ret = connect(fd, &target, sizeof(target))) != 0) {
        fprintf(stderr, "'connect' failed %d\n", ret);
        closesocket(fd);
        return false;
    }

    array_uint8_t content;
    array_init(content, 1024);
    appendf(&content, "<Connect>\n");
    appendf(&content, "<ConnType>400</ConnType>\n");
    appendf(&content, "<Address>%s</Address>\n", get_local_addr(fd).name);
    appendf(&content, "<ProductType>0</ProductType>\n");
    appendf(&content, "<ProductName>Gw</ProductName>\n");
    appendf(&content, "<AppIndex>1</AppIndex>\n");
    appendf(&content, "<Epoch>%" PRIu64 "</Epoch>\n", anet_epoch());
    appendf(&content, "<Program>1</Program>\n");
    appendf(&content, "<Build>1002</Build>\n");
    appendf(&content, "<Process>%d</Process>\n", getpid());
    appendf(&content, "</Connect>\n");

    array_uint8_t request;
    array_init(request, 1024);

    const char connect_url[] = "/Sts/Connect";
    sts_write_request(&request, connect_url, ARRAY_SIZE(connect_url) - 1,
        request.data, request.size);
    array_reset(content);

    ret = send(fd, request.data, request.size, 0);
    array_clear(request);

    const char start_tls_url[] = "/Auth/StartTls";
    sts_write_sequenced_request(&request, 1, 4000,
        start_tls_url, ARRAY_SIZE(start_tls_url) - 1, "", 0);

    ret = send(fd, request.data, request.size, 0);

    return true;
}
