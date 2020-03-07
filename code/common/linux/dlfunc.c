#ifdef OS_DLFUNC_C
#error "dlfunc.c included more than once"
#endif
#define OS_DLFUNC_C

#include "../dlfunc.h"

void *dllopen(const char *filename)
{
    return dlopen(filename, 0);
}

int dllclose(void *handle)
{
    return dlclose(handle);
}

void *dllsym(void *handle, const char *symbol)
{
    return dlsym(handle, symbol);
}
