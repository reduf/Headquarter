#ifdef CORE_AUTH_H
#error "auth.h included more than once"
#endif
#define CORE_AUTH_H

void AuthSrv_RegisterCallbacks(Connection *conn);
bool init_auth_connection(GwClient *client, const char *host);

void HandleErrorMessage(Connection *conn, size_t psize, Packet *packet);
void HandleServerReponse(Connection *conn, size_t psize, Packet *packet);

void HandleWhisperReceived(Connection *conn, size_t psize, Packet *packet);

void HandleSessionInfo(Connection *conn, size_t psize, Packet *packet);
void HandleAccountInfo(Connection *conn, size_t psize, Packet *packet);
void HandleCharacterInfo(Connection *conn, size_t psize, Packet *packet);
void HandleGameServerInfo(Connection *conn, size_t psize, Packet *packet);
void HandleAccountSettings(Connection *conn, size_t psize, Packet *packet);

void HandleFriendUpdateInfo(Connection *conn, size_t psize, Packet *packet);
void HandleFriendUpdateStatus(Connection *conn, size_t psize, Packet *packet);
void HandleFriendUpdateLocation(Connection *conn, size_t psize, Packet *packet);
void HandleFriendStreamEnd(Connection *conn, size_t psize, Packet *packet);

void AuthSrv_HeartBeat(Connection *conn, msec_t tick);
void AuthSrv_AskServerResponse(Connection *conn, uint32_t trans_id);
void AuthSrv_ComputerInfo(Connection *conn);
void AuthSrv_RequestInstance(Connection *conn, uint32_t trans_id,
    uint32_t map_id, uint32_t type, uint32_t region, uint32_t district, uint32_t language);
void AuthSrv_ChangeCharacter(Connection *conn, uint32_t trans_id, struct kstr *name);
void AuthSrv_SetPlayerStatus(Connection *conn, PlayerStatus status);
void AuthSrv_SendPacket35(Connection *conn);
