#ifdef OS_DLFUNC_C
#error "dlfunc.c included more than once"
#endif
#define OS_DLFUNC_C

#include "../dlfunc.h"

void *dlopen(const char *filename)
{
    return LoadLibraryA(filename);
}

int dlclose(void *handle)
{
    if (FreeLibrary(handle))
        return 0;
    else
        return -1;
}

void *dlsym(void *handle, const char *symbol)
{
    return (void *)GetProcAddress(handle, symbol);
}
