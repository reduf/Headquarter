#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <os/win32/win32.h>
#include <os/time.h>
#include <os/thread.h>

#include <win32/os/time.h>

#include <Headquarter.h>
#include "gw-helper.c"

#define MapID_Kamadan   449

typedef array(char *) array_charp_t;

static volatile bool running = false;
static thread_t      bot_thread;

static HANDLE hEvent;
static HANDLE mailslot;
static char   mailslot_name[256];
static char   character_name_buffer[64];
static string character_name;
static int    my_number;

static char  target_name[32];
static int   space = 0;
static int   radius = 0;

static msec_t move_deadlock = 30 * 1000;

static void   exit_with_status(const char *state, int exit_code);
static size_t read_mailslot(char *buffer, size_t n);
static size_t split_cmd(char *str, array_charp_t *cmds);

static bool safe_move(float x, float y, float randomness);
static void go_to_my_position(void);
static void go_to_my_spot_line(void);
static void go_to_my_pos_circle(void);

static int main_bot(void *param)
{
    running = true;

    while (!travel_wait(MapID_Kamadan, DISTRICT_AMERICAN, 1)) {
        if (!GetIsIngame())
            exit_with_status("Error", 1);
        time_sleep_sec(10);
    }

    char buffer[1024];
    array_charp_t args = {0};
    size_t count;

    hEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
    if (hEvent == NULL) {
        LogError("'CreateEventA' failed. ($lu)", GetLastError());
        return 1;
    }

    while (running) {
        count = read_mailslot(buffer, sizeof(buffer));      
        buffer[min(count, sizeof(buffer))] = 0;
        if (split_cmd(buffer, &args) == 0)
            continue;

        char *cmd = array_at(args, 0);
        if (!strcmp(cmd, "dance")) {
            SendChat(Channel_Emote, "dance *");
        } else if (!strcmp(cmd, "zrank")) {
            SendChat(Channel_Emote, "zrank *");
        } else if (!strcmp(cmd, "flex")) {
            SendChat(Channel_Emote, "flex *");
        } else if (!strcmp(cmd, "head")) {
            SendChat(Channel_Emote, "head *");
        } else if (!strcmp(cmd, "taunt")) {
            SendChat(Channel_Emote, "taunt *");
        } else if (!strcmp(cmd, "boo")) {
            SendChat(Channel_Emote, "boo *");
        } else if (!strcmp(cmd, "flute")) {
            SendChat(Channel_Emote, "flute *");
        } else if (!strcmp(cmd, "point")) {
            SendChat(Channel_Emote, "point *");
        } else if (!strcmp(cmd, "attention")) {
            SendChat(Channel_Emote, "attention *");
        } else if (!strcmp(cmd, "guitar")) {
            SendChat(Channel_Emote, "guitar *");
        } else if (!strcmp(cmd, "bow")) {
            SendChat(Channel_Emote, "bow *");
        } else if (!strcmp(cmd, "excited")) {
            SendChat(Channel_Emote, "excited *");
        } else if (!strcmp(cmd, "sit")) {
            SendChat(Channel_Emote, "sit *");
        } else if (!strcmp(cmd, "cascade")) {
            char temp[32];
            snprintf(temp, sizeof(temp), "zrank %d", my_number);
            SendChat(Channel_Emote, temp);
        } else if (!strcmp(cmd, "settings")) {
            // space/radius/target
            if (array_size(args) != 4) {
                LogError("Command 'settings' bad format");
                continue;
            }
            space  = atoi(array_at(args, 1));
            radius = atoi(array_at(args, 2));
            strncpy_s(target_name, countof(target_name), array_at(args, 3), countof(target_name) - 1);
        } else if (!strcmp(cmd, "positionDwa")) {
            go_to_my_position();
        } else if (!strcmp(cmd, "positionLine")) {
            go_to_my_spot_line();
        } else if (!strcmp(cmd, "positionCircle")) {
            go_to_my_pos_circle();
        } else if (!strcmp(cmd, "exit")) {
            exit_with_status("Exit", 0);
        } else if ('0' <= *cmd && *cmd <= '9') {
            ApiItem item;
            int mini_id = atoi(cmd);
            if (get_item_with_model_id(mini_id, &item)) {
                UseInventoryItem(item.item_id);
            } else {
                LogError("Couldn't find item with model id '%d'", mini_id);
            }
        } else {
            LogError("Unknow command '%s'", cmd);
        }
    }

    CloseHandle(hEvent);
    CloseHandle(mailslot);
    return 0;
}

