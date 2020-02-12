#ifdef CORE_FRIEND_H
#error "friend.h included more than once"
#endif
#define CORE_FRIEND_H

typedef struct Friend {
    PlayerStatus status;
    FriendType   type;
    string       name[20];
    string       account[20];
    int32_t      zone;
} Friend;
typedef array(Friend) FriendArray;
