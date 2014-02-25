#ifndef IO_READER_H
#define IO_READER_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct io_input_t;
struct io_reader_t;


/**
 * Load callback function.
 *   @reader: The reader.
 *   &returns: The reference.
 */

typedef void *(*io_load_f)(struct io_reader_t *reader, void *arg);


/*
 * reader function declarations
 */

struct io_reader_t *io_reader_new(struct io_input_t input);
void io_reader_delete(struct io_reader_t *reader);

struct io_input_t io_reader_input(struct io_reader_t *reader);

void *io_reader_load(struct io_reader_t *reader, io_load_f load, void *arg);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
