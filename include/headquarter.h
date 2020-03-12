#ifndef HEADQUARTER_H
#define HEADQUARTER_H

#define GUILD_WARS_VERSION          (37015)
#define HEADQUARTER_VERSION_MAJOR   (1)
#define HEADQUARTER_VERSION_MINOR   (0)

#if defined(HEADQUARTER_BUILD_EXPORTS)
# ifdef _WIN32
#  define HQAPI __declspec(dllexport)
# else
#  define HQAPI __attribute__((visibility("default")))
# endif
#else
# define HQAPI extern
#endif

#include "str.h"
#include <common/array.h>
#include <common/noreturn.h>

typedef uint32_t uint;
typedef uint64_t msec_t;
typedef uint32_t AgentId;

typedef struct Vec2f {
    float x, y;
} Vec2f;

typedef struct Vec3f {
    float x, y, z;
} Vec3f;

#include "constants.h"
#include "object/bag.h"
#include "object/item.h"
#include "object/agent.h"
#include "object/party.h"
#include "object/quest.h"
#include "object/skill.h"
#include "object/trade.h"
#include "object/title.h"
#include "object/friend.h"
#include "object/player.h"
#include "object/plugin.h"

#include "event.h"

#ifdef HEADQUARTER_RUNTIME_LINKING

typedef void(__cdecl* Log_pt)(const char* fmt, ...);
Log_pt LogError, LogDebug, LogFatal, LogInfo, LogWarn;

typedef void(__cdecl* FreePluginAndExitThread_pt)(void* module, int retval);
FreePluginAndExitThread_pt FreePluginAndExitThread;

HQAPI size_t            GetPlugins(ApiPlugin* buffer, size_t length);
HQAPI bool              LoadPlugin(const char* path);

typedef bool(__cdecl* RegisterEvent_pt)(EventType event, CallbackEntry* entry);
RegisterEvent_pt RegisterEvent;
typedef bool(__cdecl* UnRegisterEvent_pt)(CallbackEntry* entry);
UnRegisterEvent_pt UnRegisterEvent;

typedef int(__cdecl* HQMSEC_pt)(void);
HQMSEC_pt GetPing, GetPvPTimer, GetWorldTime;

typedef bool(__cdecl* HQBOOL_pt)(void);
HQBOOL_pt GetIsIngame, GetIsConnected;

typedef void(__cdecl* HQVOID_pt)(void);
HQVOID_pt LeaveHall;

typedef void(__cdecl* HQVOID_pt)(uint32_t guild_id);
HQVOID_pt InteractAgent;
HQVOID_pt TravelHall;

typedef void(__cdecl* MoveToPoint_pt)(Vec2f pos);
MoveToPoint_pt MoveToPoint;

typedef void(__cdecl* RedirectMap_pt)(uint32_t map_id, uint32_t type, District district, int district_number);
RedirectMap_pt RedirectMap;

typedef int(__cdecl* HQINT_pt)(void);
HQINT_pt GetMapId, GetDistrict, GetDistrictNumber, GetMyAgentId;

typedef Vec2f(__cdecl* GetAgentPos_pt)(AgentId agent);
GetAgentPos_pt GetAgentPos;
typedef void(__cdecl* Travel_pt)(uint32_t map_id, District district, int district_number);
Travel_pt Travel;

