#ifndef IO_WRITER_H
#define IO_WRITER_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct io_output_t;
struct io_writer_t;


/**
 * Save callback function.
 *   @ref: The reference.
 *   @writer: The writer.
 */

typedef void (*io_save_f)(void *ref, struct io_writer_t *writer, void *arg);


/*
 * writer function declarations
 */

struct io_writer_t *io_writer_new(struct io_output_t output);
void io_writer_delete(struct io_writer_t *writer);

struct io_output_t io_write_output(struct io_writer_t *writer);

void io_writer_save(struct io_writer_t *writer, void *ref, io_save_f save, void *arg);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
