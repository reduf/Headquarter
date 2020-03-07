#ifndef COMMON_DLFUNC_H
#define COMMON_DLFUNC_H

void *dllopen(const char *filename);
int dllclose(void *handle);

void *dllsym(void *handle, const char *symbol);

#endif // COMMON_DLFUNC_H
