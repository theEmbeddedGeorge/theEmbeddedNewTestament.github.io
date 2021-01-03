## Items to prepare for general embedded interview


### Operating systems/Computer Architecture:
- Process and Threads
- Process Scheduling
- Synchronization mechanisms (mutex/spinlock/semaphore)
- Memory Management (Virtual memory/paging/translation)
- Producer and Consumer Problem
- Caching (cache coherency/cache line)
- Memory mapped IO/ IO Mapped IO
- Interrupt

### Bus protocol
- UART v
- SPI v
- I2C v
- RS232/422/485 v

### Linux Kernel concepts:
- Linux boot sequence
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

### Network
- Network OSI layers v
- TCP/UDP 
- IP v

### Algorithm and data structure
- Linked list
  - reverse Linked list v
  - delete a node from the linked list v
  - delete duplicate v
  - detect cycle v
  - Implement hash table with linked list 
  - Implement queue with linked list v
- String
  - reverse string
  - check whether it is palindrome
  - implement strstr()
  - implement atoi()/itoa()
- Circular buffer
- State machine
- Stack v
- Queue v
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
- Check endianess v

