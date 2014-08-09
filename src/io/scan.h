#ifndef IO_SCAN_H
#define IO_SCAN_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * scan function declarations
 */

char *io_scan_str(struct io_input_t input);

unsigned long io_scan_ulong(struct io_input_t input, char *ch);

unsigned int str_parse_uint(const char *str);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
