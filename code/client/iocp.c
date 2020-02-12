#ifdef CORE_IOCP_C
#error "iocp.c included more than once"
#endif
#define CORE_IOCP_C

bool iocp_init(IOCompletionPort *iocp, unsigned int number_of_thread)
{
    assert(iocp != NULL);
    iocp->fd = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, number_of_thread);
    if (iocp == NULL) {
        LogError("Couldn't create io completion port (%lu)", os_errno);
        return false;
    }

    list_init(&iocp->posted_events);
    return true;
}

bool iocp_attach(IOCompletionPort *iocp, HANDLE fd)
{
    assert(iocp && fd);
    if (CreateIoCompletionPort(fd, iocp->fd, 0, 0) == iocp->fd) {
        return true;
    } else {
        return false;
    }
}

void iocp_process_all(IOCompletionPort *iocp)
{
    struct event *ev;
    ULONG_PTR     key;
    int           error;
    DWORD         bytes;
    BOOL          retval;

    for (;;) {
        retval = GetQueuedCompletionStatus(iocp->fd, &bytes, &key, cast(LPOVERLAPPED *)&ev, 0);
        if (retval) {
            error = 0;
        } else {
            error = os_errno;
        }

        if (error != 0 && ev == NULL)
            return;

        if (error) {
            // @Cleanup: We should handle this (check WSAGetOverlappedResult)
            LogDebug("Error I/O error: %lu, bytes = %lu, key = %lu", error, bytes, key);
            if (error == ERROR_NETNAME_DELETED) {
                NetConn_Remove(ev->context);
            }
            // ERROR_OPERATION_ABORTED
            continue;
        }

        ev->bytes = bytes;
        ev->handler(ev);
    }
}

void iocp_post_event(IOCompletionPort *iocp, struct event *ev)
{
    if (ev->posted) return;
    if (list_node_unlinked(&ev->node))
        list_insert_tail(&iocp->posted_events, &ev->node);
}

void iocp_process_posted_event(IOCompletionPort *iocp)
{
    struct list  *posted = &iocp->posted_events;
    struct event *ev;
    while (!list_empty(posted)) {
        ev = list_first_entry(posted, struct event, node);
        list_remove(&ev->node);
        list_node_init(&ev->node);
        ev->handler(ev);
    }
}
