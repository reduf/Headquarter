#ifdef CORE_PACKETS_C
#error "packets.c included more than once"
#endif
#define CORE_PACKETS_C

MsgField AUTH_CLIENT_0000[2] = {
    {TYPE_MSG_HEADER, 0},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0001[3] = {
    {TYPE_MSG_HEADER, 1},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 32},
};

MsgField AUTH_CLIENT_0002[3] = {
    {TYPE_MSG_HEADER, 2},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
};

MsgField AUTH_CLIENT_0003[5] = {
    {TYPE_MSG_HEADER, 3},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
    {TYPE_BLOB, 20},
    {TYPE_STRING_16, 26},
};

MsgField AUTH_CLIENT_0004[1] = {
    {TYPE_MSG_HEADER, 4},
};

MsgField AUTH_CLIENT_0005[6] = {
    {TYPE_MSG_HEADER, 5},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 20},
    {TYPE_BLOB, 20},
    {TYPE_STRING_16, 64},
};

MsgField AUTH_CLIENT_0006[3] = {
    {TYPE_MSG_HEADER, 6},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0007[3] = {
    {TYPE_MSG_HEADER, 7},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
};

MsgField AUTH_CLIENT_0008[3] = {
    {TYPE_MSG_HEADER, 8},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
};

MsgField AUTH_CLIENT_0009[4] = {
    {TYPE_MSG_HEADER, 9},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
    {TYPE_ARRAY_8, 64},
};

MsgField AUTH_CLIENT_0010[3] = {
    {TYPE_MSG_HEADER, 10},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
};

MsgField AUTH_CLIENT_0011[5] = {
    {TYPE_MSG_HEADER, 11},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0012[4] = {
    {TYPE_MSG_HEADER, 12},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
};

MsgField AUTH_CLIENT_0013[2] = {
    {TYPE_MSG_HEADER, 13},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0014[2] = {
    {TYPE_MSG_HEADER, 14},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0015[3] = {
    {TYPE_MSG_HEADER, 15},
    {TYPE_ARRAY_8, 92},
    {TYPE_BLOB, 16},
};

MsgField AUTH_CLIENT_0016[2] = {
    {TYPE_MSG_HEADER, 16},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0017[2] = {
    {TYPE_MSG_HEADER, 17},
    {TYPE_STRING_16, 256},
};

MsgField AUTH_CLIENT_0018[1] = {
    {TYPE_MSG_HEADER, 18},
};

MsgField AUTH_CLIENT_0019[1] = {
    {TYPE_MSG_HEADER, 19},
};

MsgField AUTH_CLIENT_0020[6] = {
    {TYPE_MSG_HEADER, 20},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 256},
};

MsgField AUTH_CLIENT_0021[2] = {
    {TYPE_MSG_HEADER, 21},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0022[2] = {
    {TYPE_MSG_HEADER, 22},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0023[2] = {
    {TYPE_MSG_HEADER, 23},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0024[3] = {
    {TYPE_MSG_HEADER, 24},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
};

MsgField AUTH_CLIENT_0025[5] = {
    {TYPE_MSG_HEADER, 25},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 20},
    {TYPE_BLOB, 20},
};

MsgField AUTH_CLIENT_0026[5] = {
    {TYPE_MSG_HEADER, 26},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 20},
};

MsgField AUTH_CLIENT_0027[4] = {
    {TYPE_MSG_HEADER, 27},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 20},
};

MsgField AUTH_CLIENT_0028[3] = {
    {TYPE_MSG_HEADER, 28},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 26},
};

MsgField AUTH_CLIENT_0029[3] = {
    {TYPE_MSG_HEADER, 29},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0030[4] = {
    {TYPE_MSG_HEADER, 30},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 26},
};

MsgField AUTH_CLIENT_0031[23] = {
    {TYPE_MSG_HEADER, 31},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 16},
    {TYPE_STRING_16, 256},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 4},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 4},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 12},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 64},
};

MsgField AUTH_CLIENT_0032[3] = {
    {TYPE_MSG_HEADER, 32},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_8, 512},
};

MsgField AUTH_CLIENT_0033[3] = {
    {TYPE_MSG_HEADER, 33},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0034[2] = {
    {TYPE_MSG_HEADER, 34},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0035[2] = {
    {TYPE_MSG_HEADER, 35},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0036[4] = {
    {TYPE_MSG_HEADER, 36},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 16},
};

MsgField AUTH_CLIENT_0037[7] = {
    {TYPE_MSG_HEADER, 37},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
};

MsgField AUTH_CLIENT_0038[2] = {
    {TYPE_MSG_HEADER, 38},
    {TYPE_BYTE, 0},
};

MsgField AUTH_CLIENT_0039[2] = {
    {TYPE_MSG_HEADER, 39},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0040[9] = {
    {TYPE_MSG_HEADER, 40},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 12},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 28},
};

MsgField AUTH_CLIENT_0041[7] = {
    {TYPE_MSG_HEADER, 41},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0042[4] = {
    {TYPE_MSG_HEADER, 42},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 20},
};

MsgField AUTH_CLIENT_0043[4] = {
    {TYPE_MSG_HEADER, 43},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 128},
    {TYPE_ARRAY_32, 128},
};

MsgField AUTH_CLIENT_0044[23] = {
    {TYPE_MSG_HEADER, 44},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 4},
    {TYPE_STRING_16, 4},
    {TYPE_STRING_16, 12},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 10},
    {TYPE_STRING_16, 6},
    {TYPE_ARRAY_32, 128},
    {TYPE_ARRAY_32, 128},
};

MsgField AUTH_CLIENT_0045[2] = {
    {TYPE_MSG_HEADER, 45},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0046[1] = {
    {TYPE_MSG_HEADER, 46},
};

MsgField AUTH_CLIENT_0047[1] = {
    {TYPE_MSG_HEADER, 47},
};

MsgField AUTH_CLIENT_0048[2] = {
    {TYPE_MSG_HEADER, 48},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0049[2] = {
    {TYPE_MSG_HEADER, 49},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0050[3] = {
    {TYPE_MSG_HEADER, 50},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 10},
};

MsgField AUTH_CLIENT_0051[6] = {
    {TYPE_MSG_HEADER, 51},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 18},
};

MsgField AUTH_CLIENT_0052[4] = {
    {TYPE_MSG_HEADER, 52},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0053[2] = {
    {TYPE_MSG_HEADER, 53},
    {TYPE_DWORD, 0},
};

MsgField AUTH_CLIENT_0054[23] = {
    {TYPE_MSG_HEADER, 54},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 256},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 4},
    {TYPE_STRING_16, 4},
    {TYPE_STRING_16, 12},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 28},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 10},
    {TYPE_ARRAY_32, 128},
    {TYPE_ARRAY_32, 128},
};

MsgField AUTH_CLIENT_0055[4] = {
    {TYPE_MSG_HEADER, 55},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 20},
};

