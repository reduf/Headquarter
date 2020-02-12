#ifndef COMMON_HASH_H
#define COMMON_HASH_H

#include <ctype.h>
#include <stdint.h>

#define hash_long(n) hash_int32(n)

static inline uint32_t
hash_int32(uint32_t n)
{
    uint32_t hash;

    hash = n;
    hash = ~hash + (hash << 15);
    hash ^= (hash >> 12);
    hash += (hash << 2);
    hash ^= (hash >> 4);
    hash += (hash << 3) + (hash << 11);
    hash ^= (hash >> 16);

    return hash;
}

static inline uint64_t
hash_int64(uint64_t n)
{
    uint64_t hash;

    hash = n;
    hash = ~hash + (hash << 21);
    hash ^= (hash >> 24);
    hash += (hash << 3) + (hash << 8);
    hash ^= (hash >> 14);
    hash += (hash << 2) + (hash << 4);
    hash ^= (hash >> 28);
    hash += (hash << 31);

    return hash;
}

static inline uintptr_t
hash_ptr(const void *ptr)
{
#ifdef _WIN64
    return (uintptr_t)hash_int64((uintptr_t)ptr);
#else
    return (uintptr_t)hash_int32((uintptr_t)ptr);
#endif
}

static inline unsigned long
hash_str(const char *str)
{
    unsigned long hash;
    char c;

    for (hash = 0; (c = *str) != 0; str++) {
        hash = ((hash << 5) - hash) + c;
    }

    return hash;
}

static inline unsigned long
hash_strn(const char *str, size_t size)
{
    unsigned long hash = 0;

    for (size_t i = 0; i < size; i++) {
        if (str[i] == 0)
            break;
        hash = ((hash << 5) - hash) + str[i];
    }

    return hash;
}

static inline unsigned long
hash_stri(const char *str)
{
    unsigned long hash;
    char c;

    for (hash = 0; (c = *str) != 0; str++) {
        int lc = tolower(c);
        hash = ((hash << 5) - hash) + lc;
    }

    return hash;
}

static inline unsigned long
hash_strni(const char *str, size_t size)
{
    unsigned long hash = 0;

    for (size_t i = 0; i < size; i++) {
        if (str[i] == 0)
            break;
        int lc = tolower(str[i]);
        hash = ((hash << 5) - hash) + lc;
    }

    return hash;
}

#endif // COMMON_HASH_H
