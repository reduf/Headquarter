#ifdef OS_DLFUNC_C
#error "dlfunc.c included more than once"
#endif
#define OS_DLFUNC_C

#include "../dlfunc.h"
#include <link.h>

void *dllopen(const char *filename)
{
    return dlopen(filename, RTLD_LAZY);
}

int dllclose(void *handle)
{
    return dlclose(handle);
}
int dlllocation(void* handle, char* buffer, int length) {
    int bytes = 0;
    if (!handle) {
        bytes = MIN(readlink("/proc/self/exe", buffer, length), length - 1);
        if (bytes >= 0)
            buffer[bytes] = '\0';
        return bytes;
    }
    else {
        struct link_map* lm;
        dlinfo(handle, RTLD_DI_LINKMAP, &lm);
        for (bytes = 0; lm->lm_name[bytes] && bytes < length - 1; bytes++)
            buffer[bytes] = lb->lm_name[bytes];
        buffer[bytes] = '\0';
    }
}
int dlldir(void* handle, char* buffer, int length) {
    int len = dlllocation(handle, buffer, length);
    if (len < 0)
        return len;
    char* p = strrchr(buffer, '/');
    if (p) p[0] = 0;
    return strlen(buffer);
}
void *dllsym(void *handle, const char *symbol)
{
    return dlsym(handle, symbol);
}
