#ifdef CORE_NETWORK_C
#error "network.c included more than once"
#endif
#define CORE_NETWORK_C

static bool NetIsInitialized;

typedef struct DiffieHellmanCtx {
    mbedtls_mpi prime_modulus;
    mbedtls_mpi server_public;
    mbedtls_mpi primitive_root;
} DiffieHellmanCtx;

/*
 * To avoid strange bug, the string coming from the server a interpreted as uint16_t
 * and as soon as we exist packet handlers we should use utf-8 in struct `string`.
 *
 * We provide the function to compute len here.
 */
static size_t unicode16_len(const uint16_t *s, size_t n);
static size_t unicode16_cpy(uint16_t *d, const uint16_t *s, size_t n);

void NetConn_Reset(Connection *conn)
{
    if ((conn->fd.handle != 0) && (conn->fd.handle != INVALID_SOCKET)) {
        closesocket(conn->fd.handle);
        conn->fd.handle = INVALID_SOCKET;
    }

    thread_mutex_destroy(&conn->mutex);

    array_reset(&conn->in);
    array_reset(&conn->out);

    // array_reset(&conn->srv_filter);
    // array_reset(&conn->clt_filter);

    array_reset(&conn->handlers);
}

void NetConn_HardShutdown(Connection *conn)
{
    NetConn_Shutdown(conn);
    shutdown(conn->fd.handle, SHUT_RDWR);
}

void NetConn_Shutdown(Connection *conn)
{
    conn->flags |= NETCONN_SHUTDOWN;
}

bool NetConn_IsShutdown(Connection *conn)
{
    if ((conn->fd.handle == 0) || (conn->fd.handle == INVALID_SOCKET)) {
        return true;
    } else {
        return false;
    }
}

void init_connection(Connection *conn, void *data)
{
    assert(conn != NULL);
    memzero(conn, sizeof(*conn));

    conn->data = data;

    thread_mutex_init(&conn->mutex);

    array_init(&conn->in, 5840);
    array_init(&conn->out, 5840);
}

#pragma pack(push, 1)
typedef struct {
    uint8_t source;     // 0
    uint8_t length;     // 66
    uint8_t seed[64];
} MSG_CLIENT_SEED;

typedef struct {
    uint8_t source;     // 1
    uint8_t length;     // 22
    uint8_t seed[20];
} MSG_SERVER_SEED;

typedef struct {
    uint8_t  source;    // 0
    uint8_t  length;    // 4
    uint16_t size;      // 12
    uint32_t version;
    uint32_t unk0;      // 1
    uint32_t unk1;      // 4
} AUTH_CMSG_VERSION;

typedef struct {
    uint8_t  source;    // 0
    uint8_t  length;    // 5
    uint16_t unk0;      // 12
    uint32_t version;
    uint32_t unk1;      // 1
    uint32_t world_hash;
    uint32_t map_id;
    uint32_t player_hash;
    uuid_t account_uuid;
    uuid_t character_uuid;
    uint32_t unk2;      // 0
    uint32_t unk3;      // 0
} GAME_CMSG_VERSION;

typedef struct {
    uint8_t  source;    // 0
    uint8_t  length;
    uint16_t size;
    uint32_t version;
} OBSV_CMSG_VERSION;
#pragma pack(pop)

typedef union PacketBuffer {
    uint8_t bytes[4096];
    Packet  packet;
} PacketBuffer;

static bool socket_would_block(int err);
static bool key_exchange_helper(Connection *conn, DiffieHellmanCtx *dhm);
static void arc4_hash(const uint8_t *key, uint8_t *digest);
static bool read_dhm_key_file(DiffieHellmanCtx *dhm, const char *path);

static size_t get_static_size(MsgField *field);
static size_t get_element_size(MsgField *field);

static size_t get_prefix_size(Type type);
static int unpack(const uint8_t *data, size_t data_size,
    uint8_t *buffer, size_t buff_size, MsgField *fields, size_t fields_count);
static int pack(const uint8_t *data, size_t data_size,
    uint8_t *buffer, size_t buff_size, MsgField *fields, size_t fields_count);

static mbedtls_entropy_context entropy;
static mbedtls_ctr_drbg_context ctr_drbg;
static DiffieHellmanCtx official_server_keys;
static DiffieHellmanCtx custom_server_keys;

