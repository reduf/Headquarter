#ifdef PORTAL_C
#error "portal.c included more than once"
#endif
#define PORTAL_C

typedef void (*OnPortalNotify_t)(uint32_t msgid, uint32_t unk1, void *data, void *param);

typedef void     (*PortalInitialize_t)(uint32_t port);
typedef void     (*PortalDestroy_t)(void);
typedef void     (*PortalStartCleanup_t)(void);
typedef void     (*PortalRegisterNotify_t)(OnPortalNotify_t cb, void *param);
typedef void     (*PortalLogin_t)(const wchar_t *email, const wchar_t *password, const wchar_t *region);
typedef void     (*PortalListGameAccounts_t)(const wchar_t *game);
typedef void     (*PortalRequestGameToken_t)(const wchar_t *game1, const wchar_t *game2);
typedef uint8_t *(*PortalGetUserId_t)(void);

static PortalInitialize_t           PortalInitialize;
static PortalDestroy_t              PortalDestroy;
static PortalStartCleanup_t         PortalStartCleanup;
static PortalRegisterNotify_t       PortalRegisterNotify;
static PortalLogin_t                PortalLogin;
static PortalListGameAccounts_t     PortalListGameAccounts;
static PortalRequestGameToken_t     PortalRequestGameToken;
static PortalGetUserId_t            PortalGetUserId;

static HMODULE hPortal;

bool   portal_received_key;
uuid_t portal_user_id;
uuid_t portal_session_id;

static void OnPortalNotify(uint32_t msgid, uint32_t unk1, void *data, void *param)
{
    uint8_t *user_id = NULL;
    uint8_t *bytes = (uint8_t *)data;

    switch (msgid) {
    case 0:
        PortalListGameAccounts(L"gw1");
        break;
    case 1:
        LogError("This account require double authentification");
        break;
    case 2:
        user_id = PortalGetUserId();
        for (int i = 0; i < 16; i++)
            portal_session_id[i] = bytes[i + 8];
        for (int i = 0; i < 16; i++)
            portal_user_id[i] = user_id[i];
        portal_received_key = true;
        break;
    case 4:
        PortalRequestGameToken(L"gw1", L"gw1");
        break;
    default:
        LogError("Unknow portal message: %lu", msgid);
    }
}

bool portal_init(void)
{
    hPortal = LoadLibraryA("data/GwLoginClient.dll");
    if (!hPortal) {
        LogError("Couldn't load Portal.dll (%d)", os_errno);
        return false;
    }

    *(FARPROC *)&PortalInitialize           = GetProcAddress(hPortal, "PortalInitialize");
    *(FARPROC *)&PortalDestroy              = GetProcAddress(hPortal, "PortalDestroy");
    *(FARPROC *)&PortalStartCleanup         = GetProcAddress(hPortal, "PortalStartCleanup");
    *(FARPROC *)&PortalRegisterNotify       = GetProcAddress(hPortal, "PortalRegisterNotify");
    *(FARPROC *)&PortalLogin                = GetProcAddress(hPortal, "PortalLogin");
    *(FARPROC *)&PortalListGameAccounts     = GetProcAddress(hPortal, "PortalListGameAccounts");
    *(FARPROC *)&PortalRequestGameToken     = GetProcAddress(hPortal, "PortalRequestGameToken");
    *(FARPROC *)&PortalGetUserId            = GetProcAddress(hPortal, "PortalGetUserId");

    PortalInitialize(6112);
    PortalRegisterNotify(OnPortalNotify, NULL);
    return true;
}

void portal_cleanup(void)
{
    if (!hPortal) return;
    PortalStartCleanup();
    PortalDestroy();
    FreeLibrary(hPortal);
}

void portal_login(struct kstr *email, struct kstr *password)
{
    wchar_t wemail[64];
    wchar_t wpassword[100];

    if (email->length >= ARRAY_SIZE(wemail)) {
        LogError("wemail buffer is too small %zu, but need %zu",
            ARRAY_SIZE(email), email->length + 1);
        return;
    }

    if (password->length >= ARRAY_SIZE(wpassword)) {
        LogError("wpassword buffer is too small %zu, but need %zu",
            ARRAY_SIZE(password), password->length + 1);
        return;
    }

    for (size_t i = 0; i < email->length; i++) {
        wemail[i] = email->buffer[i];
    }
    wemail[email->length] = 0;

    for (size_t i = 0; i < password->length; i++) {
        wpassword[i] = password->buffer[i];
    }
    wpassword[password->length] = 0;

    PortalLogin(wemail, wpassword, L"en");
}
