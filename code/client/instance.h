#ifdef CORE_INSTANCES_H
#error "instance.h included more than once"
#endif
#define CORE_INSTANCES_H

void begin_travel(GwClient *client);
void start_loading_new_zone(GwClient *client, struct sockaddr *host,
    uint32_t map_id, uint32_t world_hash, uint32_t player_hash, bool record_packets);

void extract_district(struct World *world,
    District district, DistrictRegion *region, DistrictLanguage *language);
