#ifndef COMMON_DLFUNC_H
#define COMMON_DLFUNC_H
#ifndef __cplusplus
# define HMODULE void*
#endif
void *dllopen(const char *filename);
int dllclose(void* handle);

void *dllsym(void* handle, const char *symbol);

#endif // COMMON_DLFUNC_H
