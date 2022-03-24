#ifdef PORTAL_LOGIN_H
#error "login.h included more than once"
#endif
#define PORTAL_LOGIN_H

int portal_init();
void portal_free();

struct portal_login_result {
    struct uuid user_id;
    struct uuid token;
};

int portal_login(struct portal_login_result *result, const char *username, const char *password);
