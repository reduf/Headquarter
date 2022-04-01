#ifndef HEADQUARTER_H
#define HEADQUARTER_H

#define GUILD_WARS_VERSION          (37169)
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

# ifndef _WIN32
#  define __cdecl __attribute__((__cdecl__))
# endif

#include "../include/str.h"
#include <common/array.h>
#include <common/noreturn.h>
#include <common/dlfunc.h>

#include <assert.h>

typedef uint32_t uint;
typedef uint64_t msec_t;
typedef uint32_t AgentId;

typedef struct Vec2f {
    float x, y;
} Vec2f;

typedef struct Vec3f {
    float x, y, z;
} Vec3f;

#include <client/constants.h>
#include <client/object/bag.h>
#include <client/object/item.h>
#include <client/object/agent.h>
#include <client/object/party.h>
#include <client/object/quest.h>
#include <client/object/skill.h>
#include <client/object/trade.h>
#include <client/object/title.h>
#include <client/object/friend.h>
#include <client/object/player.h>
#include <client/object/plugin.h>
#include <client/object/guild.h>

#include "event.h"

typedef struct PluginObject PluginObject;
typedef bool (*PluginEntry_pt)(PluginObject *);
typedef void (*PluginUnload_pt)(PluginObject *);

struct PluginObject {
    void           *module;

    PluginEntry_pt  PluginInit;
    PluginUnload_pt PluginUnload;
};

#ifdef HEADQUARTER_RUNTIME_LINKING

typedef void(__cdecl* Log_pt)(const char* fmt, ...);
static Log_pt LogError, LogDebug, LogCritical, LogInfo, LogWarn;

typedef void(__cdecl* FreePluginAndExitThread_pt)(PluginObject* plugin, int retval);
static FreePluginAndExitThread_pt FreePluginAndExitThread;

HQAPI uint32_t            GetPlugins(ApiPlugin* buffer, size_t length);
HQAPI bool              LoadPlugin(const char* path);

typedef bool(__cdecl* RegisterEvent_pt)(EventType event, CallbackEntry* entry);
static RegisterEvent_pt RegisterEvent;
typedef bool(__cdecl* UnRegisterEvent_pt)(CallbackEntry* entry);
static UnRegisterEvent_pt UnRegisterEvent;

typedef int(__cdecl* HQMSEC_pt)(void);
static HQMSEC_pt GetPing, GetPvPTimer, GetWorldTime;

typedef bool(__cdecl* HQBOOL_pt)(void);
static HQBOOL_pt GetIsIngame, GetIsConnected;

typedef void(__cdecl* HQVOID_pt)(void);
static HQVOID_pt LeaveHall, ReturnToOutpost;

typedef void(__cdecl* InteractAgent_pt)(uint32_t guild_id);
static InteractAgent_pt InteractAgent, TravelHall;

typedef void(__cdecl* MoveToPoint_pt)(Vec2f pos);
static MoveToPoint_pt MoveToPoint;
typedef void(__cdecl* MoveToCoord_pt)(float x, float y);
static MoveToCoord_pt MoveToCoord;
typedef void(__cdecl* RotateToAngle_pt)(float angle);
static RotateToAngle_pt RotateToAngle;

typedef Difficulty(__cdecl* GetDifficulty_pt)(void);
static GetDifficulty_pt GetDifficulty;
typedef void(__cdecl* SetDifficulty_pt)(Difficulty mode);
static SetDifficulty_pt SetDifficulty;

typedef void(__cdecl* RedirectMap_pt)(uint32_t map_id, uint32_t type, District district, int district_number);
static RedirectMap_pt RedirectMap;

typedef int(__cdecl* HQINT_pt)(void);
static HQINT_pt GetMapId, GetDistrict, GetDistrictNumber, GetMyAgentId, GetMyGuildId, GetMyPlayerId;

