#ifndef COMMON_DLFUNC_H
#define COMMON_DLFUNC_H

void *dlopen(const char *filename);
int dlclose(void *handle);

void *dlsym(void *handle, const char *symbol);

#endif // COMMON_DLFUNC_H
