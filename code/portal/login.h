#ifdef PORTAL_LOGIN_H
#error "login.h included more than once"
#endif
#define PORTAL_LOGIN_H

bool portal_login(const char *username, const char *password);
