#ifdef PORTAL_LOGIN_H
#error "login.h included more than once"
#endif
#define PORTAL_LOGIN_H

struct portal_login_result {
    struct uuid user_id;
    struct uuid token;
};

int portal_login(struct portal_login_result *result, const char *username, const char *password);
int portal_login_dummy(struct portal_login_result *result, const char *username, const char *password);
