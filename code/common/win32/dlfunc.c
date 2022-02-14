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
    printf("LoadLibraryA Error: %d\n", GetLastError());
    return res;
}

int dllclose(void* handle)
{
    if (FreeLibrary((HMODULE)handle))
        return 0;
    else
        return -1;
}
int dlllocation(void* handle, char* buffer, int length) {
    return GetModuleFileName((HMODULE)handle, buffer, length);
}
int dlldir(void* handle, char* buffer, int length) {
    int len = dlllocation(handle, buffer, length);
    if (len < 0)
        return len;
    char* p = strrchr(buffer, '\\');
    if (p) p[0] = 0;
    return (int)strlen(buffer);
}

void *dllsym(void* handle, const char *symbol)
{
    void* proc_address = GetProcAddress((HMODULE)handle, symbol);
    if (proc_address)
        return proc_address;
    printf("GetProcAddress Error: %d findng symbol %s\n", GetLastError(), symbol);
    return proc_address;
}
