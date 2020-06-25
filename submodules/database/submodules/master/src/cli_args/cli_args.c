#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cli_args.h"
#include "../errors/errors.h"

struct cli_args* parse_cli_args(int argc, char* argv[])
{
	struct cli_args* args = NULL;

	if ((args = malloc(sizeof(struct cli_args))) == NULL)
		{ perror("Allocate memory for cli_args"); exit(errno); }

	if (argc != 11)
		{ error_code = CLI_ARGS_INV_NUM; goto error_out; }

	for (unsigned c_arg = 1; c_arg < (unsigned)argc; ++c_arg) {
		if (!strcmp(argv[c_arg], "-w"))
			args->n_workers = atoi(argv[++c_arg]);
		else if (!strcmp(argv[c_arg], "-b"))
			args->buffer_size = atoi(argv[++c_arg]);
		else if (!strcmp(argv[c_arg], "-s"))
			args->server_ip = argv[++c_arg];
		else if (!strcmp(argv[c_arg], "-p"))
			args->server_port = atoi(argv[++c_arg]);
		else if (!strcmp(argv[c_arg], "-i"))
			args->input_dir = argv[++c_arg];
		else
			{ error_code = CLI_ARGS_INV_TOKEN; goto error_out; }
	}

	error_code = OK;
	return args;

error_out:
	free(args);
	return NULL;
}