static size_t read_mailslot(char *buffer, size_t n)
{
#if 0
    DWORD size;
    while (!GetMailslotInfo(mailslot, NULL, &size, NULL, NULL)) {
        mailslot = CreateMailslotA(mailslot_name, 0, MAILSLOT_WAIT_FOREVER, NULL);
        thread_yield();
    }

    if (size == MAILSLOT_NO_MESSAGE)
        return 0;

    if (size > n) {
        LogError("'read_mailslot' buffer too small. (%lu but need %lu)", n, size);
        exit_with_status("Error", 1);
    }
#endif

    OVERLAPPED ovlp = {0};
    ovlp.hEvent = hEvent;

    DWORD bytes;
    if (!ReadFile(mailslot, buffer, n, &bytes, &ovlp)) {
        DWORD err = GetLastError();
        if (err != ERROR_IO_PENDING) {
            LogError("'ReadFile' failed (%lu)", GetLastError());
            return 0;
        }
        err = WaitForSingleObject(hEvent, INFINITE);
        if (err != WAIT_OBJECT_0) {
            LogError("'WaitForSingleObject' failed (%lu)", GetLastError());
            return 0;
        }
        bytes = ovlp.InternalHigh;
    }

    LogInfo("MailSlot recv: '%.*s'", bytes, buffer);
    return bytes;
}

static size_t split_cmd(char *str, array_charp_t *cmds)
{
    char *next_tok;

    array_clear(*cmds);
    char *cmd = strtok_s(str, "/", &next_tok);
    while (cmd != NULL) {
        array_add(*cmds, cmd);
        cmd = strtok_s(NULL, "/", &next_tok);
    }
    return array_size(*cmds);
}

static bool safe_move(float x, float y, float randomness)
{
    Vec2f dest;
    dest.x = x + frand(-randomness, randomness);
    dest.y = y + frand(-randomness, randomness);

    LogDebug("MoveTo(%.2f, %.2f)", x, y);

    AgentId my_id = GetMyAgentId();
    if (!my_id)
        return false;

    MoveToPoint(dest);
    msec_t deadlock_end = time_get_ms() + move_deadlock;
    Vec2f pos = GetAgentPos(my_id);

    // We want to quit when we enter the radius centered at the given point
    while (dist2(dest, pos) > FLT_EPSILON) {
        if (!GetIsIngame() || (time_get_ms() >= deadlock_end)) {
            return false;
        }
        time_sleep_ms(16);
        pos = GetAgentPos(my_id);
    }
    return true;
}

static void go_to_my_position(void)
{
    ApiPlayer player;
    if (!get_player_with_name(target_name, &player)) {
        LogError("Can't find player '%s'", target_name);
        return;
    }
    ApiAgent agent;
    if (!GetAgent(&agent, player.agent_id)) {
        LogError("Couldn't find the Agent associated with the player '%s'", target_name);
        return;
    }
    safe_move(agent.position.x, agent.position.y, 50.f);
}

