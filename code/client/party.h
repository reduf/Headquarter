#ifdef CORE_PARTY_H
#error "party.h included more than once"
#endif
#define CORE_PARTY_H

typedef struct PartyPlayer {
    AgentId  agent_id;
    uint32_t player_id;
    bool     ready;
    bool     connected;
} PartyPlayer;

typedef struct PartyHenchman {
    AgentId    agent_id;
    uint32_t   hench_id;
    uint32_t   level;
    Profession prof;
} PartyHenchman;

typedef struct PartyHero {
    AgentId  agent_id;
    uint32_t owner_id; // player id
    uint32_t hero_id;
    uint32_t level;
} PartyHero;

typedef array(PartyHero)     ArrayPartyHero;
typedef array(PartyPlayer)   ArrayPartyPlayer;
typedef array(PartyHenchman) ArrayPartyHenchman;

typedef struct Party {
    int16_t    party_id;
    size_t     player_count;
    size_t     player_max;
    bool       defeated;
    Difficulty difficulty;

    // @Enhancement: Add invite list

    ArrayPartyHero     heroes;
    ArrayPartyPlayer   players;
    ArrayPartyHenchman henchmans;
} Party;
typedef array(Party) ArrayParty;

typedef struct PartySearch {
    uint32_t ps_id;
    uint16_t district;
    uint8_t  party_size;
    string   player_name;
    string   message;
    char     message_buffer[64];
    char     player_buffer[40];
    PartySearchType type;
} PartySearch;
typedef array(PartySearch) ArrayPartySearch;

struct Player *get_player_safe(struct GwClient *client, int player_id);
Party *get_party_safe(struct GwClient *client, int party_id);
PartyPlayer *get_party_player(Party *party, int player_id);
PartyHero *get_party_hero_agent(Party *party, AgentId agent_id);
