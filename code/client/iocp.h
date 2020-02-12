#ifdef CORE_IOCP_H
#error "iocp.h included more than once"
#endif
#define CORE_IOCP_H

typedef void (*EventHandler)(struct event *e);
typedef struct IOCompletionPort {
    HANDLE              fd;

    struct list         posted_events;
} IOCompletionPort;

struct event {
    OVERLAPPED          ovlp;
    HANDLE              handle;

    struct list         node;
    void               *context;

    bool                posted;
    int                 error;
    size_t              bytes;
    EventHandler        handler;

    bool                timer_set;
    struct rbnode       timer;
};

bool iocp_init(IOCompletionPort *iocp, unsigned int number_of_thread);
bool iocp_attach(IOCompletionPort *iocp, HANDLE fd);
void iocp_process_all(IOCompletionPort *iocp);
void iocp_post_event(IOCompletionPort *iocp, struct event *ev);
void iocp_process_posted_event(IOCompletionPort *iocp);
