#ifdef UNIXSTD_C
#error "unixstd.c included more than once"
#endif
#define UNIXSTD_C

#include "../process.h"

pid_t getpid(void)
{
    return (int)GetCurrentProcessId();
}
