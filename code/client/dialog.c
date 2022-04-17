#ifdef CORE_DIALOG_C
#error "dialog.c included more than once"
#endif
#define CORE_DIALOG_C

void HandleDialogButton(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int8_t icon_id;
        uint16_t caption[128];
        int32_t dialog_id;
        int32_t skill_id;
    } Button;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_DIALOG_BUTTON);
    assert(sizeof(Button) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    Button *pack = cast(Button *)packet;
    assert(client && client->game_srv.secured);

    DialogInfo *dialog = &client->dialog;
    // LogInfo("DialogButton {icon: %d, dialog_id: %X}", pack->icon_id, pack->dialog_id);

    DialogButton button = {0};
    button.dialog_id = pack->dialog_id;
    button.icon_id = pack->icon_id;

    // @Remark:
    // We do that, because we will likely just alloc this array with a big enough size
    // during the client allocation/init.
    if (array_full(&dialog->buttons)) {
        LogInfo("DialogInfo::buttons needed a reallocation. Size before reallocation: %d", dialog->buttons.size);
    }
    array_add(&dialog->buttons, button);
}

void HandleDialogBody(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t body[122];
    } DialogBody;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_DIALOG_BODY);
    assert(sizeof(DialogBody) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    DialogBody *pack = cast(DialogBody *)packet;
    assert(client && client->game_srv.secured);

    DialogInfo *dialog = &client->dialog;
    assert(sizeof(pack->body) == sizeof(dialog->body));
    memcpy(dialog->body, pack->body, sizeof(dialog->body));
}

void HandleDialogSender(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
    } DialogSender;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_DIALOG_SENDER);
    assert(sizeof(DialogSender) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    DialogSender *pack = cast(DialogSender *)packet;
    assert(client && client->game_srv.secured);

    DialogInfo *dialog = &client->dialog;
    dialog->opened = true;
    dialog->interact_with = pack->agent_id;
    client->interact_with = pack->agent_id;

    // @Cleanup: Check if we alway get this packet before
    array_clear(&dialog->buttons);

    Event event;
    Event_Init(&event, EventType_DialogOpen);
    event.DialogOpen.sender_agent_id = pack->agent_id;
    broadcast_event(&client->event_mgr, &event);
}

void GameSrv_SendDialog(GwClient *client, int dialog_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t dialog_id;
    } DialogPack;
#pragma pack(pop)

    DialogInfo *dialog = &client->dialog;
    if (!dialog->opened) {
        LogInfo("We are sending dialog %d (0x%X), but the dialog box isn't openned.", dialog_id, dialog_id);
    }

    bool found_dialog_id = false;
    DialogButton *it;
    array_foreach(it, &dialog->buttons) {
        if (it->dialog_id == dialog_id) {
            found_dialog_id = true;
            break;
        }
    }

    if (!found_dialog_id) {
        LogInfo("We are sending dialog %d (0x%X), but there is no button with that dialog id.", dialog_id, dialog_id);
    }

    DialogPack packet = NewPacket(GAME_CMSG_SEND_DIALOG);
    packet.dialog_id = dialog_id;
    SendPacket(&client->game_srv, sizeof(packet), &packet);

    dialog_info_clear(dialog);
    client->interact_with = 0;
}
