#ifdef CORE_PLAYBACK_C
#error "playback.c included more than once"
#endif
#define CORE_PLAYBACK_C

#pragma pack(push, 1)
struct replay_header {
    uint32_t magic;
    uint32_t file_version;
    uint32_t game_version;
    uint32_t number_of_segments;
    uint64_t t0;
};
struct replay_segment {
    uint64_t timestamp;
    uint32_t size;
};
#pragma pack(pop)

bool replay_open_record_file(struct replay_ctx *ctx, const char *path)
{
    ctx->t0 = time_get_ms();
    strncpy_s(ctx->path, sizeof(ctx->path), path, sizeof(ctx->path) - 1);
    ctx->file = fopen(path, "wb");
    if (!ctx->file) {
        return false;
    }
    ctx->number_of_segments = 0;
    fseek(ctx->file, sizeof(struct replay_header), SEEK_SET);
    return true;
}

void replay_close_file(struct replay_ctx *ctx)
{
    if (ctx->file) {
        replay_flush(ctx);
        fclose(ctx->file);
        ctx->file = NULL;
    }
}

void replay_flush(struct replay_ctx *ctx)
{
    assert(ctx && ctx->file);
    struct replay_header header;
    header.magic = REPLAY_SIGNATURE;
    header.t0 = ctx->t0;
    header.file_version = REPLAY_VERSION;
    header.game_version = GUILD_WARS_VERSION;
    header.number_of_segments = ctx->number_of_segments;
    long current_pos = ftell(ctx->file);
    fseek(ctx->file, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, ctx->file);
    fseek(ctx->file, current_pos, SEEK_SET);
    fflush(ctx->file);
}

void replay_record_segment(struct replay_ctx *ctx, const uint8_t *data, size_t size)
{
    ctx->number_of_segments += 1;
    struct replay_segment segment;
    segment.timestamp = time_get_ms();
    segment.size = size;
    fwrite(&segment, 1, sizeof(segment), ctx->file);
    fwrite(data, size, 1, ctx->file);
}