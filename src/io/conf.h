#ifndef IO_CONF_H
#define IO_CONF_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct io_conf_t;
struct io_conf_pair_t;
struct io_input_t;


/**
 * Configure pair iterator.
 *   @iter: The internal iterator.
 */

struct io_conf_iter_t {
	struct avltree_iter_t iter;
};

/**
 * Sub-configuration pair iterator.
 *   @iter: The internal iterator.
 */

struct io_conf_subiter_t {
	struct avltree_iter_t iter;
};

/**
 * Configuration pair.
 *   @key, value: The key and value strings.
 */

struct io_conf_pair_t {
	char *key, *value;
};


/*
 * configuration function declarations
 */

struct io_conf_t *io_conf_new();
void io_conf_delete(struct io_conf_t *conf);

struct io_conf_t *io_conf_read(struct io_input_t input);
struct io_conf_t *io_conf_load(const char *path);

const char *io_conf_value_lookup(struct io_conf_t *conf, const char *key);
void io_conf_value_add(struct io_conf_t *conf, const char *key, const char *value);
void io_conf_value_remove(struct io_conf_t *conf, const char *key);

struct io_conf_iter_t io_conf_iter_begin(struct io_conf_t *conf);
const struct io_conf_pair_t *io_conf_iter_next(struct io_conf_iter_t *iter);

struct io_conf_subiter_t io_conf_subiter_begin(struct io_conf_t *conf);
struct io_conf_t *io_conf_subiter_next(struct io_conf_subiter_t *iter);

/*
 * configuration pair function declarations
 */

void io_conf_pair_delete(struct io_conf_pair_t *pair);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