static SockAddressArray AuthSrv_IPs;

bool Net_Initialized = false;

static void DiffieHellmanCtx_Reset(DiffieHellmanCtx *dhm)
{
    mbedtls_mpi_free(&dhm->prime_modulus);
    mbedtls_mpi_free(&dhm->server_public);
    mbedtls_mpi_free(&dhm->primitive_root);
}

void Network_Init(void)
{
    if (Net_Initialized)
        return;
    
#ifdef _WIN32
    WSADATA wsaData = {0};
    int wsa_error = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsa_error != NO_ERROR) {
        printf("WSAError %d", wsa_error);
        return;
    }
#endif

    char file_path[1048];
    int length = 0;
    bool file_read_ok = false;
    char dir_path[1024];
    length = dlldir(dir_path, sizeof(dir_path));
    for (int i = 0; i < 8 && !file_read_ok; i++) {
        snprintf(file_path, sizeof(file_path), "%s/data/gw_%d.pub", dir_path, GUILD_WARS_VERSION);
        dir_path[length++] = '/';
        dir_path[length++] = '.';
        dir_path[length++] = '.';
        dir_path[length] = 0;
        file_read_ok = read_dhm_key_file(&official_server_keys, file_path);
    }
    assert(file_read_ok);
    LogInfo("gw key found @ %s", file_path);

    file_read_ok = false;
    length = dlldir(dir_path, sizeof(dir_path));
    dir_path[length] = 0;
    for (int i = 0; i < 8 && !file_read_ok; i++) {
        snprintf(file_path, sizeof(file_path), "%s/data/authkey.pub",dir_path);
        dir_path[length++] = '/';
        dir_path[length++] = '.';
        dir_path[length++] = '.';
        dir_path[length] = 0;
        file_read_ok = read_dhm_key_file(&custom_server_keys, file_path);
    }
    assert(file_read_ok);
    LogInfo("authkey.pub key found @ %s", file_path);

    const char secret[] = "Stradivarius";
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
        cast(uint8_t *)secret, sizeof(secret) - 1);

    AuthSrv_IPs = IPv4ToAddrEx("Auth1.ArenaNetworks.com", "6112");
    Net_Initialized = true;
}

void Network_Shutdown(void)
{
    if (!Net_Initialized)
        return;

    // Free stuff, see Init.
    DiffieHellmanCtx_Reset(&official_server_keys);
    DiffieHellmanCtx_Reset(&custom_server_keys);

    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    array_reset(&AuthSrv_IPs);

#ifdef _WIN32
    WSACleanup();
#endif
    Net_Initialized = false;
}

SockAddressArray IPv4ToAddrEx(const char *host, const char *port)
{
    SockAddressArray ret = {0};
    struct addrinfo  hints = {0};
    struct addrinfo *results = NULL;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(host, port, &hints, &results)) {
        LogError("'getaddrinfo' failed. (%d)", os_errno);
        return ret;
    }

    size_t found = 1;
    struct addrinfo *it = results;
    while ((it = it->ai_next) != NULL)
        found++;

    array_init(&ret, found);

    it = results;
    for (size_t i = 0; i < found; i++) {
        array_add(&ret, *it->ai_addr);
        it = it->ai_next;
    }

    freeaddrinfo(results);
    return ret;
}

bool IPv4ToAddr(const char *host, const char *port, struct sockaddr *sockaddr)
{
    struct addrinfo  hints = {0};
    struct addrinfo *results = NULL;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(host, port, &hints, &results)) {
        LogError("'getaddrinfo' failed. (%d)", os_errno);
        return false;
    }

    *sockaddr = *results[0].ai_addr;
    freeaddrinfo(results);
    return true;
}

