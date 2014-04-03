#include "../common.h"
#include "strbuf.h"
#include "../io/output.h"
#include "../mem/base.h"
#include "../mem/manage.h"
#include "../string/base.h"


/*
 * local function declarations
 */

static size_t output_write(void *ref, const void *restrict buf, size_t nbytes);

/*
 * local variables
 */

static struct io_output_i output_iface = {
	{
		NULL,
		NULL
	},
	(io_write_f)output_write
};


/**
 * Initialize the string buffer.
 *   @buf: The memory where the string buffer is stored.
 *   @initlen: The initial buffer length.
 */

_export
void strbuf_init(struct strbuf_t *buf, size_t initlen)
{
	buf->i = 0;
	buf->n = initlen;
	buf->store = mem_alloc(initlen);
}

/**
 * Create an empty string buffer.
 *   @initlen: The initial buffer length.
 *   &returns: The string buffer.
 */

_export
struct strbuf_t strbuf_empty(size_t initlen)
{
	return (struct strbuf_t){ mem_alloc(initlen), 0, initlen };
}

/**
 * Create a string buffer.
 *   @initlen: The initial buffer length.
 *   &returns: The string buffer.
 */

_export
struct strbuf_t *strbuf_new(size_t initlen)
{
	struct strbuf_t *buf;

	buf = mem_alloc(sizeof(struct strbuf_t));
	strbuf_init(buf, initlen);

	return buf;
}

/**
 * Destroy a string buffer.
 *   @buf: The string buffer.
 */

_export
void strbuf_destroy(struct strbuf_t *buf)
{
	mem_free(buf->store);
}

/**
 * Delete a string buffer.
 *   @buf: The string buffer.
 */

_export
void strbuf_delete(struct strbuf_t *buf)
{
	strbuf_destroy(buf);
	mem_free(buf);
}


/**
 * Reset the internal index to the beginning.
 *   @buf: The string buffer.
 */

_export
void strbuf_reset(struct strbuf_t *buf)
{
	buf->i = 0;
}

/**
 * Store a character in the string buffer.
 *   @buf: The string buffer.
 *   @ch: The character.
 */

_export
void strbuf_store(struct strbuf_t *buf, char ch)
{
	if(buf->i >= buf->n) {
		buf->store = mem_realloc(buf->store, 2 * buf->n);
		buf->n *= 2;
	}

	buf->store[buf->i++] = ch;
}

/**
 * Append a string onto the string buffer.
 *   @buf: The string buffer.
 *   @str: The string.
 */

_export
void strbuf_append(struct strbuf_t *buf, const char *restrict str)
{
	strbuf_write(buf, str, str_len(str));
}

/**
 * Write data onto the string buffer.
 *   @buf: The string buffer.
 *   @data: The data.
 *   @nbytes: The number of bytes.
 */

_export
void strbuf_write(struct strbuf_t *buf, const void *restrict data, size_t nbytes)
{
	unsigned int i, n;

	i = buf->i + nbytes;
	if(i >= buf->n) {
		n = buf->n;

		do
			n *= 2;
		while(i >= n);

		buf->store = mem_realloc(buf->store, n);
		buf->n = n;
	}

	mem_copy(buf->store + buf->i, data, nbytes);
	buf->i = i;
}


/**
 * Write a null byte to the end of the store, returning a pointer to the
 * allocated string. This is only compatible with 'strbuf_init'.
 *   @buf: The string buffer.
 *   &returns: The buffered string.
 */

_export
char *strbuf_done(struct strbuf_t *buf)
{
	buf->store = mem_realloc(buf->store, buf->i + 1);
	buf->store[buf->i] = '\0';

	return buf->store;
}

/**
 * Write a null byte to the end of the store, returning a pointer to the
 * allocated string. This is only compatible with 'strbuf_new'.
 *   @buf: The string buffer.
 *   &returns: The buffered string.
 */

_export
char *strbuf_detach(struct strbuf_t *buf)
{
	char *str;

	str = strbuf_done(buf);
	mem_free(buf);

	return str;
}

/**
 * Write a null byte to the end of the store, return the buffered string, and
 * reset the internal index. The returned string will remain valid until the
 * next call to 'strbuf_store' or if the string buffer is destroyed.
 *   @buf: The string buffer.
 *   &returns: The buffered string.
 */

_export
const char *strbuf_finish(struct strbuf_t *buf)
{
	strbuf_store(buf, '\0');
	buf->i = 0;

	return buf->store;
}


/**
 * Create a pseudo output device for storing data in a string buffer.
 *   @buf: The string buffer.
 *   &returns: The output device.
 */

struct io_output_t strbuf_output(struct strbuf_t *buf)
{
	return (struct io_output_t){ buf, &output_iface };
}

/**
 * Write callback for the string buffer output device.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes to written.
 */

static size_t output_write(void *ref, const void *restrict buf, size_t nbytes)
{
	strbuf_write(ref, buf, nbytes);

	return nbytes;
}
