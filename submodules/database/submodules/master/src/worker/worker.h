#ifndef WORKER_H
#define WORKER_H

struct worker {
	pid_t pid;            // The PID of the worker.
	char** countries;     // The countries the worker has indexed.
	char fifo_write[32];  // Fifo to write to child.
	int fd_w;             // File descriptor of opened 'fifo_write' FIFO.
	char fifo_read[32];   // Fifo to read from child.
	int fd_r;             // File descriptor of opened 'fifo_read' FIFO.
};

/*
 * Returns an initialized worker struct.
 *
 * Success: returns pointer to such worker struct.
 * Failure: calls 'perror()' and exits with 'errno'.
 */
struct worker* worker_init();

/*
 * Returns an array with initialized pointers to worker structs.
 *
 * Success: returns pointer to such array.
 * Failure: calls 'perror()' and exits with 'errno'.
 */
struct worker** workers_init(unsigned n_workers);

/*
 * Opens the write FIFOs of each worker and stores the file descriptor
 * at 'fd_w' member of the struct 'struct worker'.
 *
 * Success: opens the write FIFO.
 * Failure: calls 'perror()' and exits on system error.
 */
void open_workers_FIFOs(struct worker** workers, unsigned n_workers);

/*
 * Closes the write FIFOs of each worker.
 *
 * Success: opens the write FIFO.
 * Failure: calls 'perror()' and exits on system error.
 */
void close_workers_FIFOs(struct worker** workers, unsigned n_workers);

#endif  // #ifndef WORKER_H
