#ifdef CORE_BROADCAST_C
#error "broadcast.c included more than once"
#endif
#define CORE_BROADCAST_C

void broadcast_stream(Connection *conn, ByteBuffer *buffer);