#ifdef CORE_GAME_H
#error "game.h included more than once"
#endif
#define CORE_GAME_H

void GameSrv_RegisterCallbacks(Connection *conn);

void GameSrv_MoveToCoord(GwClient *client, float x, float y);
void GameSrv_InteractLiving(GwClient *client, AgentId agent_id);
void GameSrv_InteractGadget(GwClient *client, AgentId agent_id);
void GameSrv_RotateToAngle(GwClient *client, float rotation);
void GameSrv_SendChat(GwClient *client, Channel channel, struct kstr *msg);
void GameSrv_SendWhisper(GwClient *client, struct kstr *target, struct kstr *msg);
void GameSrv_Disconnect(GwClient *client);
void GameSrv_SkipCinematic(GwClient *client);
void GameSrv_ChangeGold(GwClient *client, int gold_character, int gold_storage);
void GameSrv_SendDialog(GwClient *client, int dialog_id);
void GameSrv_HeartBeat(Connection *conn);
void GameSrv_PingRequest(Connection *conn);
void GameSrv_HeroUseSkill(GwClient *client, AgentId hero_id,
    int skill_id, uint32_t flags, AgentId target_id);
void GameSrv_HeroSetBehavior(GwClient *client, AgentId agent_id, HeroBehavior behavior);
void GameSrv_HeroSkillToggle(GwClient *client, AgentId agent_id, int skill_slot);
void GameSrv_TravelGH(GwClient *client, const uuid_t guild_uuid);
void GameSrv_LeaveGH(GwClient *client);
void GameSrv_Travel(GwClient *client, int map_id, District district, int district_number);
void GameSrv_ReturnToOutpost(GwClient *client);
void GameSrv_UseItem(GwClient *client, Item *item);
void GameSrv_InteractItem(GwClient *client, uint32_t agent_id);
void GameSrv_MoveItem(GwClient *client, Item *item, Bag *bag, int slot);
void GameSrv_StartSalvage(GwClient *client, Item *kit, Item *item);
void GameSrv_CancelSalvage(GwClient *client);
void GameSrv_SalvageDone(GwClient *client);
void GameSrv_SalvageMaterials(GwClient *client);
void GameSrv_SalvageUpgrade(GwClient *client, size_t index);
void GameSrv_UnequipItem(GwClient *client, EquipedItemSlot equip_slot, Bag *bag, int slot);
void GameSrv_TransactItems(GwClient *client, TransactionType type,
    int gold_send, TransactionInfo *send_info,
    int gold_recv, TransactionInfo *recv_info);
void GameSrv_BuyMaterials(GwClient *client, TransactionType type,
    int gold_send, TransactionInfo *send_info,
    int gold_recv, TransactionInfo *recv_info);
void GameSrv_RequestQuote(GwClient *client, TransactionType type,
    QuoteInfo *send_info, QuoteInfo *recv_info);
void GameSrv_FlagHero(GwClient *client, Vec2f pos, AgentId hero_agent_id);
void GameSrv_FlagAllHero(GwClient *client, Vec2f pos);
void GameSrv_AcceptInvite(GwClient *client, int party_id);
void GameSrv_LeaveParty(GwClient *client);
void GameSrv_RefuseInvite(GwClient *client, int party_id);
void GameSrv_PartySetTick(GwClient *client, bool ticked);
void GameSrv_SetDifficulty(GwClient *client, Difficulty mode);
void GameSrv_AddHero(GwClient *client, HeroID hero_id);
void GameSrv_KickHero(GwClient *client, HeroID hero_id);
void GameSrv_PS_SeekParty(GwClient *client, PartySearchType type, struct kstr *msg);
void GameSrv_PS_CancelSeek(GwClient *client);
void GameSrv_PS_RequestJoin(GwClient *client, int party_search_id);
void GameSrv_PS_RequestReply(GwClient *client, int party_search_id);
void GameSrv_PS_ChangeType(GwClient *client, PartySearchType type);
void GameSrv_DonateFaction(GwClient *client, FactionType faction, int amount);
void GameSrv_AbandonQuest(GwClient *client, int quest_id);
void GameSrv_RequestQuestInfos(GwClient *client, int quest_id);
void GameSrv_UseSkill(GwClient *client, int skill_id, uint32_t flags, AgentId target_id);
void GameSrv_TradeAcknowledge(GwClient *client, int32_t player_id);
void GameSrv_TradeCancel(GwClient *client);
void GameSrv_TradeAddItem(GwClient *client, uint32_t item_id, uint8_t quantity);
void GameSrv_TradeSendOffer(GwClient *client, int gold);
void GameSrv_TradeRemoveItem(GwClient *client, int32_t item_id, int quantity);
void GameSrv_TradeCancelOffer(GwClient *client);
void GameSrv_TradeAccept(GwClient *client);
void GameSrv_TradeInitiate(GwClient *client, AgentId partner);


