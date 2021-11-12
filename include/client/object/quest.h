typedef enum QuestStatus {
    QuestStatus_Completed = 3,
} QuestStatus;

typedef struct ApiQuest {
    uint32_t quest_id;
    QuestStatus status;
} ApiQuest;
typedef array(ApiQuest) ArrayApiQuest;
