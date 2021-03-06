## Memory Allocation for Small memory software

### **Allocation technique**

- FIXED ALLOCATION Pre-allocating objects as the system starts running

- MEMORY DISCARD Allocating transient objects in groups, often on the stack.

- VARIABLE ALLOCATION Allocating objects dynamically as necessary from a heap.

- POOLED ALLOCATION Allocating objects dynamically from pre-allocated memory
space.

### **Issues to consider when designing memory allocation**

***Fragmentation***

Fragmentation is a significant problem with dynamic memory allocation. There are two kinds
of fragmentation: internal fragmentation, when a data structure does not use all the memory it
has been allocated; and external fragmentation, when memory lying between two allocated
structures cannot be used, generally because it is too small to store anything else.

***Memory Exhasution***

No matter what allocation strategy you choose, you can never have enough memory to meet all
eventualities: you may not pre-allocate enough objects using FIXED ALLOCATION; or a request
for a VARIABLE ALLOCATION from heap or stack memory can fail; or the memory pools for
POOLED ALLOCATION can be empty. Sooner or later you will run out of memory. When
planning your memory allocation, you also need to consider how you will handle memory
exhaustion.

- **Fixed Size Client Memories.** You can expose a fixed-size memory model directly to your
users or client components. For example, many pocket calculators make users choose one of
ten memories in which to save a value, with no suggestion that the system could have more
memory; many components support up to a fixed number of objects in their interfaces
(connections, tasks, operations or whatever) and generate an error if this number is exceeded.
This approach is easy to program, but it decreases the usability of the system, because it makes
users, or client components, take full responsibility for dealing with memory exhaustion.

- **Signal an error.** You can signal a memory exhaustion error to the client. This approach
also makes clients responsible for handling the failure, but typically leaves them with more
options than if you provided a fixed number of user memories. For example, if a graphics
editor program does not have enough memory to handle a large image, users may prefer to shut
down other applications to release more memory in the system as a whole.Signalling errors is more problematic internally, when one component sends an error to
another. Although it is quite simple to notify client components of memory errors, typically by
using exceptions or return codes, programming client components to handle errors correctly is
much more difficult (see the PARTIAL FAILURE pattern).

- **Reduce quality.** You can reduce the quantity of memory you need to allocate by reducing
the quality of the data you need to store. For example, you can truncate strings and reduce the
sampling frequency of sounds and images. Reducing quality can maintain system throughput,
but is not applicable if it discards data that is important to users. Using smaller images, for
example, may be fine in a network monitoring application, but not in a graphics manipulation
program.

- **Delete old objects.** You can delete old or unimportant objects to release memory for new
or important objects. For example telephone exchanges can run out of memory when creating
a new connection, but they can regain memory by terminating the connection that’s been
ringing for longest, because it’s least likely to be answered (FRESH WORK BEFORE STALE,
[Meszaros 1998]). Similarly, many message logs keep from overflowing by storing only a set
amount of messages and deleting older messages as new messages arrive.

- **Defer new requests.** You can delay allocation requests (and the processing that depends
on them) until sufficient memory is available. The simplest and most common approach for
this is for the system not to accept more input until the current tasks have completed. For
example many MS Windows applications change the pointer to a ‘please wait’ ikon, typically
an hourglass, meaning that the user can’t do anything else until this operation is complete. And
many communications systems have ‘flow control’ mechanisms to stop further input until the
current input has been handled. Even simpler is batch-style processing, reading elements
sequentially from a file or database and only reading the next when you’ve processed the
previous one. More complicated approaches require concurrency in the system so that one task
can block on or queue requests being processed by another. Many environments support
synchronisation primitives like semaphores, or higher-level pipes or shared queues that can
block their clients automatically when they cannot fulfil a request. In single-threaded systems
component interfaces can support callbacks or polling to notify their clients that they have
completed processing a request.

- **Ignore the problem.** You can completely ignore the problem, and allow the program to
malfunction. This strategy is, unfortunately, the default in many environments, especially
where paged virtual memory is taken for granted. For example, the Internet worm propagated
through a bug in the UNIX finger demon where long messages could overwrite a fixed-sized
buffer [Page 1988]. This approach is trivial to implement, but can have extremely serious
consequences: the worm that exploited the finger bug disabled much of the Internet for
several days.

### **Specialized Patterns**

- **FIXED ALLOCATION** ensures you’ll always have enough memory by pre-allocating structures to
handle your needs, and by avoiding dynamic memory allocation during normal
processing.
- **VARIABLE ALLOCATION** avoids unused empty memory space by using dynamic allocation to
take and return memory to a heap.
- **MEMORY DISCARD** simplifies de-allocating temporary objects by putting them in a temporary
workspace and discarding the whole workspace at once.
- **POOLED ALLOCATION** avoids the overhead of variable allocation given a large number of similar
objects, by pre-allocating them as required and maintaining a ‘free list’ of
objects to be reused.
- **COMPACTION** avoids memory fragmentation by moving allocated objects in memory to remove
the fragmentation spaces.
- **REFERENCE COUNTING** manages shared objects by keeping a count of the references to each
shared object, and deleting each object when its reference count is zero.
- **GARBAGE COLLECTION** manages shared objects by periodically identifying unreferenced objects
and deleting them.

[A memory Allocator](http://gee.cs.oswego.edu/dl/html/malloc.html)

[Small Memory Links](http://smallmemory.com/SmallMemoryLinks.html)

[Small Memory Software](http://smallmemory.com/ThinkingSmall.pdf)