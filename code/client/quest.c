#ifdef CORE_QUESTS_C
#error "quest.c included more than once"
#endif
#define CORE_QUESTS_C

Quest *find_quest_by_id(ArrayQuest *quests, int quest_id)
{
    Quest *quest;
    array_foreach(quest, quests)
        if (quest->quest_id == quest_id)
            return quest;
    return NULL;
}

void HandleQuestDescription(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t quest_id;
        uint16_t reward[128];
        uint16_t objective[128];
    } QuestDescription;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_QUEST_DESCRIPTION);
    assert(sizeof(QuestDescription) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    QuestDescription *pack = cast(QuestDescription *)packet;
    assert(client && client->game_srv.secured);

    Quest *quest = find_quest_by_id(&client->world.quests, pack->quest_id);
    if (quest == NULL) {
        LogInfo("Couldn't add a description for quest %d, because the quest was not found.", pack->quest_id);
        return;
    }

    assert(sizeof(pack->objective) == sizeof(quest->objective));
    memcpy(quest->objective, pack->objective, sizeof(pack->objective));
}

void HandleQuestAdd(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t quest_id;
        int32_t status;
        uint16_t category[8];
        uint16_t name[8];
        uint16_t givenby[8];
        int16_t map_id;
    } QuestAdd;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_QUEST_ADD);
    assert(sizeof(QuestAdd) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    QuestAdd *pack = cast(QuestAdd *)packet;
    assert(client && client->game_srv.secured);

    ArrayQuest *quests = &client->world.quests;
    Quest quest = {0};
    quest.quest_id = pack->quest_id;
    quest.status = pack->status;
    quest.map_id = pack->map_id;

    quest.id = quests->size;
    array_add(quests, quest);
    // LogInfo("Quest '%d' added !", pack->quest_id);
}

void HandleQuestUpdateMarker(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int32_t quest_id;
        Vec2f   marker_coord;
        int16_t unk1;
        int16_t unk2;
    } QuestMarker;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_QUEST_UPDATE_MARKER);
    assert(sizeof(QuestMarker) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    QuestMarker *pack = cast(QuestMarker *)packet;
    assert(client && client->game_srv.secured);

    ArrayQuest *quests = &client->world.quests;
    Quest *quest = find_quest_by_id(quests, pack->quest_id);
    quest->marker_coord = pack->marker_coord;
}

void HandleQuestRemove(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t quest_id;
    } QuestRemove;
#pragma pack(pop)
    
    assert(packet->header == GAME_SMSG_QUEST_REMOVE);
    assert(sizeof(QuestRemove) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    QuestRemove *pack = cast(QuestRemove *)packet;
    assert(client && client->game_srv.secured);

    ArrayQuest *quests = &client->world.quests;
    Quest *quest = find_quest_by_id(quests, pack->quest_id);
    if (!quest) {
#if 0 // this happens when e.g. entering fow - all the quests are getting removed first
        LogInfo("We were asked to remove a quest that we didn't have. (id: %d)", pack->quest_id);
#endif
        return;
    }

    array_remove(quests, quest->id);
}

void GameSrv_AbandonQuest(GwClient *client, int quest_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int32_t quest_id;
    } AbandonQuest;
#pragma pack(pop)

    AbandonQuest packet = NewPacket(GAME_CMSG_QUEST_ABANDON);
    packet.quest_id = quest_id;
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_RequestQuestInfos(GwClient *client, int quest_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int32_t quest_id;
    } RequestInfos;
#pragma pack(pop)

    RequestInfos packet = NewPacket(GAME_CMSG_QUEST_REQUEST_INFOS);
    packet.quest_id = quest_id;
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}