typedef Vec2f(__cdecl* GetAgentPos_pt)(AgentId agent);
static GetAgentPos_pt GetAgentPos;
typedef void(__cdecl* Travel_pt)(uint32_t map_id, District district, int district_number);
static Travel_pt Travel;

typedef bool(__cdecl* GetObject_pt)(void* object, uint32_t object_id);
static GetObject_pt GetAgent, GetGuildInfo;

typedef bool(__cdecl* GetFriend_pt)(ApiFriend* gwfriend, const void* name);
static GetFriend_pt GetFriend, GetFriendByUuid;

typedef AgentEffect(__cdecl* GetAgentEffects_pt)(AgentId agent_id);
static GetAgentEffects_pt GetAgentEffects;
typedef uint32_t(__cdecl* GetNpcIdOfAgent_pt)(AgentId agent_id);
static GetNpcIdOfAgent_pt GetNpcIdOfAgent;
typedef uint32_t(__cdecl* GetBagItems_pt)(BagEnum bag, ApiItem* buffer, size_t length);
static GetBagItems_pt GetBagItems;
typedef uint32_t(__cdecl* GetBagCapacity_pt)(BagEnum bag);
static GetBagCapacity_pt GetBagCapacity;

typedef uint32_t(__cdecl* GetBuffer_pt)(void* buffer, size_t length);
static GetBuffer_pt GetQuests, GetPlayers, GetAgents, GetFriends, GetMerchantItems, GetCharacterName, GetGuildMembers;

typedef uint32_t(__cdecl* GetObjectName_pt)(uint32_t object_id, void* buffer, size_t length);
static GetObjectName_pt GetPlayerName, GetItemName, GetItemModStruct;

typedef void(__cdecl* GetSkillbar_pt)(uint32_t* skills, AgentId agent_id);
static GetSkillbar_pt GetSkillbar;
typedef void(__cdecl* UseSkill_pt)(uint32_t skill_id, AgentId target_id);
static UseSkill_pt UseSkill;
typedef bool(__cdecl* GetSkillCasting_pt)(int pos, AgentId* target_id);
static GetSkillCasting_pt GetSkillCasting;

typedef uint32_t(__cdecl* GetGuildFaction_pt)(uint32_t guild_id, FactionType* type);
static GetGuildFaction_pt GetGuildFaction;

typedef void(__cdecl* SendChat_pt)(Channel channel, const char* msg);
static SendChat_pt SendChat;
typedef void(__cdecl* SendWhisper_pt)(const char* target, const char* msg);
static SendWhisper_pt SendWhisper;


typedef bool(__cdecl* GetItem_pt)(ApiItem* item, uint32_t item_id);
static GetItem_pt GetItem;
typedef BagEnum(__cdecl* GetItemLocation_pt)(uint32_t item_id, unsigned int* slot);
static GetItemLocation_pt GetItemLocation;
typedef bool(__cdecl* RequestItemQuote_pt)(uint32_t item_id);
static RequestItemQuote_pt RequestItemQuote;

