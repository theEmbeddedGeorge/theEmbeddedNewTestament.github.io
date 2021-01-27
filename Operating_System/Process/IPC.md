## Interprocess Communications

#### Methods in Interprocess Communication
Inter-process communication (IPC) is set of interfaces, which is usually programmed in order for the programs to communicate between series of processes. This allows running programs concurrently in an Operating System. These are the methods in IPC:

- Pipes (Same Process)
    - This allows flow of data in one direction only. Analogous to simplex systems (Keyboard). Data from the output is usually buffered until input process receives it which must have a common origin.

- Names Pipes (Different Processes)
    - This is a pipe with a specific name it can be used in processes that don’t have a shared common process origin. E.g. is FIFO where the details written to a pipe is first named.

- Message Queuing
    - This allows messages to be passed between processes using either a single queue or several message queue. This is managed by system kernel these messages are coordinated using an API.

- Semaphores
    - This is used in solving problems associated with synchronization and to avoid race condition. These are integer values which are greater than or equal to 0.

- Shared memory
This allows the interchange of data through a defined area of memory. Semaphore values have to be obtained before data can get access to shared memory.

- Sockets
This method is mostly used to communicate over a network between a client and a server. It allows for a standard connection which is computer and OS independent.

#### Detailed Explainations
- [Shared Address Space](https://courses.engr.illinois.edu/cs241/sp2012/lectures/29-IPC.pdf) (Shared Memory, Memory Mapped Files)
- [Message transported by OS from one address space to another](https://courses.engr.illinois.edu/cs241/sp2012/lectures/30-IPC.pdf)(Files, Pipes and FIFOs)
- [Some POSIX Examples](https://courses.engr.illinois.edu/cs241/fa2010/ppt/31-IPC.pdf)