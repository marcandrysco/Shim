#ifndef AVLTREE_H
#define AVLTREE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Comparison callback between a node and a key.
 *   @key: The input key.
 *   @node: The compared-to node.
 *   @arg: The callback argument.
 *   &returns: An integer representing their order.
 */

typedef int (*avltree_compare_nodekey_f)(const void *key, const struct avltree_node_t *node, void *arg);

/**
 * Comparison callback between two nodes.
 *   @n1: The first node.
 *   @n2: The seconde node.
 *   @arg: The callback argument.
 *   &returns: An integer representing their order.
 */

typedef int (*avltree_compare_nodenode_f)(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg);

/**
 * Deletion callback for a node.
 *   @node: The current node.
 *   @arg: The callback argument.
 */

typedef void (*avltree_delete_node_f)(struct avltree_node_t *node, void *arg);


/**
 * Iteration callback function on references.
 *   @ref: The reference.
 *   @arg: A user-specified argument.
 *   &returns: Non-zero to halt iteration, zero to continue.
 */

typedef short (*avltree_iterate_f)(void *ref, void *arg);

/**
 * Iteration callbcak function on keys.
 *   @ref: The reference.
 *   @arg: A user-specified argument.
 *   &returns: Non-zero to halt iteration, zero to continue.
 */

typedef short (*avltree_iterate_key_f)(const void *key, void *arg);

/**
 * Iteration callbcak function on reference structures.
 *   @ref: The reference.
 *   @arg: A user-specified argument.
 *   &returns: Non-zero to halt iteration, zero to continue.
 */

typedef short (*avltree_iterate_ref_f)(struct avltree_inst_t *ref, void *arg);


/*
 * avl tree node function declarations
 */

struct avltree_root_t avltree_root_empty();

struct avltree_node_t *avltree_root_first(struct avltree_root_t *root);
struct avltree_node_t *avltree_root_last(struct avltree_root_t *root);

struct avltree_node_t *avltree_node_first(struct avltree_node_t *node);
struct avltree_node_t *avltree_node_last(struct avltree_node_t *node);
struct avltree_node_t *avltree_node_prev(struct avltree_node_t *node);
struct avltree_node_t *avltree_node_next(struct avltree_node_t *node);

struct avltree_node_t *avltree_node_lookup(struct avltree_node_t *root, const void *key, avltree_compare_nodekey_f compare, void *arg);
struct avltree_node_t *avltree_node_nearby(struct avltree_node_t *root, const void *key, avltree_compare_nodekey_f compare, void *arg);
struct avltree_node_t *avltree_node_atleast(struct avltree_node_t *root, const void *key, avltree_compare_nodekey_f compare, void *arg);
struct avltree_node_t *avltree_node_atmost(struct avltree_node_t *root, const void *key, avltree_compare_nodekey_f compare, void *arg);

void avltree_node_insert(struct avltree_node_t **root, struct avltree_node_t *node, avltree_compare_nodenode_f compare, void *arg);
struct avltree_node_t *avltree_node_remove(struct avltree_node_t **root, const void *key, avltree_compare_nodekey_f compare, void *arg);
void avltree_node_clear(struct avltree_node_t *root, avltree_delete_node_f delete, void *arg);

struct avltree_iter_t avltree_node_iter_blank();
struct avltree_iter_t avltree_node_iter_begin(struct avltree_node_t *root);
struct avltree_node_t *avltree_node_iter_prev(struct avltree_iter_t *iter);
struct avltree_node_t *avltree_node_iter_next(struct avltree_iter_t *iter);
struct avltree_node_t *avltree_node_iter_next_depth(struct avltree_iter_t *iter);

/*
 * avl tree function declarations
 */

void avltree_init(struct avltree_t *tree, compare_f compare, delete_f delete);
struct avltree_t avltree_empty(compare_f compare, delete_f delete);
struct avltree_t *avltree_new(compare_f compare, delete_f delete);
void avltree_destroy(struct avltree_t *tree);
void avltree_delete(struct avltree_t *tree);

void *avltree_first(struct avltree_t *tree);
void *avltree_last(struct avltree_t *tree);

void *avltree_lookup(const struct avltree_t *tree, const void *key);
void *avltree_nearby(const struct avltree_t *tree, const void *key);
void *avltree_atleast(const struct avltree_t *tree, const void *key);
void *avltree_atmost(const struct avltree_t *tree, const void *key);

void avltree_insert(struct avltree_t *tree, const void *key, void *ref);
void avltree_insert_ref(struct avltree_t *tree, void *ref);
void *avltree_remove(struct avltree_t *tree, const void *key);
void avltree_purge(struct avltree_t *tree, const void *key);

void avltree_merge(struct avltree_t *dest, struct avltree_t *src);
void avltree_clear(struct avltree_t *tree);

struct avltree_iter_t avltree_iter(const struct avltree_t *tree);
struct avltree_iter_t avltree_iter_blank();
struct avltree_iter_t avltree_iter_begin(const struct avltree_t *tree);
void avltree_iter_init(struct avltree_iter_t *iter, const struct avltree_t *tree);
void *avltree_iter_prev(struct avltree_iter_t *iter);
void *avltree_iter_next(struct avltree_iter_t *iter);
void *avltree_iter_next_key(struct avltree_iter_t *iter);
struct avltree_inst_t *avltree_iter_next_ref(struct avltree_iter_t *iter);

struct iter_t avltree_iter_new(const struct avltree_t *tree);
struct iter_t avltree_iter_keys_new(const struct avltree_t *tree);
struct iter_t avltree_iter_refs_new(const struct avltree_t *tree);

struct iter_t avltree_asiter(struct avltree_t *tree);

void avltree_iterate(const struct avltree_t *tree, avltree_iterate_f func, void *arg);
void avltree_iterate_keys(const struct avltree_t *tree, avltree_iterate_key_f func, void *arg);
void avltree_iterate_refs(const struct avltree_t *tree, avltree_iterate_ref_f func, void *arg);

/*
 * avl tree instance function declarations
 */

struct avltree_inst_t *avltree_inst_first(struct avltree_t *tree);
struct avltree_inst_t *avltree_inst_last(struct avltree_t *tree);
struct avltree_inst_t *avltree_inst_prev(struct avltree_inst_t *inst);
struct avltree_inst_t *avltree_inst_next(struct avltree_inst_t *inst);

struct avltree_inst_t *avltree_inst_atleast(const struct avltree_t *tree, const void *key);
struct avltree_inst_t *avltree_inst_atmost(const struct avltree_t *tree, const void *key);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
