#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>  // Waiting children.
#include <sys/types.h>
#include <sys/stat.h>

#include "cli_args/cli_args.h"
#include "worker/worker.h"
#include "errors/errors.h"
#include "subdirs/subdirs.h"
#include "read_write/read_write.h"

int main(int argc, char* argv[])
{
	struct cli_args* args = NULL;
	if ((args = parse_cli_args(argc, argv)) == NULL) {
		print_error("Failed to parse cli arguments");
		return error_code;
	}

	// Create worker structs.
	struct worker** workers = workers_init(args->n_workers);

	char server_port[16];
	sprintf(server_port, "%u", args->server_port);
	char buffer_size[16];
	sprintf(buffer_size, "%u", args->buffer_size);
	// Start the workers.
	for (unsigned c_worker = 0; c_worker < args->n_workers; ++c_worker) {
		// Create read FIFO.
		sprintf(workers[c_worker]->fifo_read, "rfifo.%u", c_worker);
		if ((mkfifo(workers[c_worker]->fifo_read, 0600)) < 0)
			{ perror("Can't create fifo"); goto out; }
		// Create write FIFO.
		sprintf(workers[c_worker]->fifo_write, "wfifo.%u", c_worker);
		if ((mkfifo(workers[c_worker]->fifo_write, 0600)) < 0)
			{ perror("Can't create fifo"); goto out; }

		// Start worker.
		if ((workers[c_worker]->pid = fork()) == 0) {  // Child process.
			if ((execlp("./worker", "./worker",
					"-r", workers[c_worker]->fifo_write,
					"-w", workers[c_worker]->fifo_read,
					"-s", args->server_ip,
					"-p", server_port,
					"-b", buffer_size,
					(char *)NULL)) == -1)
				{ perror("Exec worker"); exit(errno); }
		}
	}

	char** all_subdirs;     // All of the subdirectories.
	unsigned n_all_subdirs;  // The number of subdirectories.
	all_subdirs = parse_subdirs(args->input_dir, &n_all_subdirs);

	printf("Number of subdirs: %u.\n", n_all_subdirs);
	for (unsigned i = 0; i < n_all_subdirs; ++i)
		printf("%s\n", all_subdirs[i]);

	//for (unsigned i = 0; i < n_all_subdirs; ++i)
	//	free(all_subdirs[i]);
	//free(all_subdirs);

	char*** splitted_subdirs;  // Splitted subdirs to 'n_workers'.
	unsigned* n_worker_subdirs;  // Number of subdirs for each worker.
	splitted_subdirs = split_subdirs(all_subdirs, n_all_subdirs,
			args->n_workers, &n_worker_subdirs);
	// 'split_subdirs()' uses the same pointers for the path strings.
	// Therefore, we just need to free the array and not the strings.
	free(all_subdirs);

	for (unsigned c_worker = 0; c_worker < args->n_workers; ++c_worker) {
		printf("Worker %u gets %u:\n", c_worker, n_worker_subdirs[c_worker]);
		for (unsigned c_subdir = 0; c_subdir < n_worker_subdirs[c_worker]; ++c_subdir) {
			printf("	%s\n", splitted_subdirs[c_worker][c_subdir]);
		}
	}

	//for (unsigned c_worker = 0; c_worker < args->n_workers; ++c_worker) {
	//	for (unsigned c_subdir = 0; c_subdir < n_worker_subdirs[c_worker]; ++c_subdir)
	//		free(splitted_subdirs[c_worker][c_subdir]);
	//	free(splitted_subdirs[c_worker]);
	//}
	//free(splitted_subdirs);
	//free(n_worker_subdirs);

	// Open the workers FIFOs.
	printf("Master: Opening FIFOs.\n");
	open_workers_FIFOs(workers, args->n_workers);
	printf("Master: Opened FIFOs.\n");

	printf("Master: Writing data.\n");
	write_data(workers[0]->fd_w, "Hello worker 0!");
	write_data(workers[1]->fd_w, "Hello worker 1!");
	printf("Master: Wrote data.\n");

	// Send the partitioned subdirectories to the workers.
	send_subdirs(workers, args->n_workers, splitted_subdirs,
			n_worker_subdirs);

	// Wait for workers.
out:
	// TODO: implement function to free worker structs.
	// Free workers structs and unlink FIFOs.
	return 0;
}