static bool read_dhm_key_file(DiffieHellmanCtx *dhm, const char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        LogError("Couldn't open the key file '%s'", path);
        return false;
    }

    uint8_t prim_root[4];
    uint8_t public_key[64];
    uint8_t prime_mod[64];

    if (fread(prim_root, sizeof(prim_root), 1, file) != 1) {
        LogError("Couldn't read the primitive root. (file: %s)", path);
        fclose(file);
        return false;
    }

    if (fread(prime_mod, sizeof(prime_mod), 1, file) != 1) {
        LogError("Couldn't read the prime modulus. (file: %s)", path);
        fclose(file);
        return false;
    }

    if (fread(public_key, sizeof(public_key), 1, file) != 1) {
        LogError("Couldn't read the public key. (file: %s)", path);
        fclose(file);
        return false;
    }

    fclose(file);
    mbedtls_mpi_read_binary(&dhm->prime_modulus, prime_mod, 64);
    mbedtls_mpi_read_binary(&dhm->server_public, public_key, 64);
    mbedtls_mpi_read_binary(&dhm->primitive_root, prim_root, 4);
    return true;
}

static bool socket_would_block(int err)
{
#ifdef _WIN32
    return err == WSAEWOULDBLOCK;
#else
# ifdef EWOULDBLOCK
    return err == EWOULDBLOCK;
# else
    return err == EAGAIN;
# endif
#endif
}

static bool socket_set_nonblock(struct socket *sock)
{
#if _WIN32
    u_long nonblock = 1;
    int iresult = ioctlsocket(sock->handle, FIONBIO, &nonblock);
    if (iresult == SOCKET_ERROR) {
        LogError("ioctlsocket failed: %d", os_errno);
        return false;
    }
    return true;
#else
    int flags = fcntl(sock->handle, F_GETFL, 0);
    if (flags == -1)
        return false;
    flags = flags | O_NONBLOCK;
    return (fcntl(sock->handle, F_SETFL, flags) == 0) ? true : false;
#endif
}

static bool key_exchange_helper(Connection *conn, DiffieHellmanCtx *dhm)
{
    // Compute g^x (mod p)
    // Send:
    //  MSG_CLIENT_SEED
    // Recv:
    //  MSG_SERVER_SEED
    // Initialize arc4 cipher.
    MSG_CLIENT_SEED client_seed;
    MSG_SERVER_SEED server_seed;

    mbedtls_mpi private_key = {0};
    mbedtls_mpi public_key = {0};
    mbedtls_mpi shared_secret = {0};

    int result;
    bool success = true;

    mbedtls_mpi_fill_random(&private_key, 64, mbedtls_ctr_drbg_random, &ctr_drbg);
    mbedtls_mpi_exp_mod(&public_key, &dhm->primitive_root, &private_key, &dhm->prime_modulus, NULL);
    mbedtls_mpi_exp_mod(&shared_secret, &dhm->server_public,  &private_key, &dhm->prime_modulus, NULL);

    if ((shared_secret.private_n == 0) || (public_key.private_n == 0)) {
        LogError("Diffie-Hellman computation failed.");
        success = false;
        goto quick_exist;
    }

    client_seed.source = 0;
    client_seed.length = 66;
    memcpy(client_seed.seed, public_key.private_p, 64);

    result = send(conn->fd.handle, cast(char *)&client_seed, sizeof(client_seed), 0);
    if (result != sizeof(client_seed)) {
        LogError("'send' failed. (%d)", os_errno);
        success = false;
        goto quick_exist;
    }

    // @Remark, We can fail here if 'GUILD_WARS_VERSION' isn't updated.
    result = recv(conn->fd.handle, cast(char *)&server_seed, sizeof(server_seed), 0);
    if (result != sizeof(server_seed)) {
        LogError("MSG_SERVER_SEED size missmatch. Expected %u bytes, but received %u bytes.", sizeof(MSG_SERVER_SEED), result);
        success = false;
        goto quick_exist;
    }

    uint8_t arc4_key[20];
    uint8_t *shared_bytes = cast(uint8_t *)shared_secret.private_p;
    for (int i = 0; i < 20; i++)
        server_seed.seed[i] ^= shared_bytes[i];
    arc4_hash(server_seed.seed, arc4_key);

    mbedtls_arc4_setup(&conn->encrypt, arc4_key, 20);
    mbedtls_arc4_setup(&conn->decrypt, arc4_key, 20);
    conn->secured = true;

quick_exist:
    mbedtls_mpi_free(&private_key);
    mbedtls_mpi_free(&public_key);
    mbedtls_mpi_free(&shared_secret);
    return success;
}

static struct socket create_socket(void)
{
    struct socket sock;
    sock.handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock.handle == INVALID_SOCKET) {
        LogError("Couldn't create socket");
        return sock;
    }

    int no_delay = 1;
    setsockopt(sock.handle, IPPROTO_TCP, TCP_NODELAY, cast(char *)&no_delay, sizeof(no_delay));
    return sock;
}