void HandleInstanceShowWin(Connection *conn, size_t psize, Packet *packet);
void HandleInstanceLoadHead(Connection *conn, size_t psize, Packet *packet);
void HandleInstanceCountdownStop(Connection *conn, size_t psize, Packet *packet);
void HandleInstanceCountdown(Connection *conn, size_t psize, Packet *packet);
void HandleInstanceLoadPlayerName(Connection *conn, size_t psize, Packet *packet);
void HandleInstanceLoadSpawnPoint(Connection *conn, size_t psize, Packet *packet);
void HandleInstanceLoadInfo(Connection *conn, size_t psize, Packet *packet);
void HandleGameTransferInfo(Connection *conn, size_t psize, Packet *packet);
void HandleReadyForMapSpawn(Connection *conn, size_t psize, Packet *packet);
void HandleCantEnterOutpost(Connection *conn, size_t psize, Packet *packet);
void HandleInstanceTravelTimer(Connection *conn, size_t psize, Packet *packet);
void HandleInstanceLoaded(Connection *conn, size_t psize, Packet *packet);
void HandleInstanceLoadFinish(Connection *conn, size_t psize, Packet *packet);
void HandleInventoryItemQuantity(Connection *conn, size_t psize, Packet *packet);
void HandleInventoryItemLocation(Connection *conn, size_t psize, Packet *packet);
void HandleInventoryCreateBag(Connection *conn, size_t psize, Packet *packet);
void HandleItemWeaponSet(Connection *conn, size_t psize, Packet *packet);
void HandleItemGeneralInfo(Connection *conn, size_t psize, Packet *packet);
void HandleItemChangeLocation(Connection *conn, size_t psize, Packet *packet);
void HandleItemRemove(Connection *conn, size_t psize, Packet *packet);
void HandleItemStreamCreate(Connection *conn, size_t psize, Packet *packet);
void HandleItemStreamDestroy(Connection *conn, size_t psize, Packet *packet);
void HandleItemPriceQuote(Connection *conn, size_t psize, Packet *packet);
void HandleMerchantReady();
void HandleWindowOwner(Connection* conn, size_t psize, Packet* packet);
void HandleWindowMerchant(Connection* conn, size_t psize, Packet* packet);
void HandleWindowItemStreamEnd(Connection *conn, size_t psize, Packet *packet);
void HandleWindowAddItems(Connection *conn, size_t psize, Packet *packet);
void HandleWindowAddPrices(Connection* conn, size_t psize, Packet* packet);
void HandleSalvageSessionStart(Connection *conn, size_t psize, Packet *packet);
void HandleSalvageSessionCancel(Connection *conn, size_t psize, Packet *packet);
void HandleSalvageSessionDone(Connection *conn, size_t psize, Packet *packet);
void HandleSalvageSessionItemKept(Connection *conn, size_t psize, Packet *packet);
void HandleAgentMovementTick(Connection *conn, size_t psize, Packet *packet);
void HandleAgentInstanceTimer(Connection *conn, size_t psize, Packet *packet);
void HandleAgentSpawned(Connection *conn, size_t psize, Packet *packet);
void HandleAgentDespawned(Connection *conn, size_t psize, Packet *packet);
void HandleAgentSetPlayer(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdateDirection(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdateSpeedBase(Connection *conn, size_t psize, Packet *packet);
void HandleAgentStopMoving(Connection *conn, size_t psize, Packet *packet);
void HandleAgentMoveToPoint(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdateSpeedModifier(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdatePosition(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdateRotation(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdateDestination(Connection *conn, size_t psize, Packet *packet);
void HandleAgentDestroyPlayer(Connection* conn, size_t psize, Packet* packet);
void HandleAgentCreatePlayer(Connection *conn, size_t psize, Packet *packet);
void HandleAgentCreateNPC(Connection *conn, size_t psize, Packet *packet);
void HandleAgentPingeg(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdateEffects(Connection *conn, size_t psize, Packet *packet);
void HandleNPCUpdateProperties(Connection *conn, size_t psize, Packet *packet);
void HandleNPCUpdateModel(Connection *conn, size_t psize, Packet *packet);
void HandlePlayerUnlockedProfession(Connection *conn, size_t psize, Packet *packet);
void HandlePlayerUpdateProfession(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdateNpcName(Connection *conn, size_t psize, Packet *packet);
void HandleAgentAttrUpdateInt(Connection *conn, size_t psize, Packet *packet);
void HandleAgentAttrUpdateIntTarget(Connection *conn, size_t psize, Packet *packet);
void HandleAgentAttrUpdateFloat(Connection *conn, size_t psize, Packet *packet);
void HandleAgentAttrUpdateFloatTarget(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdateProfession(Connection *conn, size_t psize, Packet *packet);
void HandleCinematicStart(Connection *conn, size_t psize, Packet *packet);
void HandleCinematicEnd(Connection *conn, size_t psize, Packet *packet);
void HandleCinematicData(Connection *conn, size_t psize, Packet *packet);
void HandleCinematicDataEnd(Connection *conn, size_t psize, Packet *packet);
void HandleTradeRequest(Connection *conn, size_t psize, Packet *packet);
void HandleTradeTerminate(Connection *conn, size_t psize, Packet *packet);
void HandleTradeOfferedCount(Connection *conn, size_t psize, Packet *packet);
void HandleTradeReceiveOffer(Connection *conn, size_t psize, Packet *packet);
void HandleTradeAddItem(Connection *conn, size_t psize, Packet *packet);
void HandleTradeAcknowledge(Connection *conn, size_t psize, Packet *packet);
void HandleTradeAccept(Connection *conn, size_t psize, Packet *packet);
void HandleTradeChangeOffer(Connection *conn, size_t psize, Packet *packet);
void HandleChatMessageCore(Connection *conn, size_t psize, Packet *packet);
void HandleChatMessageLocal(Connection *conn, size_t psize, Packet *packet);
void HandleChatMessageGlobal(Connection *conn, size_t psize, Packet *packet);
void HandleChatMessageServer(Connection *conn, size_t psize, Packet *packet);
void HandlePlayerAttrSet(Connection *conn, size_t psize, Packet *packet);
void HandlePlayerAttrMaxKurzick(Connection *conn, size_t psize, Packet *packet);
void HandlePlayerAttrMaxLuxon(Connection *conn, size_t psize, Packet *packet);
void HandlePlayerAttrMaxBalthazar(Connection *conn, size_t psize, Packet *packet);
void HandlePlayerAttrMaxImperial(Connection *conn, size_t psize, Packet *packet);
void HandlePlayerAttrUpdate(Connection *conn, size_t psize, Packet *packet);
void HandleSkillbarUpdateSkill(Connection *conn, size_t psize, Packet *packet);
void HandleSkillbarUpdate(Connection *conn, size_t psize, Packet *packet);
void HandleSkillAddToWindowsData(Connection *conn, size_t psize, Packet *packet);
void HandleSkillAddToWindowsEnd(Connection *conn, size_t psize, Packet *packet);
void HandleSkillActivated(Connection *conn, size_t psize, Packet *packet);
void HandleSkillActivate(Connection *conn, size_t psize, Packet *packet);
void HandleSkillRecharge(Connection *conn, size_t psize, Packet *packet);
void HandleSkillRecharged(Connection *conn, size_t psize, Packet *packet);
void HandleHeroBehavior(Connection *conn, size_t psize, Packet *packet);
void HandleHeroSkillStatus(Connection *conn, size_t psize, Packet *packet);
void HandleHeroSkillStatusBitmap(Connection *conn, size_t psize, Packet *packet);
void HandleAgentUpdateAttribute(Connection *conn, size_t psize, Packet *packet);
void HandleQuestDescription(Connection *conn, size_t psize, Packet *packet);
void HandleQuestAdd(Connection *conn, size_t psize, Packet *packet);
void HandleQuestUpdateMarker(Connection *conn, size_t psize, Packet *packet);
void HandleQuestRemove(Connection *conn, size_t psize, Packet *packet);
void HandleTitleRankData(Connection *conn, size_t psize, Packet *packet);
void HandleTitleRankDisplay(Connection *conn, size_t psize, Packet *packet);
void HandleTitleTrackInfo(Connection *conn, size_t psize, Packet *packet);
void HandleMissionAddGoal(Connection *conn, size_t psize, Packet *packet);
void HandleMissionAddObjective(Connection *conn, size_t psize, Packet *packet);
void HandleDialogButton(Connection *conn, size_t psize, Packet *packet);
void HandleDialogBody(Connection *conn, size_t psize, Packet *packet);
void HandleDialogSender(Connection *conn, size_t psize, Packet *packet);
void HandleGuildPlayerRole(Connection *conn, size_t psize, Packet *packet);
void HandleGuildGeneralInfo(Connection *conn, size_t psize, Packet *packet);
void HandleGuildChangeFaction(Connection *conn, size_t psize, Packet *packet);
void HandleGuildInviteReceived(Connection *conn, size_t psize, Packet *packet);
void HandleEffectUpkeepAdded(Connection *conn, size_t psize, Packet *packet);
void HandleEffectUpkeepRemoved(Connection *conn, size_t psize, Packet *packet);
void HandleEffectUpkeepApplied(Connection *conn, size_t psize, Packet *packet);
void HandleEffectApplied(Connection *conn, size_t psize, Packet *packet);
void HandleEffectRenewed(Connection *conn, size_t psize, Packet *packet);
void HandleEffectRemoved(Connection *conn, size_t psize, Packet *packet);
void HandlePartySetDifficulty(Connection *conn, size_t psize, Packet *packet);
void HandlePartyHeroAdd(Connection *conn, size_t psize, Packet *packet);
void HandlePartyHeroRemove(Connection *conn, size_t psize, Packet *packet);
void HandlePartyInviteAdd(Connection *conn, size_t psize, Packet *packet);
void HandlePartyJoinRequest(Connection *conn, size_t psize, Packet *packet);
void HandlePartyInviteCancel(Connection *conn, size_t psize, Packet *packet);
void HandlePartyRequestCancel(Connection *conn, size_t psize, Packet *packet);
void HandlePartyYouAreLeader(Connection *conn, size_t psize, Packet *packet);
void HandlePartyPlayerAdd(Connection *conn, size_t psize, Packet *packet);
void HandlePartyPlayerRemove(Connection *conn, size_t psize, Packet *packet);
void HandlePartyPlayerReady(Connection *conn, size_t psize, Packet *packet);
void HandlePartyCreate(Connection *conn, size_t psize, Packet *packet);
void HandlePartyMemberStreamEnd(Connection *conn, size_t psize, Packet *packet);
void HandlePartyDefeated(Connection *conn, size_t psize, Packet *packet);
void HandlePartySearchRequestJoin(Connection *conn, size_t psize, Packet *packet);
void HandlePartySearchRequestDone(Connection *conn, size_t psize, Packet *packet);
void HandlePartySearchAdvertisement(Connection *conn, size_t psize, Packet *packet);
void HandlePartySearchSeek(Connection *conn, size_t psize, Packet *packet);
void HandlePartySearchRemove(Connection *conn, size_t psize, Packet *packet);
void HandlePartySearchSize(Connection *conn, size_t psize, Packet *packet);
void HandlePartySearchType(Connection *conn, size_t psize, Packet *packet);