static bool hq_init() {
    void* hnd = dllopen(NULL);
    if (!hnd) {
        //printf("ERROR Loading current process module: %d", GetLastError());
        return false;
    }

    assert(FreePluginAndExitThread = (FreePluginAndExitThread_pt)dllsym(hnd, "FreePluginAndExitThread"));
    //return false;
    assert(GetPing = (HQMSEC_pt)dllsym(hnd, "GetPing"));
    assert(GetPvPTimer = (HQMSEC_pt)dllsym(hnd, "GetPvPTimer"));
    assert(GetWorldTime = (HQMSEC_pt)dllsym(hnd, "GetWorldTime"));

    assert(LogError = (Log_pt)dllsym(hnd, "LogError"));
    assert(LogDebug = (Log_pt)dllsym(hnd, "LogDebug"));
    assert(LogCritical = (Log_pt)dllsym(hnd, "LogCritical"));
    assert(LogInfo = (Log_pt)dllsym(hnd, "LogInfo"));
    assert(LogWarn = (Log_pt)dllsym(hnd, "LogWarn"));

    assert(LeaveHall = (HQVOID_pt)dllsym(hnd, "LeaveHall"));
    assert(ReturnToOutpost = (HQVOID_pt)dllsym(hnd, "ReturnToOutpost"));

    assert(InteractAgent = (InteractAgent_pt)dllsym(hnd, "InteractAgent"));
    assert(TravelHall = (InteractAgent_pt)dllsym(hnd, "TravelHall"));

    assert(GetIsIngame = (HQBOOL_pt)dllsym(hnd, "GetIsIngame"));
    assert(GetIsConnected = (HQBOOL_pt)dllsym(hnd, "GetIsConnected"));

    assert(RegisterEvent = (RegisterEvent_pt)dllsym(hnd, "RegisterEvent"));
    assert(UnRegisterEvent = (UnRegisterEvent_pt)dllsym(hnd, "UnRegisterEvent"));

    assert(GetMapId = (HQINT_pt)dllsym(hnd, "GetMapId"));
    assert(GetDistrict = (HQINT_pt)dllsym(hnd, "GetDistrict"));
    assert(GetDistrictNumber = (HQINT_pt)dllsym(hnd, "GetDistrictNumber"));
    assert(GetMyAgentId = (HQINT_pt)dllsym(hnd, "GetMyAgentId"));
    assert(GetMyPlayerId = (HQINT_pt)dllsym(hnd, "GetMyPlayerId"));
    assert(GetMyGuildId = (HQINT_pt)dllsym(hnd, "GetMyGuildId"));

    assert(GetAgentPos = (GetAgentPos_pt)dllsym(hnd, "GetAgentPos"));
    assert(Travel = (Travel_pt)dllsym(hnd, "Travel"));

    assert(MoveToPoint = (MoveToPoint_pt)dllsym(hnd, "MoveToPoint"));
    assert(MoveToCoord = (MoveToCoord_pt)dllsym(hnd, "MoveToCoord"));
    assert(RotateToAngle = (RotateToAngle_pt)dllsym(hnd, "RotateToAngle"));

    assert(GetDifficulty = (GetDifficulty_pt)dllsym(hnd, "GetDifficulty"));
    assert(SetDifficulty = (SetDifficulty_pt)dllsym(hnd, "SetDifficulty"));

    assert(GetAgentEffects = (GetAgentEffects_pt)dllsym(hnd, "GetAgentEffects"));
    assert(SetDifficulty = (SetDifficulty_pt)dllsym(hnd, "SetDifficulty"));

    assert(SendChat = (SendChat_pt)dllsym(hnd, "SendChat"));
    assert(SendWhisper = (SendWhisper_pt)dllsym(hnd, "SendWhisper"));

    assert(GetItem = (GetItem_pt)dllsym(hnd, "GetItem"));
    assert(GetItemLocation = (GetItemLocation_pt)dllsym(hnd, "GetItemLocation"));
    assert(GetMerchantItems = (GetBuffer_pt)dllsym(hnd, "GetMerchantItems"));
    assert(RequestItemQuote = (RequestItemQuote_pt)dllsym(hnd, "RequestItemQuote"));

    assert(GetAgents = (GetBuffer_pt)dllsym(hnd, "GetAgents"));
    assert(GetPlayers = (GetBuffer_pt)dllsym(hnd, "GetPlayers"));
    assert(GetFriends = (GetBuffer_pt)dllsym(hnd, "GetFriends"));
    assert(GetQuests = (GetBuffer_pt)dllsym(hnd, "GetQuests"));
    assert(GetGuildMembers = (GetBuffer_pt)dllsym(hnd, "GetGuildMembers"));
    assert(GetCharacterName = (GetBuffer_pt)dllsym(hnd, "GetCharacterName"));

    assert(GetNpcIdOfAgent = (GetNpcIdOfAgent_pt)dllsym(hnd, "GetNpcIdOfAgent"));

    assert(GetItemName = (GetObjectName_pt)dllsym(hnd, "GetItemName"));
    assert(GetPlayerName = (GetObjectName_pt)dllsym(hnd, "GetPlayerName"));
    assert(GetItemModStruct = (GetObjectName_pt)dllsym(hnd, "GetItemModStruct"));

    assert(GetAgent = (GetObject_pt)dllsym(hnd, "GetAgent"));
    assert(GetGuildInfo = (GetObject_pt)dllsym(hnd, "GetGuildInfo"));
    
    assert(GetFriend = (GetFriend_pt)dllsym(hnd, "GetFriend"));
    assert(GetFriendByUuid = (GetFriend_pt)dllsym(hnd, "GetFriendByUuid"));

    dllclose(hnd);
    // TODO: Other functions etc...
    return true;
}
#else
static bool hq_init() {};