bool AuthSrv_Connect(Connection *conn)
{
    assert(Net_Initialized);
    int result;

    conn->name = "AuthSrv";
    conn->proto = ConnectionType_Auth;

    if (conn->host.sa_family == 0) {
        // This could happend if we don't have an internet connection
        if (array_size(&AuthSrv_IPs) == 0) {
            NetConn_Reset(conn);
            return false;
        }
        conn->host = AuthSrv_IPs.data[0];
    }

    conn->fd = create_socket();
    conn->t0 = time_get_ms();
    conn->secured = false;

    if (conn->fd.handle == INVALID_SOCKET) {
        LogError("'socket' failed. (%d)", os_errno);
        NetConn_Reset(conn);
        return false;
    }

    result = connect(conn->fd.handle, cast(struct sockaddr *)&conn->host, sizeof(conn->host));
    if (result == SOCKET_ERROR) {
        LogError("'connect' failed. (%d)", os_errno);
        NetConn_Reset(conn);
        return false;
    }

    conn->server_msg_format.size = AUTH_SMSG_COUNT;
    conn->server_msg_format.data = AUTH_SERVER_FORMATS;
    conn->client_msg_format.size = AUTH_CMSG_COUNT;
    conn->client_msg_format.data = AUTH_CLIENT_FORMATS;

    AUTH_CMSG_VERSION version;
    version.source = 0;
    version.length = 4;
    version.size = 12;
    version.version = GUILD_WARS_VERSION;
    version.unk0 = 1;
    version.unk1 = 4;

    result = send(conn->fd.handle, cast(char *)&version, sizeof(version), 0);
    if (result == SOCKET_ERROR) {
        LogError("'send' failed. (%d)", os_errno);
        NetConn_Reset(conn);
        return false;
    }

    if (!key_exchange_helper(conn, &official_server_keys)) {
        NetConn_Reset(conn);
        return false;        
    }

    socket_set_nonblock(&conn->fd);
    LogInfo("Auth Handshake successful!");
    return true;
}

bool GameSrv_Connect(Connection *conn, uuid_t account,
    uuid_t character, uint32_t world_hash, uint32_t player_hash, uint32_t map)
{
    assert(Net_Initialized);
    int result;

    conn->name = "GameSrv";
    conn->proto = ConnectionType_Game;

    conn->fd = create_socket();
    conn->t0 = time_get_ms();
    conn->secured = false;

    if (conn->fd.handle == INVALID_SOCKET) {
        LogError("'socket' failed. (%d)", os_errno);
        NetConn_Reset(conn);
        return false;
    }

    result = connect(conn->fd.handle, cast(struct sockaddr *)&conn->host, sizeof(conn->host));
    if (result == SOCKET_ERROR) {
        LogError("'connect' failed. (%d)", os_errno);
        NetConn_Reset(conn);
        return false;
    }

    conn->server_msg_format.size = GAME_SMSG_COUNT;
    conn->server_msg_format.data = GAME_SERVER_FORMATS;
    conn->client_msg_format.size = GAME_CMSG_COUNT;
    conn->client_msg_format.data = GAME_CLIENT_FORMATS;

    GAME_CMSG_VERSION version;
    version.source = 0;
    version.length = 5;
    version.version = GUILD_WARS_VERSION;
    version.world_hash = world_hash;
    version.map_id = map;
    version.player_hash = player_hash;
    version.unk0 = 12;
    version.unk1 = 1;
    version.unk2 = 0;
    version.unk3 = 0;

    uuid_enc_le(version.account_uuid, account);
    uuid_enc_le(version.character_uuid, character);

    result = send(conn->fd.handle, cast(char *)&version, sizeof(version), 0);
    if (result == SOCKET_ERROR) {
        LogError("'send' failed. (%d)", os_errno);
        NetConn_Reset(conn);
        return false;
    }

    if (!key_exchange_helper(conn, &official_server_keys)) {
        NetConn_Reset(conn);
        return false;
    }

    socket_set_nonblock(&conn->fd);
    LogInfo("Game Handshake successful!");
    return true;
}

