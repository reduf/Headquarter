#ifndef COMMON_RBTREE_H
#define COMMON_RBTREE_H

typedef uint64_t rbtree_key_t;

#define rbtree_set_red(n)   ((n)->color = 1)
#define rbtree_set_black(n) ((n)->color = 0)
#define rbtree_is_red(n)    ((n)->color == 1)
#define rbtree_is_black(n)  ((n)->color == 0)

struct rbnode {
    rbtree_key_t    key;

    struct rbnode  *left;
    struct rbnode  *right;
    struct rbnode  *parent;

    uint8_t         color;
};

struct rbtree {
    struct rbnode *root;
    struct rbnode *sentinel;
};

void rbtree_init(struct rbtree *tree, struct rbnode *sentinel);
struct rbnode * rbtree_min(struct rbtree *tree, struct rbnode *node);
struct rbnode * rbtree_next(struct rbtree *tree, struct rbnode *node);
void rbtree_insert(struct rbtree *tree, struct rbnode *node);
void rbtree_delete(struct rbtree *tree, struct rbnode *node);

#endif // COMMON_RBTREE_H
