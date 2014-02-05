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


/**
 * Read function.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes read.
 */

typedef size_t (*io_read_f)(void *ref, void *restrict buf, size_t nbytes);

/**
 * Input interface.
 *   @device: The base device interface.
 *   @write: Write.
 */

struct io_input_i {
	struct io_device_i device;

	io_read_f read;
};

/**
 * Input structure.
 *   @ref: The reference.
 *   @iface: The interface.
 */

struct io_input_t {
	void *ref;
	const struct io_input_i *iface;
};


/**
 * Seek whence enumerator.
 *   @io_whence_set_e: Beginning of the file.
 *   @io_whence_cur_e: Current position.
 *   @io_whence_end_e: End of the file.
 */

enum io_whence_e {
	io_whence_set_e,
	io_whence_cur_e,
	io_whence_end_e
};


/**
 * Retrieve the file offset.
 *   @ref: The reference.
 *   &returns: The offset from the beginning of the file.
 */

typedef uint64_t (*io_tell_f)(void *ref);

/**
 * Seek to a given file offset.
 *   @ref: The reference.
 *   @offset: The offset.
 *   @whence: The position from which to seek.
 *   &returns: The new offset from the beginning of the file.
 */

typedef uint64_t (*io_seek_f)(void *ref, int64_t offset, enum io_whence_e whence);

/**
 * File interface.
 *   @device: The base device interface.
 *   @read: Read.
 *   @write: Write.
 *   @tell: Tell.
 *   @seek: Seek.
 */

struct io_file_i {
	struct io_device_i device;

	io_read_f read;
	io_write_f write;

	io_tell_f tell;
	io_seek_f seek;
};

/**
 * File structure.
 *   @ref: The reference.
 *   @iface: The interface.
 */

struct io_file_t {
	void *ref;
	const struct io_file_i *iface;
};

/**
 * File open options enumerator.
 *   @io_read_e: Read.
 *   @io_write_e: Write.
 *   @io_append_e: Append.
 *   @io_trunc_e: Truncate.
 *   @io_unbuf_e: Unbuffered.
 *   @io_create_e: Create.
 */

enum io_file_e {
	io_read_e = 0x01,
	io_write_e = 0x02,
	io_append_e = 0x04,
	io_trunc_e = 0x08,
	io_unbuf_e = 0x10,
	io_create_e = 0x20
};


/*
 * special value definitions
 */

#define IO_EOS		(-1)

/*
 * control definitions
 */

enum io_ctrl_e {
	io_ctrl_eos_e
};
#define IO_CTRL_EOS	(1)

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