static bool hq_init() {
    HMODULE hnd = GetModuleHandle(NULL);
    if (!hnd) {
        printf("ERROR Loading current process module: %d", GetLastError());
        return false;
    }
    assert(FreePluginAndExitThread = (FreePluginAndExitThread_pt)dllsym(hnd, "FreePluginAndExitThread"));

    assert(GetPing = (HQMSEC_pt)dllsym(hnd, "GetPing"));
    assert(GetPvPTimer = (HQMSEC_pt)dllsym(hnd, "GetPvPTimer"));
    assert(GetWorldTime = (HQMSEC_pt)dllsym(hnd, "GetWorldTime"));

    assert(LogError = (Log_pt)dllsym(hnd, "LogError"));
    assert(LogDebug = (Log_pt)dllsym(hnd, "LogDebug"));
    assert(LogFatal = (Log_pt)dllsym(hnd, "LogFatal"));
    assert(LogInfo = (Log_pt)dllsym(hnd, "LogInfo"));
    assert(LogWarn = (Log_pt)dllsym(hnd, "LogWarn"));

    assert(LeaveHall = (HQVOID_pt)dllsym(hnd, "LeaveHall"));
    assert(InteractAgent = (HQVOID_pt)dllsym(hnd, "InteractAgent"));
    assert(TravelHall = (HQVOID_pt)dllsym(hnd, "TravelHall"));

    assert(GetIsIngame = (HQBOOL_pt)dllsym(hnd, "GetIsIngame"));
    assert(GetIsConnected = (HQBOOL_pt)dllsym(hnd, "GetIsConnected"));

    assert(RegisterEvent = (RegisterEvent_pt)dllsym(hnd, "RegisterEvent"));
    assert(UnRegisterEvent = (UnRegisterEvent_pt)dllsym(hnd, "UnRegisterEvent"));

    assert(GetMapId = (HQINT_pt)dllsym(hnd, "GetMapId"));
    assert(GetDistrict = (HQINT_pt)dllsym(hnd, "GetDistrict"));
    assert(GetDistrictNumber = (HQINT_pt)dllsym(hnd, "GetDistrictNumber"));
    assert(GetMyAgentId = (HQINT_pt)dllsym(hnd, "GetMyAgentId"));

    assert(GetAgentPos = (GetAgentPos_pt)dllsym(hnd, "GetAgentPos"));
    assert(Travel = (Travel_pt)dllsym(hnd, "Travel"));

    // TODO: Other functions etc...
    return true;
}
#else
static bool hq_init() {};

HQAPI void __cdecl      LogError(const char* fmt, ...);
HQAPI void __cdecl      LogDebug(const char* fmt, ...);
HQAPI void __cdecl      LogFatal(const char* fmt, ...);
HQAPI void __cdecl      LogInfo(const char* fmt, ...);
HQAPI void __cdecl      LogWarn(const char* fmt, ...);

HQAPI _Noreturn void    FreePluginAndExitThread(void* module, int retval);
HQAPI size_t            GetPlugins(ApiPlugin* buffer, size_t length);
HQAPI bool              LoadPlugin(const char* path);

HQAPI bool              RegisterEvent(EventType event, CallbackEntry* entry);
HQAPI bool              UnRegisterEvent(CallbackEntry* entry);

HQAPI msec_t            GetPing(void);
HQAPI msec_t            GetPvPTimer(void);
HQAPI msec_t            GetWorldTime(void);

HQAPI bool              GetIsIngame(void);
HQAPI bool              GetIsConnected(void);
HQAPI void              LogoutToCharselect(void);

HQAPI int               GetMapId(void);
HQAPI District          GetDistrict(void);
HQAPI int               GetDistrictNumber(void);
HQAPI void              Travel(uint32_t map_id, District district, int district_number);
HQAPI void              TravelHall(uint32_t guild_id);
HQAPI void              LeaveHall(void);
HQAPI void              RedirectMap(uint32_t map_id, uint32_t type, District district, int district_number);

HQAPI bool              GetInCinematic(void);
HQAPI void              SkipCinematic(void);

HQAPI size_t            GetCharacterName(char* buffer, size_t length);

HQAPI size_t            GetPlayers(ApiPlayer* buffer, size_t length);
HQAPI size_t            GetPlayerName(uint32_t player_id, char* buffer, size_t length);
HQAPI bool              GetPlayerIsPartyLeader(uint32_t player_id);
HQAPI size_t            GetPlayersOfParty(uint32_t party_id, uint32_t* buffer, size_t length);

HQAPI AgentId           GetMyAgentId(void);
HQAPI uint32_t          GetMyGuildId(void);
HQAPI uint32_t          GetMyPlayerId(void);

HQAPI uint32_t          GetGuildOfAgent(uint32_t agent_id);
HQAPI uint32_t          GetGuildFaction(uint32_t guild_id, FactionType* type);

HQAPI bool              GetAgent(ApiAgent* agent, AgentId agent_id);
HQAPI Vec2f             GetAgentPos(AgentId agent_id);
HQAPI bool              GetAgentOfItem(ApiAgent* agent, uint32_t item_id);
HQAPI bool              GetAgentOfPlayer(ApiAgent* agent, uint32_t player_id);

