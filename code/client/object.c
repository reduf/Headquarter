#ifdef OBJECT_MANAGER_C_INC
#error "object_manager.c included more than once"
#endif
#define OBJECT_MANAGER_C_INC

static size_t game_object_size[ObjectType_Count] = {
    sizeof(Bag),
    sizeof(Item),
    sizeof(Agent),
    sizeof(Player)
};

static int object_list_empty(GameObject *obj);

void init_object_manager(ObjectManager *mgr)
{
    for (int i = 0; i < ObjectType_Count; i++) {
        GameObject *current = &mgr->pool[i];
        current->next = current;
        current->prev = current;
    }
}

void *game_object_alloc(ObjectManager *mgr, int type)
{
    assert(type < ObjectType_Count);
    GameObject *head = &mgr->pool[type];
    GameObject *obj = NULL;

    if (!object_list_empty(head)) {
        obj = head->next;
        obj->prev->next = obj->next;
        obj->next->prev = obj->prev;

        assert(obj->type == type);
        obj->ref_count = 0;
        obj->need_free = 0;

        size_t size = game_object_size[type];
        memzero(obj, size);
        return obj;
    }

    mgr->allocation_count[type]++;

    size_t size = game_object_size[type];
    obj = cast(GameObject *)calloc(1, size);
    obj->type = type;
    return obj;
}

void game_object_free(ObjectManager *mgr, GameObject *object)
{
    if (!object) return;

    assert(object->type < ObjectType_Count);
    object->need_free = true;
    if (object->ref_count != 0) {
        LogWarn("Error memory leak of %p because we tried to free when to object was acquired by the api", object);
        return;
    }

    GameObject *head = &mgr->pool[object->type];
    object->next = head;
    object->prev = head->prev;

    head->prev->next = object;
    head->prev = object;
}

static int object_list_empty(GameObject *obj)
{
    return obj == obj->next;
}

