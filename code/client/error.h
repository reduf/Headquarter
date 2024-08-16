#ifdef ERROR_H_INC
#error "error.h included more than once"
#endif
#define ERROR_H_INC

#if defined(_WIN32)
void win32_perror(const char *str);
# define os_errno (int)GetLastError()
# define os_perror(s) win32_perror(s)
#else
# include <errno.h>
# define os_errno errno
# define os_perror perror
#endif

const char *get_error_s(int error_code);
void win32_perror(const char *str);
