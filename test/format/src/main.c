#include "common.h"


/**
 * I/O length test.
 *   &returns: True of success, false on failure.
 */

bool test_io_len()
{
	size_t len;
	struct io_output_t output;

	printf("testing io len... ");

	output = str_output_len(&len);
	io_printf(output, "Test %u", 123);
	io_output_close(output);

	if(len != 8)
		return printf("failed\n"), false;

	output = str_output_len(&len);
	io_printf(output, "Hello %s", "World");
	io_output_close(output);

	if(len != 11)
		return printf("failed\n"), false;

	printf("okay\n", len);

	return true;
}

/**
 * I/O accumulation test.
 *   &returns: True of success, false on failure.
 */

bool test_io_accum()
{
	char *buf;
	size_t len = 0;
	struct io_output_t output;

	printf("testing io accum... ");

	output = str_output_accum(&buf, &len);
	io_printf(output, "%04u%u%u%u%u%u number -------------------------------------------------------------------------------", 1, 2, 3, 4, 5, 6);
	io_output_close(output);

	//if((len != 11) || !str_isequal(buf, "0001 number -------------------------------------------------------------------------------"))
	if(!str_isequal(buf, "000123456 number -------------------------------------------------------------------------------"))
		return printf("failed\n"), false;

	mem_free(buf);

	printf("okay\n");

	return true;
}

/**
 * String formatting test.
 *   &returns: True of success, false on failure.
 */

bool test_str_printf()
{
	char buf[1024];

	printf("testing string printf... ");

	str_printf(buf, "simple %s", "test");
	if(!str_isequal(buf, "simple test"))
		return printf("failed\n"), false;

	str_printf(buf, "%5s", "pad");
	if(!str_isequal(buf, "  pad"))
		return printf("failed\n"), false;

	str_printf(buf, "%C %C", io_chunk_str("hello"), io_chunk_str("world"));
	if(!str_isequal(buf, "hello world"))
		return printf("failed\n"), false;

	str_printf(buf, "%x", 0x1a94);
	if(!str_isequal(buf, "1a94"))
		return printf("failed\n"), false;

	str_printf(buf, "%c %c", 'g', 'L');
	if(!str_isequal(buf, "g L"))
		return printf("failed\n"), false;

	printf("okay\n");

	return true;
}

/**
 * I/O scanning test.
 *   &returns: True of success, false on failure.
 */

bool test_io_scan()
{
	struct io_input_t input;
	char ch;
	const char *str, *ptr;
	unsigned long ulong;

	printf("testing io scanning... ");

	ch = '\0';
	ptr = str = "123 string";
	input = str_input_ref(&ptr);

	ulong = io_scan_ulong(input, &ch);
	if((ulong != 123) || (ptr != (str + 4)))
		return printf("failed\n"), false;

	io_input_close(input);

	ch = '\0';
	ptr = str = "0x123 string";
	input = str_input_ref(&ptr);

	ulong = io_scan_ulong(input, &ch);
	if((ulong != 0x123) || (ptr != (str + 6)))
		return printf("failed\n"), false;

	io_input_close(input);

	ch = '\0';
	ptr = str = "0b101";
	input = str_input_ref(&ptr);

	ulong = io_scan_ulong(input, &ch);
	if((ulong != 5) || (*ptr != '\0'))
		return printf("failed\n"), false;

	io_input_close(input);

	ch = '\0';
	ptr = str = "0x";
	input = str_input_ref(&ptr);

	ulong = io_scan_ulong(input, &ch);
	if((ulong != 0) || (*ptr != '\0'))
		return printf("failed\n"), false;

	io_input_close(input);

	printf("okay\n");

	return true;
}


/**
 * Main entry point.
 *   @argc: The number of arguments.
 *   @argv: The argument array.
 *   &returns: The return status code.
 */

int main(int argc, char *argv[])
{
	bool suc = true;

	suc &= test_io_len();
	suc &= test_io_accum();
	suc &= test_io_scan();
	suc &= test_str_printf();

	return suc ? 0 : 1;
}
