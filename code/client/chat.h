#ifdef CORE_CHAT_H
#error "chat.h included more than once"
#endif
#define CORE_CHAT_H

typedef array(uint16_t) array_u16_t;

typedef struct ChatMessage {
    Channel     channel;
    struct kstr str;
    size_t      allocated;
} ChatMessage;
typedef array(ChatMessage) ChatMessageArray;

typedef struct Chat {
    size_t next_message_entry;
    ChatMessageArray messages;

    array_u16_t str_builder;
} Chat;
