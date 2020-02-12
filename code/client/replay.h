#ifdef CORE_PLAYBACK_H
#error "playback.c included more than once"
#endif
#define CORE_PLAYBACK_H

#define REPLAY_VERSION      1
#define REPLAY_SIGNATURE    0x00525747 // GWR\0

struct replay_ctx {
    char        path[255];
    FILE       *file;
    uint32_t    number_of_segments;
    uint64_t    t0;
};

bool replay_open_record_file(struct replay_ctx *ctx, const char *path);

void replay_close_file(struct replay_ctx *ctx);

void replay_flush(struct replay_ctx *ctx);

void replay_record_segment(struct replay_ctx *ctx, const uint8_t *data, size_t size);