static void arc4_hash(const uint8_t *key, uint8_t *digest)
{
    typedef union U20 {
        uint8_t  u8[20];
        uint32_t u32[5];
    } U20;

    uint32_t A = 0x67452301;
    uint32_t B = 0xEFCDAB89;
    uint32_t C = 0x98BADCFE;
    uint32_t D = 0x10325476;
    uint32_t E = 0xC3D2E1F0;
    uint32_t F;

    U20 *output = cast(U20 *)digest;
    const U20 *input = cast(const U20 *)key;

    F = (D ^ (B & (C ^ D)));
    E += input->u32[0] + ROL32(A, 5) + F + 0x5A827999;
    B = ROL32(B, 30);

    F = (C ^ (A & (B ^ C)));
    D += input->u32[1] + ROL32(E, 5) + F + 0x5A827999;
    A = ROL32(A, 30);

    F = (B ^ (E & (A ^ B)));
    C += input->u32[2] + ROL32(D, 5) + F + 0x5A827999;
    E = ROL32(E, 30);

    F = (B ^ C ^ D);
    E += input->u32[3] + ROL32(A, 5) + F + 0x6ED9EBA1;
    B = ROL32(B, 30);

    F = (A ^ B ^ C);
    D += input->u32[4] + ROL32(E, 5) + F + 0x6ED9EBA1;
    A = ROL32(A, 30);

    output->u32[0] = input->u32[0] + A;
    output->u32[1] = input->u32[1] + B;
    output->u32[2] = input->u32[2] + C;
    output->u32[3] = input->u32[3] + D;
    output->u32[4] = input->u32[4] + E;
}

size_t NetMsg_Pack(Packet *packet, size_t pack_size,
    uint8_t *buffer, size_t buff_size, MsgFormat *format)
{
    int retval = pack(cast(uint8_t *)packet, pack_size, buffer, buff_size, format->fields, format->count);
    if (retval < 0)
        return 0;
    return (size_t)retval;
}

size_t NetMsg_Unpack(const uint8_t *data, size_t data_size,
    Packet *packet, size_t pack_size, MsgFormat *format)
{
    int retval = unpack(data, data_size, cast(uint8_t *)packet, pack_size, format->fields, format->count);
    if (retval < 0)
        return 0;
    return (size_t)retval;
}

void SendPacket(Connection *conn, size_t size, void *p)
{
    thread_mutex_lock(&conn->mutex);

    if (conn->flags & NETCONN_REMOVE)
        goto leave;

    // @Robustness: This is undefined behaviors ! (Well not int practice)
    Packet *packet = cast(Packet *)p;
    Header header = packet->header;

    assert(array_inside(&conn->client_msg_format, header));

    MsgFormat format = conn->client_msg_format.data[header];
    assert(header == format.header);
    assert(size == format.unpack_size);

    ByteBuffer *out = &conn->out;
    size_t availaible_length = out->capacity - out->size;
    if (availaible_length < sizeof(Header)) {
        LogError("Send buffer not big enough, flush socket stream", header);
        NetConn_Send(conn);
        availaible_length = out->capacity - out->size;
    }
    
    assert(sizeof(Header) <= availaible_length);
    uint8_t *buffer = out->data + out->size;
    size_t written = NetMsg_Pack(packet, size, buffer, availaible_length, &format);
    if (written == 0) {
        LogError("NetMsg_Pack failed for message %u", header);
        goto leave;
    }

    out->size += written;
leave:
    thread_mutex_unlock(&conn->mutex);
}

void NetConn_Send(Connection *conn)
{
    assert(conn && conn->secured);

    thread_mutex_lock(&conn->mutex);
    if (conn->flags & NETCONN_REMOVE) {
        thread_mutex_unlock(&conn->mutex);
        return;
    }

    ByteBuffer *out = &conn->out;
    size_t size = out->size;
    uint8_t *buff = out->data;
    if (size == 0) {
        thread_mutex_unlock(&conn->mutex);
        return;
    }

    mbedtls_arc4_crypt(&conn->encrypt, size, buff, buff);

    int result = send(conn->fd.handle, cast(const char *)out->data, out->size, 0);
    if (result == SOCKET_ERROR) {
        LogError("send failed: %d", os_errno);
        NetConn_HardShutdown(conn);
        goto leave;
    }

    size_t new_size = out->size - (size_t)result;
    if (new_size)
        memmove(out->data, out->data + (size_t)result, new_size);
    out->size = new_size;

leave:
    thread_mutex_unlock(&conn->mutex);
}