HQAPI bool              GetAgentIsMoving(AgentId agent_id);
HQAPI AgentEffect       GetAgentEffects(AgentId agent_id);
HQAPI size_t            GetAgents(ApiAgent* buffer, size_t length);
HQAPI uint32_t          GetNpcIdOfAgent(AgentId agent_id);
HQAPI bool              GetItem(ApiItem* item, uint32_t item_id);
HQAPI bool              GetItemOfAgent(ApiItem* item, AgentId agent_id);
HQAPI BagEnum           GetItemLocation(uint32_t item_id, unsigned int* slot);
HQAPI size_t            GetBagCapacity(BagEnum bag);
HQAPI size_t            GetBagItems(BagEnum bag, ApiItem* buffer, size_t length);
HQAPI size_t            GetMerchantItems(ApiItem* buffer, size_t length);

HQAPI bool              GetQuest(ApiQuest* quest, uint32_t quest_id);
HQAPI size_t            GetQuests(ApiQuest* buffer, size_t length);

HQAPI FactionPoint      GetLuxonPoints(void);
HQAPI FactionPoint      GetKurzickPoints(void);
HQAPI FactionPoint      GetImperialPoints(void);
HQAPI FactionPoint      GetBalthazarPoints(void);
HQAPI void              DonateFaction(FactionType faction);

HQAPI void              MoveToPoint(Vec2f pos);
HQAPI void              MoveToCoord(float x, float y);
HQAPI void              RotateToAngle(float angle);
HQAPI void              ReturnToOutpost(void);
HQAPI Difficulty        GetDifficulty(void);
HQAPI void              SetDifficulty(Difficulty mode);

HQAPI void              SendChat(Channel channel, const char* msg);
HQAPI void              SendWhisper(const char* target, const char* msg);
HQAPI void              SendDialog(uint32_t dialog_id);

HQAPI void              MoveItem(uint32_t item_id, BagEnum bag, int slot);
HQAPI void              UnequipItem(EquipedItemSlot equip_slot, BagEnum bag, int slot);
HQAPI void              UseInventoryItem(uint32_t item_id);
HQAPI void              PickupItem(uint32_t item_id);
HQAPI void              InteractAgent(AgentId agent_id);

HQAPI void              DepositGold(int quant);
HQAPI void              WithdrawGold(int quant);
HQAPI int               GetGoldStorage(void);
HQAPI int               GetGoldCharacter(void);

HQAPI const SkillInfo* GetSkillInfo(uint32_t skill_id);
HQAPI void              GetSkillbar(uint32_t* skills, AgentId agent_id);
HQAPI bool              GetSkillCasting(int pos, AgentId* target_id);
HQAPI msec_t            GetSkillRecharge(int pos);
HQAPI void              UseSkill(uint32_t skill_id, AgentId target_id);

HQAPI void              HeroFlag(Vec2f pos, uint32_t hero_index);
HQAPI void              HeroUseSkill(uint32_t hero_index, uint32_t skill_id, AgentId target_id);
HQAPI void              HeroEnableSkill(uint32_t hero_index, uint32_t skill_id);
HQAPI void              HeroDisableSkill(uint32_t hero_index, uint32_t skill_id);
HQAPI void              HeroSetBehavior(uint32_t hero_index, HeroBehavior behavior);

HQAPI size_t            GetPartySize(uint32_t party_id);
HQAPI uint32_t          GetPartyLeader(uint32_t party_id);
HQAPI uint32_t          GetPartyOfPlayer(uint32_t player_id);
HQAPI bool              GetPartyIsDefeated(void);

HQAPI void              LeaveParty(void);
HQAPI void              AcceptInvite(uint32_t party_id);
HQAPI void              RefuseInvite(uint32_t party_id);
HQAPI void              GetPartyRecvInvites(uint32_t* buffer, size_t length);
HQAPI void              GetPartySentInvites(uint32_t* buffer, size_t length);
HQAPI void              SeekParty(PartySearchType type, const char* msg);

HQAPI uint32_t          GetTraderId(void);
HQAPI int               GetTradeGold(void);
HQAPI TradeState        GetTradeState(void);
HQAPI size_t            GetTradeItems(TradeItem* buffer, size_t length);

HQAPI void              TradeInitiate(AgentId partner);
HQAPI void              TradeAddItem(uint32_t item_id, uint32_t quantity);
HQAPI void              TradeRemoveItem(uint32_t item_id, uint32_t quantity);
HQAPI void              TradeSendOffer(int gold);
HQAPI void              TradeAccept(void);
HQAPI void              TradeCancel(void);
#endif
#endif // HEADQUARTER_H
