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


/**
 * Configuration pair structure.
 *   @id: The identifier.
 *   @nvalues: The number of values.
 *   @value: The value array.
 */

struct io_conf_pair_t {
	char *id;

	unsigned int nvalues;
	char **value;
};


/**
 * Configure pair iterator.
 *   @inner: The internal iterator.
 */

struct io_conf_iter_t {
	struct avlitree_iter_t inner;
};

/**
 * Sub-configuration pair iterator.
 *   @inner: The internal iterator.
 */

struct io_conf_subiter_t {
	struct avlitree_iter_t inner;
};


/*
 * configuration function declarations
 */

struct io_conf_t *io_conf_new();
void io_conf_delete(struct io_conf_t *conf);

struct io_conf_t *io_conf_read(struct io_input_t input);
struct io_conf_t *io_conf_load(const char *path);

const char *io_conf_id(struct io_conf_t *conf);
unsigned int io_conf_accum(struct io_conf_t *conf);

const char *const *io_conf_lookup_values(struct io_conf_t *conf, const char *id);
struct io_conf_pair_t *io_conf_lookup(struct io_conf_t *conf, const char *id);
const char *io_conf_lookup_single(struct io_conf_t *conf, const char *id);

struct io_conf_pair_t *io_conf_remove(struct io_conf_t *conf, const char *id);
char *io_conf_remove_single(struct io_conf_t *conf, const char *id);

struct io_conf_iter_t io_conf_iter_begin(struct io_conf_t *conf);
const struct io_conf_pair_t *io_conf_iter_next(struct io_conf_iter_t *iter);

struct io_conf_subiter_t io_conf_subiter_begin(struct io_conf_t *conf);
struct io_conf_t *io_conf_subiter_next(struct io_conf_subiter_t *iter);

/*
 * configuration pair function declarations
 */

struct io_conf_pair_t *io_conf_pair_new(const char *id);
void io_conf_pair_delete(struct io_conf_pair_t *pair);

void io_conf_pair_add(struct io_conf_pair_t *pair, const char *value);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
