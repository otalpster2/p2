AM: 201400327
Name: Platon - Nikolaos Kiorpelidis

# Assignment 3
To compile run make.

## Design Desicions
I've implemented the following generic structures:
* list
* AVL tree
* max-min heap
* hashtable

These structures are used by the workers to index the patient records.
The internal implementation of each node like the 'struct node' is hidden
from the user of the data structure. In order to iterate or query the data
stored in the data structure you can use the iterator I've implemented
(in the case of list) or use the data structure's API to query for data that
satify the requirements (like in the case of avl tree or hashtable).
The implementation of max-min heap is partial and is implemented over the
avl tree implementation, sharing the same balancing algorithms. When an avl tree
is heapified, functions used to query the avl tree have undefined behaviour.
All of these data structures are generic, meaning the use void pointer, and to
specify how to sort the data (like in the case of avl) a function pointer is
required when initializing the structure.

---

I've implemented a read/write data to file descriptors library, which is immune
to signal interrupts by writting or reading until a number of bytes is reached.
When reading, to know the number of bytes to read, the sender before writing the
message, sends the number of bytes he is going to write. Afterwards, he writes
the actual message. When reading, we read the number of bytes of the incomming
message and we read until the number of bytes has been read.

The size of the message can be up to 'uint32' when sending over sockets, because
I use the 'ntohl' and 'htonl' to convert from the host endianness to network
endianness, which is big-endian and is 'uint32' type. When sending over named
pipes the size of the messages can be up to 'size' type.

---

When using conditional variables for thread synchronization, the waiting thread
can restart without any thread signaling. According to the specification,
spurious wakeups from wait stats can occur. This means you cannot assume that
when a thread is restarted from a wait some other thread has necessarily
signaled or broadcast. To solve this problem, waits are surrounded by while loop
until a global variable that indicates that a signal has occured and test it to
make sure that this is a read wake up call. If it's not then you can wait on the
condition variable again.

---

To improve the performance of the server-database interaction, the workers
inform the server about the countries they have indexed. This helps when there
are queries about a specific country, like in the case of 'diseaseFrequency'
when a 'country' is specified, or in the case of 'topk-AgeRanges'. The workers
that are not associated with the query are not startled by the server.

---

To synchronize the printing to stdout by the threads of the server when a query
has been satisfied, thread synchronization techniques like mutexes take place.

---

The workers bind to port 0 which means that the socket is bound to an
OS-assigned unique port to the application from a dynamic client port.
Hardcoding the port number is a bad idea, as it might be used by any other
application running on the same PC. In order to send the listening port to the
server, we need to know which port was picked. We can do that with 'getsockname'
after the binding has been performed.

---

I suppose that the connections from the server come first and then the
connections from the clients come second.

---

In order to start the threads all together I use a conditional variable and when
all the threads are ready, the main thread broadcasts the signal so all the
waiting threads start. In order for the main thread to know that all of the
threads are waiting, the threads increase a global variable surrounded with
mutexes. When the global variable reaches the number of threads, the main thread
broadcasts.

---

The server accepts connections from the workers and clients using 'poll'. These
connections are distinguished from each other by the port number of the client.
The threads will therefore server the clients depending their type.

---

The project is seperated in submodules. Three submodules exist and Makefile
recursion is used to compile them. The compiled submodules executables are
placed in the bin directory. You should run the server first, then the database
and last start the clients.
