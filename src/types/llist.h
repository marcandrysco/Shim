#ifndef TYPES_LLIST_H
#define TYPES_LLIST_H

/*
 * project headers
 */

#include "compare.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Linked list root structure.
 *   @head, tail: The head and tail nodes.
 */

struct llist_root_t {
	struct llist_node_t *head, *tail;
};

/**
 * Linked list node structure.
 *   @prev, next: The previous and next nodes.
 */

struct llist_node_t {
	struct llist_node_t *prev, *next;
};


/**
 * Clear callback.
 *   @node: The node.
 *   @arg: The argument.
 */

typedef void (*llist_clear_f)(struct llist_node_t *node, void *arg);


/**
 * Linked list structure.
 *   @root: The list root.
 *   @len: The length.
 *   @delete: The deletion callback.
 */

struct llist_t {
	struct llist_root_t root;
	unsigned int len;

	delete_f delete;
};

/**
 * Linked list instance.
 *   @ref: The reference.
 *   @node: The node.
 */

struct llist_inst_t {
	void *ref;

	struct llist_node_t node;
};


/**
 * Linked list iterator.
 *   @inst: The current instance.
 */

struct llist_iter_t {
	struct llist_inst_t *inst;
};


/*
 * linked list root function declarations
 */

void llist_root_init(struct llist_root_t *root);
struct llist_root_t llist_root_empty();
void llist_root_clear(struct llist_root_t *root, llist_clear_f clear, void *arg);

void llist_root_prepend(struct llist_root_t *root, struct llist_node_t *node);
void llist_root_append(struct llist_root_t *root, struct llist_node_t *node);

void llist_root_insert_before(struct llist_root_t *root, struct llist_node_t *cur, struct llist_node_t *node);
void llist_root_insert_after(struct llist_root_t *root, struct llist_node_t *cur, struct llist_node_t *node);

void llist_root_remove(struct llist_root_t *root, struct llist_node_t *node);

/*
 * linked list function declarations
 */

struct llist_t llist_empty(delete_f delete);
struct llist_t *llist_new(delete_f delete);
void llist_destroy(struct llist_t *list);
void llist_delete(struct llist_t *list);

void llist_clear(struct llist_t *list);

struct llist_inst_t *llist_scan(struct llist_t *list, void *ref);

struct llist_inst_t *llist_prepend(struct llist_t *list, void *ref);
struct llist_inst_t *llist_append(struct llist_t *list, void *ref);
void llist_pull(struct llist_t *list, struct iter_t iter);;

void *llist_front(struct llist_t *list);
void *llist_front_remove(struct llist_t *list);
void *llist_back(struct llist_t *list);
void *llist_back_remove(struct llist_t *list);

void llist_push(struct llist_t *list, void *ref);
void *llist_pop(struct llist_t *list);

struct llist_inst_t *llist_insert_before(struct llist_t *list, struct llist_inst_t *cur, void *ref);
struct llist_inst_t *llist_insert_after(struct llist_t *list, struct llist_inst_t *cur, void *ref);

struct llist_iter_t llist_iter_begin(struct llist_t *list);
void *llist_iter_next(struct llist_iter_t *iter);

struct iter_t llist_iter(struct llist_t *list);
struct iter_t llist_asiter(struct llist_t *list);

/*
 * linked list instance function declarations
 */

struct llist_inst_t *llist_inst_front(struct llist_t *list);
struct llist_inst_t *llist_inst_back(struct llist_t *list);

struct llist_inst_t *llist_inst_prev(struct llist_inst_t *inst);
struct llist_inst_t *llist_inst_next(struct llist_inst_t *inst);

void *llist_inst_remove(struct llist_t *list, struct llist_inst_t *inst);
void llist_inst_purge(struct llist_t *list, struct llist_inst_t *inst);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
