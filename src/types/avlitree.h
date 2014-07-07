#ifndef TYPES_AVLITREE_H
#define TYPES_AVLITREE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * avl index tree node function declarations
 */

struct avlitree_root_t avlitree_root_empty();

struct avlitree_node_t *avlitree_root_first(struct avlitree_root_t *root);
struct avlitree_node_t *avlitree_root_last(struct avlitree_root_t *root);
struct avlitree_node_t *avlitree_root_get(struct avlitree_root_t *root, unsigned int index);

void avlitree_root_insert(struct avlitree_root_t *root, unsigned int index, struct avlitree_node_t *node);
struct avlitree_node_t *avlitree_root_slice(struct avlitree_root_t *root, unsigned int index);
struct avlitree_node_t *avlitree_root_set(struct avlitree_root_t *root, unsigned int index, struct avlitree_node_t *node);

struct avlitree_node_t *avlitree_node_prev(struct avlitree_node_t *node);
struct avlitree_node_t *avlitree_node_next(struct avlitree_node_t *node);

struct avlitree_node_t *avlitree_node_first(struct avlitree_node_t *root);
struct avlitree_node_t *avlitree_node_last(struct avlitree_node_t *root);
struct avlitree_node_t *avlitree_node_get(struct avlitree_node_t *root, unsigned int index);
struct avlitree_node_t *avlitree_node_set(struct avlitree_node_t **root, unsigned int index, struct avlitree_node_t *node);
unsigned int avlitree_node_index(struct avlitree_node_t *node);

void avlitree_node_insert(struct avlitree_node_t **root, unsigned int index, struct avlitree_node_t *node);
struct avlitree_node_t *avlitree_node_slice(struct avlitree_node_t **root, unsigned int index);
void avlitree_node_clear(struct avlitree_node_t *root, avlitree_delete_node_f delete, void *arg);

struct avlitree_iter_t avlitree_node_iter_new(struct avlitree_node_t *root);
struct avlitree_node_t *avlitree_node_iter_next(struct avlitree_iter_t *iter);

/**
 * avl index tree function declarations
 */

void avlitree_init(struct avlitree_t *tree, delete_f delete);
struct avlitree_t avlitree_empty(delete_f delete);
struct avlitree_t *avlitree_new(delete_f delete);
void avlitree_destroy(struct avlitree_t *tree);
void avlitree_delete(struct avlitree_t *tree);

void *avlitree_get(const struct avlitree_t *tree, unsigned int index);
void *avlitree_set(struct avlitree_t *tree, unsigned int index, void *ref);
void *avlitree_set(struct avlitree_t *tree, unsigned int index, void *ref);

void avlitree_insert(struct avlitree_t *tree, unsigned int index, void *ref);
void *avlitree_slice(struct avlitree_t *tree, unsigned int index);
void avlitree_erase(struct avlitree_t *tree, unsigned int index);
void avlitree_clear(struct avlitree_t *tree);

void avlitree_append(struct avlitree_t *tree, void *ref);
void avlitree_prepend(struct avlitree_t *tree, void *ref);

struct avlitree_iter_t avlitree_iter_begin(const struct avlitree_t *tree);
void *avlitree_iter_next(struct avlitree_iter_t *iter);
struct avlitree_ref_t *avlitree_iter_next_ref(struct avlitree_iter_t *iter);

struct iter_t avlitree_valiter_new(const struct avlitree_t *tree);
struct iter_t avlitree_refiter_new(const struct avlitree_t *tree);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
