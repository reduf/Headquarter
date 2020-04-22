#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#include <stddef.h>

#define CAST_STRUCT_FROM_MEMBER(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define cast(T) (T)

#define indexof(base, offset) (int)(offset - base)

#define ROL16(x, n) ((x << n) | ((x & 0xFFFF)     >> (16 - n)))
#define ROL32(x, n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define ALIGN(v, a) (((v) + (a - 1)) & ~(a - 1))
#define ALIGN16(v)  ((v + 1) & ~1)
#define ALIGN32(v)  ((v + 3) & ~3)

#define MAX(a, b) (((b) < (a)) ? (a) : (b))
#define MIN(a, b) (((b) > (a)) ? (b) : (a))

#endif // COMMON_MACRO_H