HQAPI void      LogError(const char *fmt, ...);
HQAPI void      LogDebug(const char *fmt, ...);
HQAPI void      LogCritical(const char *fmt, ...);
HQAPI void      LogInfo(const char *fmt, ...);
HQAPI void      LogWarn(const char *fmt, ...);

HQAPI _Noreturn void    FreePluginAndExitThread(PluginObject *module, int retval);
HQAPI size_t            GetPlugins(ApiPlugin *buffer, size_t length);
HQAPI bool              LoadPlugin(const char *path);

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
HQAPI void              Travel(uint32_t map_id, District district, uint16_t district_number);
HQAPI void              TravelHall(uint32_t guild_id);
HQAPI void              LeaveHall(void);
HQAPI void              RedirectMap(uint32_t map_id, uint32_t type, District district, int district_number);

HQAPI bool              GetInCinematic(void);
HQAPI void              SkipCinematic(void);

HQAPI size_t            GetCharacterName(char* buffer, size_t length);

HQAPI size_t            GetPlayers(ApiPlayer* buffer, size_t length);
HQAPI size_t            GetPlayerName(uint32_t player_id, uint16_t* buffer, size_t length);
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

HQAPI bool              GetQuest(ApiQuest *quest, uint32_t quest_id);
HQAPI size_t            GetQuests(ApiQuest *buffer, size_t length);

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

HQAPI void              MoveItem(uint32_t item_id, BagEnum bag, uint8_t slot);
HQAPI void              UnequipItem(EquipedItemSlot equip_slot, BagEnum bag, uint8_t slot);
HQAPI void              UseInventoryItem(uint32_t item_id);
HQAPI void              PickupItem(uint32_t item_id);
HQAPI void              InteractAgent(AgentId agent_id);

HQAPI void              DepositGold(int quant);
HQAPI void              WithdrawGold(int quant);
HQAPI int               GetGoldStorage(void);
HQAPI int               GetGoldCharacter(void);

HQAPI const SkillInfo  *GetSkillInfo(uint32_t skill_id);
HQAPI void              GetSkillbar(uint32_t *skills, AgentId agent_id);
HQAPI bool              GetSkillCasting(size_t pos, AgentId *target_id);
HQAPI msec_t            GetSkillRecharge(int pos);
HQAPI void              UseSkill(uint32_t skill_id, AgentId target_id);
HQAPI void              Attack(AgentId target_id);

HQAPI void              LoadPlayerSkillTemplate(const char* temp);
HQAPI void              LoadHeroSkillTemplate(const char* temp, uint32_t hero_index);

HQAPI void              HeroAdd(HeroID hero_id);
HQAPI void              HeroKick(HeroID hero_id);
HQAPI void              HeroKickAll();
HQAPI void              HeroFlagCancel(uint32_t hero_index);
HQAPI void              HeroFlag(Vec2f pos, uint32_t hero_index);
HQAPI void              HeroFlagCancelAll();
HQAPI void              HeroFlagAll(Vec2f pos);
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
