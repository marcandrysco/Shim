#ifndef DEBUG_RES_H
#define DEBUG_RES_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct io_output_t;
struct io_chunk_t;

/*
 * resource debuggin function declarations
 */

void _dbg_res_alloc(void *res, const char *file, unsigned int line, struct io_chunk_t info);
void _dbg_res_free(void *res);
void _dbg_res_atexit();

unsigned int dbg_res_count();
void dbg_res_dump(struct io_output_t output);

#define dbg_res_alloc(res) _dbg_res_alloc(res, __FILE__, __LINE__)
#define dbg_res_free(res) _dbg_res_free(res)

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
