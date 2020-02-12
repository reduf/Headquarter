#ifndef COMMON_ALIGN_H
#define COMMON_ALIGN_H

#if !defined(__clang__) && defined(_MSC_VER)
# define _Alignas(a)    __declspec(align(a))
# define _Alignof       __alignof
# define alignas        _Alignas
# define alignof        _Alignof
#else
# include <stdalign.h>
#endif

#endif // COMMON_ALIGN_H
