#ifndef READ_WRITE_H
#define READ_WRITE_H

/*
 * Read the "size" of the incoming message/packet from a file descriptor.
 * e.g.: depending on the protocol between the two processes this function
 * could describe to the reader the number of messages incomming or the size
 * of the next message in bytes, from the writer.
 *
 * Note that 'read_data()' reads the number of incomming bytes before reading.
 * You don't need to 'read_size()' and pass the bytes to read into 'read_data()'.
 * This function server as a way to implement more complex communication.
 * e.g.: writer is going to write 8 strings. Before doing so he must inform the
 * reader of the number of strings to read. Then for each string we call
 * 'write_data()' to send each string. As already noted 'read_data()' and
 * 'write_data()' reads and writes the bytes of the incomming or outgoing string
 * using 'read_size()' and 'write_size()' respectively.
 *
 * TODO: Both 'read_size()' and 'write_size()' convert the number to send to network
 * byte order, making them safe to use over sockets.
 *
 * 'fd_r': file descriptor of the opened file to read from.
 * 'size': the size of the incomming message/packet.
 *
 * The functions 'read_size()' and 'write_size()' read and write 'size_t' types.
 *
 * Success: returns the number of bytes read.
 * Failure: calls 'perror()' and exits with 'errno' on system error.
 */
int read_size(int fd_r, size_t* size);

/*
 * Write the "size" of the outgoing message/packet to a file descriptor.
 * For more checkout 'read_size()'.
 *
 * 'fd_w': file descriptor of the opened file to write to.
 * 'size': size of the outgoing message/packet.
 *
 * The functions 'read_size()' and 'write_size()' read and write 'size_t' types.
 *
 * Success: returns the number of bytes wrote.
 * Failure: calls 'perror()' and exits with 'errno' on system error.
 */
int write_size(int fd_w, size_t size);

/*
 * Read data from a file opened at file descriptor 'fd_r'.
 * It's implemented to read the whole message between signal interrupts.
 *
 * 'fd_r': opened file descriptor of a file you want to read from.
 * e.g.: named pipe, pipe, socket, etc.
 * 'buffer': buffer to store the read message from file descriptor.
 * Pass the address of a NULL string. The function will allocate memory for the
 * string and store the read message in it along with the NULL terminating byte.
 *
 * Success: returns the number of bytes read.
 * Failure: calls 'perror()' and exits with 'errno' on system error.
 */
int read_data(int fd_r, char** buffer);

/*
 * Write data to a file opened at file descriptor 'fd_w'.
 * It's implemented to write the whole message between signal interrupts.
 *
 * 'fd_w': opened file descriptor of a file you want to write to.
 * e.g.: named pipe, pipe, socket, etc.
 * 'buffer': the NULL terminated message to write to file descriptor.
 *
 * Success: returns the number of bytes read.
 * Failure: calls 'perror()' and exits with 'errno' on system error.
 */
int write_data(int fd_w, char* buffer);

#endif  // #ifndef READ_WRITE_H
