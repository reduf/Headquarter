/*
 * Copyright (c) 2017, Laurent Dufresne
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
 */

#ifndef STR_H_INC
#define STR_H_INC

#include <ctype.h>
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#define memzero(b, s) memset(b, 0, s)

#define strnpos    ((size_t)-1)

#define strnul(s)  ((s == NULL) || (*s == 0))
#define wcsnul(s)  ((s == NULL) || (*s == 0))

static void *
memrcpy(void *dest, const void *src, size_t num)
{
    char *s = (char *)src;
    char *d = (char *)dest;
    for (size_t i = num; i < num; i--)
        d[i] = s[i];
    return dest;
}

static inline char *
vmprintf(const char *fmt, va_list args)
{
    size_t count = vsnprintf(NULL, 0, fmt, args) + 1;
    size_t size = sizeof(char) * count;
    char *buffer = (char *)malloc(size);
    vsnprintf(buffer, size, fmt, args);
    return buffer;
}

static inline char *
mprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char *str = vmprintf(fmt, args);
    va_end(args);
    return str;
}

static inline wchar_t *
vmwprintf(const wchar_t *fmt, va_list args)
{
    size_t count = vswprintf(NULL, 0, fmt, args);
    size_t size = count * sizeof(wchar_t) + 2;
    wchar_t *str = (wchar_t *)malloc(size);
    vswprintf(str, size, fmt, args);
    return str;
}

static inline wchar_t *
mwprintf(const wchar_t *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    wchar_t *str = vmwprintf(fmt, args);
    va_end(args);
    return str;
}

static char *
strlwc(char *str, size_t len)
{
    unsigned char *s = (unsigned char *)str;
    for (size_t i = 0; i < len; i++) {
        if (s[i] == 0) break;
        s[i] = (unsigned char)tolower(s[i]);
    }
    return str;
}

static wchar_t *
wcslwc(wchar_t *str, size_t len)
{
    wchar_t *s = (wchar_t *)str;
    for (size_t i = 0; i < len; i++) {
        if (s[i] == 0) break;
        s[i] = (wchar_t)tolower(s[i]);
    }
    return str;
}

static char *
strupc(char *str, size_t len)
{
    unsigned char *s = (unsigned char *)str;
    for (size_t i = 0; i < len; i++) {
        if (s[i] == 0) break;
        s[i] = (unsigned char)toupper(s[i]);
    }
    return str;
}

static wchar_t *
wcsupc(wchar_t *str, size_t len)
{
    wchar_t *s = (wchar_t *)str;
    for (size_t i = 0; i < len; i++) {
        if (s[i] == 0) break;
        s[i] = (wchar_t)toupper(s[i]);
    }
    return str;
}

static size_t
strtowcs(wchar_t *dest, const char *src, size_t n)
{
    size_t i;
    unsigned char *s = (unsigned char *)src;
    for (i = 0; i < n-1; i++) {
        if (s[i] & ~0x7f)
            return 0;
        dest[i] = s[i];
        if (s[i] == 0) break;
    }
    dest[i] = 0;
    return i;
}

static size_t
wcstostr(char *dest, const wchar_t *src, size_t n)
{
    size_t i;
    unsigned char *d = (unsigned char *)dest;
    for (i = 0; i < n-1; i++) {
        if (src[i] & ~0x7f)
            return 0;
        d[i] = src[i] & 0x7f;
        if (src[i] == 0) break;
    }
    d[i] = 0;
    return i;
}

static int
stricmp(const char *str1, const char *str2)
{
    unsigned char *s1 = (unsigned char *)str1;
    unsigned char *s2 = (unsigned char *)str2;

    if (s1 == s2) return 0;
    if (s1 == NULL) return -*s2;
    if (s2 == NULL) return  *s1;

    for (; *s1 != 0; s1++, s2++) {
        if (tolower(*s1) != tolower(*s2))
            break;
    }

    return *s1 - *s2;
}

static int
wcsicmp(const wchar_t *s1, const wchar_t *s2)
{
    if (s1 == s2) return 0;
    if (s1 == NULL) return -*s2;
    if (s2 == NULL) return  *s1;

    for (; *s1 != 0; s1++, s2++) {
        if (tolower(*s1) != tolower(*s2))
            break;
    }

    return *s1 - *s2;
}

static int
strnicmp(const char *str1, const char *str2, size_t count)
{
    unsigned char *s1 = (unsigned char *)str1;
    unsigned char *s2 = (unsigned char *)str2;

    if ((s1 == s2) || (count == 0)) return 0;
    if (s1 == NULL) return -*s2;
    if (s2 == NULL) return  *s1;

    for (size_t n = 0; (*s1 != 0) && (n < count); s1++, s2++, n++) {
        if (tolower(*s1) != tolower(*s2))
            break;
    }

    return *s1 - *s2;
}

static int
wcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t count)
{
    if ((s1 == s2) || (count == 0)) return 0;
    if (s1 == NULL) return -*s2;
    if (s2 == NULL) return  *s1;

    for (size_t n = 0; (*s1 != 0) && (n < count); s1++, s2++, n++) {
        if (tolower(*s1) != tolower(*s2))
            break;
    }

    return *s1 - *s2;
}

/*
 * "size" is the number of character in "dest" buffer.
 * If "src" is longer than "size - 1", dest is an empty strings.
 * Otherwise, "src" is copied to "dest" and a nul character is
 * appended to the end of the string in dest.
 *
 * 1 is returned if all the characters where copied successfully.
 * 0 is returned otherwise.
 */
static int
safe_strcpy(char *dest, size_t size, const char *src)
{
    assert(dest != NULL);
    size_t length = src ? strlen(src) : 0;
    if (length >= size) {
        if (size > 0)
            dest[0] = 0;
        return 0;
    } else {
        memcpy(dest, src, length);
        dest[length] = 0;
        return 1;
    }
}

#endif // STR_H_INC
