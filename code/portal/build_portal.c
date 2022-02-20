#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
# include <common/win32/win32.h>
# include <Winsock2.h>
# include <Ws2tcpip.h>
# pragma comment(lib, "Ws2_32.lib")
#else
#endif

#include <common/array.h>
#include <common/endian.h>
#include <common/macro.h>
#include <common/process.h>

#include "login.h"
#include "sts.h"

#include "login.c"
#include "sts.c"
