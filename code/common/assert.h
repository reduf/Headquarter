#ifndef COMMON_ASSERT_H
#define COMMON_ASSERT_H

#include <assert.h>

#if !defined(__clang__) && defined(_MSC_VER)
# define _Static_assert static_assert
# else
# define static_assert _Static_assert
#endif

#endif // COMMON_ASSERT_H
