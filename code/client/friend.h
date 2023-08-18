#ifdef CORE_FRIEND_H
#error "friend.h included more than once"
#endif
#define CORE_FRIEND_H

typedef struct Friend {
    PlayerStatus    status;
    FriendType      type;
    struct kstr_hdr name;
    uint16_t        name_buffer[20];
    struct kstr_hdr account;
    uint16_t        account_buffer[20];
    struct uuid     uuid;
    uint32_t        zone;
} Friend;
typedef array(Friend) FriendArray;

Friend* get_friend(const struct uuid *u, const uint16_t* name);

void init_friend(Friend* friend)
{
    kstr_hdr_init(&friend->name, friend->name_buffer, ARRAY_SIZE(friend->name_buffer));
    kstr_hdr_init(&friend->account, friend->account_buffer, ARRAY_SIZE(friend->account_buffer));
    friend->status = PlayerStatus_Offline;
    friend->type = FriendType_Unknow;
    friend->zone = 0;
}

static void api_make_friend(ApiFriend* dest, Friend* src)
{
    dest->type = src->type;
    dest->status = src->status;
    dest->map_id = src->zone;
    uuid_enc_le(dest->uuid, &src->uuid);
    kstr_hdr_write(&src->account, dest->account, ARRAY_SIZE(dest->account));
    kstr_hdr_write(&src->name, dest->playing, ARRAY_SIZE(dest->playing));
}