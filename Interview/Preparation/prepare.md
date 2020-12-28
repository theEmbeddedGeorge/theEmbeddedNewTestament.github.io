## Items to prepare for general embedded interview


### Operating systems/Computer Architecture:
- Process and Threads
- Process Scheduling
- Synchronization mechanisms (mutex/spinlock/semaphore)
- Memory Management (Virtual memory/paging/translation)
- Producer and Consumer Problem
- Caching (cache coherency/cache line)
- Memory mapped IO/ IO Mapped IO

### Bus protocol
- UART
- SPI
- I2C
- RS232/422/485

### Linux Kernel concepts:

- Buffer sharing: DMA Buf & ION
- Wait events/Wait queues
- ISR handling (Top half/Bottom half)
- Bottom Half - Tasklet/Workqueue/SoftIRQ
- Platform driver
- Driver probe (Module init, driver register, compatibility string)
- Device Tree (Device nodes and parsing)
- Clocks/Regulators/GPIO/Pinctrl
- IOMMU/MMU
- Timer Library
- IOCTL, notify dirent
- kmalloc/vmalloc
- kmap/mmap/ioremap
- sysfs/debugfs/procfs

### C programming:
- What is static keywords?
- What is volatile for?
- Macro usage

### Algorithm and data structure
- Linked list
  - reverse Linked list
  - delete a node from the linked list
  - delete duplicate
  - detect cycle
  - Implement hash table with linked list
- String
  - reverse string
  - check whether it is palindrome
  - implement strstr()
  - implement atoi()/itoa()
- Circular buffer
- State machine
- Stack
- Queue
- Binary search tree
- Memory api
  - safe memcpy/memmove
  - sizeof
  - alligned_malloc
  - wrapper api to read/write arbitrary bytes with fix-byte API
- Bitewise operation
  - hamming distance
  - plus without using '+'
  - number of 1 bits
  - reverse bits
