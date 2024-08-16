#ifdef CORE_QUEST_H
#error "quest.h included more than once"
#endif
#define CORE_QUEST_H

typedef struct Quest {
    size_t      id; // This id is the index position.

    int         quest_id;
    int         map_id;
    QuestStatus status;
    Vec2f       marker_coord;
    uint16_t    objective[128];
} Quest;
typedef array(Quest) ArrayQuest;

void api_make_quest(ApiQuest *dest, Quest *src)
{
    dest->quest_id = src->quest_id;
    dest->status   = src->status;
}

Quest *find_quest_by_id(ArrayQuest *quests, int quest_id);
