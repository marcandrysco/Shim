#ifndef IO_DEFS_H
#define IO_DEFS_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Control a device.
 *   @ref: The refernce.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: True if the signal is handle, false otherwise.
 */

typedef bool (*io_ctrl_f)(void *ref, unsigned int id, void *data);

/**
 * Close a device.
 *   @ref: The reference.
 */

typedef void (*io_close_f)(void *ref);


/**
 * Device interface structure.
 *   @ctrl: Control.
 *   @close: Close.
 */

struct io_device_i {
	io_ctrl_f ctrl;
	io_close_f close;
};

/**
 * Device structure.
 *   @ref: The reference.
 *   @iface: The interface.
 */

struct io_device_t {
	void *ref;
	const struct io_device_t *iface;
};


/**
 * Write function.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes written.
 */

typedef size_t (*io_write_f)(void *ref, const void *restrict buf, size_t nbytes);

/**
 * Output interface.
 *   @device: The base device interface.
 *   @write: Write.
 */

struct io_output_i {
	struct io_device_i device;

	io_write_f write;
};

/**
 * Output structure.
 *   @ref: The reference.
 *   @iface: The interface.
 */

struct io_output_t {
	void *ref;
	const struct io_output_i *iface;
};

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
