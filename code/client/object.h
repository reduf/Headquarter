#ifdef CORE_OBJECT_H
#error "object.h included more than once"
#endif
#define CORE_OBJECT_H

typedef enum ObjectType {
    ObjectType_Bag,
    ObjectType_Item,
    ObjectType_Agent,
    ObjectType_Player,

    ObjectType_Count
} ObjectType;

typedef struct GameObject {
    int type;
    int ref_count;
    int need_free;

    struct GameObject *next;
    struct GameObject *prev;
} GameObject;

typedef struct ObjectManager {
    GameObject   pool[ObjectType_Count];
    unsigned int allocation_count[ObjectType_Count];
} ObjectManager;

void init_object_manager(ObjectManager *mgr);
void *game_object_alloc(ObjectManager *mgr, int type);
void game_object_free(ObjectManager *mgr, GameObject *object);