static void go_to_my_spot_line(void)
{
    /*
     * Find player from name, use his position & rotation to compute direction line (L').
     * Use L' to compute the perpendicular line L.
     */

    const int n_account = 12;
    int half_n_account = n_account / 2;

    int my_pos_number;
    if (my_number <= half_n_account)
        my_pos_number = my_number - half_n_account - 1;
    else
        my_pos_number = my_number - half_n_account;

    ApiPlayer player;
    if (!get_player_with_name(target_name, &player)) {
        LogError("Can't find player '%s'", target_name);
        return;
    }
    ApiAgent agent;
    if (!GetAgent(&agent, player.agent_id)) {
        LogError("Couldn't find the Agent associated with the player '%s'", target_name);
        return;
    }

    float rot = agent.rotation;
    float x = agent.position.x;
    float y = agent.position.y;

    float per_a = tanf(rot);
    float a = -1.f / per_a;
    // float b = y - (a * x);

    /*
     * We have L = ax + b & a point (x, y) which is the position of the player.
     * We compute (u, v) which is the unit vector for the sloap "a".
     *  - We have two points, (x, y) & (x+1, y+a) => direction vector is (1, a)
     *  - Unit direction vector is (1, a) / ||(1, a)||
     */
    float norm = sqrtf(1 + (a*a));
    float u = 1 / norm;
    float v = a / norm;

    /*
     * Our equation for the line is now L = (x, y) + p*s*(u, v) where
     *  - p is the player id (1, 2, ...)
     *  - s is the wanted space between the players
     */
    float new_x = x + my_pos_number * space * u;
    float new_y = y + my_pos_number * space * v;

    safe_move(new_x, new_y, 0.f);
    RotateToAngle(rot);
}

static void go_to_my_pos_circle(void)
{
    ApiPlayer player;
    if (!get_player_with_name(target_name, &player)) {
        LogError("Can't find player '%s'", target_name);
        return;
    }
    ApiAgent agent;
    if (!GetAgent(&agent, player.agent_id)) {
        LogError("Couldn't find the Agent associated with the player '%s'", target_name);
        return;
    }
        
    const int sector_count = 12;
    float angle = (float)(my_number * ((2 * M_PI) / 12));

    float x = agent.position.x;
    float y = agent.position.y;

    float s = sinf(angle);
    float c = cosf(angle);

    float x1 = x + s * (radius + 80.f);
    float y1 = y + c * (radius + 80.f);
    safe_move(x1, y1, 0);

    float x2 = x + s * radius;
    float y2 = y + c * radius;
    safe_move(x2, y2, 0);
}

static void exit_with_status(const char *state, int exit_code)
{
    CloseHandle(mailslot);
    LogFatal("%s (code=%d)", state, exit_code);
    exit(exit_code);
}

DllExport bool OnPluginLoad(void)
{
    character_name.bytes = character_name_buffer;
    character_name.count = GetCharacterName(character_name_buffer, sizeof(character_name_buffer));
    if (!character_name.count) {
        LogError("Couldn't get the player name");
        return false;
    }

    char buffer[32];
    const uint8_t *s = character_name.bytes;
    size_t k = 0;
    for (size_t i = 0; i < character_name.count; i++) {
        if (s[i] == ' ')
            continue;
        if (s[i] & ~0x7f) {
            LogError("Non-ascii character");
            return false;
        }
        buffer[k++] = s[i];
        if (s[i] == 0)
            break;
    }

    snprintf(mailslot_name, 256, "\\\\.\\mailslot\\%.*s", k, buffer);
    LogDebug("mailslot_name = %s", mailslot_name);
    mailslot = CreateMailslotA(mailslot_name, 0, MAILSLOT_WAIT_FOREVER, NULL);
    if (mailslot == INVALID_HANDLE_VALUE) {
        LogError("Couldn't open mailslot '%s'", mailslot_name);
        return false;
    }

    static const char *names[] = {
        "Character 1",
        "Character 2",
        "Character 3",
        "Character 4",
        "Character 5",
        "Character 6",
        "Character 7",
        "Character 8",
        "Character 9",
        "Character 10",
        "Character 11",
        "Character 12",
    };

    for (my_number = 0; my_number < ARRAY_SIZE(names); ++i) {
        if (!str_cmp(character_name, strmake(names[i]))) {
            break;
        }
    }

    if (my_number == ARRAY_SIZE(names)) {
        LogError("Character name '%.*s' not expected", character_name.count, character_name.bytes);
        return false;
    }

    ++my_number;

    thread_create(&bot_thread, main_bot, NULL);
    thread_detach(bot_thread);
    return true;
}

DllExport void OnPluginUnload(void)
{
    running = false;
}

DllExport void on_panic(const char *msg)
{
    exit_with_status("Error", 1);
}

#include <os/win32/thread.c>
