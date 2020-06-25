#ifndef CLI_ARGS_H
#define CLI_ARGS_H

struct cli_args {
	char* fifo_read;    // FIFO to read from parent.
	char* fifo_write;   // FIFO to write to parent.
	char* server_ip;    // The server IP.
	unsigned server_port;  // Port number of server.
	unsigned buffer_size;  // Buffer size for reading/writing FIFOs.
};

/*
 * Takes command line arguments, checks their validity, stores them in
 * 'struct cli_args' and returns a pointer to the allocated struct.
 *
 * argc: number of arguments in 'argv'.
 * argv: arguments to parse.
 *
 * Make sure to free the allocated memory for the struct after use.
 *
 * Success: Returns pointer to an allocated 'struct cli_args'.
 * Failure: Returns NULL and sets 'error_code' accordingly.
 */
struct cli_args* parse_cli_args(int argc, char* argv[]);

#endif // #ifndef CLI_ARGS_H
