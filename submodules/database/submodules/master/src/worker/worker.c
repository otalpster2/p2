#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "worker.h"

struct worker* worker_init()
{
	struct worker* worker = NULL;
	if ((worker = malloc(sizeof(struct worker))) == NULL)
		{ perror("Malloc worker"); exit(errno); }

	worker->pid = 0;
	worker->countries = NULL;

	return worker;
}

struct worker** workers_init(unsigned n_workers)
{
	struct worker** workers = NULL;
	if ((workers = malloc(sizeof(struct worker*) * n_workers)) == NULL)
		{ perror("Malloc workers"); exit(errno); }

	for (unsigned c_worker = 0; c_worker < n_workers; ++c_worker)
		workers[c_worker] = worker_init();

	return workers;
}

void open_workers_FIFOs(struct worker** workers, unsigned n_workers)
{
	for (unsigned c_worker = 0; c_worker < n_workers; ++c_worker) {
		workers[c_worker]->fd_w = open(workers[c_worker]->fifo_write, O_WRONLY);
		if (workers[c_worker]->fd_w < 0)
			{ perror("Master: Open write FIFO"); exit(errno); }
	}
}

void close_workers_FIFOs(struct worker** workers, unsigned n_workers)
{
	for (unsigned c_worker = 0; c_worker < n_workers; ++c_worker)
		if (close(workers[c_worker]->fd_w) < 0)
			{ perror("Master: close write FIFO"); exit(errno); }
}
