#ifdef RBTREE_C
#error "rbtree.c included more than once"
#endif
#define RBTREE_C

#include "rbtree.h"

static void rbtree_insert_core(struct rbnode *root,
    struct rbnode *sentinel, struct rbnode *node);
static void rbtree_rotate_right(struct rbnode **root,
    struct rbnode *sentinel, struct rbnode *node);
static void rbtree_rotate_left(struct rbnode **root,
    struct rbnode *sentinel, struct rbnode *node);

void rbtree_init(struct rbtree *tree, struct rbnode *sentinel)
{
    tree->root = sentinel;
    tree->sentinel = sentinel;
    rbtree_set_black(sentinel);
}

struct rbnode * rbtree_min(struct rbtree *tree, struct rbnode *node)
{
    struct rbnode *sentinel = tree->sentinel;

    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}

struct rbnode * rbtree_next(struct rbtree *tree, struct rbnode *node)
{
    struct rbnode *sentinel = tree->sentinel;

    if (node->right != sentinel) {
        return rbtree_min(tree, node->right);
    }

    struct rbnode *root = tree->root;
    struct rbnode *parent;

    for (;; ) {
        parent = node->parent;

        if (node == root) {
            return NULL;
        }

        if (node == parent->left) {
            return parent;
        }

        node = parent;
    }
}

void rbtree_insert(struct rbtree *tree, struct rbnode *node)
{
    assert(tree && node && (tree->root));

    struct rbnode *sentinel = tree->sentinel;
    struct rbnode **root = &tree->root;

    if (*root == sentinel) {
        node->left = sentinel;
        node->right = sentinel;
        node->parent = NULL;
        rbtree_set_black(node);
        tree->root = node;
        return;
    }

    rbtree_insert_core(*root, sentinel, node);

    struct rbnode *uncle;
    while ((node != *root) && rbtree_is_red(node->parent)) {
        if (node->parent == node->parent->parent->left) {
            uncle = node->parent->parent->right;
            if (rbtree_is_red(uncle)) {
                rbtree_set_black(node->parent);
                rbtree_set_black(uncle);
                rbtree_set_red(node->parent->parent);

                // We recursively re-apply the algo to the grandparent.
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rbtree_rotate_left(root, sentinel, node);
                }

                rbtree_set_black(node->parent);
                rbtree_set_red(node->parent->parent);
                rbtree_rotate_right(root, sentinel, node->parent->parent);
            }
        } else {
            uncle = node->parent->parent->left;
            if (rbtree_is_red(uncle)) {
                rbtree_set_black(node->parent);
                rbtree_set_black(uncle);
                rbtree_set_red(node->parent->parent);

                // We recursively re-apply the algo to the grandparent.
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rbtree_rotate_right(root, sentinel, node);
                }

                rbtree_set_black(node->parent);
                rbtree_set_red(node->parent->parent);
                rbtree_rotate_left(root, sentinel, node->parent->parent);
            }
        }
    }

    rbtree_set_black(*root);
}

static void rbtree_insert_core(struct rbnode *root,
    struct rbnode *sentinel, struct rbnode *node)
{
    struct rbnode **it;

    for (;;) {
        if (node->key < root->key)
            it = &root->left;
        else
            it = &root->right;

        if (*it == sentinel) {
            node->left = sentinel;
            node->right = sentinel;
            node->parent = root;
            rbtree_set_red(node);

            *it = node;
            return;
        }

        root = *it;
    }
}

static void rbtree_rotate_right(struct rbnode **root,
    struct rbnode *sentinel, struct rbnode *node)
{
    struct rbnode *temp = node->left;
    node->left = temp->right;
    if (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->right = node;
    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;
    } else if (node == node->parent->left) {
        node->parent->left = temp;
    } else {
        node->parent->right = temp;
    }

    node->parent = temp;
}

static void rbtree_rotate_left(struct rbnode **root,
    struct rbnode *sentinel, struct rbnode *node)
{
    struct rbnode *temp = node->right;
    node->right = temp->left;

    if (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->left = node;
    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;
    } else if (node == node->parent->left) {
        node->parent->left = temp;
    } else {
        node->parent->right = temp;
    }

    node->parent = temp;
}

void rbtree_delete(struct rbtree *tree, struct rbnode *node)
{
    struct rbnode **root = &tree->root;
    struct rbnode *sentinel = tree->sentinel;
    struct rbnode *subst, *temp, *sibling;

    if (node->left == sentinel) {
        temp = node->right;
        subst = node;
    } else if (node->right == sentinel) {
        temp = node->left;
        subst = node;
    } else {
        subst = rbtree_min(tree, node->right);

        if (subst->left != sentinel) {
            temp = subst->left;
        } else {
            temp = subst->right;
        }
    }

    if (subst == *root) {
        *root = temp;
        rbtree_set_black(temp);
    #if !defined(_NDEBUG)
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
    #endif

        return;
    }

    int subst_is_red = rbtree_is_red(subst);

    if (subst == subst->parent->left) {
        subst->parent->left = temp;
    } else {
        subst->parent->right = temp;
    }

    if (subst == node) {
        temp->parent = subst->parent;
    } else {
        if (subst->parent == node) {
            temp->parent = subst;
        } else {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        subst->color = node->color;

        if (node == *root) {
            *root = subst;
        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }

        if (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        if (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

#if !defined(_NDEBUG)
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
#endif

    if (subst_is_red) {
        return;
    }

    while (temp != *root && rbtree_is_black(temp)) {
        if (temp == temp->parent->left) {
            sibling = temp->parent->right;

            if (rbtree_is_red(sibling)) {
                rbtree_set_black(sibling);
                rbtree_set_red(temp->parent);
                rbtree_rotate_left(root, sentinel, temp->parent);
                sibling = temp->parent->right;
            }

            if (rbtree_is_black(sibling->left) && rbtree_is_black(sibling->right)) {
                rbtree_set_red(sibling);
                temp = temp->parent;
            } else {
                if (rbtree_is_black(sibling->right)) {
                    rbtree_set_black(sibling->left);
                    rbtree_set_red(sibling);
                    rbtree_rotate_right(root, sentinel, sibling);
                    sibling = temp->parent->right;
                }

                sibling->color = temp->parent->color;
                rbtree_set_black(temp->parent);
                rbtree_set_black(sibling->right);
                rbtree_rotate_left(root, sentinel, temp->parent);
                temp = *root;
            }

        } else {
            sibling = temp->parent->left;

            if (rbtree_is_red(sibling)) {
                rbtree_set_black(sibling);
                rbtree_set_red(temp->parent);
                rbtree_rotate_right(root, sentinel, temp->parent);
                sibling = temp->parent->left;
            }

            if (rbtree_is_black(sibling->left) && rbtree_is_black(sibling->right)) {
                rbtree_set_red(sibling);
                temp = temp->parent;
            } else {
                if (rbtree_is_black(sibling->left)) {
                    rbtree_set_black(sibling->right);
                    rbtree_set_red(sibling);
                    rbtree_rotate_left(root, sentinel, sibling);
                    sibling = temp->parent->left;
                }

                sibling->color = temp->parent->color;
                rbtree_set_black(temp->parent);
                rbtree_set_black(sibling->left);
                rbtree_rotate_right(root, sentinel, temp->parent);
                temp = *root;
            }
        }
    }

    rbtree_set_black(temp);
}
