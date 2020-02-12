#ifndef COMMON_UUID_H
#define COMMON_UUID_H

#define UUID_DEFINED
typedef unsigned char uuid_t[16];

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "endian.h"

#define UUID_NODE_LEN 6

struct uuid {
    uint32_t time_low;
    uint16_t time_mid;
    uint16_t time_hi_and_version;
    uint8_t  clock_seq_hi_and_reserved;
    uint8_t  clock_seq_low;
    uint8_t  node[UUID_NODE_LEN];
};

static const uuid_t null_uuid = {0};

#define uuid_equals(a, b) (uuid_cmp(a, b) == 0)

static inline int
uuid_is_null(const uuid_t uu)
{
    return !memcmp(uu, null_uuid, sizeof(uuid_t));
}

static inline int
uuid_cmp(const uuid_t a, const uuid_t b)
{
    if (a == b) return 1;
    if (a == NULL) return uuid_is_null(b);
    if (b == NULL) return uuid_is_null(a);
    return memcmp(a, b, sizeof(uuid_t));
}

static inline void
uuid_copy(uuid_t dest, const uuid_t src)
{
    if (!dest || (dest == src)) return;
    if (src) {
        memcpy(dest, src, sizeof(uuid_t));
    } else {
        memcpy(dest, null_uuid, sizeof(uuid_t));
    }
}

static inline void
uuid_clear(uuid_t uu)
{
    if (!uu) return;
    memset(uu, 0, sizeof(uuid_t));
}

static inline uint16_t
uuid_hash(const uuid_t uu)
{
    struct uuid *u = (struct uuid *)uu;
    return (u ? (u->time_low & 0xffff) : 0);
}

static inline void
uuid_fprint(FILE *stream, const uuid_t uu)
{
    if (!uu) uu = null_uuid;
    const struct uuid *u = (const struct uuid *)uu;
    fprintf(stream, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        u->time_low, u->time_mid, u->time_hi_and_version,
        u->clock_seq_hi_and_reserved, u->clock_seq_low, u->node[0],
        u->node[1], u->node[2], u->node[3], u->node[4], u->node[5]);
}

static inline void
uuid_snprint(char *s, size_t n, const uuid_t uu)
{
    if (!uu) uu = null_uuid;
    const struct uuid *u = (const struct uuid *)uu;
    snprintf(s, n, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        u->time_low, u->time_mid, u->time_hi_and_version,
        u->clock_seq_hi_and_reserved, u->clock_seq_low, u->node[0],
        u->node[1], u->node[2], u->node[3], u->node[4], u->node[5]);
}

static inline void
uuid_enc_le(void *buf, const uuid_t uu)
{
    uint8_t *b = (uint8_t *)buf;
    const struct uuid *u = (const struct uuid *)uu;
    le32enc(b + 0, u->time_low);
    le16enc(b + 4, u->time_mid);
    le16enc(b + 6, u->time_hi_and_version);
    b[8] = u->clock_seq_hi_and_reserved;
    b[9] = u->clock_seq_low;
    for (int i = 0; i < UUID_NODE_LEN; i++)
        b[10 + i] = u->node[i];
}

static inline void
uuid_dec_le(const void *buf, uuid_t uu)
{
    const uint8_t *b = (const uint8_t *)buf;
    struct uuid *u = (struct uuid *)uu;
    u->time_low = le32dec(b);
    u->time_mid = le16dec(b + 4);
    u->time_hi_and_version = le16dec(b + 6);
    u->clock_seq_hi_and_reserved = b[8];
    u->clock_seq_low = b[9];
    for (int i = 0; i < UUID_NODE_LEN; i++)
        u->node[i] = b[10 + i];
}

static inline void
uuid_enc_be(void *buf, const uuid_t uu)
{
    uint8_t *b = (uint8_t *)buf;
    const struct uuid *u = (const struct uuid *)uu;
    be32enc(b + 0, u->time_low);
    be16enc(b + 4, u->time_mid);
    be16enc(b + 6, u->time_hi_and_version);
    b[8] = u->clock_seq_hi_and_reserved;
    b[9] = u->clock_seq_low;
    for (int i = 0; i < UUID_NODE_LEN; i++)
        b[10 + i] = u->node[i];
}

static inline void
uuid_dec_be(const void *buf, uuid_t uu)
{
    const uint8_t *b = (const uint8_t *)buf;
    struct uuid *u = (struct uuid *)uu;
    u->time_low = be32dec(b);
    u->time_mid = be16dec(b + 4);
    u->time_hi_and_version = be16dec(b + 6);
    u->clock_seq_hi_and_reserved = b[8];
    u->clock_seq_low = b[9];
    for (int i = 0; i < UUID_NODE_LEN; i++)
        u->node[i] = b[10 + i];
}

#endif // COMMON_UUID_H
