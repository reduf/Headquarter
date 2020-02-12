#ifndef COMMON_NORETURN_H
#define COMMON_NORETURN_H

#if !defined(__clang__) && defined(_MSC_VER)
# define _Noreturn  __declspec(noreturn)
#else
# include <stdnoreturn.h>
#endif

#endif // COMMON_NORETURN_H
