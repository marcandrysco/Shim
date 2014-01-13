#ifndef TYPES_STRBUF_H
#define TYPES_STRBUF_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct io_output_t;


/**
 * String buffer storage.
 *   @store: The internal buffer storage.
 *   @i, n: The current index and buffer length.
 */

struct strbuf_t {
	char *store;
	size_t i, n;
};


/*
 * string buffer function declarations
 */

void strbuf_init(struct strbuf_t *buf, size_t initlen);
struct strbuf_t *strbuf_new(size_t initlen);
void strbuf_destroy(struct strbuf_t *buf);
void strbuf_delete(struct strbuf_t *buf);

void strbuf_reset(struct strbuf_t *buf);
void strbuf_store(struct strbuf_t *buf, char ch);
void strbuf_append(struct strbuf_t *buf, const char *restrict str);
void strbuf_write(struct strbuf_t *buf, const void *restrict data, size_t nbytes);

char *strbuf_done(struct strbuf_t *buf);
char *strbuf_detach(struct strbuf_t *buf);
const char *strbuf_finish(struct strbuf_t *buf);

struct io_output_t strbuf_output(struct strbuf_t *buf);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
