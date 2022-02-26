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
    (void)password;
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
        content.data, content.size);
    array_reset(content);

    ret = send(fd, (const char *)request.data, request.size, 0);

    if ((ret < 0) || ((size_t)ret != request.size)) {
        fprintf(stderr, "Failed to send initial packet\n");
        array_reset(request);
        closesocket(fd);
        return false;
    }

    const char start_tls_url[] = "/Auth/StartTls";
    const uint8_t empty[] = "";

    array_clear(request);
    sts_write_sequenced_request(&request, 1, 4000,
        start_tls_url, ARRAY_SIZE(start_tls_url) - 1, empty, 0);
    ret = send(fd, (const char *)request.data, request.size, 0);

    if ((ret < 0) || ((size_t)ret != request.size)) {
        array_reset(request);
        closesocket(fd);
        return false;
    }

    array_reset(request);

    uint8_t buffer[1024];
    size_t length = 0;
    struct sts_request sts_request = {0};

    for (;;)
    {
        if (length == sizeof(buffer))
            break;

        uint8_t *p = buffer + length;
        ret = recv(fd, (char *)p, sizeof(buffer) - length, 0);

        if (ret <= 0) {
            closesocket(fd);
            return false;
        }

        length += (size_t)ret;

        ret = parse_sts_request(&sts_request, buffer, length);

        if (ret != STSE_INCOMPLETE_CONTENT && ret != STSE_INCOMPLETE_HEADER)
            break;
    }

    if (ret != STSE_SUCCESS) {
        closesocket(fd);
        return false;
    }

    if (sts_request.status_code != 400) {
        fprintf(stderr, "Couldn't start a STS connection, status: %d\n", sts_request.status_code);
        closesocket(fd);
        return false;
    }

    struct ssl_tls12_context ctx;
    ssl_tls2_init(&ctx);
    ctx.random_time = (uint32_t)anet_epoch();
    memset(ctx.random_bytes, 0xCC, sizeof(ctx.random_bytes));
    size_t username_len = strlen(username);
    if (sizeof(ctx.srp_username) <= username_len + 1) {
        // @Cleanup: Free stuff?
        return false;
    }
    strcpy(ctx.srp_username, username);
    ctx.srp_username_len = strlen(ctx.srp_username);

    if (ssl_tls12_write_client_hello(&ctx) != 0) {
        closesocket(fd);
        ssl_tls2_free(&ctx);
        return false;
    }

    array_uint8_t packet;
    array_init(packet, 1024);
    ssl_tls12_write_auth_packet(&packet, ctx.buffer.data, ctx.buffer.size);

    ret = send(fd, (const char *)packet.data, packet.size, 0);
    if ((ret < 0) || ((size_t)ret != packet.size)) {
        fprintf(stderr, "Failed to send all the data\n");
        array_reset(packet);
        closesocket(fd);
        return false;
    }

    array_reset(packet);
    ret = recv(fd, (char *)buffer, sizeof(buffer), 0);

    closesocket(fd);
    return true;
}