void NetConn_Recv(Connection *conn)
{
    assert(conn && conn->secured);

    uint8_t buffer[5840];
    size_t size = conn->in.capacity - conn->in.size;
    int iresult = recv(conn->fd.handle, cast(char *)buffer, size, 0);

    int err = os_errno;
    if (iresult == SOCKET_ERROR) {
        if (!socket_would_block(err)) {
            LogError("WSARecv failed. (%d)", err);
            NetConn_HardShutdown(conn);
            client->ingame = false;
        }
        thread_mutex_unlock(&conn->mutex);
        return;
    }

    uint8_t *dest = conn->in.data + conn->in.size;
    mbedtls_arc4_crypt(&conn->decrypt, cast(size_t)iresult, buffer, dest);
    conn->in.size += cast(size_t)iresult;

    NetConn_DispatchPackets(conn);
}

void NetConn_DispatchPackets(Connection *conn)
{
    PacketBuffer buffer;

    uint8_t *data = conn->in.data;
    size_t   size = conn->in.size;

    while (size >= sizeof(Header)) {
        Header header = le16dec(data);
        assert(array_inside(&conn->server_msg_format, header));

        MsgFormat format = array_at(&conn->server_msg_format, header);
        assert(format.header == header);

        memzero(buffer.bytes, format.unpack_size);
        size_t readed = NetMsg_Unpack(data, size,
            &buffer.packet, format.unpack_size, &format);

        if (readed == 0)
            break;

        assert(size >= readed);
        data += readed;
        size -= readed;

        if (!array_inside(&conn->handlers, header)) {
            LogError("Received a unvalid header '%u' max is '%zu'", header, conn->handlers.size);
            NetConn_HardShutdown(conn);
            return;
        }

        MsgHandler handler = array_at(&conn->handlers, header);
        if (handler)
            handler(conn, format.unpack_size, &buffer.packet);

        // @Remark: We don't want to dispatch packet if we close the connection. but
        // it might make us loose some packet.
        if (conn->flags & NETCONN_REMOVE) {
            array_clear(&conn->in);
            return;
        }
    }

    if (size > 0) memmove(conn->in.data, data, size);
    conn->in.size = size;
}

void NetConn_Update(Connection *conn)
{
    if ((conn->fd.handle == 0) || (conn->fd.handle == INVALID_SOCKET))
        return;

    NetConn_Send(conn);
    NetConn_Recv(conn);

    if (conn->flags & NETCONN_SHUTDOWN) {
        LogDebug("Shutdown connection '%s'", conn->name);

        closesocket(conn->fd.handle);
        conn->fd.handle = INVALID_SOCKET;

        conn->flags = 0;
        conn->ping = 0;
        conn->pong = 0;
        conn->latency = 0;
        conn->last_tick_time = 0;

        array_clear(&conn->in);
        array_clear(&conn->out);
        conn->secured = false;
    }
}

// @Cleanup:
// This is currently unused and the size for string isn't sync with the packets format dump.
static size_t get_static_size(MsgField *field)
{
    int param = field->param;
    switch (field->type) {
        case TYPE_MSG_HEADER:   return sizeof(Header);
        case TYPE_AGENT_ID:     return sizeof(AgentId);
        case TYPE_FLOAT:        return sizeof(float);
        case TYPE_VECT2:        return sizeof(Vec2f);
        case TYPE_VECT3:        return sizeof(Vec3f);
        case TYPE_BYTE:         return sizeof(int8_t);
        case TYPE_WORD:         return sizeof(int16_t);
        case TYPE_DWORD:        return sizeof(int32_t);
        case TYPE_BLOB:         return param;

        case TYPE_STRING_16:    return (sizeof(int16_t) * param) + sizeof(size_t);
        case TYPE_ARRAY_8:      return (sizeof(int8_t)  * param) + sizeof(size_t);
        case TYPE_ARRAY_16:     return (sizeof(int16_t) * param) + sizeof(size_t);
        case TYPE_ARRAY_32:     return (sizeof(int32_t) * param) + sizeof(size_t);

        case TYPE_NESTED_STRUCT:
            break;
    }
    return 0;
}

