#include "../common.h"
#include "llist.h"
#include "../debug/exception.h"
#include "../mem/manage.h"


/*
 * local function declarations
 */

static struct llist_inst_t *inst_cast(struct llist_node_t *node);


/**
 * Initialize the the root.
 *   @root; The root.
 */

_export
void llist_root_init(struct llist_root_t *root)
{
	root->head = NULL;
	root->tail = NULL;
}

/**
 * Clear the list using a callback.
 *   @root: The root.
 *   @clear: The clear function.
 *   @arg: The argument.
 */

_export
void llist_root_clear(struct llist_root_t *root, llist_clear_f clear, void *arg)
{
	struct llist_node_t *cur, *next;

	for(cur = root->head; cur != NULL; cur = next) {
		next = cur->next;
		clear(cur, arg);
	}

	root->head = NULL;
	root->tail = NULL;
}


/**
 * Prepend a node to the root.
 *   @root: The root.
 *   @node: The node.
 */

_export
void llist_root_prepend(struct llist_root_t *root, struct llist_node_t *node)
{
	node->next = root->head;
	node->prev = NULL;

	if(root->head != NULL)
		root->head->prev = node;
	else
		root->tail = node;

	root->head = node;
}

/**
 * Append a node to the root.
 *   @root: The root.
 *   @node: The node.
 */

_export
void llist_root_append(struct llist_root_t *root, struct llist_node_t *node)
{
	node->prev = root->tail;
	node->next = NULL;

	if(root->tail != NULL)
		root->tail->next = node;
	else
		root->head = node;

	root->tail = node;
}


/**
 * Insert a node before the cursor.
 *   @root: The root.
 *   @cur: The cursor node.
 *   @node: The node to insert.
 */

_export
void llist_root_insert_before(struct llist_root_t *root, struct llist_node_t *cur, struct llist_node_t *node)
{
	if(cur->prev != NULL)
		cur->prev->next = node;
	else
		root->head = node;

	node->next = cur;
	node->prev = cur->prev;
	cur->prev = node;
}

/**
 * Insert a node after the cursor.
 *   @root: The root.
 *   @cur: The cursor node.
 *   @node: The node to insert.
 */

_export
void llist_root_insert_after(struct llist_root_t *root, struct llist_node_t *cur, struct llist_node_t *node)
{
	if(cur->next != NULL)
		cur->next->prev = node;
	else
		root->tail = node;

	node->prev = cur;
	node->next = cur->next;
	cur->next = node;
}


/**
 * Remove a node from the root.
 *   @root: The root.
 *   @node: The node.
 */

_export
void llist_root_remove(struct llist_root_t *root, struct llist_node_t *node)
{
	if(node->next == NULL)
		root->tail = node->prev;
	else
		node->next->prev = node->prev;

	if(node->prev == NULL)
		root->head = node->next;
	else
		node->prev->next = node->next;
}


/**
 * Initialize the linked list.
 *   @list: The list.
 *   @delete: The deletion function.
 */

_export
void llist_init(struct llist_t *list, delete_f delete)
{
	list->len = 0;
	list->delete = delete;
	llist_root_init(&list->root);
}

/**
 * Create a ne wlinked list.
 *   @delete: The deletion function.
 *   &returns: The list.
 */

_export
struct llist_t *llist_new(delete_f delete)
{
	struct llist_t *list;

	list = mem_alloc(sizeof(struct llist_t));
	llist_init(list, delete);

	return list;
}

/**
 * Destroy the linked list.
 *   @list: The list.
 */

_export
void llist_destroy(struct llist_t *list)
{
	struct llist_node_t *cur, *next;

	for(cur = list->root.head; cur != NULL; cur = next) {
		struct llist_inst_t *inst = inst_cast(cur);

		if(list->delete != NULL)
			list->delete(inst->ref);

		next = cur->next;
		mem_free(inst);
	}
}

/**
 * Delete the linked list.
 *   @list: The list.
 */

_export
void llist_delete(struct llist_t *list)
{
	llist_destroy(list);
	mem_free(list);
}


/**
 * Prepend a reference onto the linked list.
 *   @list: The list.
 *   @ref: The reference.
 *   &returns: The instance.
 */

_export
struct llist_inst_t *llist_prepend(struct llist_t *list, void *ref)
{
	struct llist_inst_t *inst;

	inst = mem_alloc(sizeof(struct llist_inst_t));
	inst->ref = ref;

	list->len++;
	llist_root_prepend(&list->root, &inst->node);

	return inst;
}

/**
 * Append a reference onto the linked list.
 *   @list: The list.
 *   @ref: The reference.
 *   &returns: The instance.
 */

_export
struct llist_inst_t *llist_append(struct llist_t *list, void *ref)
{
	struct llist_inst_t *inst;

	inst = mem_alloc(sizeof(struct llist_inst_t));
	inst->ref = ref;

	list->len++;
	llist_root_append(&list->root, &inst->node);

	return inst;
}


/**
 * Insert a reference before the cursor.
 *   @list: The list.
 *   @cur: The cursor instance.
 *   @Ref: The reference.
 *   &returns: The instance.
 */

_export
struct llist_inst_t *llist_insert_before(struct llist_t *list, struct llist_inst_t *cur, void *ref)
{
	struct llist_inst_t *inst;

	inst = mem_alloc(sizeof(struct llist_inst_t));
	inst->ref = ref;

	list->len++;
	llist_root_insert_before(&list->root, &cur->node, &inst->node);

	return inst;
}

/**
 * Insert a reference after the cursor.
 *   @list: The list.
 *   @cur: The cursor instance.
 *   @Ref: The reference.
 *   &returns: The instance.
 */

_export
struct llist_inst_t *llist_insert_after(struct llist_t *list, struct llist_inst_t *cur, void *ref)
{
	struct llist_inst_t *inst;

	inst = mem_alloc(sizeof(struct llist_inst_t));
	inst->ref = ref;

	list->len++;
	llist_root_insert_after(&list->root, &cur->node, &inst->node);

	return inst;
}


/**
 * Cast to instance.
 *   @node: The node.
 *   &returns: The instance.
 */

static struct llist_inst_t *inst_cast(struct llist_node_t *node)
{
	return (void *)node - offsetof(struct llist_inst_t, node);
}
