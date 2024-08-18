#ifdef CORE_DIALOG_H
#error "core.h included more than once"
#endif
#define CORE_DIALOG_H

typedef struct DialogButton {
    int dialog_id;
    int icon_id;
} DialogButton;
typedef array(DialogButton) ArrayDialogButton;

typedef struct DialogInfo {
    bool     opened;
    AgentId  interact_with;
    uint16_t body[122];
    ArrayDialogButton buttons;
} DialogInfo;

void free_dialog_info(DialogInfo *info)
{
    info->opened = false;
    info->interact_with = 0;
    array_clear(&info->buttons);
}