static size_t get_element_size(MsgField *field)
{
    switch (field->type) {
        case TYPE_MSG_HEADER:   return sizeof(Header);
        case TYPE_AGENT_ID:     return sizeof(AgentId);
        case TYPE_FLOAT:        return sizeof(float);
        case TYPE_VECT2:        return sizeof(Vec2f);
        case TYPE_VECT3:        return sizeof(Vec3f);
        case TYPE_BYTE:         return sizeof(int8_t);
        case TYPE_WORD:         return sizeof(int16_t);
        case TYPE_DWORD:        return sizeof(int32_t);
        case TYPE_BLOB:         return field->param;

        case TYPE_STRING_16:    return sizeof(uint16_t);
        case TYPE_ARRAY_8:      return sizeof(int8_t);
        case TYPE_ARRAY_16:     return sizeof(int16_t);
        case TYPE_ARRAY_32:     return sizeof(int32_t);

        case TYPE_NESTED_STRUCT:
            break;
    }
    return 0;
}

static size_t get_prefix_size(Type type)
{
    switch (type) {
        case TYPE_MSG_HEADER:
        case TYPE_AGENT_ID:
        case TYPE_FLOAT:
        case TYPE_VECT2:
        case TYPE_VECT3:
        case TYPE_BYTE:
        case TYPE_WORD:
        case TYPE_DWORD:
        case TYPE_BLOB:
            return 0;

        case TYPE_STRING_16:
        case TYPE_ARRAY_8:
        case TYPE_ARRAY_16:
        case TYPE_ARRAY_32:
            return 2;

        case TYPE_NESTED_STRUCT:
            return 1;
    }

    return 0;
}

static int pack(const uint8_t *data, size_t data_size,
    uint8_t *buffer, size_t buff_size, MsgField *fields, size_t fields_count)
{
#pragma pack(push, 1)
    struct array {
        uint32_t size;
    };
#pragma pack(pop)

    int readed = 0;
    size_t written = 0;

    for (size_t i = 0; i < fields_count; i++)
    {
        MsgField field = fields[i];

        const uint8_t *rpos = data + readed;
        uint8_t *wpos = buffer + written;

        size_t readable = data_size - readed;
        size_t writable = buff_size - written;

        size_t packed_elem_count = 1;
        size_t static_elem_count = 1;

        // @Robustness: Type aliasing
        struct array *a = cast(struct array *)rpos;
        uint16_t *s = cast(uint16_t *)rpos;

        size_t prefix_size = get_prefix_size(field.type);
        if (prefix_size) {
            if (writable < prefix_size)
                return -1;

            static_elem_count = field.param;
            if (field.type == TYPE_STRING_16) {
                packed_elem_count = unicode16_len(s, field.param);
            } else {
                packed_elem_count = a->size;
                readed += sizeof(struct array);
            }

            assert(packed_elem_count <= field.param);
            if (prefix_size == 1) {
                assert(packed_elem_count <= 255);
                *cast(uint8_t *)wpos = (uint8_t)packed_elem_count;
            } else if (prefix_size == 2) {
                assert(packed_elem_count <= 0xffff);
                le16enc(wpos, (uint16_t)packed_elem_count);
            }
            
            // readed += sizeof(array);
            written += prefix_size;

            rpos = data   + readed;
            wpos = buffer + written;

            readable = data_size - readed;
            writable = buff_size - written;
        }

        if (field.type != TYPE_NESTED_STRUCT) {
            size_t elem_size = get_element_size(&field);

            size_t bytes_to_write = elem_size * packed_elem_count;
            if (writable < bytes_to_write)
                return -1;

            #if _BYTE_ORDER == _LITTLE_ENDIAN
                memcpy(wpos, rpos, bytes_to_write);
            #else // @Cleanup: not tested
                memrcpy(wpos, rpos, bytes_to_write);
            #endif

            readed  += static_elem_count * elem_size;
            written += bytes_to_write;
        } else { // field.type == TYPE_NESTED_STRUCT
            assert(i + 1 < fields_count);

            MsgField *struct_fields = fields + i + 1;
            size_t struct_fields_count = fields_count - i - 1;

            for (size_t j = 0; j < packed_elem_count; j++) {
                const uint8_t *d = data + readed;
                int d_size = data_size - readed;

                uint8_t *b = buffer + written;
                int b_size = buff_size - written;

                int tmp = pack(d, d_size, b, b_size, struct_fields, struct_fields_count);
                if (tmp < 0) return -1;
                written += tmp;
                readed += field.size;
            }

            // readed += field.size * remaining_count;
            return written;
        }
    }

    return written;
}

