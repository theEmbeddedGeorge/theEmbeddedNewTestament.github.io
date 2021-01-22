# Embedded System is Life, Embedded System is love.


## Concepts by Domain:

### A. C/C++ Coding Specific
1.  [Tenouk's Ultimate C/C++ Tutorials](https://www.tenouk.com/Sitemap.html)
3.  Attributes
4.  [C/C++ Preprocessor Directives](https://www.tenouk.com/Module10.html)
5.  [C/C++ Type Specifiers](https://www.tenouk.com/Module11.html)
6.  [Structure Member Alignment, Padding and Data Packing](https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/)
7. [Memory Alignment](https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/)
8. [The Function Stack](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow2a.html#:~:text=Arguments%20passed%20to%20a%20C,called%20the%20C%20stack%20frame.)
9. [C/C++ Compile, Link and Running Programs](https://www.tenouk.com/ModuleW.html)
10. [C Storage Class, Scope and Memory allocation](https://www.tenouk.com/ModuleZ.html)
11. [C/C++ Storage Classes: const, volatile, local and global](https://www.tenouk.com/Module20.html)
12. Inline functions
13. Declarations in C
14. Runtime data strtuctures 
15. [Buffer Overflow](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow1.html)
    1. [The Basic of x86 Architecture](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow1a.html)
    2. [An Assembly Language](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow1b.html)
    3. [A Compiler, Assembler, Linker & Loader](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow1c.html)
    4. [C/C++ Process Memory Layout & Function Call](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow1c.html)
    5. [The Function Stack](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow2a.html)
    6. [The Stack Operation](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow3.html)
    7. [A Stack-based Buffer Overflow](https://www.clublexus.com/how-tos/slideshows/6-mods-every-lexus-rc-owner-should-jump-on-440204#6-weight-reduction)
    8. [A Shellcode: The Payload](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow5.html)

### B. Programming Languages
1. [C](https://www.geeksforgeeks.org/c-programming-language/)
2. [C++](https://www.geeksforgeeks.org/c-plus-plus/?ref=leftbar)
3. Bash
4. [Python](https://www.geeksforgeeks.org/python-programming-language/?ref=leftbar)
5. Assembly

### C. Data Structures & Implementation
1. [Ring Buffer](./Data_Struct_Implementation/circularRingBuffer/README.md)
2. [Hash Table](./Data_Struct_Implementation/hashTable/README.md)
3. [Heap (priority queue)](./Data_Struct_Implementation/binaryHeap/README.md)
4. [Stack](./Data_Struct_Implementation/stack/README.md)
5. [Queue](./Data_Struct_Implementation/queue/README.md)
6. [Binary Search Tree](Data_Struct_Implementation/BST/README.md)
7. Red Black Tree
8. [Finite State Machine](Data_Struct_Implementation/stateMachine/README.md)
9. Simple Task Scheduler
10. [Endianess Check](Data_Struct_Implementation/endianess/README.md)
11. Signness Check
12. [Other Common C algorithm](https://github.com/fragglet/c-algorithms)
13. Common STL function implementation
    1. [Safe memcpy (memmove)](Data_Struct_Implementation/memcpy_memmove/README.md)
    2. [itoa](Data_Struct_Implementation/itoa/README.md)
    3. atoi
    4. [sizeof](Data_Struct_Implementation/sizeof/README.md)
    5. [Aligned Malloc](Data_Struct_Implementation/alignedMalloc/README.md)
    6. Malloc
    7. strstr()
14. [C++ Big O Cheat Sheet](https://github.com/gibsjose/cpp-cheat-sheet/blob/master/Data%20Structures%20and%20Algorithms.md#41-insertion-sort)

### D. Basic Algorithms
1. Sort
   1.  Bubble Sort
   2.  Merge Sort
   3.  Quick Sort
   4.  Binary Sort
   5.  Heap Sort
2.  Search
    1.  [Hash](Data_Struct_Implementation/hashTable/README.md)
    2.  [Binary search Tree](Data_Struct_Implementation/BST/README.md)
    3.  Graph

### E. Programming API 
1. Memory management
2. Multi-thread/Multi-processing
   1. POSIX Thread
   2. [Unix IPC](Resources/beeje_guide_unix_ipc.pdf)
3. Network 
   1. [POSIX Socket Programming](Resources/beeje_guide_network_programming.pdf)
   2. [Unix Socket (private google doc)](https://drive.google.com/file/d/1BlcMJzdqdaSYS0jKmI_ZXQqukEG8beCq/view?usp=sharing)

### F. Bus
1.  [Serial Communication](Bus_Protocol/Serial_communication.md)
    1.  Asynchronous Serial Protocols
        1.  [UART](Bus_Protocol/Uart.md)
        2.  [RS232/RS422/RS485](Bus_Protocol/RS232_RS422_RS485.md)
    2.  Synchronous Serial Protocols
        1.  [SPI](./Bus_Protocol/spi.md)
        2.  [I2C](./Bus_Protocol/i2c.md)
        3.  CAN
        4.  USB
2.  PCIE

### G. Linux Commands 
1. [Linux Fundamentals](http://linux-training.be/linuxfun.pdf)
2. [Linux System Administration](http://linux-training.be/linuxsys.pdf)
3. [Linux Servers](http://linux-training.be/linuxsrv.pdf)
4. [Linux Storage Management](http://linux-training.be/linuxsto.pdf)
5. [Linux Security](http://linux-training.be/linuxsec.pdf)
6. [Linux Networking](http://linux-training.be/linuxnet.pdf)


### H. Operating Systems
1.  [Interrupt](./Operating_System/Interrupt.md)
    1.  [Exception Handling](Operating_System/Exception_ARM64.md)
    2.  System call
2.  [Process Management](Operating_System/Process/Process.md)
    1.  Process Manager
    2.  [Scheduling](Operating_System/Process/Process_thread_scheduling.md)
    3.  Threads and Concurrency
3.  Interprocess Synchronization/Communication
    1.  IPC
    2.  Sychronization Facilities
4.  Memory Management
    1.  Memory Hardware
    2.  [Virtual Memory](Operating_System/virtual_memory.md)
5.  File System Management
6.  Device I/O Management
7.  RTOS
    1.  FreeRTOS
        1.  [Firmware Stack Layer](Operating_System/freertos/firmware_stack.md)
        2.  IPC/Sychronization Mechanism
        3.  Scheduler
        4.  Interrupt
        5.  Timers and time management
    2.  [QNX](http://www.qnx.com/developers/docs/qnxcar2/index.jsp?topic=%2Fcom.qnx.doc.neutrino.getting_started%2Ftopic%2Fs3_qnx2nto_Device_driver_model.html)
8.  [Linux](https://0xax.gitbooks.io/linux-insides/content/)
    1.  [Kernel Source Code All Versions](https://elixir.bootlin.com/linux/v4.9.242/source)
    2.  [Linux Device Model](./Operating_System/Linux/Linux_device_model.md)
    3.  Synchronization primitives
    4.  Booting
    5.  [Linux Booting Process](Operating_System/Linux/booting.md)
    6.  Intialization
    7.  Interrupt
    8.  [System Call](Operating_System/Linux/syscall.md)
    9.  [Boot Loader](Operating_System/Linux/boot_loader.md)
    10. Timers and time management
    11. Memory Managment
    12. [Process Management](https://tldp.org/LDP/intro-linux/html/chap_04.html)
    13. [Linux Data Structures](https://tldp.org/LDP/tlk/ds/ds.html)
    14. [Linux Socket Programming](https://www.tenouk.com/cnlinuxsockettutorials.html)
       

### I. Computer Architecture
1.  ARM programmer's model
2.  Pipeline
3.  CPU Architecture
    1.  ARM
        1.  [ARM Assembly](https://azeria-labs.com/writing-arm-assembly-part-1/)
    2.  Intel
4.  Memory Hierachy
5.  [Cache coherence](https://www.geeksforgeeks.org/cache-coherence/)
6.  [DMA](./Computer_architecture/dma.md)
7.  TCM
8.  [TLB](./Computer_architecture/tlb.md)
9.  NUMA
10. Memory translation

### J. HW Modules
1.  RAM & ROM
2.  [Flash Storage](./HW_Module/Flash_Storage.md)
3.  LED/LCD
4.  Timer
5.  Sensors
6.  Bluetooth/BLE
7.  Wifi
8.  LTE
9.  USB devices
10. Memory Protection Unit (MPU)
11. Memory Management Unit (MMU)
12. Floating Point Unit (FPU)
13. Crypto Engine
14. [STM32 MCU](http://www.emcu.it/STM32.html)

### K. Debugging Techniques
1.  [JTAG](https://www.xjtag.com/about-jtag/jtag-a-technical-overview/)
2.  Trace
3.  [GDB](http://beej.us/guide/bggdb/)
4.  Sysfs
5.  Printk

### L. Version Control
1.  GIT
2.  SVN

### M. [IDE Selection](IDE_Selection/IDE.md)

### N. Embedded Security
1.  CRC
2.  TPM
3.  [Arm Trust Zone](https://embeddedsecurity.io/?utm_source=linkedin&utm_medium=social&utm_campaign=phillipjohnston&utm_content=linkstoshare)
4.  Intel SGX
5.  [Embedded Reverse Engineering](https://blog.3or.de/starting-embedded-reverse-engineering-freertos-libopencm3-on-stm32f103c8t6.html)

### O. Network
1.  [Network OSI layer](https://www.geeksforgeeks.org/layers-of-osi-model/?ref=lbp)
    1.  Application layer
        1.  [DHCP](Network/dhcp.md)
        2.  PTP
        3.  HTTP
        4.  DNS
    2.  Transport layer
        1.  [TCP/UDP](Network/tcp_udp_ip.md)
    3.  [Network layer](Network/network.md)
        1.  [IP](Network/ip.md)
        2.  [ICMP](Network/icmp.md)
        3.  [NAT](Network/nat.md)
        4.  [Routing and Protocols (RIP, OSPF, and BGP)](Network/routing.md)
        5.  [Router](Network/router_switch.md)
    4.  Link layer
        1.  PPP
        2.  [ARP/MAC](Network/ARP.md)
        3.  [PTP/gPTP](Network/PTP.md)
        4.  [Ethernet](Network/ethernet.md)
        5.  [Switch](Network/switch.md)
    5.  Physical layer
2.  [LTE](Network/LTE.md)
3.  Wifi
4.  BLE
5.  Bluetooth
6.  [Multicast](Network/Multicast.md)
7.  [TSN/AVB](Network/AVB_TSN.md)

### P. Embedded Programming with Modern C++
1. [C++11 Q/A](https://www.stroustrup.com/C++11FAQ.html#specific-aims)
2. [C++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
3. Resources
   1. [Modern C++ for Embedded Öffentliche Vorträge Conference Articles](https://www.grimm-jaud.de/index.php/oeffentliche-vortraege)
   2. [C++ core guideline by topics](https://www.modernescpp.com/index.php/category/modern-c)
   3. [Technical Report on C++ Performance](http://www.open-std.org/jtc1/sc22/wg21/docs/TR18015.pdf)

## Interview Questions

### [A.  LeetCode Questions for Embedded Developers](Interview/Algorithm/LeetCode_for_Embedded_Developer.md)
1.  Bitwise Operations
2.  [Linked list](Interview/Algorithm/linked_list.md)
3.  String 
4.  Array
5.  Math

### B.  Other Algorithms Questions
1.  Sliding Window
2.  Two Pointers
3.  Faster & Slow Pointers
4.  Merge Intervals
5.  Cyclic Sort
6.  In-place Reversal of a Linked List
7.  BFS
8.  DFS
9.  Two Heaps
10. Subsets
11. Modified Binary Search
12. Bitwise XOR
13. Top 'K' Elements
14. K-way Merge
15. Knapsack (DP)
16. Graph Topological Sort
17. [Bitwise operation C questions](https://aticleworld.com/interview-questions-on-bitwise-operators-in-c/)
18. [Must Do Coding Questions for Companies like Amazon, Microsoft, Adobe](https://www.geeksforgeeks.org/must-do-coding-questions-for-companies-like-amazon-microsoft-adobe/?ref=leftbar-rightbar)


### C.  [Concept Questions](Interview/Concept/Concept_questions.md)
1.  Bus Protocol
    1. [I2C Interview Questions](Interview/Concept/I2C_interview_questions.md)
    2. [SPI Interview Questions](Interview/Concept/SPI_interview_questions.md)
    3. [CAN Interview Questions](Interview/Concept/CAN_interview_questions.md)
    4. [UART Interview Questions](Interview/Concept/UART_interview_questions.md)
    5. USB Interview Questions
    6. PCIe Interview Questions
2. Operating System
   1. [Commonly Asked Operating Systems Interview Questions Sets](https://www.geeksforgeeks.org/commonly-asked-operating-systems-interview-questions-set-1/)
   2. [Commonly Asked Operating Systems Interview Questions Sets2](https://www.researchgate.net/profile/Qasim_Hussein/publication/280315098_Operating_System_Questions_and_their_answers_Processes_and_Deadlock_Part_1/links/55b3e99f08aed621de011166.pdf)
   3. Interrupt Interview Questions
   4. Synchronization Interview Questions
   5. [General OS questions](https://practice.geeksforgeeks.org/tag-page.php?tag=Operating%20Systems)
   6. [Linux](https://practice.geeksforgeeks.org/tag-page.php?tag=Linux)
3. Programming Language
   1. [10 Questions on dynamic memory allocation in C](https://aticleworld.com/10-interview-question-on-dynamic-memory-allocation/)
   2. [Top 15 string Interview Questions in C](https://aticleworld.com/string-interview-questions-in-c/)
   3. [C++ Interview Questions](https://aticleworld.com/cpp-interview-questions/)
   4. [C Puzzles](https://www.geeksforgeeks.org/puzzles/#CPuzzles)
4. Computer Architecture
   1. [Memory](https://practice.geeksforgeeks.org/tag-page.php?tag=memory)
   2.  [Computer Organization Architecture](https://practice.geeksforgeeks.org/tag-page.php?tag=Computer-Organization-Architecture)
5. Computer Networks
   1. [Computer Networks](https://practice.geeksforgeeks.org/tag-page.php?tag=Computer%20Networks)
   2. [Commonly asked Computer Networks Interview Questions](https://www.geeksforgeeks.org/commonly-asked-computer-networks-interview-questions-set-1/)
   

6. [10 Most Common Embedded Interview Questions](Interview/Concept/Common_embedded_interview.md)
7.  [Embedded Interview Questions](Interview/Concept/embedded_interview_questions.md)
8.  [Embedded Interview Questions Repo](https://github.com/Embedded-Systems-Guide/interview-questions)
9.  [Embedded C Interview Questions and answers](https://www.wisdomjobs.com/e-university/embedded-c-interview-questions.html)

### D. Behavior Question
1.  Amazon Leading Principles

### E. [Brain Teaser](https://www.geeksforgeeks.org/puzzles/)

### F. By Company
1.  [Tesla](Interview/Company/tesla.md)
2.  [Google](https://www.geeksforgeeks.org/google-interview-preparation/)
3.  [Qualcomm](Interview/Company/qualcomm.md)
4.  [Nvidia](Interview/Company/Nvidia.md)
5.  [Cisco](Interview/Company/Cisco.md)

### G. Mock Tests
1.  [Computer networks mock tests](https://www.geeksforgeeks.org/quiz-corner-gq/#Computer%20Networks%20Mock%20Tests)
2.  [C++ Mock tests](https://www.geeksforgeeks.org/quiz-corner-gq/#C++%20Programming%20Mock%20Tests)
3.  [Operating system mock tests](https://www.geeksforgeeks.org/quiz-corner-gq/#Operating%20Systems%20Mock%20Tests)

### H. [Interview Preparation](Interview/Preparation/prepare.md)

## Book Recommendation
1.  Network
    1. Unix Network Programming, volumes 1-2 by W. Richard Stevens.
    2. Internetworking with TCP/IP, volume I by Douglas E. Comer.
    3. TCP/IP Illustrated, volumes 1-3 by W. Richard Stevens and Gary R. Wright.
    4. TCP/IP Network Administration by Craig Hunt. Published by O’Reilly & Associates, Inc.
    5. Advanced Programming in the UNIX Environment by W. Richard Stevens.
    6. Computer Networking - A Top-Down Approach

## Useful Links
[Embedded software interview archive](https://docs.google.com/document/d/1uW030FMfBxKLxXz-eIwyzlMJdERN5DMEwtUnJMYsF-g/edit)

[Embedded Noob Engineer Guide](https://drive.google.com/file/d/0B8K8lEdVL_HUZHgwd0t5Zmc1d3c/view?usp=sharing)

[C 语言资源大全中文版](https://github.com/jobbole/awesome-c-cn#build-systems)

[Awesome Embedded Github](https://github.com/embedded-boston/awesome-embedded-systems)

[Awesome C Github](https://github.com/kozross/awesome-c)

[Awesome Embedded Interview Link](https://docs.google.com/document/d/18HMyd-lFu1hWiixFLS2Pc7-SgyzDDqitzXbfAnUVeBE/mobilebasic)

[Embedded system guide repo collection](https://github.com/Embedded-Systems-Guide)

[CS Notes](https://notes.eddyerburgh.me/)

[The Linux Documentation Project](https://tldp.org/guides.html)

[tenouk](https://www.tenouk.com/Sitemap.html)

[Linux Journal](https://www.linuxjournal.com)

[Linux Kernel Documentation](https://www.kernel.org/doc/html/v4.10/index.html)

[Memory Management References](https://www.memorymanagement.org/)

