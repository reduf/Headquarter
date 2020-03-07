#ifndef COMMON_UNIXSTD_H
#define COMMON_UNIXSTD_H

#ifdef _WIN32

typedef int pid_t;
pid_t getpid(void);

#else
# include <unistd.h>
#endif

#endif // COMMON_UNIXSTD_H
