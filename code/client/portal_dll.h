#ifdef PORTAL_DLL_H
#error "portal_dll.h included more than once"
#endif
#define PORTAL_DLL_H

bool portal_dll_init(void);
void portal_dll_cleanup(void);
void portal_dll_login(struct kstr *email, struct kstr *password);

extern bool   portal_received_key;
extern uuid_t portal_user_id;
extern uuid_t portal_session_id;
