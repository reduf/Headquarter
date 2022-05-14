typedef enum PlayerStatus {
    PlayerStatus_Offline        = 0,
    PlayerStatus_Online         = 1,
    PlayerStatus_DND            = 2,
    PlayerStatus_Away           = 3,
    PlayerStatus_Blank          = 4,
} PlayerStatus;

typedef enum FriendType {
    FriendType_Unknow           = 0,
    FriendType_Friend           = 1,
    FriendType_Ignored          = 2,
    FriendType_RecentlyPlayed   = 3,
    FriendType_RecentlyPraded   = 4,
} FriendType;

typedef struct ApiFriend {
    uint8_t  uuid[16];
    uint32_t type;
    uint32_t status;
    uint32_t map_id;
    uint16_t account[20];
    uint16_t playing[20];
} ApiFriend;
