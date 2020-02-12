#ifdef CORE_OBSV_PACKETS_C
#error "obsv_packets.c included more than once"
#endif
#define CORE_OBSV_PACKETS_C

MsgField OBSV_CLIENT_0000[2] = {
    {TYPE_MSG_HEADER, 0},
    {TYPE_DWORD, 0},
};

MsgField OBSV_CLIENT_0001[1] = {
    {TYPE_MSG_HEADER, 0},
};

MsgField OBSV_CLIENT_0002[2] = {
    {TYPE_MSG_HEADER, 0},
    {TYPE_ARRAY_8, 4096},
};

MsgField OBSV_SERVER_0000[2] = {
    {TYPE_MSG_HEADER, 0},
    {TYPE_DWORD, 0},
};

MsgFormat OBSV_CLIENT_FORMATS[OBSV_CMSG_COUNT] = {
// header | field_count | fields | max_size | name
    {0, 2, OBSV_CLIENT_0000, 6, 0},
    {1, 1, OBSV_CLIENT_0001, 2, 0},
    {2, 2, OBSV_CLIENT_0002, 4102, 0},
};

MsgFormat OBSV_SERVER_FORMATS[OBSV_SMSG_COUNT] = {
// header | field_count | fields | max_size | name
    {0, 2, OBSV_SERVER_0000, 6, 0},
};