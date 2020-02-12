#ifdef CORE_TITLE_H
#error "title.h included more than once"
#endif
#define CORE_TITLE_H

typedef struct Title {
    int title_id;

    int rank_max;
    int rank_current;
    int points_in_cur_rank;
    int points_for_rank_up;
} Title;
typedef array(Title) ArrayTitle;