#ifdef CORE_BROADCAST_H
#error "broadcast.h included more than once"
#endif
#define CORE_BROADCAST_H

void broadcast_stream(Connection *conn, ByteBuffer *buffer)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        size_t  n_buffer;
        uint8_t buffer[4096];
    } BroadcastStream;
#pragma pack(pop)

    BroadcastStream packet = NewPacket(OBSV_CMSG_BROADCAST_STREAM);

    // @Enhancement:
    // There is one too much copy here, zZz
    // assert(buffer->size <= sizeof(packet.buffer));

    size_t buffer_size = MIN(sizeof(packet.buffer), buffer->size);
    packet.n_buffer = buffer_size;
    memcpy(packet.buffer, buffer->data, buffer_size);

    SendPacket(conn, sizeof(packet), &packet);
}