## Interprocess Communications

#### **Methods in Interprocess Communication**
Inter-process communication (IPC) is set of interfaces, which is usually programmed in order for the programs to communicate between series of processes. This allows running programs concurrently in an Operating System. These are the methods in IPC:

- Pipes (Same Process)
    - This allows flow of data in one direction only. Analogous to simplex systems (Keyboard). Data from the output is usually buffered until input process receives it which must have a common origin.
    - [Explanation & Example](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_pipes.htm)

- Named Pipes (Different Processes)
    - This is a pipe with a specific name it can be used in processes that don’t have a shared common process origin. E.g. is FIFO where the details written to a pipe is first named.
    - [Explanation & Example](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_named_pipes.htm)

- Message Queuing
    - This allows messages to be passed between processes using either a single queue or several message queue. This is managed by system kernel these messages are coordinated using an API.
    - [Explanation & Example](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_message_queues.htm)

- Semaphores
    - This is used in solving problems associated with synchronization and to avoid race condition. These are integer values which are greater than or equal to 0.
    - [Explanation & Exmaple](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_semaphores.htm)

- Shared memory
    - This allows the interchange of data through a defined area of memory. Semaphore values have to be obtained before data can get access to shared memory.
    - [Explanation & Exmaple](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_semaphores.htm)

- Sockets
    - This method is mostly used to communicate over a network between a client and a server. It allows for a standard connection which is computer and OS independent.

- Signals
    - [Explanation & Exmaple](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_signals.htm)

- Memory Mapping
    - [Explanation & Examples](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_memory_mapping.htm)

#### Detailed Explanations
- [Shared Address Space](https://courses.engr.illinois.edu/cs241/sp2012/lectures/29-IPC.pdf) (Shared Memory, Memory Mapped Files)
- [Message transported by OS from one address space to another](https://courses.engr.illinois.edu/cs241/sp2012/lectures/30-IPC.pdf)(Files, Pipes and FIFOs)
- [Some POSIX Examples](https://courses.engr.illinois.edu/cs241/fa2010/ppt/31-IPC.pdf)


#### Implementation
[Brendan's Multi-tasking Tutorial](https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial)

This tutorial will describe a way to implement multi-tasking and task switching for a kernel that uses "kernel stack per task". It has been designed to allow the reader to implement a full featured scheduler (while avoiding common pitfalls) in steps that can be tested before moving on to the next step.
