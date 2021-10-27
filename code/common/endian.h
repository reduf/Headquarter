/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2002 Thomas Moestl <tmm@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef COMMON_ENDIAN_H
#define COMMON_ENDIAN_H

#include <stdint.h>
#include <stdlib.h>

/*
 * General byte order swapping functions.
 */
#ifdef _WIN32
# define bswap16(x)  _byteswap_ushort(x)
# define bswap32(x)  _byteswap_ulong(x)
# define bswap64(x)  _byteswap_uint64(x)
#else
#include <byteswap.h>
# define bswap16(x)  bswap_16(x)
# define bswap32(x)  bswap_32(x)
# define bswap64(x)  bswap_64(x)
#endif

/*
 * Host to big endian, host to little endian, big endian to host, and little
 * endian to host byte order functions.
 */
#ifndef htobe16
#if _BYTE_ORDER == _LITTLE_ENDIAN
#define htobe16(x)  bswap16((x))
#define htobe32(x)  bswap32((x))
#define htobe64(x)  bswap64((x))
#define htole16(x)  ((uint16_t)(x))
#define htole32(x)  ((uint32_t)(x))
#define htole64(x)  ((uint64_t)(x))

#define be16toh(x)  bswap16((x))
#define be32toh(x)  bswap32((x))
#define be64toh(x)  bswap64((x))
#define le16toh(x)  ((uint16_t)(x))
#define le32toh(x)  ((uint32_t)(x))
#define le64toh(x)  ((uint64_t)(x))
#else /* _BYTE_ORDER != _LITTLE_ENDIAN */
#define htobe16(x)  ((uint16_t)(x))
#define htobe32(x)  ((uint32_t)(x))
#define htobe64(x)  ((uint64_t)(x))
#define htole16(x)  bswap16((x))
#define htole32(x)  bswap32((x))
#define htole64(x)  bswap64((x))

#define be16toh(x)  ((uint16_t)(x))
#define be32toh(x)  ((uint32_t)(x))
#define be64toh(x)  ((uint64_t)(x))
#define le16toh(x)  bswap16((x))
#define le32toh(x)  bswap32((x))
#define le64toh(x)  bswap64((x))
#endif /* _BYTE_ORDER == _LITTLE_ENDIAN */
#endif

static inline uint16_t
be16dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;

    return ((p[0] << 8) | p[1]);
}

static inline uint32_t
be32dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;

    return (((unsigned)p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
}

static inline uint64_t
be64dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;

    return (((uint64_t)be32dec(p) << 32) | be32dec(p + 4));
}

static inline uint16_t
le16dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;

    return ((p[1] << 8) | p[0]);
}

static inline uint32_t
le32dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;

    return (((unsigned)p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0]);
}

static inline uint64_t
le64dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;

    return (((uint64_t)le32dec(p + 4) << 32) | le32dec(p));
}

static inline void
be16enc(void *pp, uint16_t u)
{
    uint8_t *p = (uint8_t *)pp;

    p[0] = (u >> 8) & 0xff;
    p[1] = u & 0xff;
}

static inline void
be32enc(void *pp, uint32_t u)
{
    uint8_t *p = (uint8_t *)pp;

    p[0] = (u >> 24) & 0xff;
    p[1] = (u >> 16) & 0xff;
    p[2] = (u >> 8) & 0xff;
    p[3] = u & 0xff;
}

static inline void
be64enc(void *pp, uint64_t u)
{
    uint8_t *p = (uint8_t *)pp;

    be32enc(p, (uint32_t)(u >> 32));
    be32enc(p + 4, (uint32_t)(u & 0xffffffff));
}

static inline void
le16enc(void *pp, uint16_t u)
{
    uint8_t *p = (uint8_t *)pp;

    p[0] = u & 0xff;
    p[1] = (u >> 8) & 0xff;
}

static inline void
le32enc(void *pp, uint32_t u)
{
    uint8_t *p = (uint8_t *)pp;

    p[0] = u & 0xff;
    p[1] = (u >> 8) & 0xff;
    p[2] = (u >> 16) & 0xff;
    p[3] = (u >> 24) & 0xff;
}

static inline void
le64enc(void *pp, uint64_t u)
{
    uint8_t *p = (uint8_t *)pp;

    le32enc(p, (uint32_t)(u & 0xffffffff));
    le32enc(p + 4, (uint32_t)(u >> 32));
}

#endif // COMMON_ENDIAN_H
