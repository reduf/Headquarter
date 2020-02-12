#ifdef PORTAL_H
#error "portal.h included more than once"
#endif
#define PORTAL_H

bool portal_init(void);
void portal_cleanup(void);
void portal_login(string email, string password);

extern bool   portal_received_key;
extern uuid_t portal_user_id;
extern uuid_t portal_session_id;
