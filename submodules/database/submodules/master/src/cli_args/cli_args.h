#ifndef CLI_ARGS_H
#define CLI_ARGS_H

struct cli_args {
	unsigned n_workers;    // Number of workers to create.
	unsigned buffer_size;  // Buffer size for reading FIFOs.
	char* server_ip;       // IP address of server (statistics and queries).
	unsigned server_port;  // Port number of server.
	char* input_dir;       // Directory path with patients.
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
