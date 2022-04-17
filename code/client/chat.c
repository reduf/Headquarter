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
    array_init(&chat->messages);
    array_resize(&chat->messages, 256);
    array_init(&chat->str_builder);
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
    assert(client && client->game_srv.secured);

    array_u16_t *sb = &client->chat.str_builder;
    array_reserve(sb, 123);
    for (size_t i = 0; i < 122; i++) {
        array_add(sb, pack->msg[i]);
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
    assert(client && client->game_srv.secured);

    if(!is_event_subscribed(&client->event_mgr, EventType_ChatMessage))
        goto clear_buffer_and_exit;

    Channel channel = (Channel)pack->channel;
    if (channel >= Channel_Count) {
        LogError("Channel received is too big, %u when max is %u", channel, Channel_Count);
        goto clear_buffer_and_exit;
    }

    World  *world  = &client->world;
    Player *player = array_at(&world->players, pack->sender);
    if (player == NULL) {
        LogError("Player '%d' wasn't found in the world '%08X'", pack->sender, world->hash);
        goto clear_buffer_and_exit;
    }

    array_u16_t *sb = &client->chat.str_builder;

    Event params;
    Event_Init(&params, EventType_ChatMessage);
    params.ChatMessage.extra_id = 0;
    params.ChatMessage.channel = channel;
    params.ChatMessage.sender.length = player->name.length;
    params.ChatMessage.sender.buffer = player->name.buffer;
    params.ChatMessage.message.length = sb->size;
    params.ChatMessage.message.buffer = sb->data;

    broadcast_event(&client->event_mgr, &params);

clear_buffer_and_exit:
    array_clear(&client->chat.str_builder);
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
    assert(client && client->game_srv.secured);

    if (!is_event_subscribed(&client->event_mgr, EventType_ChatMessage))
        goto clear_buffer_and_exit;

    Channel channel = (Channel)pack->channel;
    if (channel >= Channel_Count) {
        LogError("Channel received is too big, %u when max is %u", channel, Channel_Count);
        goto clear_buffer_and_exit;
    }
    array_u16_t* sb = &client->chat.str_builder;

    Event params;
    Event_Init(&params, EventType_ChatMessage);
    params.ChatMessage.extra_id = 0;
    params.ChatMessage.channel = channel;
    uint16_t sender_with_tag[48];
    int length = 0;
    for (int i = 0; i < ARRAY_SIZE(pack->sender) && pack->sender[i] != 0; i++) {
        sender_with_tag[length++] = pack->sender[i];
    }
    sender_with_tag[length++] = ' ';
    sender_with_tag[length++] = '[';
    for (int i = 0; i < ARRAY_SIZE(pack->tag) && pack->tag[i] != 0; i++) {
        sender_with_tag[length++] = pack->tag[i];
    }
    sender_with_tag[length++] = ']';
    sender_with_tag[length++] = 0;

    params.ChatMessage.sender.buffer = sender_with_tag;
    params.ChatMessage.sender.length = length;
    params.ChatMessage.message.length = sb->size;
    params.ChatMessage.message.buffer = sb->data;
    broadcast_event(&client->event_mgr, &params);

clear_buffer_and_exit:
    array_clear(&client->chat.str_builder);
}

void HandleChatMessageServer(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t id; // some kind of ID of the affected target
        uint8_t  channel; // enum ChatChannel above.
    } MessageServer;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_CHAT_MESSAGE_SERVER);
    assert(sizeof(MessageServer) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MessageServer *pack = cast(MessageServer *)packet;
    assert(client && client->game_srv.secured);

    Channel channel = (Channel)pack->channel;
    if (channel >= Channel_Count) {
        LogError("Channel received is too big, %u when max is %u", channel, Channel_Count);
        goto clear_buffer_and_exit;
    }

    // @Enhancement:
    // At some point we will need to catch some messages, for instance when a new version
    // is ready to be downloaded. We can then react accordingly and restart the custom
    // client.

    if (!is_event_subscribed(&client->event_mgr, EventType_ChatMessage))
        goto clear_buffer_and_exit;

    array_u16_t* sb = &client->chat.str_builder;

    Event params;
    Event_Init(&params, EventType_ChatMessage);
    params.ChatMessage.channel = channel;
    params.ChatMessage.sender.buffer = NULL;
    params.ChatMessage.sender.length = 0;
    params.ChatMessage.message.length = sb->size;
    params.ChatMessage.message.buffer = sb->data;
    params.ChatMessage.extra_id = pack->id;
    broadcast_event(&client->event_mgr, &params);

clear_buffer_and_exit:
    array_clear(&client->chat.str_builder);
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
    assert(client && client->game_srv.secured);

    if (!is_event_subscribed(&client->event_mgr, EventType_ChatMessage))
        return;

    DECLARE_KSTR(sender, 20);
    DECLARE_KSTR(message, 256);

    kstr_read(&sender, pack->sender, ARRAY_SIZE(pack->sender));
    kstr_read(&message, pack->message, ARRAY_SIZE(pack->message));

    Event params;
    Event_Init(&params, EventType_ChatMessage);

    params.ChatMessage.extra_id = 0;
    params.ChatMessage.channel = Channel_Whisper;
    params.ChatMessage.sender.buffer = pack->sender;
    params.ChatMessage.sender.length = u16len(pack->sender, ARRAY_SIZE(pack->sender));
    params.ChatMessage.message.buffer = pack->message;
    params.ChatMessage.message.length = u16len(pack->message, ARRAY_SIZE(pack->message));

    broadcast_event(&client->event_mgr, &params);
}

void GameSrv_SendChat(GwClient *client, Channel channel, struct kstr *msg)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        uint16_t buffer[138];
    } ChatMessage;
#pragma pack(pop)

    ChatMessage packet = NewPacket(GAME_CMSG_SEND_CHAT_MESSAGE);

    char chan_char = get_channel_character(channel);
    if (chan_char == 0)
        return;
    packet.buffer[0] = chan_char;
    if (!kstr_write(msg, &packet.buffer[1], ARRAY_SIZE(packet.buffer) - 1)) {
        LogError("Couldn't send a string, length of %d is too big (%d max)",msg->length, ARRAY_SIZE(packet.buffer) - 1);
        return;
    }
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_SendWhisper(GwClient *client, struct kstr *target, struct kstr *msg)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        uint16_t buffer[138];
    } ChatMessage;
#pragma pack(pop)

    ChatMessage packet = NewPacket(GAME_CMSG_SEND_CHAT_MESSAGE);
    packet.agent_id = 0;
    /*
     * The format of packet.buffer is:
     * "target,message
     *
     * So, the final message length is 2 (for " & ,) + target->length + msg->length
     */

    size_t final_length = 2 + target->length + msg->length;
    if (final_length >= ARRAY_SIZE(packet.buffer))
    {
        LogError("Maximum size in packet is %zu, but we need %zu", ARRAY_SIZE(packet.buffer), final_length + 1);
        return;
    }

    uint16_t *buffer = packet.buffer;
    size_t wpos = 0;

    buffer[wpos++] = '"';
    for (size_t i = 0; i < target->length; i++)
        buffer[wpos++] = target->buffer[i];

    buffer[wpos++] = ',';
    for (size_t i = 0; i < msg->length; i++)
        buffer[wpos++] = msg->buffer[i];

    buffer[wpos++] = 0;
    assert(wpos <= ARRAY_SIZE(packet.buffer));
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}
