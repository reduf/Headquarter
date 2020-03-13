#ifdef OS_DLFUNC_C
#error "dlfunc.c included more than once"
#endif
#define OS_DLFUNC_C

#include "../dlfunc.h"

void *dllopen(const char *filename)
{
    if (filename == NULL)
        return GetModuleHandle(NULL);
    return LoadLibraryA(filename);
}

int dllclose(void *handle)
{
    if (FreeLibrary(handle))
        return 0;
    else
        return -1;
}

void *dllsym(void *handle, const char *symbol)
{
    return (void *)GetProcAddress(handle, symbol);
}
