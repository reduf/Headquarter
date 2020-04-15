#ifdef OS_DLFUNC_C
#error "dlfunc.c included more than once"
#endif
#define OS_DLFUNC_C
#include "../dlfunc.h"

void *dllopen(const char *filename)
{
    if (filename == NULL)
        return GetModuleHandle(NULL);
    HMODULE res = LoadLibraryA(filename);
    if (res)
        return res;
    printf("LoadLibraryA Error: %d", GetLastError());
    return res;
}

int dllclose(void* handle)
{
    if (FreeLibrary((HMODULE)handle))
        return 0;
    else
        return -1;
}

void *dllsym(void* handle, const char *symbol)
{
    void* proc_address = GetProcAddress((HMODULE)handle, symbol);
    if (proc_address)
        return proc_address;
    printf("GetProcAddress Error: %d", GetLastError());
    return proc_address;
}
