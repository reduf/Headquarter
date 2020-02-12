#ifdef WIN32_H
#error "win32.h included more than once"
#endif
#define WIN32_H

#ifndef STRICT
# define STRICT
#endif

#ifndef NOMINMAX
# define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
