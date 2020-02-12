#ifdef CORE_CHAT_C
#error "chat.c included more than once"
#endif
#define CORE_CHAT_C

static char get_channel_character(Channel channel)
{
    switch (channel) {
        case Channel_Alliance:  return '%';
        case Channel_Allies:    return '#';
        // case Channel_GWCA1:     return
        case Channel_All:       return '!';
        // case Channel_GWCA2:     return
        // case Channel_Moderator: return
        case Channel_Emote:     return '/';
        // case Channel_Warning:   return
        // case Channel_GWCA3:     return
        case Channel_Guild:     return '@';
        // case Channel_Global:    return
        case Channel_Group:     return '#';
        case Channel_Trade:     return '$';
        // case Channel_Advisory:  return
        case Channel_Whisper:   return '"';
        default: return 0;
    }
}

void init_chat(Chat *chat)
{
    chat->next_message_entry = 0;
    array_init2(chat->messages, 256);
    array_init(chat->str_builder, 256);
}

static inline char *
GetChannelName(Channel chan)
{
    switch(chan) {
        case Channel_Alliance:  return "Alliance";
        case Channel_Allies:    return "Allies";
        case Channel_GWCA1:     return "GWCA1";
        case Channel_All:       return "All";
        case Channel_GWCA2:     return "GWCA2";
        case Channel_Moderator: return "Moderator";
        case Channel_Emote:     return "Emote";
        case Channel_Warning:   return "Warning";
        case Channel_GWCA3:     return "GWCA3";
        case Channel_Guild:     return "Guild";
        case Channel_Global:    return "Global";
        case Channel_Group:     return "Group";
        case Channel_Trade:     return "Trade";
        case Channel_Advisory:  return "Advisory";
        case Channel_Whisper:   return "Whisper";
        default:                return "Unknow";
    }
}

void HandleChatMessageCore(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t msg[122];
    } MessageCore;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_CHAT_MESSAGE_CORE);
    assert(sizeof(MessageCore) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MessageCore *pack = cast(MessageCore *)packet;
    assert(client && client->ingame);

    array_u16_t *sb = &client->chat.str_builder;
    array_reserve(*sb, 123);
    for (size_t i = 0; i < 122; i++) {
        array_add(*sb, pack->msg[i]);
        if (pack->msg[i] == 0)
            break;
    }
}

void HandleChatMessageLocal(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint16_t sender;
        uint8_t  channel;
    } MessageLocal;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_CHAT_MESSAGE_LOCAL);
    assert(sizeof(MessageLocal) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MessageLocal *pack = cast(MessageLocal *)packet;
    assert(client && client->ingame);

    Channel channel = (Channel)pack->channel;
    if (channel >= Channel_Count) {
        LogError("Channel received is too big, %u when max is %u", channel, Channel_Count);
        goto clear_buffer_and_exit;
    }

    World  *world  = &client->world;
    Player *player = array_at(world->players, pack->sender);
    if (player == NULL) {
        LogError("Player '%d' wasn't found in the world '%08X'", pack->sender, world->hash);
        goto clear_buffer_and_exit;
    }

    array_u16_t *sb = &client->chat.str_builder;

    char buffer[512];
    string message;
    message.bytes = buffer;
    message.count = unicode16_to_utf8(buffer, sizeof(buffer), sb->data, sb->size);

    Event_ChatMessage params;
    params.channel = channel;
    params.sender  = player->name;
    params.message = message;

    broadcast_event(&client->event_mgr, EventType_ChatMessage, &params);

clear_buffer_and_exit:
    array_clear(client->chat.str_builder);
}

void HandleChatMessageGlobal(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint8_t  channel;
        uint16_t sender[32];
        uint16_t tag[6];
    } MessageGlobal;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_CHAT_MESSAGE_GLOBAL);
    assert(sizeof(MessageGlobal) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MessageGlobal *pack = cast(MessageGlobal *)packet;
    assert(client && client->ingame);

    Channel channel = (Channel)pack->channel;
    if (channel >= Channel_Count) {
        LogError("Channel received is too big, %u when max is %u", channel, Channel_Count);
        goto clear_buffer_and_exit;
    }

    /*
    string message;
    message.bytes = chat->str_builder.data;
    message.count = chat->str_builder.size;
    uint16_t *sender  = pack->sender;
    */

clear_buffer_and_exit:
    array_clear(client->chat.str_builder);
}

void HandleChatMessageServer(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_CHAT_MESSAGE_SERVER);
}

void HandleWhisperReceived(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        int32_t  unk0;
        uint16_t sender[20];
        uint16_t message[256];
    } WhisperReceived;
#pragma pack(pop)
    
    assert(packet->header == AUTH_SMSG_WHISPER_RECEIVED);
    assert(sizeof(WhisperReceived) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    WhisperReceived *pack = cast(WhisperReceived *)packet;
    assert(client && client->ingame);

    string sender, message;
    char sender_buffer[40];
    char msg_buffer[512];

    sender.bytes = sender_buffer;
    message.bytes = msg_buffer;
    sender.count = unicode16_to_utf8(sender_buffer, sizeof(sender_buffer), pack->sender, -1) - 1;
    message.count = unicode16_to_utf8(msg_buffer, sizeof(msg_buffer), pack->message, -1) - 1;

    Event_ChatMessage params;
    params.channel = Channel_Whisper;
    params.sender  = sender;
    params.message = message;

    broadcast_event(&client->event_mgr, EventType_ChatMessage, &params);
}

void GameSrv_SendChat(GwClient *client, Channel channel, const char *msg)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t buffer[140];
    } ChatMessage;
#pragma pack(pop)

    ChatMessage packet = NewPacket(GAME_CMSG_SEND_CHAT_MESSAGE);

    char chan_char = get_channel_character(channel);
    if (chan_char == 0)
        return;
    packet.buffer[0] = chan_char;
    if (!utf8_to_unicode16(packet.buffer + 1, 139, msg, -1)) {
        LogError("Couldn't 'strtoc16' in 'GameSrv_SendChat'");
        return;
    }
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_SendWhisper(GwClient *client, const char *target, const char *msg)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t buffer[140];
    } ChatMessage;
#pragma pack(pop)

    ChatMessage packet = NewPacket(GAME_CMSG_SEND_CHAT_MESSAGE);

    size_t n, k = 0;
    uint16_t *buffer = packet.buffer;

    buffer[k++] = '"';
    if (!(n = utf8_to_unicode16(buffer + k, 20, target, -1))) {
        LogError("Couldn't 'strtoc16' for target in 'GameSrv_SendWhisper'");
        return;
    }
    k += n - 1;
    buffer[k++] = ',';
    if (!utf8_to_unicode16(buffer + k, 140 - k, msg, -1)) {
        LogError("Couldn't 'strtoc16' for msg in 'GameSrv_SendWhisper'");
        return;
    }

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}
