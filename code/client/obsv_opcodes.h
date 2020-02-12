#ifdef CORE_OBSV_OPCODES_H
#error "obsv_opcodes.h included more than once"
#endif
#define CORE_OBSV_OPCODES_H

enum
{
    OBSV_CMSG_HEARTBEAT         = 0x000, // 0
    OBSV_CMSG_DISCONNECT        = 0x001, // 1
    OBSV_CMSG_BROADCAST_STREAM  = 0x002, // 2

    OBSV_CMSG_COUNT
};

enum
{
    OBSV_SMSG_HEARTBEAT         = 0x000, // 0

    OBSV_SMSG_COUNT
};