MsgField AUTH_CLIENT_0056[5] = {
    {TYPE_MSG_HEADER, 56},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 0006B670
MsgField AUTH_SERVER_0000[2] = {
    {TYPE_MSG_HEADER, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008CB20
MsgField AUTH_SERVER_0001[4] = {
    {TYPE_MSG_HEADER, 1},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008C6A0
MsgField AUTH_SERVER_0002[3] = {
    {TYPE_MSG_HEADER, 2},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008A8B0
MsgField AUTH_SERVER_0003[3] = {
    {TYPE_MSG_HEADER, 3},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008CD30
MsgField AUTH_SERVER_0004[3] = {
    {TYPE_MSG_HEADER, 4},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 2048},
};

// Handler Rva: 00000000
MsgField AUTH_SERVER_0005[1] = {
    {TYPE_MSG_HEADER, 5},
};

// Handler Rva: 0008C4F0
MsgField AUTH_SERVER_0006[7] = {
    {TYPE_MSG_HEADER, 6},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 8},
    {TYPE_BLOB, 8},
    {TYPE_BLOB, 8},
    {TYPE_BLOB, 8},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008C6F0
MsgField AUTH_SERVER_0007[6] = {
    {TYPE_MSG_HEADER, 7},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
    {TYPE_ARRAY_8, 64},
};

// Handler Rva: 0008C7E0
MsgField AUTH_SERVER_0008[5] = {
    {TYPE_MSG_HEADER, 8},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 24},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008CAF0
MsgField AUTH_SERVER_0009[6] = {
    {TYPE_MSG_HEADER, 9},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 24},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008C720
MsgField AUTH_SERVER_0010[5] = {
    {TYPE_MSG_HEADER, 10},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 0008C780
MsgField AUTH_SERVER_0011[5] = {
    {TYPE_MSG_HEADER, 11},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 0008CAD0
MsgField AUTH_SERVER_0012[4] = {
    {TYPE_MSG_HEADER, 12},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 256},
};

// Handler Rva: 0008A940
MsgField AUTH_SERVER_0013[5] = {
    {TYPE_MSG_HEADER, 13},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 256},
    {TYPE_STRING_16, 256},
};

// Handler Rva: 0008C670
MsgField AUTH_SERVER_0014[4] = {
    {TYPE_MSG_HEADER, 14},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_8, 32},
};

// Handler Rva: 00000000
MsgField AUTH_SERVER_0015[1] = {
    {TYPE_MSG_HEADER, 15},
};

// Handler Rva: 0008C850
MsgField AUTH_SERVER_0016[7] = {
    {TYPE_MSG_HEADER, 16},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 256},
};

// Handler Rva: 0008C570
MsgField AUTH_SERVER_0017[12] = {
    {TYPE_MSG_HEADER, 17},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 8},
    {TYPE_BLOB, 8},
    {TYPE_BLOB, 16},
    {TYPE_BLOB, 16},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_8, 200},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008C880
MsgField AUTH_SERVER_0018[7] = {
    {TYPE_MSG_HEADER, 18},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 256},
};

// Handler Rva: 0008C830
MsgField AUTH_SERVER_0019[2] = {
    {TYPE_MSG_HEADER, 19},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008C550
MsgField AUTH_SERVER_0020[3] = {
    {TYPE_MSG_HEADER, 20},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008C4C0
MsgField AUTH_SERVER_0021[7] = {
    {TYPE_MSG_HEADER, 21},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 8},
    {TYPE_BLOB, 8},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008C530
MsgField AUTH_SERVER_0022[3] = {
    {TYPE_MSG_HEADER, 22},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_8, 2048},
};

// Handler Rva: 0008C8B0
MsgField AUTH_SERVER_0023[2] = {
    {TYPE_MSG_HEADER, 23},
    {TYPE_ARRAY_8, 4096},
};

// Handler Rva: 0008C640
MsgField AUTH_SERVER_0024[5] = {
    {TYPE_MSG_HEADER, 24},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 8},
    {TYPE_BLOB, 8},
    {TYPE_ARRAY_8, 200},
};

// Handler Rva: 00000000
MsgField AUTH_SERVER_0025[1] = {
    {TYPE_MSG_HEADER, 25},
};

// Handler Rva: 0008CBF0
MsgField AUTH_SERVER_0026[5] = {
    {TYPE_MSG_HEADER, 26},
    {TYPE_DWORD, 0},
    {TYPE_NESTED_STRUCT, 11},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 0008C6C0
MsgField AUTH_SERVER_0027[5] = {
    {TYPE_MSG_HEADER, 27},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008CD10
MsgField AUTH_SERVER_0028[3] = {
    {TYPE_MSG_HEADER, 28},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008CC10
MsgField AUTH_SERVER_0029[4] = {
    {TYPE_MSG_HEADER, 29},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008CCE0
MsgField AUTH_SERVER_0030[10] = {
    {TYPE_MSG_HEADER, 30},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008CC60
MsgField AUTH_SERVER_0031[4] = {
    {TYPE_MSG_HEADER, 31},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
    {TYPE_ARRAY_32, 16},
};

// Handler Rva: 0008C750
MsgField AUTH_SERVER_0032[6] = {
    {TYPE_MSG_HEADER, 32},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 0008C7B0
MsgField AUTH_SERVER_0033[6] = {
    {TYPE_MSG_HEADER, 33},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 0008C810
MsgField AUTH_SERVER_0034[3] = {
    {TYPE_MSG_HEADER, 34},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 12},
};

// Handler Rva: 0008CC30
MsgField AUTH_SERVER_0035[7] = {
    {TYPE_MSG_HEADER, 35},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_BLOB, 16},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 16},
};

// Handler Rva: 0008CC90
MsgField AUTH_SERVER_0036[7] = {
    {TYPE_MSG_HEADER, 36},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 4},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 16},
};

// Handler Rva: 00000000
MsgField AUTH_SERVER_0037[1] = {
    {TYPE_MSG_HEADER, 37},
};

// Handler Rva: 0008CCC0
MsgField AUTH_SERVER_0038[3] = {
    {TYPE_MSG_HEADER, 38},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0000[2] = {
    {TYPE_MSG_HEADER, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0001[1] = {
    {TYPE_MSG_HEADER, 1},
};

MsgField GAME_CLIENT_0002[3] = {
    {TYPE_MSG_HEADER, 2},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0003[2] = {
    {TYPE_MSG_HEADER, 3},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0004[1] = {
    {TYPE_MSG_HEADER, 4},
};

MsgField GAME_CLIENT_0005[3] = {
    {TYPE_MSG_HEADER, 5},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0006[1] = {
    {TYPE_MSG_HEADER, 6},
};

MsgField GAME_CLIENT_0007[1] = {
    {TYPE_MSG_HEADER, 7},
};

MsgField GAME_CLIENT_0008[1] = {
    {TYPE_MSG_HEADER, 8},
};

MsgField GAME_CLIENT_0009[2] = {
    {TYPE_MSG_HEADER, 9},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0010[1] = {
    {TYPE_MSG_HEADER, 10},
};

MsgField GAME_CLIENT_0011[1] = {
    {TYPE_MSG_HEADER, 11},
};

MsgField GAME_CLIENT_0012[4] = {
    {TYPE_MSG_HEADER, 12},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0013[4] = {
    {TYPE_MSG_HEADER, 13},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0014[4] = {
    {TYPE_MSG_HEADER, 14},
    {TYPE_AGENT_ID, 0},
    {TYPE_ARRAY_32, 16},
    {TYPE_ARRAY_32, 16},
};

MsgField GAME_CLIENT_0015[2] = {
    {TYPE_MSG_HEADER, 15},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0016[2] = {
    {TYPE_MSG_HEADER, 16},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0017[1] = {
    {TYPE_MSG_HEADER, 17},
};

MsgField GAME_CLIENT_0018[3] = {
    {TYPE_MSG_HEADER, 18},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0019[3] = {
    {TYPE_MSG_HEADER, 19},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0020[3] = {
    {TYPE_MSG_HEADER, 20},
    {TYPE_AGENT_ID, 0},
    {TYPE_AGENT_ID, 0},
};

MsgField GAME_CLIENT_0021[2] = {
    {TYPE_MSG_HEADER, 21},
    {TYPE_AGENT_ID, 0},
};

MsgField GAME_CLIENT_0022[2] = {
    {TYPE_MSG_HEADER, 22},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0023[3] = {
    {TYPE_MSG_HEADER, 23},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0024[4] = {
    {TYPE_MSG_HEADER, 24},
    {TYPE_AGENT_ID, 0},
    {TYPE_VECT2, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0025[3] = {
    {TYPE_MSG_HEADER, 25},
    {TYPE_VECT2, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0026[5] = {
    {TYPE_MSG_HEADER, 26},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_AGENT_ID, 0},
};

MsgField GAME_CLIENT_0027[4] = {
    {TYPE_MSG_HEADER, 27},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0028[2] = {
    {TYPE_MSG_HEADER, 28},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0029[2] = {
    {TYPE_MSG_HEADER, 29},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0030[2] = {
    {TYPE_MSG_HEADER, 30},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0031[3] = {
    {TYPE_MSG_HEADER, 31},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0032[3] = {
    {TYPE_MSG_HEADER, 32},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0033[3] = {
    {TYPE_MSG_HEADER, 33},
    {TYPE_BYTE, 0},
    {TYPE_AGENT_ID, 0},
};

MsgField GAME_CLIENT_0034[3] = {
    {TYPE_MSG_HEADER, 34},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0035[4] = {
    {TYPE_MSG_HEADER, 35},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0036[3] = {
    {TYPE_MSG_HEADER, 36},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0037[5] = {
    {TYPE_MSG_HEADER, 37},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0038[1] = {
    {TYPE_MSG_HEADER, 38},
};

MsgField GAME_CLIENT_0039[2] = {
    {TYPE_MSG_HEADER, 39},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0040[2] = {
    {TYPE_MSG_HEADER, 40},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0041[3] = {
    {TYPE_MSG_HEADER, 41},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
};

MsgField GAME_CLIENT_0042[3] = {
    {TYPE_MSG_HEADER, 42},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0043[1] = {
    {TYPE_MSG_HEADER, 43},
};

MsgField GAME_CLIENT_0044[1] = {
    {TYPE_MSG_HEADER, 44},
};

MsgField GAME_CLIENT_0045[2] = {
    {TYPE_MSG_HEADER, 45},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0046[2] = {
    {TYPE_MSG_HEADER, 46},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0047[3] = {
    {TYPE_MSG_HEADER, 47},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0048[2] = {
    {TYPE_MSG_HEADER, 48},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0049[2] = {
    {TYPE_MSG_HEADER, 49},
    {TYPE_AGENT_ID, 0},
};

MsgField GAME_CLIENT_0050[4] = {
    {TYPE_MSG_HEADER, 50},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_ARRAY_32, 4},
};

MsgField GAME_CLIENT_0051[4] = {
    {TYPE_MSG_HEADER, 51},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0052[4] = {
    {TYPE_MSG_HEADER, 52},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 6},
};

MsgField GAME_CLIENT_0053[3] = {
    {TYPE_MSG_HEADER, 53},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 4},
};

MsgField GAME_CLIENT_0054[9] = {
    {TYPE_MSG_HEADER, 54},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0055[3] = {
    {TYPE_MSG_HEADER, 55},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0056[3] = {
    {TYPE_MSG_HEADER, 56},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 4},
};

MsgField GAME_CLIENT_0057[2] = {
    {TYPE_MSG_HEADER, 57},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0058[2] = {
    {TYPE_MSG_HEADER, 58},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0059[5] = {
    {TYPE_MSG_HEADER, 59},
    {TYPE_VECT2, 0},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0060[3] = {
    {TYPE_MSG_HEADER, 60},
    {TYPE_VECT2, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0061[3] = {
    {TYPE_MSG_HEADER, 61},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0062[3] = {
    {TYPE_MSG_HEADER, 62},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0063[3] = {
    {TYPE_MSG_HEADER, 63},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0064[2] = {
    {TYPE_MSG_HEADER, 64},
    {TYPE_AGENT_ID, 0},
};

MsgField GAME_CLIENT_0065[2] = {
    {TYPE_MSG_HEADER, 65},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0066[2] = {
    {TYPE_MSG_HEADER, 66},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0067[2] = {
    {TYPE_MSG_HEADER, 67},
    {TYPE_ARRAY_8, 256},
};

MsgField GAME_CLIENT_0068[5] = {
    {TYPE_MSG_HEADER, 68},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0069[3] = {
    {TYPE_MSG_HEADER, 69},
    {TYPE_VECT2, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0070[3] = {
    {TYPE_MSG_HEADER, 70},
    {TYPE_VECT2, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0071[2] = {
    {TYPE_MSG_HEADER, 71},
    {TYPE_AGENT_ID, 0},
};

MsgField GAME_CLIENT_0072[6] = {
    {TYPE_MSG_HEADER, 72},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
};

MsgField GAME_CLIENT_0073[2] = {
    {TYPE_MSG_HEADER, 73},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0074[7] = {
    {TYPE_MSG_HEADER, 74},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
};

MsgField GAME_CLIENT_0075[8] = {
    {TYPE_MSG_HEADER, 75},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
    {TYPE_ARRAY_8, 16},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
    {TYPE_ARRAY_8, 16},
};

MsgField GAME_CLIENT_0076[3] = {
    {TYPE_MSG_HEADER, 76},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 122},
};

MsgField GAME_CLIENT_0077[4] = {
    {TYPE_MSG_HEADER, 77},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0078[5] = {
    {TYPE_MSG_HEADER, 78},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0079[3] = {
    {TYPE_MSG_HEADER, 79},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0080[3] = {
    {TYPE_MSG_HEADER, 80},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0081[2] = {
    {TYPE_MSG_HEADER, 81},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0082[8] = {
    {TYPE_MSG_HEADER, 82},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0083[2] = {
    {TYPE_MSG_HEADER, 83},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0084[3] = {
    {TYPE_MSG_HEADER, 84},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0085[3] = {
    {TYPE_MSG_HEADER, 85},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0086[2] = {
    {TYPE_MSG_HEADER, 86},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0087[1] = {
    {TYPE_MSG_HEADER, 87},
};

MsgField GAME_CLIENT_0088[1] = {
    {TYPE_MSG_HEADER, 88},
};

MsgField GAME_CLIENT_0089[2] = {
    {TYPE_MSG_HEADER, 89},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0090[5] = {
    {TYPE_MSG_HEADER, 90},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0091[3] = {
    {TYPE_MSG_HEADER, 91},
    {TYPE_AGENT_ID, 0},
    {TYPE_ARRAY_32, 8},
};

MsgField GAME_CLIENT_0092[6] = {
    {TYPE_MSG_HEADER, 92},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0093[5] = {
    {TYPE_MSG_HEADER, 93},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0094[3] = {
    {TYPE_MSG_HEADER, 94},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0095[2] = {
    {TYPE_MSG_HEADER, 95},
    {TYPE_STRING_16, 32},
};

MsgField GAME_CLIENT_0096[3] = {
    {TYPE_MSG_HEADER, 96},
    {TYPE_STRING_16, 32},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0097[1] = {
    {TYPE_MSG_HEADER, 97},
};

MsgField GAME_CLIENT_0098[3] = {
    {TYPE_MSG_HEADER, 98},
    {TYPE_AGENT_ID, 0},
    {TYPE_STRING_16, 138},
};

MsgField GAME_CLIENT_0099[2] = {
    {TYPE_MSG_HEADER, 99},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0100[4] = {
    {TYPE_MSG_HEADER, 100},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0101[5] = {
    {TYPE_MSG_HEADER, 101},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0102[2] = {
    {TYPE_MSG_HEADER, 102},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0103[2] = {
    {TYPE_MSG_HEADER, 103},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0104[3] = {
    {TYPE_MSG_HEADER, 104},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 4},
};

MsgField GAME_CLIENT_0105[3] = {
    {TYPE_MSG_HEADER, 105},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0106[3] = {
    {TYPE_MSG_HEADER, 106},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0107[3] = {
    {TYPE_MSG_HEADER, 107},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0108[3] = {
    {TYPE_MSG_HEADER, 108},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0109[3] = {
    {TYPE_MSG_HEADER, 109},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0110[2] = {
    {TYPE_MSG_HEADER, 110},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0111[5] = {
    {TYPE_MSG_HEADER, 111},
    {TYPE_BYTE, 0},
    {TYPE_NESTED_STRUCT, 6},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0112[4] = {
    {TYPE_MSG_HEADER, 112},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0113[2] = {
    {TYPE_MSG_HEADER, 113},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0114[3] = {
    {TYPE_MSG_HEADER, 114},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0115[5] = {
    {TYPE_MSG_HEADER, 115},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0116[1] = {
    {TYPE_MSG_HEADER, 116},
};

MsgField GAME_CLIENT_0117[4] = {
    {TYPE_MSG_HEADER, 117},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0118[1] = {
    {TYPE_MSG_HEADER, 118},
};

MsgField GAME_CLIENT_0119[1] = {
    {TYPE_MSG_HEADER, 119},
};

MsgField GAME_CLIENT_0120[1] = {
    {TYPE_MSG_HEADER, 120},
};

MsgField GAME_CLIENT_0121[2] = {
    {TYPE_MSG_HEADER, 121},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0122[3] = {
    {TYPE_MSG_HEADER, 122},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0123[4] = {
    {TYPE_MSG_HEADER, 123},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0124[2] = {
    {TYPE_MSG_HEADER, 124},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0125[3] = {
    {TYPE_MSG_HEADER, 125},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0126[3] = {
    {TYPE_MSG_HEADER, 126},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0127[1] = {
    {TYPE_MSG_HEADER, 127},
};

MsgField GAME_CLIENT_0128[1] = {
    {TYPE_MSG_HEADER, 128},
};

MsgField GAME_CLIENT_0129[2] = {
    {TYPE_MSG_HEADER, 129},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0130[3] = {
    {TYPE_MSG_HEADER, 130},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0131[2] = {
    {TYPE_MSG_HEADER, 131},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0132[7] = {
    {TYPE_MSG_HEADER, 132},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_ARRAY_16, 4},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0133[4] = {
    {TYPE_MSG_HEADER, 133},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0134[1] = {
    {TYPE_MSG_HEADER, 134},
};

MsgField GAME_CLIENT_0135[1] = {
    {TYPE_MSG_HEADER, 135},
};

MsgField GAME_CLIENT_0136[1] = {
    {TYPE_MSG_HEADER, 136},
};

MsgField GAME_CLIENT_0137[3] = {
    {TYPE_MSG_HEADER, 137},
    {TYPE_STRING_16, 20},
    {TYPE_BLOB, 8},
};

MsgField GAME_CLIENT_0138[1] = {
    {TYPE_MSG_HEADER, 138},
};

MsgField GAME_CLIENT_0139[3] = {
    {TYPE_MSG_HEADER, 139},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0140[2] = {
    {TYPE_MSG_HEADER, 140},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0141[1] = {
    {TYPE_MSG_HEADER, 141},
};

MsgField GAME_CLIENT_0142[2] = {
    {TYPE_MSG_HEADER, 142},
    {TYPE_BLOB, 16},
};

MsgField GAME_CLIENT_0143[3] = {
    {TYPE_MSG_HEADER, 143},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0144[2] = {
    {TYPE_MSG_HEADER, 144},
    {TYPE_ARRAY_8, 112},
};

MsgField GAME_CLIENT_0145[2] = {
    {TYPE_MSG_HEADER, 145},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0146[2] = {
    {TYPE_MSG_HEADER, 146},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0147[2] = {
    {TYPE_MSG_HEADER, 147},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0148[4] = {
    {TYPE_MSG_HEADER, 148},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0149[1] = {
    {TYPE_MSG_HEADER, 149},
};

MsgField GAME_CLIENT_0150[1] = {
    {TYPE_MSG_HEADER, 150},
};

MsgField GAME_CLIENT_0151[1] = {
    {TYPE_MSG_HEADER, 151},
};

MsgField GAME_CLIENT_0152[1] = {
    {TYPE_MSG_HEADER, 152},
};

MsgField GAME_CLIENT_0153[2] = {
    {TYPE_MSG_HEADER, 153},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0154[2] = {
    {TYPE_MSG_HEADER, 154},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0155[2] = {
    {TYPE_MSG_HEADER, 155},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0156[2] = {
    {TYPE_MSG_HEADER, 156},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0157[2] = {
    {TYPE_MSG_HEADER, 157},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0158[2] = {
    {TYPE_MSG_HEADER, 158},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0159[2] = {
    {TYPE_MSG_HEADER, 159},
    {TYPE_STRING_16, 20},
};

MsgField GAME_CLIENT_0160[1] = {
    {TYPE_MSG_HEADER, 160},
};

MsgField GAME_CLIENT_0161[1] = {
    {TYPE_MSG_HEADER, 161},
};

MsgField GAME_CLIENT_0162[3] = {
    {TYPE_MSG_HEADER, 162},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 64},
};

MsgField GAME_CLIENT_0163[2] = {
    {TYPE_MSG_HEADER, 163},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0164[4] = {
    {TYPE_MSG_HEADER, 164},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0165[1] = {
    {TYPE_MSG_HEADER, 165},
};

MsgField GAME_CLIENT_0166[2] = {
    {TYPE_MSG_HEADER, 166},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0167[2] = {
    {TYPE_MSG_HEADER, 167},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0168[4] = {
    {TYPE_MSG_HEADER, 168},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 32},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0169[1] = {
    {TYPE_MSG_HEADER, 169},
};

MsgField GAME_CLIENT_0170[2] = {
    {TYPE_MSG_HEADER, 170},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0171[2] = {
    {TYPE_MSG_HEADER, 171},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0172[2] = {
    {TYPE_MSG_HEADER, 172},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0173[2] = {
    {TYPE_MSG_HEADER, 173},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0174[3] = {
    {TYPE_MSG_HEADER, 174},
    {TYPE_BLOB, 16},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0175[6] = {
    {TYPE_MSG_HEADER, 175},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0176[2] = {
    {TYPE_MSG_HEADER, 176},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0177[3] = {
    {TYPE_MSG_HEADER, 177},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
};

MsgField GAME_CLIENT_0178[3] = {
    {TYPE_MSG_HEADER, 178},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
};

MsgField GAME_CLIENT_0179[4] = {
    {TYPE_MSG_HEADER, 179},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0180[4] = {
    {TYPE_MSG_HEADER, 180},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0181[4] = {
    {TYPE_MSG_HEADER, 181},
    {TYPE_DWORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0182[3] = {
    {TYPE_MSG_HEADER, 182},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
};

MsgField GAME_CLIENT_0183[3] = {
    {TYPE_MSG_HEADER, 183},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
};

MsgField GAME_CLIENT_0184[3] = {
    {TYPE_MSG_HEADER, 184},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
};

MsgField GAME_CLIENT_0185[2] = {
    {TYPE_MSG_HEADER, 185},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0186[2] = {
    {TYPE_MSG_HEADER, 186},
    {TYPE_DWORD, 0},
};

MsgField GAME_CLIENT_0187[3] = {
    {TYPE_MSG_HEADER, 187},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
};

MsgField GAME_CLIENT_0188[3] = {
    {TYPE_MSG_HEADER, 188},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 256},
};

MsgField GAME_CLIENT_0189[4] = {
    {TYPE_MSG_HEADER, 189},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 32},
};

MsgField GAME_CLIENT_0190[3] = {
    {TYPE_MSG_HEADER, 190},
    {TYPE_STRING_16, 32},
    {TYPE_BYTE, 0},
};

MsgField GAME_CLIENT_0191[3] = {
    {TYPE_MSG_HEADER, 191},
    {TYPE_AGENT_ID, 0},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 003DA420
MsgField GAME_SERVER_0000[2] = {
    {TYPE_MSG_HEADER, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003DA2E0
MsgField GAME_SERVER_0001[2] = {
    {TYPE_MSG_HEADER, 1},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003DA450
MsgField GAME_SERVER_0002[3] = {
    {TYPE_MSG_HEADER, 2},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003DA330
MsgField GAME_SERVER_0003[2] = {
    {TYPE_MSG_HEADER, 3},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003DA360
MsgField GAME_SERVER_0004[3] = {
    {TYPE_MSG_HEADER, 4},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003DA390
MsgField GAME_SERVER_0005[2] = {
    {TYPE_MSG_HEADER, 5},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003DA3C0
MsgField GAME_SERVER_0006[1] = {
    {TYPE_MSG_HEADER, 6},
};

// Handler Rva: 003DA3E0
MsgField GAME_SERVER_0007[1] = {
    {TYPE_MSG_HEADER, 7},
};

// Handler Rva: 003DA400
MsgField GAME_SERVER_0008[1] = {
    {TYPE_MSG_HEADER, 8},
};

// Handler Rva: 003DA310
MsgField GAME_SERVER_0009[1] = {
    {TYPE_MSG_HEADER, 9},
};

// Handler Rva: 0008A8B0
MsgField GAME_SERVER_0010[3] = {
    {TYPE_MSG_HEADER, 10},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008A890
MsgField GAME_SERVER_0011[2] = {
    {TYPE_MSG_HEADER, 11},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008A8D0
MsgField GAME_SERVER_0012[1] = {
    {TYPE_MSG_HEADER, 12},
};

// Handler Rva: 0008A920
MsgField GAME_SERVER_0013[2] = {
    {TYPE_MSG_HEADER, 13},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0008A940
MsgField GAME_SERVER_0014[5] = {
    {TYPE_MSG_HEADER, 14},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 256},
    {TYPE_STRING_16, 256},
};

// Handler Rva: 003869D0
MsgField GAME_SERVER_0015[4] = {
    {TYPE_MSG_HEADER, 15},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 00386A00
MsgField GAME_SERVER_0016[2] = {
    {TYPE_MSG_HEADER, 16},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00386A30
MsgField GAME_SERVER_0017[2] = {
    {TYPE_MSG_HEADER, 17},
    {TYPE_WORD, 0},
};

// Handler Rva: 00386A60
MsgField GAME_SERVER_0018[2] = {
    {TYPE_MSG_HEADER, 18},
    {TYPE_WORD, 0},
};

// Handler Rva: 00386A90
MsgField GAME_SERVER_0019[2] = {
    {TYPE_MSG_HEADER, 19},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00386AB0
MsgField GAME_SERVER_0020[4] = {
    {TYPE_MSG_HEADER, 20},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00386AE0
MsgField GAME_SERVER_0021[3] = {
    {TYPE_MSG_HEADER, 21},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00386B10
MsgField GAME_SERVER_0022[2] = {
    {TYPE_MSG_HEADER, 22},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 00386B30
MsgField GAME_SERVER_0023[6] = {
    {TYPE_MSG_HEADER, 23},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00386B60
MsgField GAME_SERVER_0024[2] = {
    {TYPE_MSG_HEADER, 24},
    {TYPE_ARRAY_32, 8},
};

// Handler Rva: 00386B90
MsgField GAME_SERVER_0025[5] = {
    {TYPE_MSG_HEADER, 25},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_NESTED_STRUCT, 64},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00386C10
MsgField GAME_SERVER_0026[4] = {
    {TYPE_MSG_HEADER, 26},
    {TYPE_WORD, 0},
    {TYPE_NESTED_STRUCT, 64},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00386C90
MsgField GAME_SERVER_0027[1] = {
    {TYPE_MSG_HEADER, 27},
};

// Handler Rva: 00386CB0
MsgField GAME_SERVER_0028[3] = {
    {TYPE_MSG_HEADER, 28},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00386CE0
MsgField GAME_SERVER_0029[2] = {
    {TYPE_MSG_HEADER, 29},
    {TYPE_ARRAY_32, 128},
};

// Handler Rva: 001BBD70
MsgField GAME_SERVER_0030[2] = {
    {TYPE_MSG_HEADER, 30},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BBE10
MsgField GAME_SERVER_0031[2] = {
    {TYPE_MSG_HEADER, 31},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BBE80
MsgField GAME_SERVER_0032[24] = {
    {TYPE_MSG_HEADER, 32},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_BYTE, 0},
    {TYPE_FLOAT, 0},
    {TYPE_FLOAT, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 001BC0C0
MsgField GAME_SERVER_0033[2] = {
    {TYPE_MSG_HEADER, 33},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BC150
MsgField GAME_SERVER_0034[3] = {
    {TYPE_MSG_HEADER, 34},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BC1C0
MsgField GAME_SERVER_0035[3] = {
    {TYPE_MSG_HEADER, 35},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BC250
MsgField GAME_SERVER_0036[3] = {
    {TYPE_MSG_HEADER, 36},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BC310
MsgField GAME_SERVER_0037[4] = {
    {TYPE_MSG_HEADER, 37},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 001BC410
MsgField GAME_SERVER_0038[3] = {
    {TYPE_MSG_HEADER, 38},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 001BC4D0
MsgField GAME_SERVER_0039[3] = {
    {TYPE_MSG_HEADER, 39},
    {TYPE_DWORD, 0},
    {TYPE_FLOAT, 0},
};

// Handler Rva: 001BC5A0
MsgField GAME_SERVER_0040[2] = {
    {TYPE_MSG_HEADER, 40},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BC660
MsgField GAME_SERVER_0041[5] = {
    {TYPE_MSG_HEADER, 41},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 001BC700
MsgField GAME_SERVER_0042[6] = {
    {TYPE_MSG_HEADER, 42},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BC7A0
MsgField GAME_SERVER_0043[4] = {
    {TYPE_MSG_HEADER, 43},
    {TYPE_DWORD, 0},
    {TYPE_FLOAT, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 001BC820
MsgField GAME_SERVER_0044[4] = {
    {TYPE_MSG_HEADER, 44},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 001BC940
MsgField GAME_SERVER_0045[2] = {
    {TYPE_MSG_HEADER, 45},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BCA30
MsgField GAME_SERVER_0046[4] = {
    {TYPE_MSG_HEADER, 46},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 001BCB40
MsgField GAME_SERVER_0047[3] = {
    {TYPE_MSG_HEADER, 47},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473420
MsgField GAME_SERVER_0048[8] = {
    {TYPE_MSG_HEADER, 48},
    {TYPE_STRING_16, 32},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473450
MsgField GAME_SERVER_0049[2] = {
    {TYPE_MSG_HEADER, 49},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 00473470
MsgField GAME_SERVER_0050[7] = {
    {TYPE_MSG_HEADER, 50},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004734A0
MsgField GAME_SERVER_0051[2] = {
    {TYPE_MSG_HEADER, 51},
    {TYPE_STRING_16, 64},
};

// Handler Rva: 004734C0
MsgField GAME_SERVER_0052[3] = {
    {TYPE_MSG_HEADER, 52},
    {TYPE_WORD, 0},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 004734E0
MsgField GAME_SERVER_0053[4] = {
    {TYPE_MSG_HEADER, 53},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473510
MsgField GAME_SERVER_0054[3] = {
    {TYPE_MSG_HEADER, 54},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473530
MsgField GAME_SERVER_0055[4] = {
    {TYPE_MSG_HEADER, 55},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473550
MsgField GAME_SERVER_0056[3] = {
    {TYPE_MSG_HEADER, 56},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473570
MsgField GAME_SERVER_0057[3] = {
    {TYPE_MSG_HEADER, 57},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473590
MsgField GAME_SERVER_0058[3] = {
    {TYPE_MSG_HEADER, 58},
    {TYPE_AGENT_ID, 0},
    {TYPE_ARRAY_32, 48},
};

// Handler Rva: 004735D0
MsgField GAME_SERVER_0059[5] = {
    {TYPE_MSG_HEADER, 59},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473600
MsgField GAME_SERVER_0060[5] = {
    {TYPE_MSG_HEADER, 60},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 128},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473630
MsgField GAME_SERVER_0061[2] = {
    {TYPE_MSG_HEADER, 61},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00473650
MsgField GAME_SERVER_0062[6] = {
    {TYPE_MSG_HEADER, 62},
    {TYPE_AGENT_ID, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473680
MsgField GAME_SERVER_0063[3] = {
    {TYPE_MSG_HEADER, 63},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004736A0
MsgField GAME_SERVER_0064[6] = {
    {TYPE_MSG_HEADER, 64},
    {TYPE_AGENT_ID, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004736D0
MsgField GAME_SERVER_0065[6] = {
    {TYPE_MSG_HEADER, 65},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473700
MsgField GAME_SERVER_0066[5] = {
    {TYPE_MSG_HEADER, 66},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473730
MsgField GAME_SERVER_0067[3] = {
    {TYPE_MSG_HEADER, 67},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473750
MsgField GAME_SERVER_0068[3] = {
    {TYPE_MSG_HEADER, 68},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473780
MsgField GAME_SERVER_0069[4] = {
    {TYPE_MSG_HEADER, 69},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 004737B0
MsgField GAME_SERVER_0070[5] = {
    {TYPE_MSG_HEADER, 70},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 00473800
MsgField GAME_SERVER_0071[3] = {
    {TYPE_MSG_HEADER, 71},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473820
MsgField GAME_SERVER_0072[10] = {
    {TYPE_MSG_HEADER, 72},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 8},
    {TYPE_WORD, 0},
};

// Handler Rva: 00473870
MsgField GAME_SERVER_0073[2] = {
    {TYPE_MSG_HEADER, 73},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473890
MsgField GAME_SERVER_0074[2] = {
    {TYPE_MSG_HEADER, 74},
    {TYPE_ARRAY_32, 64},
};

// Handler Rva: 004738B0
MsgField GAME_SERVER_0075[4] = {
    {TYPE_MSG_HEADER, 75},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 128},
    {TYPE_STRING_16, 128},
};

// Handler Rva: 004738E0
MsgField GAME_SERVER_0076[5] = {
    {TYPE_MSG_HEADER, 76},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 00473950
MsgField GAME_SERVER_0077[4] = {
    {TYPE_MSG_HEADER, 77},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00000000
MsgField GAME_SERVER_0078[1] = {
    {TYPE_MSG_HEADER, 78},
};

// Handler Rva: 00473920
MsgField GAME_SERVER_0079[7] = {
    {TYPE_MSG_HEADER, 79},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 8},
    {TYPE_WORD, 0},
};

// Handler Rva: 00473970
MsgField GAME_SERVER_0080[5] = {
    {TYPE_MSG_HEADER, 80},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 004739B0
MsgField GAME_SERVER_0081[2] = {
    {TYPE_MSG_HEADER, 81},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004739D0
MsgField GAME_SERVER_0082[5] = {
    {TYPE_MSG_HEADER, 82},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 00473A10
MsgField GAME_SERVER_0083[3] = {
    {TYPE_MSG_HEADER, 83},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 128},
};

// Handler Rva: 00000000
MsgField GAME_SERVER_0084[1] = {
    {TYPE_MSG_HEADER, 84},
};

// Handler Rva: 00473A30
MsgField GAME_SERVER_0085[10] = {
    {TYPE_MSG_HEADER, 85},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 8},
};

// Handler Rva: 00473AB0
MsgField GAME_SERVER_0086[3] = {
    {TYPE_MSG_HEADER, 86},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 8},
};

// Handler Rva: 00473B20
MsgField GAME_SERVER_0087[3] = {
    {TYPE_MSG_HEADER, 87},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 00473B40
MsgField GAME_SERVER_0088[8] = {
    {TYPE_MSG_HEADER, 88},
    {TYPE_DWORD, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 00473BB0
MsgField GAME_SERVER_0089[2] = {
    {TYPE_MSG_HEADER, 89},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473BD0
MsgField GAME_SERVER_0090[2] = {
    {TYPE_MSG_HEADER, 90},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473BF0
MsgField GAME_SERVER_0091[4] = {
    {TYPE_MSG_HEADER, 91},
    {TYPE_AGENT_ID, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473C10
MsgField GAME_SERVER_0092[2] = {
    {TYPE_MSG_HEADER, 92},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 00473C30
MsgField GAME_SERVER_0093[3] = {
    {TYPE_MSG_HEADER, 93},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473C50
MsgField GAME_SERVER_0094[4] = {
    {TYPE_MSG_HEADER, 94},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 8},
};

// Handler Rva: 00473C70
MsgField GAME_SERVER_0095[4] = {
    {TYPE_MSG_HEADER, 95},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 6},
};

// Handler Rva: 00473C90
MsgField GAME_SERVER_0096[3] = {
    {TYPE_MSG_HEADER, 96},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473CB0
MsgField GAME_SERVER_0097[3] = {
    {TYPE_MSG_HEADER, 97},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473CD0
MsgField GAME_SERVER_0098[3] = {
    {TYPE_MSG_HEADER, 98},
    {TYPE_AGENT_ID, 0},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00473CF0
MsgField GAME_SERVER_0099[4] = {
    {TYPE_MSG_HEADER, 99},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473D10
MsgField GAME_SERVER_0100[3] = {
    {TYPE_MSG_HEADER, 100},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473D30
MsgField GAME_SERVER_0101[4] = {
    {TYPE_MSG_HEADER, 101},
    {TYPE_AGENT_ID, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 00473D70
MsgField GAME_SERVER_0102[3] = {
    {TYPE_MSG_HEADER, 102},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 00473DB0
MsgField GAME_SERVER_0103[4] = {
    {TYPE_MSG_HEADER, 103},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473DD0
MsgField GAME_SERVER_0104[2] = {
    {TYPE_MSG_HEADER, 104},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00473DF0
MsgField GAME_SERVER_0105[2] = {
    {TYPE_MSG_HEADER, 105},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473E10
MsgField GAME_SERVER_0106[3] = {
    {TYPE_MSG_HEADER, 106},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473E30
MsgField GAME_SERVER_0107[4] = {
    {TYPE_MSG_HEADER, 107},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473E50
MsgField GAME_SERVER_0108[4] = {
    {TYPE_MSG_HEADER, 108},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473E70
MsgField GAME_SERVER_0109[11] = {
    {TYPE_MSG_HEADER, 109},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473E90
MsgField GAME_SERVER_0110[4] = {
    {TYPE_MSG_HEADER, 110},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473EB0
MsgField GAME_SERVER_0111[2] = {
    {TYPE_MSG_HEADER, 111},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473ED0
MsgField GAME_SERVER_0112[2] = {
    {TYPE_MSG_HEADER, 112},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00473EF0
MsgField GAME_SERVER_0113[5] = {
    {TYPE_MSG_HEADER, 113},
    {TYPE_WORD, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00473F20
MsgField GAME_SERVER_0114[9] = {
    {TYPE_MSG_HEADER, 114},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00473FA0
MsgField GAME_SERVER_0115[21] = {
    {TYPE_MSG_HEADER, 115},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 00474040
MsgField GAME_SERVER_0116[2] = {
    {TYPE_MSG_HEADER, 116},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00474060
MsgField GAME_SERVER_0117[2] = {
    {TYPE_MSG_HEADER, 117},
    {TYPE_WORD, 0},
};

// Handler Rva: 00474080
MsgField GAME_SERVER_0118[2] = {
    {TYPE_MSG_HEADER, 118},
    {TYPE_WORD, 0},
};

// Handler Rva: 004740A0
MsgField GAME_SERVER_0119[8] = {
    {TYPE_MSG_HEADER, 119},
    {TYPE_BYTE, 0},
    {TYPE_NESTED_STRUCT, 32},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474300
MsgField GAME_SERVER_0120[3] = {
    {TYPE_MSG_HEADER, 120},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474320
MsgField GAME_SERVER_0121[6] = {
    {TYPE_MSG_HEADER, 121},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 8},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474350
MsgField GAME_SERVER_0122[2] = {
    {TYPE_MSG_HEADER, 122},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474370
MsgField GAME_SERVER_0123[2] = {
    {TYPE_MSG_HEADER, 123},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474480
MsgField GAME_SERVER_0124[2] = {
    {TYPE_MSG_HEADER, 124},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474390
MsgField GAME_SERVER_0125[5] = {
    {TYPE_MSG_HEADER, 125},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 128},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00000000
MsgField GAME_SERVER_0126[1] = {
    {TYPE_MSG_HEADER, 126},
};

// Handler Rva: 004743D0
MsgField GAME_SERVER_0127[2] = {
    {TYPE_MSG_HEADER, 127},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 004743F0
MsgField GAME_SERVER_0128[2] = {
    {TYPE_MSG_HEADER, 128},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00474410
MsgField GAME_SERVER_0129[3] = {
    {TYPE_MSG_HEADER, 129},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 00474430
MsgField GAME_SERVER_0130[4] = {
    {TYPE_MSG_HEADER, 130},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004744A0
MsgField GAME_SERVER_0131[2] = {
    {TYPE_MSG_HEADER, 131},
    {TYPE_ARRAY_32, 16},
};

// Handler Rva: 004744C0
MsgField GAME_SERVER_0132[2] = {
    {TYPE_MSG_HEADER, 132},
    {TYPE_BYTE, 0},
};

// Handler Rva: 004744E0
MsgField GAME_SERVER_0133[2] = {
    {TYPE_MSG_HEADER, 133},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474500
MsgField GAME_SERVER_0134[19] = {
    {TYPE_MSG_HEADER, 134},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474530
MsgField GAME_SERVER_0135[2] = {
    {TYPE_MSG_HEADER, 135},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474550
MsgField GAME_SERVER_0136[4] = {
    {TYPE_MSG_HEADER, 136},
    {TYPE_BYTE, 0},
    {TYPE_VECT3, 0},
    {TYPE_VECT2, 0},
};

// Handler Rva: 00474570
MsgField GAME_SERVER_0137[2] = {
    {TYPE_MSG_HEADER, 137},
    {TYPE_ARRAY_32, 64},
};

// Handler Rva: 00474590
MsgField GAME_SERVER_0138[4] = {
    {TYPE_MSG_HEADER, 138},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004745C0
MsgField GAME_SERVER_0139[3] = {
    {TYPE_MSG_HEADER, 139},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004745F0
MsgField GAME_SERVER_0140[8] = {
    {TYPE_MSG_HEADER, 140},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 8},
};

// Handler Rva: 00474640
MsgField GAME_SERVER_0141[2] = {
    {TYPE_MSG_HEADER, 141},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474660
MsgField GAME_SERVER_0142[3] = {
    {TYPE_MSG_HEADER, 142},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474680
MsgField GAME_SERVER_0143[3] = {
    {TYPE_MSG_HEADER, 143},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004746A0
MsgField GAME_SERVER_0144[4] = {
    {TYPE_MSG_HEADER, 144},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
};

// Handler Rva: 00474740
MsgField GAME_SERVER_0145[3] = {
    {TYPE_MSG_HEADER, 145},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474770
MsgField GAME_SERVER_0146[3] = {
    {TYPE_MSG_HEADER, 146},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474790
MsgField GAME_SERVER_0147[6] = {
    {TYPE_MSG_HEADER, 147},
    {TYPE_ARRAY_32, 32},
    {TYPE_ARRAY_32, 32},
    {TYPE_ARRAY_32, 32},
    {TYPE_ARRAY_32, 32},
    {TYPE_ARRAY_32, 32},
};

// Handler Rva: 00474800
MsgField GAME_SERVER_0148[3] = {
    {TYPE_MSG_HEADER, 148},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474820
MsgField GAME_SERVER_0149[6] = {
    {TYPE_MSG_HEADER, 149},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474850
MsgField GAME_SERVER_0150[3] = {
    {TYPE_MSG_HEADER, 150},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 128},
};

// Handler Rva: 00474870
MsgField GAME_SERVER_0151[5] = {
    {TYPE_MSG_HEADER, 151},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004748A0
MsgField GAME_SERVER_0152[3] = {
    {TYPE_MSG_HEADER, 152},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 004748C0
MsgField GAME_SERVER_0153[3] = {
    {TYPE_MSG_HEADER, 153},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004748E0
MsgField GAME_SERVER_0154[3] = {
    {TYPE_MSG_HEADER, 154},
    {TYPE_AGENT_ID, 0},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 00474900
MsgField GAME_SERVER_0155[3] = {
    {TYPE_MSG_HEADER, 155},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474920
MsgField GAME_SERVER_0156[4] = {
    {TYPE_MSG_HEADER, 156},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474950
MsgField GAME_SERVER_0157[5] = {
    {TYPE_MSG_HEADER, 157},
    {TYPE_AGENT_ID, 0},
    {TYPE_STRING_16, 32},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 00474980
MsgField GAME_SERVER_0158[4] = {
    {TYPE_MSG_HEADER, 158},
    {TYPE_DWORD, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004749A0
MsgField GAME_SERVER_0159[5] = {
    {TYPE_MSG_HEADER, 159},
    {TYPE_DWORD, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004749D0
MsgField GAME_SERVER_0160[7] = {
    {TYPE_MSG_HEADER, 160},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474A20
MsgField GAME_SERVER_0161[4] = {
    {TYPE_MSG_HEADER, 161},
    {TYPE_DWORD, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474A50
MsgField GAME_SERVER_0162[5] = {
    {TYPE_MSG_HEADER, 162},
    {TYPE_DWORD, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474A80
MsgField GAME_SERVER_0163[8] = {
    {TYPE_MSG_HEADER, 163},
    {TYPE_AGENT_ID, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474AD0
MsgField GAME_SERVER_0164[3] = {
    {TYPE_MSG_HEADER, 164},
    {TYPE_AGENT_ID, 0},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 00474AF0
MsgField GAME_SERVER_0165[4] = {
    {TYPE_MSG_HEADER, 165},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474B10
MsgField GAME_SERVER_0166[4] = {
    {TYPE_MSG_HEADER, 166},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474B30
MsgField GAME_SERVER_0167[4] = {
    {TYPE_MSG_HEADER, 167},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474B70
MsgField GAME_SERVER_0168[3] = {
    {TYPE_MSG_HEADER, 168},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474B90
MsgField GAME_SERVER_0169[4] = {
    {TYPE_MSG_HEADER, 169},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474BB0
MsgField GAME_SERVER_0170[1] = {
    {TYPE_MSG_HEADER, 170},
};

// Handler Rva: 00474BC0
MsgField GAME_SERVER_0171[3] = {
    {TYPE_MSG_HEADER, 171},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474BE0
MsgField GAME_SERVER_0172[3] = {
    {TYPE_MSG_HEADER, 172},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474C00
MsgField GAME_SERVER_0173[3] = {
    {TYPE_MSG_HEADER, 173},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474C20
MsgField GAME_SERVER_0174[2] = {
    {TYPE_MSG_HEADER, 174},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 00474C40
MsgField GAME_SERVER_0175[3] = {
    {TYPE_MSG_HEADER, 175},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474CD0
MsgField GAME_SERVER_0176[3] = {
    {TYPE_MSG_HEADER, 176},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 00474C60
MsgField GAME_SERVER_0177[7] = {
    {TYPE_MSG_HEADER, 177},
    {TYPE_AGENT_ID, 0},
    {TYPE_AGENT_ID, 0},
    {TYPE_STRING_16, 32},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474C90
MsgField GAME_SERVER_0178[2] = {
    {TYPE_MSG_HEADER, 178},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00474CB0
MsgField GAME_SERVER_0179[3] = {
    {TYPE_MSG_HEADER, 179},
    {TYPE_AGENT_ID, 0},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 00474CF0
MsgField GAME_SERVER_0180[2] = {
    {TYPE_MSG_HEADER, 180},
    {TYPE_ARRAY_8, 256},
};

// Handler Rva: 00474D10
MsgField GAME_SERVER_0181[3] = {
    {TYPE_MSG_HEADER, 181},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474D30
MsgField GAME_SERVER_0182[5] = {
    {TYPE_MSG_HEADER, 182},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474D60
MsgField GAME_SERVER_0183[1] = {
    {TYPE_MSG_HEADER, 183},
};

// Handler Rva: 00474D70
MsgField GAME_SERVER_0184[3] = {
    {TYPE_MSG_HEADER, 184},
    {TYPE_STRING_16, 128},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474D90
MsgField GAME_SERVER_0185[1] = {
    {TYPE_MSG_HEADER, 185},
};

// Handler Rva: 00474DA0
MsgField GAME_SERVER_0186[4] = {
    {TYPE_MSG_HEADER, 186},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 128},
};

// Handler Rva: 00474DC0
MsgField GAME_SERVER_0187[2] = {
    {TYPE_MSG_HEADER, 187},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474DE0
MsgField GAME_SERVER_0188[3] = {
    {TYPE_MSG_HEADER, 188},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 128},
};

// Handler Rva: 00474E00
MsgField GAME_SERVER_0189[2] = {
    {TYPE_MSG_HEADER, 189},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00474E20
MsgField GAME_SERVER_0190[2] = {
    {TYPE_MSG_HEADER, 190},
    {TYPE_ARRAY_32, 64},
};

// Handler Rva: 00474E40
MsgField GAME_SERVER_0191[6] = {
    {TYPE_MSG_HEADER, 191},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 128},
    {TYPE_STRING_16, 128},
};

// Handler Rva: 00474E70
MsgField GAME_SERVER_0192[3] = {
    {TYPE_MSG_HEADER, 192},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474E90
MsgField GAME_SERVER_0193[3] = {
    {TYPE_MSG_HEADER, 193},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474EB0
MsgField GAME_SERVER_0194[3] = {
    {TYPE_MSG_HEADER, 194},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474ED0
MsgField GAME_SERVER_0195[2] = {
    {TYPE_MSG_HEADER, 195},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00474EF0
MsgField GAME_SERVER_0196[3] = {
    {TYPE_MSG_HEADER, 196},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 00474F10
MsgField GAME_SERVER_0197[3] = {
    {TYPE_MSG_HEADER, 197},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474F70
MsgField GAME_SERVER_0198[4] = {
    {TYPE_MSG_HEADER, 198},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 16},
};

// Handler Rva: 00474F30
MsgField GAME_SERVER_0199[4] = {
    {TYPE_MSG_HEADER, 199},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474F50
MsgField GAME_SERVER_0200[3] = {
    {TYPE_MSG_HEADER, 200},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00474FC0
MsgField GAME_SERVER_0201[3] = {
    {TYPE_MSG_HEADER, 201},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00474FE0
MsgField GAME_SERVER_0202[2] = {
    {TYPE_MSG_HEADER, 202},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00475000
MsgField GAME_SERVER_0203[2] = {
    {TYPE_MSG_HEADER, 203},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00475020
MsgField GAME_SERVER_0204[5] = {
    {TYPE_MSG_HEADER, 204},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00475050
MsgField GAME_SERVER_0205[3] = {
    {TYPE_MSG_HEADER, 205},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475070
MsgField GAME_SERVER_0206[3] = {
    {TYPE_MSG_HEADER, 206},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475090
MsgField GAME_SERVER_0207[2] = {
    {TYPE_MSG_HEADER, 207},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 004750B0
MsgField GAME_SERVER_0208[5] = {
    {TYPE_MSG_HEADER, 208},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004750E0
MsgField GAME_SERVER_0209[4] = {
    {TYPE_MSG_HEADER, 209},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475100
MsgField GAME_SERVER_0210[2] = {
    {TYPE_MSG_HEADER, 210},
    {TYPE_ARRAY_32, 128},
};

// Handler Rva: 00475120
MsgField GAME_SERVER_0211[1] = {
    {TYPE_MSG_HEADER, 211},
};

// Handler Rva: 00475150
MsgField GAME_SERVER_0212[1] = {
    {TYPE_MSG_HEADER, 212},
};

// Handler Rva: 00475160
MsgField GAME_SERVER_0213[4] = {
    {TYPE_MSG_HEADER, 213},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004744A0
MsgField GAME_SERVER_0214[2] = {
    {TYPE_MSG_HEADER, 214},
    {TYPE_ARRAY_32, 8},
};

// Handler Rva: 00475130
MsgField GAME_SERVER_0215[2] = {
    {TYPE_MSG_HEADER, 215},
    {TYPE_ARRAY_32, 4},
};

// Handler Rva: 00475180
MsgField GAME_SERVER_0216[5] = {
    {TYPE_MSG_HEADER, 216},
    {TYPE_AGENT_ID, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004751B0
MsgField GAME_SERVER_0217[5] = {
    {TYPE_MSG_HEADER, 217},
    {TYPE_AGENT_ID, 0},
    {TYPE_ARRAY_32, 8},
    {TYPE_ARRAY_32, 8},
    {TYPE_BYTE, 0},
};

// Handler Rva: 004751E0
MsgField GAME_SERVER_0218[2] = {
    {TYPE_MSG_HEADER, 218},
    {TYPE_ARRAY_32, 128},
};

// Handler Rva: 00475200
MsgField GAME_SERVER_0219[3] = {
    {TYPE_MSG_HEADER, 219},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475220
MsgField GAME_SERVER_0220[3] = {
    {TYPE_MSG_HEADER, 220},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475240
MsgField GAME_SERVER_0221[2] = {
    {TYPE_MSG_HEADER, 221},
    {TYPE_WORD, 0},
};

// Handler Rva: 00475260
MsgField GAME_SERVER_0222[3] = {
    {TYPE_MSG_HEADER, 222},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475280
MsgField GAME_SERVER_0223[3] = {
    {TYPE_MSG_HEADER, 223},
    {TYPE_BYTE, 0},
    {TYPE_ARRAY_32, 16},
};

// Handler Rva: 004752B0
MsgField GAME_SERVER_0224[2] = {
    {TYPE_MSG_HEADER, 224},
    {TYPE_BYTE, 0},
};

// Handler Rva: 004752D0
MsgField GAME_SERVER_0225[4] = {
    {TYPE_MSG_HEADER, 225},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004752D0
MsgField GAME_SERVER_0226[4] = {
    {TYPE_MSG_HEADER, 226},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004752F0
MsgField GAME_SERVER_0227[4] = {
    {TYPE_MSG_HEADER, 227},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475310
MsgField GAME_SERVER_0228[5] = {
    {TYPE_MSG_HEADER, 228},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475340
MsgField GAME_SERVER_0229[4] = {
    {TYPE_MSG_HEADER, 229},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475360
MsgField GAME_SERVER_0230[4] = {
    {TYPE_MSG_HEADER, 230},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475380
MsgField GAME_SERVER_0231[6] = {
    {TYPE_MSG_HEADER, 231},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004753B0
MsgField GAME_SERVER_0232[16] = {
    {TYPE_MSG_HEADER, 232},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004753D0
MsgField GAME_SERVER_0233[2] = {
    {TYPE_MSG_HEADER, 233},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004753F0
MsgField GAME_SERVER_0234[2] = {
    {TYPE_MSG_HEADER, 234},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475410
MsgField GAME_SERVER_0235[2] = {
    {TYPE_MSG_HEADER, 235},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475430
MsgField GAME_SERVER_0236[2] = {
    {TYPE_MSG_HEADER, 236},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475450
MsgField GAME_SERVER_0237[3] = {
    {TYPE_MSG_HEADER, 237},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475470
MsgField GAME_SERVER_0238[3] = {
    {TYPE_MSG_HEADER, 238},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475490
MsgField GAME_SERVER_0239[3] = {
    {TYPE_MSG_HEADER, 239},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004754B0
MsgField GAME_SERVER_0240[3] = {
    {TYPE_MSG_HEADER, 240},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004754D0
MsgField GAME_SERVER_0241[2] = {
    {TYPE_MSG_HEADER, 241},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004754F0
MsgField GAME_SERVER_0242[5] = {
    {TYPE_MSG_HEADER, 242},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 8},
};

// Handler Rva: 00475530
MsgField GAME_SERVER_0243[3] = {
    {TYPE_MSG_HEADER, 243},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475550
MsgField GAME_SERVER_0244[3] = {
    {TYPE_MSG_HEADER, 244},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475570
MsgField GAME_SERVER_0245[12] = {
    {TYPE_MSG_HEADER, 245},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 8},
};

// Handler Rva: 004755E0
MsgField GAME_SERVER_0246[3] = {
    {TYPE_MSG_HEADER, 246},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475600
MsgField GAME_SERVER_0247[2] = {
    {TYPE_MSG_HEADER, 247},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 00475130
MsgField GAME_SERVER_0248[2] = {
    {TYPE_MSG_HEADER, 248},
    {TYPE_ARRAY_32, 16},
};

// Handler Rva: 00475620
MsgField GAME_SERVER_0249[2] = {
    {TYPE_MSG_HEADER, 249},
    {TYPE_ARRAY_32, 32},
};

// Handler Rva: 00475640
MsgField GAME_SERVER_0250[4] = {
    {TYPE_MSG_HEADER, 250},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475660
MsgField GAME_SERVER_0251[3] = {
    {TYPE_MSG_HEADER, 251},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00475680
MsgField GAME_SERVER_0252[3] = {
    {TYPE_MSG_HEADER, 252},
    {TYPE_STRING_16, 32},
    {TYPE_BYTE, 0},
};

// Handler Rva: 004774D0
MsgField GAME_SERVER_0253[1] = {
    {TYPE_MSG_HEADER, 253},
};

// Handler Rva: 004774E0
MsgField GAME_SERVER_0254[3] = {
    {TYPE_MSG_HEADER, 254},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00477500
MsgField GAME_SERVER_0255[2] = {
    {TYPE_MSG_HEADER, 255},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00477520
MsgField GAME_SERVER_0256[2] = {
    {TYPE_MSG_HEADER, 256},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00477540
MsgField GAME_SERVER_0257[4] = {
    {TYPE_MSG_HEADER, 257},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_STRING_16, 80},
};

// Handler Rva: 00477560
MsgField GAME_SERVER_0258[1] = {
    {TYPE_MSG_HEADER, 258},
};

// Handler Rva: 00477570
MsgField GAME_SERVER_0259[2] = {
    {TYPE_MSG_HEADER, 259},
    {TYPE_ARRAY_8, 1024},
};

// Handler Rva: 00477590
MsgField GAME_SERVER_0260[1] = {
    {TYPE_MSG_HEADER, 260},
};

// Handler Rva: 003AE880
MsgField GAME_SERVER_0261[2] = {
    {TYPE_MSG_HEADER, 261},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00477640
MsgField GAME_SERVER_0262[6] = {
    {TYPE_MSG_HEADER, 262},
    {TYPE_AGENT_ID, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 00477690
MsgField GAME_SERVER_0263[2] = {
    {TYPE_MSG_HEADER, 263},
    {TYPE_AGENT_ID, 0},
};

// Handler Rva: 004776B0
MsgField GAME_SERVER_0264[3] = {
    {TYPE_MSG_HEADER, 264},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 004776D0
MsgField GAME_SERVER_0265[5] = {
    {TYPE_MSG_HEADER, 265},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 122},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 00477710
MsgField GAME_SERVER_0266[2] = {
    {TYPE_MSG_HEADER, 266},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 00477860
MsgField GAME_SERVER_0267[2] = {
    {TYPE_MSG_HEADER, 267},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004778C0
MsgField GAME_SERVER_0268[2] = {
    {TYPE_MSG_HEADER, 268},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004778E0
MsgField GAME_SERVER_0269[4] = {
    {TYPE_MSG_HEADER, 269},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00477900
MsgField GAME_SERVER_0270[3] = {
    {TYPE_MSG_HEADER, 270},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00477920
MsgField GAME_SERVER_0271[3] = {
    {TYPE_MSG_HEADER, 271},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00477940
MsgField GAME_SERVER_0272[4] = {
    {TYPE_MSG_HEADER, 272},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 00477960
MsgField GAME_SERVER_0273[3] = {
    {TYPE_MSG_HEADER, 273},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00477980
MsgField GAME_SERVER_0274[3] = {
    {TYPE_MSG_HEADER, 274},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 004779A0
MsgField GAME_SERVER_0275[3] = {
    {TYPE_MSG_HEADER, 275},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 004779C0
MsgField GAME_SERVER_0276[3] = {
    {TYPE_MSG_HEADER, 276},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003BE9F0
MsgField GAME_SERVER_0277[5] = {
    {TYPE_MSG_HEADER, 277},
    {TYPE_WORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003BEA80
MsgField GAME_SERVER_0278[2] = {
    {TYPE_MSG_HEADER, 278},
    {TYPE_BLOB, 16},
};

// Handler Rva: 003BECD0
MsgField GAME_SERVER_0279[4] = {
    {TYPE_MSG_HEADER, 279},
    {TYPE_WORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BEDC0
MsgField GAME_SERVER_0280[1] = {
    {TYPE_MSG_HEADER, 280},
};

// Handler Rva: 003BEAE0
MsgField GAME_SERVER_0281[14] = {
    {TYPE_MSG_HEADER, 281},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 6},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BEB60
MsgField GAME_SERVER_0282[3] = {
    {TYPE_MSG_HEADER, 282},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BEBD0
MsgField GAME_SERVER_0283[1] = {
    {TYPE_MSG_HEADER, 283},
};

// Handler Rva: 003BEB90
MsgField GAME_SERVER_0284[2] = {
    {TYPE_MSG_HEADER, 284},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BEC10
MsgField GAME_SERVER_0285[3] = {
    {TYPE_MSG_HEADER, 285},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003BEC90
MsgField GAME_SERVER_0286[3] = {
    {TYPE_MSG_HEADER, 286},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
};

// Handler Rva: 003BEE40
MsgField GAME_SERVER_0287[16] = {
    {TYPE_MSG_HEADER, 287},
    {TYPE_WORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 6},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BEF10
MsgField GAME_SERVER_0288[20] = {
    {TYPE_MSG_HEADER, 288},
    {TYPE_WORD, 0},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 6},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BF000
MsgField GAME_SERVER_0289[4] = {
    {TYPE_MSG_HEADER, 289},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003BF2E0
MsgField GAME_SERVER_0290[8] = {
    {TYPE_MSG_HEADER, 290},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 6},
    {TYPE_STRING_16, 20},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BF330
MsgField GAME_SERVER_0291[3] = {
    {TYPE_MSG_HEADER, 291},
    {TYPE_BLOB, 16},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BF080
MsgField GAME_SERVER_0292[3] = {
    {TYPE_MSG_HEADER, 292},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003BF100
MsgField GAME_SERVER_0293[3] = {
    {TYPE_MSG_HEADER, 293},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BF370
MsgField GAME_SERVER_0294[10] = {
    {TYPE_MSG_HEADER, 294},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 20},
    {TYPE_STRING_16, 64},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BF3E0
MsgField GAME_SERVER_0295[2] = {
    {TYPE_MSG_HEADER, 295},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 003BF420
MsgField GAME_SERVER_0296[2] = {
    {TYPE_MSG_HEADER, 296},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 003BF480
MsgField GAME_SERVER_0297[2] = {
    {TYPE_MSG_HEADER, 297},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 003BF590
MsgField GAME_SERVER_0298[3] = {
    {TYPE_MSG_HEADER, 298},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
};

// Handler Rva: 003BF5F0
MsgField GAME_SERVER_0299[3] = {
    {TYPE_MSG_HEADER, 299},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003BF660
MsgField GAME_SERVER_0300[2] = {
    {TYPE_MSG_HEADER, 300},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BF180
MsgField GAME_SERVER_0301[2] = {
    {TYPE_MSG_HEADER, 301},
    {TYPE_WORD, 0},
};

// Handler Rva: 003BF6B0
MsgField GAME_SERVER_0302[3] = {
    {TYPE_MSG_HEADER, 302},
    {TYPE_STRING_16, 256},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 003BF1C0
MsgField GAME_SERVER_0303[3] = {
    {TYPE_MSG_HEADER, 303},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003BF240
MsgField GAME_SERVER_0304[1] = {
    {TYPE_MSG_HEADER, 304},
};

// Handler Rva: 003BF750
MsgField GAME_SERVER_0305[3] = {
    {TYPE_MSG_HEADER, 305},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003BF790
MsgField GAME_SERVER_0306[2] = {
    {TYPE_MSG_HEADER, 306},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003BF2C0
MsgField GAME_SERVER_0307[1] = {
    {TYPE_MSG_HEADER, 307},
};

// Handler Rva: 003C3FE0
MsgField GAME_SERVER_0308[4] = {
    {TYPE_MSG_HEADER, 308},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4090
MsgField GAME_SERVER_0309[2] = {
    {TYPE_MSG_HEADER, 309},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C40C0
MsgField GAME_SERVER_0310[3] = {
    {TYPE_MSG_HEADER, 310},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4100
MsgField GAME_SERVER_0311[7] = {
    {TYPE_MSG_HEADER, 311},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 8},
    {TYPE_STRING_16, 8},
};

// Handler Rva: 003C4140
MsgField GAME_SERVER_0312[3] = {
    {TYPE_MSG_HEADER, 312},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4290
MsgField GAME_SERVER_0313[3] = {
    {TYPE_MSG_HEADER, 313},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
};

// Handler Rva: 003C4340
MsgField GAME_SERVER_0314[3] = {
    {TYPE_MSG_HEADER, 314},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4370
MsgField GAME_SERVER_0315[3] = {
    {TYPE_MSG_HEADER, 315},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 0006B670
MsgField GAME_SERVER_0316[4] = {
    {TYPE_MSG_HEADER, 316},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C43B0
MsgField GAME_SERVER_0317[5] = {
    {TYPE_MSG_HEADER, 317},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C4470
MsgField GAME_SERVER_0318[7] = {
    {TYPE_MSG_HEADER, 318},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4550
MsgField GAME_SERVER_0319[3] = {
    {TYPE_MSG_HEADER, 319},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C45A0
MsgField GAME_SERVER_0320[3] = {
    {TYPE_MSG_HEADER, 320},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C45F0
MsgField GAME_SERVER_0321[2] = {
    {TYPE_MSG_HEADER, 321},
    {TYPE_WORD, 0},
};

// Handler Rva: 003C4620
MsgField GAME_SERVER_0322[3] = {
    {TYPE_MSG_HEADER, 322},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003C4690
MsgField GAME_SERVER_0323[3] = {
    {TYPE_MSG_HEADER, 323},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C46E0
MsgField GAME_SERVER_0324[2] = {
    {TYPE_MSG_HEADER, 324},
    {TYPE_WORD, 0},
};

// Handler Rva: 003C4730
MsgField GAME_SERVER_0325[6] = {
    {TYPE_MSG_HEADER, 325},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C47F0
MsgField GAME_SERVER_0326[5] = {
    {TYPE_MSG_HEADER, 326},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4880
MsgField GAME_SERVER_0327[3] = {
    {TYPE_MSG_HEADER, 327},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C48D0
MsgField GAME_SERVER_0328[4] = {
    {TYPE_MSG_HEADER, 328},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 00000000
MsgField GAME_SERVER_0329[1] = {
    {TYPE_MSG_HEADER, 329},
};

// Handler Rva: 003C4950
MsgField GAME_SERVER_0330[5] = {
    {TYPE_MSG_HEADER, 330},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C4A00
MsgField GAME_SERVER_0331[4] = {
    {TYPE_MSG_HEADER, 331},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C4A60
MsgField GAME_SERVER_0332[3] = {
    {TYPE_MSG_HEADER, 332},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4AE0
MsgField GAME_SERVER_0333[3] = {
    {TYPE_MSG_HEADER, 333},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003C4B60
MsgField GAME_SERVER_0334[3] = {
    {TYPE_MSG_HEADER, 334},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4BB0
MsgField GAME_SERVER_0335[3] = {
    {TYPE_MSG_HEADER, 335},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4C00
MsgField GAME_SERVER_0336[3] = {
    {TYPE_MSG_HEADER, 336},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4C70
MsgField GAME_SERVER_0337[4] = {
    {TYPE_MSG_HEADER, 337},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4D20
MsgField GAME_SERVER_0338[5] = {
    {TYPE_MSG_HEADER, 338},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C4DD0
MsgField GAME_SERVER_0339[4] = {
    {TYPE_MSG_HEADER, 339},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C4E10
MsgField GAME_SERVER_0340[3] = {
    {TYPE_MSG_HEADER, 340},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4E70
MsgField GAME_SERVER_0341[2] = {
    {TYPE_MSG_HEADER, 341},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4EF0
MsgField GAME_SERVER_0342[5] = {
    {TYPE_MSG_HEADER, 342},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C4EC0
MsgField GAME_SERVER_0343[3] = {
    {TYPE_MSG_HEADER, 343},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C4F20
MsgField GAME_SERVER_0344[3] = {
    {TYPE_MSG_HEADER, 344},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003C4F90
MsgField GAME_SERVER_0345[3] = {
    {TYPE_MSG_HEADER, 345},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C4FE0
MsgField GAME_SERVER_0346[3] = {
    {TYPE_MSG_HEADER, 346},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C5010
MsgField GAME_SERVER_0347[10] = {
    {TYPE_MSG_HEADER, 347},
    {TYPE_WORD, 0},
    {TYPE_NESTED_STRUCT, 7},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003C50C0
MsgField GAME_SERVER_0348[3] = {
    {TYPE_MSG_HEADER, 348},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C53C0
MsgField GAME_SERVER_0349[10] = {
    {TYPE_MSG_HEADER, 349},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C5470
MsgField GAME_SERVER_0350[10] = {
    {TYPE_MSG_HEADER, 350},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C5380
MsgField GAME_SERVER_0351[7] = {
    {TYPE_MSG_HEADER, 351},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
    {TYPE_NESTED_STRUCT, 64},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C5190
MsgField GAME_SERVER_0352[15] = {
    {TYPE_MSG_HEADER, 352},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
    {TYPE_NESTED_STRUCT, 64},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C5270
MsgField GAME_SERVER_0353[15] = {
    {TYPE_MSG_HEADER, 353},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
    {TYPE_NESTED_STRUCT, 64},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C5550
MsgField GAME_SERVER_0354[4] = {
    {TYPE_MSG_HEADER, 354},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 3},
};

// Handler Rva: 003C55B0
MsgField GAME_SERVER_0355[1] = {
    {TYPE_MSG_HEADER, 355},
};

// Handler Rva: 003C55D0
MsgField GAME_SERVER_0356[1] = {
    {TYPE_MSG_HEADER, 356},
};

// Handler Rva: 003C55F0
MsgField GAME_SERVER_0357[1] = {
    {TYPE_MSG_HEADER, 357},
};

// Handler Rva: 003C5610
MsgField GAME_SERVER_0358[2] = {
    {TYPE_MSG_HEADER, 358},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003C5640
MsgField GAME_SERVER_0359[3] = {
    {TYPE_MSG_HEADER, 359},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003C5660
MsgField GAME_SERVER_0360[3] = {
    {TYPE_MSG_HEADER, 360},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003C56A0
MsgField GAME_SERVER_0361[4] = {
    {TYPE_MSG_HEADER, 361},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_ARRAY_32, 3},
};

// Handler Rva: 003C5700
MsgField GAME_SERVER_0362[1] = {
    {TYPE_MSG_HEADER, 362},
};

// Handler Rva: 003C5720
MsgField GAME_SERVER_0363[1] = {
    {TYPE_MSG_HEADER, 363},
};

// Handler Rva: 003C5740
MsgField GAME_SERVER_0364[2] = {
    {TYPE_MSG_HEADER, 364},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CC3D0
MsgField GAME_SERVER_0365[2] = {
    {TYPE_MSG_HEADER, 365},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CC460
MsgField GAME_SERVER_0366[1] = {
    {TYPE_MSG_HEADER, 366},
};

// Handler Rva: 003CC480
MsgField GAME_SERVER_0367[1] = {
    {TYPE_MSG_HEADER, 367},
};

// Handler Rva: 003CC4A0
MsgField GAME_SERVER_0368[24] = {
    {TYPE_MSG_HEADER, 368},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CC590
MsgField GAME_SERVER_0369[2] = {
    {TYPE_MSG_HEADER, 369},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CC5B0
MsgField GAME_SERVER_0370[2] = {
    {TYPE_MSG_HEADER, 370},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CC5D0
MsgField GAME_SERVER_0371[24] = {
    {TYPE_MSG_HEADER, 371},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CC6C0
MsgField GAME_SERVER_0372[5] = {
    {TYPE_MSG_HEADER, 372},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 64},
};

// Handler Rva: 003CC6F0
MsgField GAME_SERVER_0373[3] = {
    {TYPE_MSG_HEADER, 373},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003CC710
MsgField GAME_SERVER_0374[5] = {
    {TYPE_MSG_HEADER, 374},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 64},
};

// Handler Rva: 003CC740
MsgField GAME_SERVER_0375[21] = {
    {TYPE_MSG_HEADER, 375},
    {TYPE_WORD, 0},
    {TYPE_STRING_16, 64},
    {TYPE_STRING_16, 6},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CC860
MsgField GAME_SERVER_0376[4] = {
    {TYPE_MSG_HEADER, 376},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CC890
MsgField GAME_SERVER_0377[2] = {
    {TYPE_MSG_HEADER, 377},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CC930
MsgField GAME_SERVER_0378[1] = {
    {TYPE_MSG_HEADER, 378},
};

// Handler Rva: 003CC9B0
MsgField GAME_SERVER_0379[5] = {
    {TYPE_MSG_HEADER, 379},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CC9F0
MsgField GAME_SERVER_0380[2] = {
    {TYPE_MSG_HEADER, 380},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 003CCA20
MsgField GAME_SERVER_0381[1] = {
    {TYPE_MSG_HEADER, 381},
};

// Handler Rva: 003CCA40
MsgField GAME_SERVER_0382[2] = {
    {TYPE_MSG_HEADER, 382},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CCA70
MsgField GAME_SERVER_0383[5] = {
    {TYPE_MSG_HEADER, 383},
    {TYPE_STRING_16, 122},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CCAB0
MsgField GAME_SERVER_0384[2] = {
    {TYPE_MSG_HEADER, 384},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CCAE0
MsgField GAME_SERVER_0385[3] = {
    {TYPE_MSG_HEADER, 385},
    {TYPE_STRING_16, 122},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CCB30
MsgField GAME_SERVER_0386[3] = {
    {TYPE_MSG_HEADER, 386},
    {TYPE_STRING_16, 128},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CCB50
MsgField GAME_SERVER_0387[3] = {
    {TYPE_MSG_HEADER, 387},
    {TYPE_STRING_16, 128},
    {TYPE_STRING_16, 256},
};

// Handler Rva: 003CCB70
MsgField GAME_SERVER_0388[3] = {
    {TYPE_MSG_HEADER, 388},
    {TYPE_STRING_16, 128},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CCB10
MsgField GAME_SERVER_0389[1] = {
    {TYPE_MSG_HEADER, 389},
};

// Handler Rva: 003CCB90
MsgField GAME_SERVER_0390[2] = {
    {TYPE_MSG_HEADER, 390},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CCBB0
MsgField GAME_SERVER_0391[5] = {
    {TYPE_MSG_HEADER, 391},
    {TYPE_BLOB, 16},
    {TYPE_STRING_16, 20},
    {TYPE_WORD, 0},
    {TYPE_ARRAY_8, 1024},
};

// Handler Rva: 003CCC20
MsgField GAME_SERVER_0392[1] = {
    {TYPE_MSG_HEADER, 392},
};

// Handler Rva: 003CCC50
MsgField GAME_SERVER_0393[1] = {
    {TYPE_MSG_HEADER, 393},
};

// Handler Rva: 003CCC70
MsgField GAME_SERVER_0394[2] = {
    {TYPE_MSG_HEADER, 394},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CCCA0
MsgField GAME_SERVER_0395[2] = {
    {TYPE_MSG_HEADER, 395},
    {TYPE_BYTE, 0},
};

// Handler Rva: 0006B670
MsgField GAME_SERVER_0396[2] = {
    {TYPE_MSG_HEADER, 396},
    {TYPE_WORD, 0},
};

// Handler Rva: 003CCCD0
MsgField GAME_SERVER_0397[1] = {
    {TYPE_MSG_HEADER, 397},
};

// Handler Rva: 003CCD10
MsgField GAME_SERVER_0398[4] = {
    {TYPE_MSG_HEADER, 398},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CCE00
MsgField GAME_SERVER_0399[3] = {
    {TYPE_MSG_HEADER, 399},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CCD60
MsgField GAME_SERVER_0400[2] = {
    {TYPE_MSG_HEADER, 400},
    {TYPE_WORD, 0},
};

// Handler Rva: 003CCE30
MsgField GAME_SERVER_0401[9] = {
    {TYPE_MSG_HEADER, 401},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 64},
    {TYPE_NESTED_STRUCT, 8},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 003CCEC0
MsgField GAME_SERVER_0402[3] = {
    {TYPE_MSG_HEADER, 402},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CCEF0
MsgField GAME_SERVER_0403[2] = {
    {TYPE_MSG_HEADER, 403},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CCF30
MsgField GAME_SERVER_0404[7] = {
    {TYPE_MSG_HEADER, 404},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BLOB, 8},
};

// Handler Rva: 003CCFE0
MsgField GAME_SERVER_0405[2] = {
    {TYPE_MSG_HEADER, 405},
    {TYPE_ARRAY_8, 1024},
};

// Handler Rva: 003CD010
MsgField GAME_SERVER_0406[4] = {
    {TYPE_MSG_HEADER, 406},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD040
MsgField GAME_SERVER_0407[2] = {
    {TYPE_MSG_HEADER, 407},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CD070
MsgField GAME_SERVER_0408[7] = {
    {TYPE_MSG_HEADER, 408},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CD0F0
MsgField GAME_SERVER_0409[3] = {
    {TYPE_MSG_HEADER, 409},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD110
MsgField GAME_SERVER_0410[2] = {
    {TYPE_MSG_HEADER, 410},
    {TYPE_WORD, 0},
};

// Handler Rva: 003CD130
MsgField GAME_SERVER_0411[2] = {
    {TYPE_MSG_HEADER, 411},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 003CD160
MsgField GAME_SERVER_0412[18] = {
    {TYPE_MSG_HEADER, 412},
    {TYPE_DWORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_NESTED_STRUCT, 2},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 48},
};

// Handler Rva: 003CD180
MsgField GAME_SERVER_0413[1] = {
    {TYPE_MSG_HEADER, 413},
};

// Handler Rva: 003CD1B0
MsgField GAME_SERVER_0414[4] = {
    {TYPE_MSG_HEADER, 414},
    {TYPE_NESTED_STRUCT, 16},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD1E0
MsgField GAME_SERVER_0415[9] = {
    {TYPE_MSG_HEADER, 415},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 122},
    {TYPE_STRING_16, 122},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD270
MsgField GAME_SERVER_0416[2] = {
    {TYPE_MSG_HEADER, 416},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CD2A0
MsgField GAME_SERVER_0417[7] = {
    {TYPE_MSG_HEADER, 417},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD320
MsgField GAME_SERVER_0418[4] = {
    {TYPE_MSG_HEADER, 418},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 122},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 003CD390
MsgField GAME_SERVER_0419[14] = {
    {TYPE_MSG_HEADER, 419},
    {TYPE_AGENT_ID, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_VECT2, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 20},
    {TYPE_BLOB, 8},
};

// Handler Rva: 003CD4B0
MsgField GAME_SERVER_0420[7] = {
    {TYPE_MSG_HEADER, 420},
    {TYPE_BLOB, 24},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD600
MsgField GAME_SERVER_0421[10] = {
    {TYPE_MSG_HEADER, 421},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 122},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 122},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CD690
MsgField GAME_SERVER_0422[2] = {
    {TYPE_MSG_HEADER, 422},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CD5A0
MsgField GAME_SERVER_0423[3] = {
    {TYPE_MSG_HEADER, 423},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD5D0
MsgField GAME_SERVER_0424[3] = {
    {TYPE_MSG_HEADER, 424},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD6E0
MsgField GAME_SERVER_0425[2] = {
    {TYPE_MSG_HEADER, 425},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003CD760
MsgField GAME_SERVER_0426[2] = {
    {TYPE_MSG_HEADER, 426},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD790
MsgField GAME_SERVER_0427[5] = {
    {TYPE_MSG_HEADER, 427},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 003CD7C0
MsgField GAME_SERVER_0428[7] = {
    {TYPE_MSG_HEADER, 428},
    {TYPE_WORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 20},
};

// Handler Rva: 003CD800
MsgField GAME_SERVER_0429[13] = {
    {TYPE_MSG_HEADER, 429},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 6},
};

// Handler Rva: 003CD880
MsgField GAME_SERVER_0430[2] = {
    {TYPE_MSG_HEADER, 430},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 003CD920
MsgField GAME_SERVER_0431[2] = {
    {TYPE_MSG_HEADER, 431},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003CD950
MsgField GAME_SERVER_0432[4] = {
    {TYPE_MSG_HEADER, 432},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003D4970
MsgField GAME_SERVER_0433[3] = {
    {TYPE_MSG_HEADER, 433},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D49A0
MsgField GAME_SERVER_0434[7] = {
    {TYPE_MSG_HEADER, 434},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 32},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003D49D0
MsgField GAME_SERVER_0435[1] = {
    {TYPE_MSG_HEADER, 435},
};

// Handler Rva: 003D49F0
MsgField GAME_SERVER_0436[7] = {
    {TYPE_MSG_HEADER, 436},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 32},
    {TYPE_DWORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003D4A20
MsgField GAME_SERVER_0437[1] = {
    {TYPE_MSG_HEADER, 437},
};

// Handler Rva: 003D4A40
MsgField GAME_SERVER_0438[2] = {
    {TYPE_MSG_HEADER, 438},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4A60
MsgField GAME_SERVER_0439[2] = {
    {TYPE_MSG_HEADER, 439},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4A80
MsgField GAME_SERVER_0440[1] = {
    {TYPE_MSG_HEADER, 440},
};

// Handler Rva: 003D4AA0
MsgField GAME_SERVER_0441[1] = {
    {TYPE_MSG_HEADER, 441},
};

// Handler Rva: 003D4AC0
MsgField GAME_SERVER_0442[4] = {
    {TYPE_MSG_HEADER, 442},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 003D4AF0
MsgField GAME_SERVER_0443[2] = {
    {TYPE_MSG_HEADER, 443},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4B10
MsgField GAME_SERVER_0444[2] = {
    {TYPE_MSG_HEADER, 444},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003D4B30
MsgField GAME_SERVER_0445[2] = {
    {TYPE_MSG_HEADER, 445},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4B50
MsgField GAME_SERVER_0446[6] = {
    {TYPE_MSG_HEADER, 446},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_STRING_16, 20},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4B80
MsgField GAME_SERVER_0447[3] = {
    {TYPE_MSG_HEADER, 447},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4BB0
MsgField GAME_SERVER_0448[2] = {
    {TYPE_MSG_HEADER, 448},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4BD0
MsgField GAME_SERVER_0449[6] = {
    {TYPE_MSG_HEADER, 449},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4C00
MsgField GAME_SERVER_0450[4] = {
    {TYPE_MSG_HEADER, 450},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4CD0
MsgField GAME_SERVER_0451[2] = {
    {TYPE_MSG_HEADER, 451},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4CB0
MsgField GAME_SERVER_0452[2] = {
    {TYPE_MSG_HEADER, 452},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4C50
MsgField GAME_SERVER_0453[2] = {
    {TYPE_MSG_HEADER, 453},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4C30
MsgField GAME_SERVER_0454[2] = {
    {TYPE_MSG_HEADER, 454},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4C90
MsgField GAME_SERVER_0455[2] = {
    {TYPE_MSG_HEADER, 455},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4C70
MsgField GAME_SERVER_0456[2] = {
    {TYPE_MSG_HEADER, 456},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4CF0
MsgField GAME_SERVER_0457[2] = {
    {TYPE_MSG_HEADER, 457},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4D10
MsgField GAME_SERVER_0458[4] = {
    {TYPE_MSG_HEADER, 458},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4D40
MsgField GAME_SERVER_0459[3] = {
    {TYPE_MSG_HEADER, 459},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4D70
MsgField GAME_SERVER_0460[3] = {
    {TYPE_MSG_HEADER, 460},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4DA0
MsgField GAME_SERVER_0461[3] = {
    {TYPE_MSG_HEADER, 461},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4DD0
MsgField GAME_SERVER_0462[3] = {
    {TYPE_MSG_HEADER, 462},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4E00
MsgField GAME_SERVER_0463[3] = {
    {TYPE_MSG_HEADER, 463},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4E30
MsgField GAME_SERVER_0464[4] = {
    {TYPE_MSG_HEADER, 464},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4E60
MsgField GAME_SERVER_0465[2] = {
    {TYPE_MSG_HEADER, 465},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D4E80
MsgField GAME_SERVER_0466[2] = {
    {TYPE_MSG_HEADER, 466},
    {TYPE_WORD, 0},
};

// Handler Rva: 003CD130
MsgField GAME_SERVER_0467[2] = {
    {TYPE_MSG_HEADER, 467},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 003D4EA0
MsgField GAME_SERVER_0468[1] = {
    {TYPE_MSG_HEADER, 468},
};

// Handler Rva: 003D4EC0
MsgField GAME_SERVER_0469[2] = {
    {TYPE_MSG_HEADER, 469},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D4EE0
MsgField GAME_SERVER_0470[6] = {
    {TYPE_MSG_HEADER, 470},
    {TYPE_ARRAY_16, 16},
    {TYPE_NESTED_STRUCT, 16},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003D5160
MsgField GAME_SERVER_0471[1] = {
    {TYPE_MSG_HEADER, 471},
};

// Handler Rva: 003D5180
MsgField GAME_SERVER_0472[4] = {
    {TYPE_MSG_HEADER, 472},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 003D51C0
MsgField GAME_SERVER_0473[3] = {
    {TYPE_MSG_HEADER, 473},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D5230
MsgField GAME_SERVER_0474[3] = {
    {TYPE_MSG_HEADER, 474},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D5260
MsgField GAME_SERVER_0475[3] = {
    {TYPE_MSG_HEADER, 475},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D5290
MsgField GAME_SERVER_0476[14] = {
    {TYPE_MSG_HEADER, 476},
    {TYPE_WORD, 0},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_STRING_16, 32},
    {TYPE_STRING_16, 20},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
    {TYPE_DWORD, 0},
};

// Handler Rva: 003D5300
MsgField GAME_SERVER_0477[2] = {
    {TYPE_MSG_HEADER, 477},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D5330
MsgField GAME_SERVER_0478[2] = {
    {TYPE_MSG_HEADER, 478},
    {TYPE_WORD, 0},
};

// Handler Rva: 003D5360
MsgField GAME_SERVER_0479[4] = {
    {TYPE_MSG_HEADER, 479},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D5390
MsgField GAME_SERVER_0480[4] = {
    {TYPE_MSG_HEADER, 480},
    {TYPE_WORD, 0},
    {TYPE_BYTE, 0},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D53C0
MsgField GAME_SERVER_0481[2] = {
    {TYPE_MSG_HEADER, 481},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D53E0
MsgField GAME_SERVER_0482[2] = {
    {TYPE_MSG_HEADER, 482},
    {TYPE_STRING_16, 122},
};

// Handler Rva: 003D5410
MsgField GAME_SERVER_0483[2] = {
    {TYPE_MSG_HEADER, 483},
    {TYPE_BYTE, 0},
};

// Handler Rva: 003D5430
MsgField GAME_SERVER_0484[2] = {
    {TYPE_MSG_HEADER, 484},
    {TYPE_BYTE, 0},
};

MsgFormat AUTH_CLIENT_FORMATS[57] = {
// header | field_count | fields | max_size | name
    {0  , 2  , AUTH_CLIENT_0000, 6  , 0},
    {1  , 3  , AUTH_CLIENT_0001, 130, 0},
    {2  , 3  , AUTH_CLIENT_0002, 22 , 0},
    {3  , 5  , AUTH_CLIENT_0003, 206, 0},
    {4  , 1  , AUTH_CLIENT_0004, 2  , 0},
    {5  , 6  , AUTH_CLIENT_0005, 178, 0},
    {6  , 3  , AUTH_CLIENT_0006, 10 , 0},
    {7  , 3  , AUTH_CLIENT_0007, 46 , 0},
    {8  , 3  , AUTH_CLIENT_0008, 46 , 0},
    {9  , 4  , AUTH_CLIENT_0009, 114, 0},
    {10 , 3  , AUTH_CLIENT_0010, 46 , 0},
    {11 , 5  , AUTH_CLIENT_0011, 18 , 0},
    {12 , 4  , AUTH_CLIENT_0012, 26 , 0},
    {13 , 2  , AUTH_CLIENT_0013, 6  , 0},
    {14 , 2  , AUTH_CLIENT_0014, 6  , 0},
    {15 , 3  , AUTH_CLIENT_0015, 114, 0},
    {16 , 2  , AUTH_CLIENT_0016, 6  , 0},
    {17 , 2  , AUTH_CLIENT_0017, 514, 0},
    {18 , 1  , AUTH_CLIENT_0018, 2  , 0},
    {19 , 1  , AUTH_CLIENT_0019, 2  , 0},
    {20 , 6  , AUTH_CLIENT_0020, 778, 0},
    {21 , 2  , AUTH_CLIENT_0021, 6  , 0},
    {22 , 2  , AUTH_CLIENT_0022, 6  , 0},
    {23 , 2  , AUTH_CLIENT_0023, 6  , 0},
    {24 , 3  , AUTH_CLIENT_0024, 134, 0},
    {25 , 5  , AUTH_CLIENT_0025, 50 , 0},
    {26 , 5  , AUTH_CLIENT_0026, 90 , 0},
    {27 , 4  , AUTH_CLIENT_0027, 62 , 0},
    {28 , 3  , AUTH_CLIENT_0028, 58 , 0},
    {29 , 3  , AUTH_CLIENT_0029, 10 , 0},
    {30 , 4  , AUTH_CLIENT_0030, 62 , 0},
    {31 , 23 , AUTH_CLIENT_0031, 1386, 0},
    {32 , 3  , AUTH_CLIENT_0032, 522, 0},
    {33 , 3  , AUTH_CLIENT_0033, 10 , 0},
    {34 , 2  , AUTH_CLIENT_0034, 6  , 0},
    {35 , 2  , AUTH_CLIENT_0035, 6  , 0},
    {36 , 4  , AUTH_CLIENT_0036, 166, 0},
    {37 , 7  , AUTH_CLIENT_0037, 38 , 0},
    {38 , 2  , AUTH_CLIENT_0038, 3  , 0},
    {39 , 2  , AUTH_CLIENT_0039, 6  , 0},
    {40 , 9  , AUTH_CLIENT_0040, 382, 0},
    {41 , 7  , AUTH_CLIENT_0041, 26 , 0},
    {42 , 4  , AUTH_CLIENT_0042, 86 , 0},
    {43 , 4  , AUTH_CLIENT_0043, 1038, 0},
    {44 , 23 , AUTH_CLIENT_0044, 1898, 0},
    {45 , 2  , AUTH_CLIENT_0045, 6  , 0},
    {46 , 1  , AUTH_CLIENT_0046, 2  , 0},
    {47 , 1  , AUTH_CLIENT_0047, 2  , 0},
    {48 , 2  , AUTH_CLIENT_0048, 6  , 0},
    {49 , 2  , AUTH_CLIENT_0049, 6  , 0},
    {50 , 3  , AUTH_CLIENT_0050, 26 , 0},
    {51 , 6  , AUTH_CLIENT_0051, 54 , 0},
    {52 , 4  , AUTH_CLIENT_0052, 14 , 0},
    {53 , 2  , AUTH_CLIENT_0053, 6  , 0},
    {54 , 23 , AUTH_CLIENT_0054, 2142, 0},
    {55 , 4  , AUTH_CLIENT_0055, 86 , 0},
    {56 , 5  , AUTH_CLIENT_0056, 78 , 0},
};

MsgFormat AUTH_SERVER_FORMATS[39] = {
// header | field_count | fields | max_size | name
    {0  , 2  , AUTH_SERVER_0000, 6  , 0},
    {1  , 4  , AUTH_SERVER_0001, 14 , 0},
    {2  , 3  , AUTH_SERVER_0002, 10 , 0},
    {3  , 3  , AUTH_SERVER_0003, 10 , 0},
    {4  , 3  , AUTH_SERVER_0004, 4102, 0},
    {5  , 1  , AUTH_SERVER_0005, 2  , 0},
    {6  , 7  , AUTH_SERVER_0006, 42 , 0},
    {7  , 6  , AUTH_SERVER_0007, 134, 0},
    {8  , 5  , AUTH_SERVER_0008, 38 , 0},
    {9  , 6  , AUTH_SERVER_0009, 42 , 0},
    {10 , 5  , AUTH_SERVER_0010, 66 , 0},
    {11 , 5  , AUTH_SERVER_0011, 102, 0},
    {12 , 4  , AUTH_SERVER_0012, 558, 0},
    {13 , 5  , AUTH_SERVER_0013, 1034, 0},
    {14 , 4  , AUTH_SERVER_0014, 46 , 0},
    {15 , 1  , AUTH_SERVER_0015, 2  , 0},
    {16 , 7  , AUTH_SERVER_0016, 618, 0},
    {17 , 12 , AUTH_SERVER_0017, 275, 0},
    {18 , 7  , AUTH_SERVER_0018, 618, 0},
    {19 , 2  , AUTH_SERVER_0019, 6  , 0},
    {20 , 3  , AUTH_SERVER_0020, 10 , 0},
    {21 , 7  , AUTH_SERVER_0021, 34 , 0},
    {22 , 3  , AUTH_SERVER_0022, 2058, 0},
    {23 , 2  , AUTH_SERVER_0023, 4102, 0},
    {24 , 5  , AUTH_SERVER_0024, 226, 0},
    {25 , 1  , AUTH_SERVER_0025, 2  , 0},
    {26 , 5  , AUTH_SERVER_0026, 32 , 0},
    {27 , 5  , AUTH_SERVER_0027, 18 , 0},
    {28 , 3  , AUTH_SERVER_0028, 10 , 0},
    {29 , 4  , AUTH_SERVER_0029, 14 , 0},
    {30 , 10 , AUTH_SERVER_0030, 38 , 0},
    {31 , 4  , AUTH_SERVER_0031, 142, 0},
    {32 , 6  , AUTH_SERVER_0032, 50 , 0},
    {33 , 6  , AUTH_SERVER_0033, 50 , 0},
    {34 , 3  , AUTH_SERVER_0034, 18 , 0},
    {35 , 7  , AUTH_SERVER_0035, 90 , 0},
    {36 , 7  , AUTH_SERVER_0036, 94 , 0},
    {37 , 1  , AUTH_SERVER_0037, 2  , 0},
    {38 , 3  , AUTH_SERVER_0038, 10 , 0},
};

MsgFormat GAME_CLIENT_FORMATS[192] = {
// header | field_count | fields | max_size | name
    {0  , 2  , GAME_CLIENT_0000, 6  , 0},
    {1  , 1  , GAME_CLIENT_0001, 2  , 0},
    {2  , 3  , GAME_CLIENT_0002, 7  , 0},
    {3  , 2  , GAME_CLIENT_0003, 6  , 0},
    {4  , 1  , GAME_CLIENT_0004, 2  , 0},
    {5  , 3  , GAME_CLIENT_0005, 7  , 0},
    {6  , 1  , GAME_CLIENT_0006, 2  , 0},
    {7  , 1  , GAME_CLIENT_0007, 2  , 0},
    {8  , 1  , GAME_CLIENT_0008, 2  , 0},
    {9  , 2  , GAME_CLIENT_0009, 6  , 0},
    {10 , 1  , GAME_CLIENT_0010, 2  , 0},
    {11 , 1  , GAME_CLIENT_0011, 2  , 0},
    {12 , 4  , GAME_CLIENT_0012, 14 , 0},
    {13 , 4  , GAME_CLIENT_0013, 14 , 0},
    {14 , 4  , GAME_CLIENT_0014, 142, 0},
    {15 , 2  , GAME_CLIENT_0015, 6  , 0},
    {16 , 2  , GAME_CLIENT_0016, 6  , 0},
    {17 , 1  , GAME_CLIENT_0017, 2  , 0},
    {18 , 3  , GAME_CLIENT_0018, 7  , 0},
    {19 , 3  , GAME_CLIENT_0019, 10 , 0},
    {20 , 3  , GAME_CLIENT_0020, 10 , 0},
    {21 , 2  , GAME_CLIENT_0021, 6  , 0},
    {22 , 2  , GAME_CLIENT_0022, 6  , 0},
    {23 , 3  , GAME_CLIENT_0023, 10 , 0},
    {24 , 4  , GAME_CLIENT_0024, 18 , 0},
    {25 , 3  , GAME_CLIENT_0025, 14 , 0},
    {26 , 5  , GAME_CLIENT_0026, 18 , 0},
    {27 , 4  , GAME_CLIENT_0027, 14 , 0},
    {28 , 2  , GAME_CLIENT_0028, 4  , 0},
    {29 , 2  , GAME_CLIENT_0029, 4  , 0},
    {30 , 2  , GAME_CLIENT_0030, 3  , 0},
    {31 , 3  , GAME_CLIENT_0031, 10 , 0},
    {32 , 3  , GAME_CLIENT_0032, 10 , 0},
    {33 , 3  , GAME_CLIENT_0033, 7  , 0},
    {34 , 3  , GAME_CLIENT_0034, 10 , 0},
    {35 , 4  , GAME_CLIENT_0035, 14 , 0},
    {36 , 3  , GAME_CLIENT_0036, 7  , 0},
    {37 , 5  , GAME_CLIENT_0037, 15 , 0},
    {38 , 1  , GAME_CLIENT_0038, 2  , 0},
    {39 , 2  , GAME_CLIENT_0039, 6  , 0},
    {40 , 2  , GAME_CLIENT_0040, 6  , 0},
    {41 , 3  , GAME_CLIENT_0041, 74 , 0},
    {42 , 3  , GAME_CLIENT_0042, 7  , 0},
    {43 , 1  , GAME_CLIENT_0043, 2  , 0},
    {44 , 1  , GAME_CLIENT_0044, 2  , 0},
    {45 , 2  , GAME_CLIENT_0045, 6  , 0},
    {46 , 2  , GAME_CLIENT_0046, 6  , 0},
    {47 , 3  , GAME_CLIENT_0047, 10 , 0},
    {48 , 2  , GAME_CLIENT_0048, 3  , 0},
    {49 , 2  , GAME_CLIENT_0049, 6  , 0},
    {50 , 4  , GAME_CLIENT_0050, 27 , 0},
    {51 , 4  , GAME_CLIENT_0051, 11 , 0},
    {52 , 4  , GAME_CLIENT_0052, 82 , 0},
    {53 , 3  , GAME_CLIENT_0053, 26 , 0},
    {54 , 9  , GAME_CLIENT_0054, 14 , 0},
    {55 , 3  , GAME_CLIENT_0055, 7  , 0},
    {56 , 3  , GAME_CLIENT_0056, 26 , 0},
    {57 , 2  , GAME_CLIENT_0057, 6  , 0},
    {58 , 2  , GAME_CLIENT_0058, 6  , 0},
    {59 , 5  , GAME_CLIENT_0059, 26 , 0},
    {60 , 3  , GAME_CLIENT_0060, 14 , 0},
    {61 , 3  , GAME_CLIENT_0061, 7  , 0},
    {62 , 3  , GAME_CLIENT_0062, 10 , 0},
    {63 , 3  , GAME_CLIENT_0063, 7  , 0},
    {64 , 2  , GAME_CLIENT_0064, 6  , 0},
    {65 , 2  , GAME_CLIENT_0065, 3  , 0},
    {66 , 2  , GAME_CLIENT_0066, 6  , 0},
    {67 , 2  , GAME_CLIENT_0067, 262, 0},
    {68 , 5  , GAME_CLIENT_0068, 15 , 0},
    {69 , 3  , GAME_CLIENT_0069, 14 , 0},
    {70 , 3  , GAME_CLIENT_0070, 14 , 0},
    {71 , 2  , GAME_CLIENT_0071, 6  , 0},
    {72 , 6  , GAME_CLIENT_0072, 147, 0},
    {73 , 2  , GAME_CLIENT_0073, 3  , 0},
    {74 , 7  , GAME_CLIENT_0074, 148, 0},
    {75 , 8  , GAME_CLIENT_0075, 187, 0},
    {76 , 3  , GAME_CLIENT_0076, 247, 0},
    {77 , 4  , GAME_CLIENT_0077, 6  , 0},
    {78 , 5  , GAME_CLIENT_0078, 10 , 0},
    {79 , 3  , GAME_CLIENT_0079, 7  , 0},
    {80 , 3  , GAME_CLIENT_0080, 10 , 0},
    {81 , 2  , GAME_CLIENT_0081, 6  , 0},
    {82 , 8  , GAME_CLIENT_0082, 15 , 0},
    {83 , 2  , GAME_CLIENT_0083, 6  , 0},
    {84 , 3  , GAME_CLIENT_0084, 8  , 0},
    {85 , 3  , GAME_CLIENT_0085, 10 , 0},
    {86 , 2  , GAME_CLIENT_0086, 6  , 0},
    {87 , 1  , GAME_CLIENT_0087, 2  , 0},
    {88 , 1  , GAME_CLIENT_0088, 2  , 0},
    {89 , 2  , GAME_CLIENT_0089, 6  , 0},
    {90 , 5  , GAME_CLIENT_0090, 18 , 0},
    {91 , 3  , GAME_CLIENT_0091, 42 , 0},
    {92 , 6  , GAME_CLIENT_0092, 22 , 0},
    {93 , 5  , GAME_CLIENT_0093, 18 , 0},
    {94 , 3  , GAME_CLIENT_0094, 4  , 0},
    {95 , 2  , GAME_CLIENT_0095, 66 , 0},
    {96 , 3  , GAME_CLIENT_0096, 67 , 0},
    {97 , 1  , GAME_CLIENT_0097, 2  , 0},
    {98 , 3  , GAME_CLIENT_0098, 282, 0},
    {99 , 2  , GAME_CLIENT_0099, 6  , 0},
    {100, 4  , GAME_CLIENT_0100, 11 , 0},
    {101, 5  , GAME_CLIENT_0101, 10 , 0},
    {102, 2  , GAME_CLIENT_0102, 4  , 0},
    {103, 2  , GAME_CLIENT_0103, 6  , 0},
    {104, 3  , GAME_CLIENT_0104, 26 , 0},
    {105, 3  , GAME_CLIENT_0105, 7  , 0},
    {106, 3  , GAME_CLIENT_0106, 10 , 0},
    {107, 3  , GAME_CLIENT_0107, 10 , 0},
    {108, 3  , GAME_CLIENT_0108, 10 , 0},
    {109, 3  , GAME_CLIENT_0109, 10 , 0},
    {110, 2  , GAME_CLIENT_0110, 6  , 0},
    {111, 5  , GAME_CLIENT_0111, 37 , 0},
    {112, 4  , GAME_CLIENT_0112, 9  , 0},
    {113, 2  , GAME_CLIENT_0113, 4  , 0},
    {114, 3  , GAME_CLIENT_0114, 4  , 0},
    {115, 5  , GAME_CLIENT_0115, 13 , 0},
    {116, 1  , GAME_CLIENT_0116, 2  , 0},
    {117, 4  , GAME_CLIENT_0117, 12 , 0},
    {118, 1  , GAME_CLIENT_0118, 2  , 0},
    {119, 1  , GAME_CLIENT_0119, 2  , 0},
    {120, 1  , GAME_CLIENT_0120, 2  , 0},
    {121, 2  , GAME_CLIENT_0121, 3  , 0},
    {122, 3  , GAME_CLIENT_0122, 10 , 0},
    {123, 4  , GAME_CLIENT_0123, 9  , 0},
    {124, 2  , GAME_CLIENT_0124, 6  , 0},
    {125, 3  , GAME_CLIENT_0125, 7  , 0},
    {126, 3  , GAME_CLIENT_0126, 8  , 0},
    {127, 1  , GAME_CLIENT_0127, 2  , 0},
    {128, 1  , GAME_CLIENT_0128, 2  , 0},
    {129, 2  , GAME_CLIENT_0129, 3  , 0},
    {130, 3  , GAME_CLIENT_0130, 4  , 0},
    {131, 2  , GAME_CLIENT_0131, 4  , 0},
    {132, 7  , GAME_CLIENT_0132, 21 , 0},
    {133, 4  , GAME_CLIENT_0133, 5  , 0},
    {134, 1  , GAME_CLIENT_0134, 2  , 0},
    {135, 1  , GAME_CLIENT_0135, 2  , 0},
    {136, 1  , GAME_CLIENT_0136, 2  , 0},
    {137, 3  , GAME_CLIENT_0137, 50 , 0},
    {138, 1  , GAME_CLIENT_0138, 2  , 0},
    {139, 3  , GAME_CLIENT_0139, 5  , 0},
    {140, 2  , GAME_CLIENT_0140, 6  , 0},
    {141, 1  , GAME_CLIENT_0141, 2  , 0},
    {142, 2  , GAME_CLIENT_0142, 18 , 0},
    {143, 3  , GAME_CLIENT_0143, 4  , 0},
    {144, 2  , GAME_CLIENT_0144, 118, 0},
    {145, 2  , GAME_CLIENT_0145, 4  , 0},
    {146, 2  , GAME_CLIENT_0146, 3  , 0},
    {147, 2  , GAME_CLIENT_0147, 3  , 0},
    {148, 4  , GAME_CLIENT_0148, 9  , 0},
    {149, 1  , GAME_CLIENT_0149, 2  , 0},
    {150, 1  , GAME_CLIENT_0150, 2  , 0},
    {151, 1  , GAME_CLIENT_0151, 2  , 0},
    {152, 1  , GAME_CLIENT_0152, 2  , 0},
    {153, 2  , GAME_CLIENT_0153, 3  , 0},
    {154, 2  , GAME_CLIENT_0154, 4  , 0},
    {155, 2  , GAME_CLIENT_0155, 4  , 0},
    {156, 2  , GAME_CLIENT_0156, 4  , 0},
    {157, 2  , GAME_CLIENT_0157, 4  , 0},
    {158, 2  , GAME_CLIENT_0158, 4  , 0},
    {159, 2  , GAME_CLIENT_0159, 42 , 0},
    {160, 1  , GAME_CLIENT_0160, 2  , 0},
    {161, 1  , GAME_CLIENT_0161, 2  , 0},
    {162, 3  , GAME_CLIENT_0162, 131, 0},
    {163, 2  , GAME_CLIENT_0163, 3  , 0},
    {164, 4  , GAME_CLIENT_0164, 9  , 0},
    {165, 1  , GAME_CLIENT_0165, 2  , 0},
    {166, 2  , GAME_CLIENT_0166, 4  , 0},
    {167, 2  , GAME_CLIENT_0167, 4  , 0},
    {168, 4  , GAME_CLIENT_0168, 69 , 0},
    {169, 1  , GAME_CLIENT_0169, 2  , 0},
    {170, 2  , GAME_CLIENT_0170, 4  , 0},
    {171, 2  , GAME_CLIENT_0171, 4  , 0},
    {172, 2  , GAME_CLIENT_0172, 3  , 0},
    {173, 2  , GAME_CLIENT_0173, 3  , 0},
    {174, 3  , GAME_CLIENT_0174, 19 , 0},
    {175, 6  , GAME_CLIENT_0175, 9  , 0},
    {176, 2  , GAME_CLIENT_0176, 3  , 0},
    {177, 3  , GAME_CLIENT_0177, 46 , 0},
    {178, 3  , GAME_CLIENT_0178, 46 , 0},
    {179, 4  , GAME_CLIENT_0179, 71 , 0},
    {180, 4  , GAME_CLIENT_0180, 23 , 0},
    {181, 4  , GAME_CLIENT_0181, 23 , 0},
    {182, 3  , GAME_CLIENT_0182, 46 , 0},
    {183, 3  , GAME_CLIENT_0183, 8  , 0},
    {184, 3  , GAME_CLIENT_0184, 46 , 0},
    {185, 2  , GAME_CLIENT_0185, 6  , 0},
    {186, 2  , GAME_CLIENT_0186, 6  , 0},
    {187, 3  , GAME_CLIENT_0187, 46 , 0},
    {188, 3  , GAME_CLIENT_0188, 518, 0},
    {189, 4  , GAME_CLIENT_0189, 71 , 0},
    {190, 3  , GAME_CLIENT_0190, 67 , 0},
    {191, 3  , GAME_CLIENT_0191, 10 , 0},
};

MsgFormat GAME_SERVER_FORMATS[485] = {
// header | field_count | fields | max_size | name
    {0  , 2  , GAME_SERVER_0000, 6  , 0},
    {1  , 2  , GAME_SERVER_0001, 3  , 0},
    {2  , 3  , GAME_SERVER_0002, 7  , 0},
    {3  , 2  , GAME_SERVER_0003, 6  , 0},
    {4  , 3  , GAME_SERVER_0004, 7  , 0},
    {5  , 2  , GAME_SERVER_0005, 6  , 0},
    {6  , 1  , GAME_SERVER_0006, 2  , 0},
    {7  , 1  , GAME_SERVER_0007, 2  , 0},
    {8  , 1  , GAME_SERVER_0008, 2  , 0},
    {9  , 1  , GAME_SERVER_0009, 2  , 0},
    {10 , 3  , GAME_SERVER_0010, 10 , 0},
    {11 , 2  , GAME_SERVER_0011, 6  , 0},
    {12 , 1  , GAME_SERVER_0012, 2  , 0},
    {13 , 2  , GAME_SERVER_0013, 6  , 0},
    {14 , 5  , GAME_SERVER_0014, 1034, 0},
    {15 , 4  , GAME_SERVER_0015, 8  , 0},
    {16 , 2  , GAME_SERVER_0016, 3  , 0},
    {17 , 2  , GAME_SERVER_0017, 4  , 0},
    {18 , 2  , GAME_SERVER_0018, 4  , 0},
    {19 , 2  , GAME_SERVER_0019, 3  , 0},
    {20 , 4  , GAME_SERVER_0020, 11 , 0},
    {21 , 3  , GAME_SERVER_0021, 7  , 0},
    {22 , 2  , GAME_SERVER_0022, 246, 0},
    {23 , 6  , GAME_SERVER_0023, 13 , 0},
    {24 , 2  , GAME_SERVER_0024, 38 , 0},
    {25 , 5  , GAME_SERVER_0025, 265, 0},
    {26 , 4  , GAME_SERVER_0026, 264, 0},
    {27 , 1  , GAME_SERVER_0027, 2  , 0},
    {28 , 3  , GAME_SERVER_0028, 5  , 0},
    {29 , 2  , GAME_SERVER_0029, 518, 0},
    {30 , 2  , GAME_SERVER_0030, 6  , 0},
    {31 , 2  , GAME_SERVER_0031, 6  , 0},
    {32 , 24 , GAME_SERVER_0032, 99 , 0},
    {33 , 2  , GAME_SERVER_0033, 6  , 0},
    {34 , 3  , GAME_SERVER_0034, 10 , 0},
    {35 , 3  , GAME_SERVER_0035, 10 , 0},
    {36 , 3  , GAME_SERVER_0036, 10 , 0},
    {37 , 4  , GAME_SERVER_0037, 15 , 0},
    {38 , 3  , GAME_SERVER_0038, 7  , 0},
    {39 , 3  , GAME_SERVER_0039, 10 , 0},
    {40 , 2  , GAME_SERVER_0040, 6  , 0},
    {41 , 5  , GAME_SERVER_0041, 18 , 0},
    {42 , 6  , GAME_SERVER_0042, 22 , 0},
    {43 , 4  , GAME_SERVER_0043, 11 , 0},
    {44 , 4  , GAME_SERVER_0044, 16 , 0},
    {45 , 2  , GAME_SERVER_0045, 6  , 0},
    {46 , 4  , GAME_SERVER_0046, 14 , 0},
    {47 , 3  , GAME_SERVER_0047, 10 , 0},
    {48 , 8  , GAME_SERVER_0048, 90 , 0},
    {49 , 2  , GAME_SERVER_0049, 66 , 0},
    {50 , 7  , GAME_SERVER_0050, 26 , 0},
    {51 , 2  , GAME_SERVER_0051, 130, 0},
    {52 , 3  , GAME_SERVER_0052, 8  , 0},
    {53 , 4  , GAME_SERVER_0053, 14 , 0},
    {54 , 3  , GAME_SERVER_0054, 10 , 0},
    {55 , 4  , GAME_SERVER_0055, 8  , 0},
    {56 , 3  , GAME_SERVER_0056, 7  , 0},
    {57 , 3  , GAME_SERVER_0057, 7  , 0},
    {58 , 3  , GAME_SERVER_0058, 202, 0},
    {59 , 5  , GAME_SERVER_0059, 9  , 0},
    {60 , 5  , GAME_SERVER_0060, 279, 0},
    {61 , 2  , GAME_SERVER_0061, 6  , 0},
    {62 , 6  , GAME_SERVER_0062, 20 , 0},
    {63 , 3  , GAME_SERVER_0063, 10 , 0},
    {64 , 6  , GAME_SERVER_0064, 20 , 0},
    {65 , 6  , GAME_SERVER_0065, 20 , 0},
    {66 , 5  , GAME_SERVER_0066, 18 , 0},
    {67 , 3  , GAME_SERVER_0067, 10 , 0},
    {68 , 3  , GAME_SERVER_0068, 10 , 0},
    {69 , 4  , GAME_SERVER_0069, 14 , 0},
    {70 , 5  , GAME_SERVER_0070, 20 , 0},
    {71 , 3  , GAME_SERVER_0071, 7  , 0},
    {72 , 10 , GAME_SERVER_0072, 72 , 0},
    {73 , 2  , GAME_SERVER_0073, 6  , 0},
    {74 , 2  , GAME_SERVER_0074, 262, 0},
    {75 , 4  , GAME_SERVER_0075, 518, 0},
    {76 , 5  , GAME_SERVER_0076, 18 , 0},
    {77 , 4  , GAME_SERVER_0077, 14 , 0},
    {78 , 1  , GAME_SERVER_0078, 2  , 0},
    {79 , 7  , GAME_SERVER_0079, 60 , 0},
    {80 , 5  , GAME_SERVER_0080, 18 , 0},
    {81 , 2  , GAME_SERVER_0081, 6  , 0},
    {82 , 5  , GAME_SERVER_0082, 18 , 0},
    {83 , 3  , GAME_SERVER_0083, 262, 0},
    {84 , 1  , GAME_SERVER_0084, 2  , 0},
    {85 , 10 , GAME_SERVER_0085, 44 , 0},
    {86 , 3  , GAME_SERVER_0086, 42 , 0},
    {87 , 3  , GAME_SERVER_0087, 70 , 0},
    {88 , 8  , GAME_SERVER_0088, 87 , 0},
    {89 , 2  , GAME_SERVER_0089, 6  , 0},
    {90 , 2  , GAME_SERVER_0090, 6  , 0},
    {91 , 4  , GAME_SERVER_0091, 14 , 0},
    {92 , 2  , GAME_SERVER_0092, 246, 0},
    {93 , 3  , GAME_SERVER_0093, 5  , 0},
    {94 , 4  , GAME_SERVER_0094, 23 , 0},
    {95 , 4  , GAME_SERVER_0095, 79 , 0},
    {96 , 3  , GAME_SERVER_0096, 5  , 0},
    {97 , 3  , GAME_SERVER_0097, 10 , 0},
    {98 , 3  , GAME_SERVER_0098, 10 , 0},
    {99 , 4  , GAME_SERVER_0099, 8  , 0},
    {100, 3  , GAME_SERVER_0100, 7  , 0},
    {101, 4  , GAME_SERVER_0101, 16 , 0},
    {102, 3  , GAME_SERVER_0102, 12 , 0},
    {103, 4  , GAME_SERVER_0103, 12 , 0},
    {104, 2  , GAME_SERVER_0104, 6  , 0},
    {105, 2  , GAME_SERVER_0105, 6  , 0},
    {106, 3  , GAME_SERVER_0106, 7  , 0},
    {107, 4  , GAME_SERVER_0107, 14 , 0},
    {108, 4  , GAME_SERVER_0108, 14 , 0},
    {109, 11 , GAME_SERVER_0109, 42 , 0},
    {110, 4  , GAME_SERVER_0110, 14 , 0},
    {111, 2  , GAME_SERVER_0111, 3  , 0},
    {112, 2  , GAME_SERVER_0112, 6  , 0},
    {113, 5  , GAME_SERVER_0113, 16 , 0},
    {114, 9  , GAME_SERVER_0114, 17 , 0},
    {115, 21 , GAME_SERVER_0115, 125, 0},
    {116, 2  , GAME_SERVER_0116, 6  , 0},
    {117, 2  , GAME_SERVER_0117, 4  , 0},
    {118, 2  , GAME_SERVER_0118, 4  , 0},
    {119, 8  , GAME_SERVER_0119, 391, 0},
    {120, 3  , GAME_SERVER_0120, 8  , 0},
    {121, 6  , GAME_SERVER_0121, 34 , 0},
    {122, 2  , GAME_SERVER_0122, 6  , 0},
    {123, 2  , GAME_SERVER_0123, 6  , 0},
    {124, 2  , GAME_SERVER_0124, 6  , 0},
    {125, 5  , GAME_SERVER_0125, 267, 0},
    {126, 1  , GAME_SERVER_0126, 2  , 0},
    {127, 2  , GAME_SERVER_0127, 246, 0},
    {128, 2  , GAME_SERVER_0128, 6  , 0},
    {129, 3  , GAME_SERVER_0129, 8  , 0},
    {130, 4  , GAME_SERVER_0130, 11 , 0},
    {131, 2  , GAME_SERVER_0131, 70 , 0},
    {132, 2  , GAME_SERVER_0132, 3  , 0},
    {133, 2  , GAME_SERVER_0133, 3  , 0},
    {134, 19 , GAME_SERVER_0134, 46 , 0},
    {135, 2  , GAME_SERVER_0135, 3  , 0},
    {136, 4  , GAME_SERVER_0136, 23 , 0},
    {137, 2  , GAME_SERVER_0137, 262, 0},
    {138, 4  , GAME_SERVER_0138, 14 , 0},
    {139, 3  , GAME_SERVER_0139, 10 , 0},
    {140, 8  , GAME_SERVER_0140, 41 , 0},
    {141, 2  , GAME_SERVER_0141, 6  , 0},
    {142, 3  , GAME_SERVER_0142, 7  , 0},
    {143, 3  , GAME_SERVER_0143, 10 , 0},
    {144, 4  , GAME_SERVER_0144, 76 , 0},
    {145, 3  , GAME_SERVER_0145, 10 , 0},
    {146, 3  , GAME_SERVER_0146, 10 , 0},
    {147, 6  , GAME_SERVER_0147, 662, 0},
    {148, 3  , GAME_SERVER_0148, 8  , 0},
    {149, 6  , GAME_SERVER_0149, 22 , 0},
    {150, 3  , GAME_SERVER_0150, 259, 0},
    {151, 5  , GAME_SERVER_0151, 15 , 0},
    {152, 3  , GAME_SERVER_0152, 5  , 0},
    {153, 3  , GAME_SERVER_0153, 10 , 0},
    {154, 3  , GAME_SERVER_0154, 70 , 0},
    {155, 3  , GAME_SERVER_0155, 10 , 0},
    {156, 4  , GAME_SERVER_0156, 14 , 0},
    {157, 5  , GAME_SERVER_0157, 315, 0},
    {158, 4  , GAME_SERVER_0158, 14 , 0},
    {159, 5  , GAME_SERVER_0159, 18 , 0},
    {160, 7  , GAME_SERVER_0160, 20 , 0},
    {161, 4  , GAME_SERVER_0161, 14 , 0},
    {162, 5  , GAME_SERVER_0162, 18 , 0},
    {163, 8  , GAME_SERVER_0163, 29 , 0},
    {164, 3  , GAME_SERVER_0164, 250, 0},
    {165, 4  , GAME_SERVER_0165, 8  , 0},
    {166, 4  , GAME_SERVER_0166, 14 , 0},
    {167, 4  , GAME_SERVER_0167, 16 , 0},
    {168, 3  , GAME_SERVER_0168, 10 , 0},
    {169, 4  , GAME_SERVER_0169, 14 , 0},
    {170, 1  , GAME_SERVER_0170, 2  , 0},
    {171, 3  , GAME_SERVER_0171, 7  , 0},
    {172, 3  , GAME_SERVER_0172, 7  , 0},
    {173, 3  , GAME_SERVER_0173, 10 , 0},
    {174, 2  , GAME_SERVER_0174, 66 , 0},
    {175, 3  , GAME_SERVER_0175, 5  , 0},
    {176, 3  , GAME_SERVER_0176, 6  , 0},
    {177, 7  , GAME_SERVER_0177, 86 , 0},
    {178, 2  , GAME_SERVER_0178, 6  , 0},
    {179, 3  , GAME_SERVER_0179, 70 , 0},
    {180, 2  , GAME_SERVER_0180, 262, 0},
    {181, 3  , GAME_SERVER_0181, 10 , 0},
    {182, 5  , GAME_SERVER_0182, 9  , 0},
    {183, 1  , GAME_SERVER_0183, 2  , 0},
    {184, 3  , GAME_SERVER_0184, 262, 0},
    {185, 1  , GAME_SERVER_0185, 2  , 0},
    {186, 4  , GAME_SERVER_0186, 266, 0},
    {187, 2  , GAME_SERVER_0187, 6  , 0},
    {188, 3  , GAME_SERVER_0188, 262, 0},
    {189, 2  , GAME_SERVER_0189, 6  , 0},
    {190, 2  , GAME_SERVER_0190, 262, 0},
    {191, 6  , GAME_SERVER_0191, 526, 0},
    {192, 3  , GAME_SERVER_0192, 10 , 0},
    {193, 3  , GAME_SERVER_0193, 10 , 0},
    {194, 3  , GAME_SERVER_0194, 7  , 0},
    {195, 2  , GAME_SERVER_0195, 6  , 0},
    {196, 3  , GAME_SERVER_0196, 247, 0},
    {197, 3  , GAME_SERVER_0197, 7  , 0},
    {198, 4  , GAME_SERVER_0198, 75 , 0},
    {199, 4  , GAME_SERVER_0199, 11 , 0},
    {200, 3  , GAME_SERVER_0200, 4  , 0},
    {201, 3  , GAME_SERVER_0201, 7  , 0},
    {202, 2  , GAME_SERVER_0202, 3  , 0},
    {203, 2  , GAME_SERVER_0203, 3  , 0},
    {204, 5  , GAME_SERVER_0204, 9  , 0},
    {205, 3  , GAME_SERVER_0205, 10 , 0},
    {206, 3  , GAME_SERVER_0206, 10 , 0},
    {207, 2  , GAME_SERVER_0207, 6  , 0},
    {208, 5  , GAME_SERVER_0208, 16 , 0},
    {209, 4  , GAME_SERVER_0209, 12 , 0},
    {210, 2  , GAME_SERVER_0210, 518, 0},
    {211, 1  , GAME_SERVER_0211, 2  , 0},
    {212, 1  , GAME_SERVER_0212, 2  , 0},
    {213, 4  , GAME_SERVER_0213, 10 , 0},
    {214, 2  , GAME_SERVER_0214, 38 , 0},
    {215, 2  , GAME_SERVER_0215, 22 , 0},
    {216, 5  , GAME_SERVER_0216, 13 , 0},
    {217, 5  , GAME_SERVER_0217, 79 , 0},
    {218, 2  , GAME_SERVER_0218, 518, 0},
    {219, 3  , GAME_SERVER_0219, 8  , 0},
    {220, 3  , GAME_SERVER_0220, 8  , 0},
    {221, 2  , GAME_SERVER_0221, 4  , 0},
    {222, 3  , GAME_SERVER_0222, 10 , 0},
    {223, 3  , GAME_SERVER_0223, 71 , 0},
    {224, 2  , GAME_SERVER_0224, 3  , 0},
    {225, 4  , GAME_SERVER_0225, 12 , 0},
    {226, 4  , GAME_SERVER_0226, 12 , 0},
    {227, 4  , GAME_SERVER_0227, 12 , 0},
    {228, 5  , GAME_SERVER_0228, 16 , 0},
    {229, 4  , GAME_SERVER_0229, 12 , 0},
    {230, 4  , GAME_SERVER_0230, 12 , 0},
    {231, 6  , GAME_SERVER_0231, 20 , 0},
    {232, 16 , GAME_SERVER_0232, 62 , 0},
    {233, 2  , GAME_SERVER_0233, 6  , 0},
    {234, 2  , GAME_SERVER_0234, 6  , 0},
    {235, 2  , GAME_SERVER_0235, 6  , 0},
    {236, 2  , GAME_SERVER_0236, 6  , 0},
    {237, 3  , GAME_SERVER_0237, 10 , 0},
    {238, 3  , GAME_SERVER_0238, 10 , 0},
    {239, 3  , GAME_SERVER_0239, 10 , 0},
    {240, 3  , GAME_SERVER_0240, 10 , 0},
    {241, 2  , GAME_SERVER_0241, 6  , 0},
    {242, 5  , GAME_SERVER_0242, 30 , 0},
    {243, 3  , GAME_SERVER_0243, 8  , 0},
    {244, 3  , GAME_SERVER_0244, 10 , 0},
    {245, 12 , GAME_SERVER_0245, 70 , 0},
    {246, 3  , GAME_SERVER_0246, 10 , 0},
    {247, 2  , GAME_SERVER_0247, 6  , 0},
    {248, 2  , GAME_SERVER_0248, 70 , 0},
    {249, 2  , GAME_SERVER_0249, 134, 0},
    {250, 4  , GAME_SERVER_0250, 12 , 0},
    {251, 3  , GAME_SERVER_0251, 10 , 0},
    {252, 3  , GAME_SERVER_0252, 67 , 0},
    {253, 1  , GAME_SERVER_0253, 2  , 0},
    {254, 3  , GAME_SERVER_0254, 4  , 0},
    {255, 2  , GAME_SERVER_0255, 3  , 0},
    {256, 2  , GAME_SERVER_0256, 3  , 0},
    {257, 4  , GAME_SERVER_0257, 168, 0},
    {258, 1  , GAME_SERVER_0258, 2  , 0},
    {259, 2  , GAME_SERVER_0259, 1030, 0},
    {260, 1  , GAME_SERVER_0260, 2  , 0},
    {261, 2  , GAME_SERVER_0261, 6  , 0},
    {262, 6  , GAME_SERVER_0262, 79 , 0},
    {263, 2  , GAME_SERVER_0263, 6  , 0},
    {264, 3  , GAME_SERVER_0264, 250, 0},
    {265, 5  , GAME_SERVER_0265, 253, 0},
    {266, 2  , GAME_SERVER_0266, 246, 0},
    {267, 2  , GAME_SERVER_0267, 6  , 0},
    {268, 2  , GAME_SERVER_0268, 6  , 0},
    {269, 4  , GAME_SERVER_0269, 9  , 0},
    {270, 3  , GAME_SERVER_0270, 5  , 0},
    {271, 3  , GAME_SERVER_0271, 8  , 0},
    {272, 4  , GAME_SERVER_0272, 12 , 0},
    {273, 3  , GAME_SERVER_0273, 7  , 0},
    {274, 3  , GAME_SERVER_0274, 70 , 0},
    {275, 3  , GAME_SERVER_0275, 10 , 0},
    {276, 3  , GAME_SERVER_0276, 10 , 0},
    {277, 5  , GAME_SERVER_0277, 25 , 0},
    {278, 2  , GAME_SERVER_0278, 18 , 0},
    {279, 4  , GAME_SERVER_0279, 21 , 0},
    {280, 1  , GAME_SERVER_0280, 2  , 0},
    {281, 14 , GAME_SERVER_0281, 94 , 0},
    {282, 3  , GAME_SERVER_0282, 7  , 0},
    {283, 1  , GAME_SERVER_0283, 2  , 0},
    {284, 2  , GAME_SERVER_0284, 3  , 0},
    {285, 3  , GAME_SERVER_0285, 8  , 0},
    {286, 3  , GAME_SERVER_0286, 134, 0},
    {287, 16 , GAME_SERVER_0287, 111, 0},
    {288, 20 , GAME_SERVER_0288, 124, 0},
    {289, 4  , GAME_SERVER_0289, 9  , 0},
    {290, 8  , GAME_SERVER_0290, 143, 0},
    {291, 3  , GAME_SERVER_0291, 19 , 0},
    {292, 3  , GAME_SERVER_0292, 8  , 0},
    {293, 3  , GAME_SERVER_0293, 5  , 0},
    {294, 10 , GAME_SERVER_0294, 267, 0},
    {295, 2  , GAME_SERVER_0295, 42 , 0},
    {296, 2  , GAME_SERVER_0296, 42 , 0},
    {297, 2  , GAME_SERVER_0297, 42 , 0},
    {298, 3  , GAME_SERVER_0298, 134, 0},
    {299, 3  , GAME_SERVER_0299, 10 , 0},
    {300, 2  , GAME_SERVER_0300, 3  , 0},
    {301, 2  , GAME_SERVER_0301, 4  , 0},
    {302, 3  , GAME_SERVER_0302, 554, 0},
    {303, 3  , GAME_SERVER_0303, 8  , 0},
    {304, 1  , GAME_SERVER_0304, 2  , 0},
    {305, 3  , GAME_SERVER_0305, 7  , 0},
    {306, 2  , GAME_SERVER_0306, 6  , 0},
    {307, 1  , GAME_SERVER_0307, 2  , 0},
    {308, 4  , GAME_SERVER_0308, 12 , 0},
    {309, 2  , GAME_SERVER_0309, 6  , 0},
    {310, 3  , GAME_SERVER_0310, 10 , 0},
    {311, 7  , GAME_SERVER_0311, 71 , 0},
    {312, 3  , GAME_SERVER_0312, 10 , 0},
    {313, 3  , GAME_SERVER_0313, 70 , 0},
    {314, 3  , GAME_SERVER_0314, 10 , 0},
    {315, 3  , GAME_SERVER_0315, 7  , 0},
    {316, 4  , GAME_SERVER_0316, 9  , 0},
    {317, 5  , GAME_SERVER_0317, 11 , 0},
    {318, 7  , GAME_SERVER_0318, 13 , 0},
    {319, 3  , GAME_SERVER_0319, 8  , 0},
    {320, 3  , GAME_SERVER_0320, 8  , 0},
    {321, 2  , GAME_SERVER_0321, 4  , 0},
    {322, 3  , GAME_SERVER_0322, 6  , 0},
    {323, 3  , GAME_SERVER_0323, 5  , 0},
    {324, 2  , GAME_SERVER_0324, 4  , 0},
    {325, 6  , GAME_SERVER_0325, 12 , 0},
    {326, 5  , GAME_SERVER_0326, 13 , 0},
    {327, 3  , GAME_SERVER_0327, 5  , 0},
    {328, 4  , GAME_SERVER_0328, 7  , 0},
    {329, 1  , GAME_SERVER_0329, 2  , 0},
    {330, 5  , GAME_SERVER_0330, 11 , 0},
    {331, 4  , GAME_SERVER_0331, 6  , 0},
    {332, 3  , GAME_SERVER_0332, 8  , 0},
    {333, 3  , GAME_SERVER_0333, 6  , 0},
    {334, 3  , GAME_SERVER_0334, 8  , 0},
    {335, 3  , GAME_SERVER_0335, 8  , 0},
    {336, 3  , GAME_SERVER_0336, 8  , 0},
    {337, 4  , GAME_SERVER_0337, 12 , 0},
    {338, 5  , GAME_SERVER_0338, 11 , 0},
    {339, 4  , GAME_SERVER_0339, 8  , 0},
    {340, 3  , GAME_SERVER_0340, 10 , 0},
    {341, 2  , GAME_SERVER_0341, 6  , 0},
    {342, 5  , GAME_SERVER_0342, 18 , 0},
    {343, 3  , GAME_SERVER_0343, 4  , 0},
    {344, 3  , GAME_SERVER_0344, 8  , 0},
    {345, 3  , GAME_SERVER_0345, 7  , 0},
    {346, 3  , GAME_SERVER_0346, 10 , 0},
    {347, 10 , GAME_SERVER_0347, 92 , 0},
    {348, 3  , GAME_SERVER_0348, 10 , 0},
    {349, 10 , GAME_SERVER_0349, 25 , 0},
    {350, 10 , GAME_SERVER_0350, 25 , 0},
    {351, 7  , GAME_SERVER_0351, 402, 0},
    {352, 15 , GAME_SERVER_0352, 421, 0},
    {353, 15 , GAME_SERVER_0353, 421, 0},
    {354, 4  , GAME_SERVER_0354, 24 , 0},
    {355, 1  , GAME_SERVER_0355, 2  , 0},
    {356, 1  , GAME_SERVER_0356, 2  , 0},
    {357, 1  , GAME_SERVER_0357, 2  , 0},
    {358, 2  , GAME_SERVER_0358, 3  , 0},
    {359, 3  , GAME_SERVER_0359, 6  , 0},
    {360, 3  , GAME_SERVER_0360, 7  , 0},
    {361, 4  , GAME_SERVER_0361, 24 , 0},
    {362, 1  , GAME_SERVER_0362, 2  , 0},
    {363, 1  , GAME_SERVER_0363, 2  , 0},
    {364, 2  , GAME_SERVER_0364, 3  , 0},
    {365, 2  , GAME_SERVER_0365, 3  , 0},
    {366, 1  , GAME_SERVER_0366, 2  , 0},
    {367, 1  , GAME_SERVER_0367, 2  , 0},
    {368, 24 , GAME_SERVER_0368, 57 , 0},
    {369, 2  , GAME_SERVER_0369, 6  , 0},
    {370, 2  , GAME_SERVER_0370, 6  , 0},
    {371, 24 , GAME_SERVER_0371, 57 , 0},
    {372, 5  , GAME_SERVER_0372, 137, 0},
    {373, 3  , GAME_SERVER_0373, 8  , 0},
    {374, 5  , GAME_SERVER_0374, 137, 0},
    {375, 21 , GAME_SERVER_0375, 175, 0},
    {376, 4  , GAME_SERVER_0376, 9  , 0},
    {377, 2  , GAME_SERVER_0377, 3  , 0},
    {378, 1  , GAME_SERVER_0378, 2  , 0},
    {379, 5  , GAME_SERVER_0379, 6  , 0},
    {380, 2  , GAME_SERVER_0380, 42 , 0},
    {381, 1  , GAME_SERVER_0381, 2  , 0},
    {382, 2  , GAME_SERVER_0382, 6  , 0},
    {383, 5  , GAME_SERVER_0383, 255, 0},
    {384, 2  , GAME_SERVER_0384, 6  , 0},
    {385, 3  , GAME_SERVER_0385, 247, 0},
    {386, 3  , GAME_SERVER_0386, 262, 0},
    {387, 3  , GAME_SERVER_0387, 770, 0},
    {388, 3  , GAME_SERVER_0388, 262, 0},
    {389, 1  , GAME_SERVER_0389, 2  , 0},
    {390, 2  , GAME_SERVER_0390, 6  , 0},
    {391, 5  , GAME_SERVER_0391, 1088, 0},
    {392, 1  , GAME_SERVER_0392, 2  , 0},
    {393, 1  , GAME_SERVER_0393, 2  , 0},
    {394, 2  , GAME_SERVER_0394, 6  , 0},
    {395, 2  , GAME_SERVER_0395, 3  , 0},
    {396, 2  , GAME_SERVER_0396, 4  , 0},
    {397, 1  , GAME_SERVER_0397, 2  , 0},
    {398, 4  , GAME_SERVER_0398, 10 , 0},
    {399, 3  , GAME_SERVER_0399, 7  , 0},
    {400, 2  , GAME_SERVER_0400, 4  , 0},
    {401, 9  , GAME_SERVER_0401, 463, 0},
    {402, 3  , GAME_SERVER_0402, 5  , 0},
    {403, 2  , GAME_SERVER_0403, 3  , 0},
    {404, 7  , GAME_SERVER_0404, 26 , 0},
    {405, 2  , GAME_SERVER_0405, 1030, 0},
    {406, 4  , GAME_SERVER_0406, 9  , 0},
    {407, 2  , GAME_SERVER_0407, 3  , 0},
    {408, 7  , GAME_SERVER_0408, 15 , 0},
    {409, 3  , GAME_SERVER_0409, 10 , 0},
    {410, 2  , GAME_SERVER_0410, 4  , 0},
    {411, 2  , GAME_SERVER_0411, 246, 0},
    {412, 18 , GAME_SERVER_0412, 229, 0},
    {413, 1  , GAME_SERVER_0413, 2  , 0},
    {414, 4  , GAME_SERVER_0414, 102, 0},
    {415, 9  , GAME_SERVER_0415, 508, 0},
    {416, 2  , GAME_SERVER_0416, 3  , 0},
    {417, 7  , GAME_SERVER_0417, 20 , 0},
    {418, 4  , GAME_SERVER_0418, 491, 0},
    {419, 14 , GAME_SERVER_0419, 79 , 0},
    {420, 7  , GAME_SERVER_0420, 38 , 0},
    {421, 10 , GAME_SERVER_0421, 506, 0},
    {422, 2  , GAME_SERVER_0422, 3  , 0},
    {423, 3  , GAME_SERVER_0423, 7  , 0},
    {424, 3  , GAME_SERVER_0424, 7  , 0},
    {425, 2  , GAME_SERVER_0425, 3  , 0},
    {426, 2  , GAME_SERVER_0426, 6  , 0},
    {427, 5  , GAME_SERVER_0427, 52 , 0},
    {428, 7  , GAME_SERVER_0428, 51 , 0},
    {429, 13 , GAME_SERVER_0429, 96 , 0},
    {430, 2  , GAME_SERVER_0430, 246, 0},
    {431, 2  , GAME_SERVER_0431, 6  , 0},
    {432, 4  , GAME_SERVER_0432, 9  , 0},
    {433, 3  , GAME_SERVER_0433, 5  , 0},
    {434, 7  , GAME_SERVER_0434, 77 , 0},
    {435, 1  , GAME_SERVER_0435, 2  , 0},
    {436, 7  , GAME_SERVER_0436, 77 , 0},
    {437, 1  , GAME_SERVER_0437, 2  , 0},
    {438, 2  , GAME_SERVER_0438, 3  , 0},
    {439, 2  , GAME_SERVER_0439, 3  , 0},
    {440, 1  , GAME_SERVER_0440, 2  , 0},
    {441, 1  , GAME_SERVER_0441, 2  , 0},
    {442, 4  , GAME_SERVER_0442, 249, 0},
    {443, 2  , GAME_SERVER_0443, 3  , 0},
    {444, 2  , GAME_SERVER_0444, 6  , 0},
    {445, 2  , GAME_SERVER_0445, 3  , 0},
    {446, 6  , GAME_SERVER_0446, 48 , 0},
    {447, 3  , GAME_SERVER_0447, 6  , 0},
    {448, 2  , GAME_SERVER_0448, 4  , 0},
    {449, 6  , GAME_SERVER_0449, 10 , 0},
    {450, 4  , GAME_SERVER_0450, 8  , 0},
    {451, 2  , GAME_SERVER_0451, 4  , 0},
    {452, 2  , GAME_SERVER_0452, 4  , 0},
    {453, 2  , GAME_SERVER_0453, 4  , 0},
    {454, 2  , GAME_SERVER_0454, 4  , 0},
    {455, 2  , GAME_SERVER_0455, 4  , 0},
    {456, 2  , GAME_SERVER_0456, 4  , 0},
    {457, 2  , GAME_SERVER_0457, 3  , 0},
    {458, 4  , GAME_SERVER_0458, 7  , 0},
    {459, 3  , GAME_SERVER_0459, 6  , 0},
    {460, 3  , GAME_SERVER_0460, 6  , 0},
    {461, 3  , GAME_SERVER_0461, 6  , 0},
    {462, 3  , GAME_SERVER_0462, 6  , 0},
    {463, 3  , GAME_SERVER_0463, 6  , 0},
    {464, 4  , GAME_SERVER_0464, 7  , 0},
    {465, 2  , GAME_SERVER_0465, 4  , 0},
    {466, 2  , GAME_SERVER_0466, 4  , 0},
    {467, 2  , GAME_SERVER_0467, 246, 0},
    {468, 1  , GAME_SERVER_0468, 2  , 0},
    {469, 2  , GAME_SERVER_0469, 3  , 0},
    {470, 6  , GAME_SERVER_0470, 234, 0},
    {471, 1  , GAME_SERVER_0471, 2  , 0},
    {472, 4  , GAME_SERVER_0472, 248, 0},
    {473, 3  , GAME_SERVER_0473, 5  , 0},
    {474, 3  , GAME_SERVER_0474, 6  , 0},
    {475, 3  , GAME_SERVER_0475, 6  , 0},
    {476, 14 , GAME_SERVER_0476, 122, 0},
    {477, 2  , GAME_SERVER_0477, 4  , 0},
    {478, 2  , GAME_SERVER_0478, 4  , 0},
    {479, 4  , GAME_SERVER_0479, 6  , 0},
    {480, 4  , GAME_SERVER_0480, 6  , 0},
    {481, 2  , GAME_SERVER_0481, 3  , 0},
    {482, 2  , GAME_SERVER_0482, 246, 0},
    {483, 2  , GAME_SERVER_0483, 3  , 0},
    {484, 2  , GAME_SERVER_0484, 3  , 0},
};
