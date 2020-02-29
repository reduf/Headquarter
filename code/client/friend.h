#ifdef CORE_FRIEND_H
#error "friend.h included more than once"
#endif
#define CORE_FRIEND_H

typedef struct Friend {
    PlayerStatus    status;
    FriendType      type;
    struct kstr     name;
    char            name_buffer;
    struct kstr     account;
    char            account_buffer;
    uint32_t        zone;
} Friend;
typedef array(Friend) FriendArray;
