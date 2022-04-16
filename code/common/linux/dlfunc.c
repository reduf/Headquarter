#ifdef OS_DLFUNC_C
#error "dlfunc.c included more than once"
#endif
#define OS_DLFUNC_C

#include "../dlfunc.h"

void *dllopen(const char *filename)
{
    void* handle = dlopen(filename, RTLD_LAZY);
    if (handle)
        return handle;
    printf("dlopen Error: %s\n", dlerror());
    return handle;
}

int dllclose(void *handle)
{
    return dlclose(handle);
}

int dlllocation(char* buffer, int length)
{
    int ret = readlink("/proc/self/exe", buffer, length);
    int bytes = MIN(ret, length - 1);
    if (bytes >= 0)
        buffer[bytes] = '\0';
    return bytes;
}

int dlldir(char* buffer, int length)
{
    int len = dlllocation(buffer, length);
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