static int unpack(const uint8_t *data, size_t data_size, uint8_t *buffer,
    size_t buff_size, MsgField *fields, size_t fields_count)
{
#pragma pack(push, 1)
    struct array {
        uint32_t size;
    };
#pragma pack(pop)

    int readed = 0;
    int written = 0;

    for (size_t i = 0; i < fields_count; i++) {
        MsgField field = fields[i];

        const uint8_t *rpos = data + readed;
        uint8_t *wpos = buffer + written;

        size_t readable = data_size - readed;
        size_t writable = buff_size - written;

        size_t packed_elem_count = 1;
        size_t static_elem_count = 1;

        // @Robustness: Type aliasing
        uint16_t *s = cast(uint16_t *)wpos;
        struct array *a = cast(struct array *)wpos;

        size_t prefix_size = get_prefix_size(field.type);
        if (prefix_size) {
            if (readable < prefix_size)
                return -1;

            // @Remark:
            // We could support more size, but it doesn't seem usefull since
            // we shouldn't send 65535 or more bytes.
            if (prefix_size == 1) {
                packed_elem_count = *cast(uint8_t *)rpos;
            } else if (prefix_size == 2) {
                packed_elem_count = le16dec(rpos);
            }

            assert(packed_elem_count <= field.param);

            static_elem_count = field.param;
            if (field.type == TYPE_STRING_16) {
                // We want to ensure last character is null.
                uint32_t end = MIN(field.param - 1, packed_elem_count);
                s[end] = 0;
            } else {
                written += sizeof(struct array);
                a->size = packed_elem_count;
            }
            
            readed += prefix_size;
            // written += sizeof(array);

            rpos = data   + readed;
            wpos = buffer + written;

            readable = data_size - readed;
            writable = buff_size - written;
        }

        if (field.type != TYPE_NESTED_STRUCT) {
            size_t elem_size = get_element_size(&field);

            size_t bytes_to_read = elem_size * packed_elem_count;
            if (readable < bytes_to_read)
                return -1;

            #if _BYTE_ORDER == _LITTLE_ENDIAN
                memcpy(wpos, rpos, bytes_to_read);
            #else // @Cleanup: not tested
                memrcpy(wpos, rpos, bytes_to_read);
            #endif

            readed  += bytes_to_read;
            written += static_elem_count * elem_size;
        } else { // field.type == TYPE_NESTED_STRUCT
            assert(i + 1 < fields_count);
            MsgField *next_fields = fields + i + 1;
            int next_fields_count = fields_count - i - 1;

            for (size_t j = 0; j < packed_elem_count; j++) {
                const uint8_t *d = data + readed;
                int d_size = data_size - readed;

                uint8_t *b = buffer + written;
                int b_size = buff_size - written;

                int tmp = unpack(d, d_size, b, b_size, next_fields, next_fields_count);
                if (tmp < 0) return -1;
                readed += tmp;
                written += tmp;// field.size;
            }

            return readed;
        }
    }

    assert(readed >= 0 && (size_t)readed <= data_size);
    return readed;
}

void Sha1(const void *data, size_t size, char digest[20])
{
    // @Robustness: Type aliasing
    mbedtls_sha1(cast(uint8_t *)data, size, cast(uint8_t *)digest);
    uint32_t *W = cast(uint32_t *)digest;

    W[0] = bswap32(W[0]);
    W[1] = bswap32(W[1]);
    W[2] = bswap32(W[2]);
    W[3] = bswap32(W[3]);
    W[4] = bswap32(W[4]);
}

static size_t unicode16_len(const uint16_t *s, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++) {
        if (s[i] == 0)
            break;
    }
    return i;
}

static size_t unicode16_cpy(uint16_t *d, const uint16_t *s, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++) {
        d[i] = s[i];
        if (s[i] == 0)
            break;
    }
    return i;
}
