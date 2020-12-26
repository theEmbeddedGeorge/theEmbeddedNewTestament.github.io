# Embedded System is Life, Embedded System is love.


## Concepts by Domain:

### A. C Coding Specific
1.  Function pointers
2.  Bit fields
3.  Attributes
4.  lds script
5.  Linker and loader 

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
10. [Other Common C algorithm](https://github.com/fragglet/c-algorithms)
11. Common STL function implementation
    1. [Safe memcpy (memmove)](Data_Struct_Implementation/memcpy_memmove/README.md)
    2. [itoa](Data_Struct_Implementation/itoa/README.md)
    3. atoi
    4. sizeof
    5. [Aligned Malloc](Data_Struct_Implementation/alignedMalloc/README.md)

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
1. [Unix IPC](Resources/beeje_guide_unix_ipc.pdf) 
2. Memory management
3. Multi-thread/Multi-processing
4. [Socket Programming](Resources/beeje_guide_network_programming.pdf)

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

### G. Linux Command 
1.  Bash
2.  Code Collaboration Tools
3.  Cross Compilation

### H. Operating Systems
1.  User Space
2.  Kernel Space
3.  [Exception Handling](Operating_System/Exception_ARM64.md)
4.  Virtual Memory
5.  File System
6.  [Process/Thread & Scheduling](Operating_System/Process_thread_scheduling.md)
7.  [Interrupt](./Operating_System/Interrupt.md)
8.  System call
9.  Interprocess communication
10. Mutiprocessing/Multithreading
11. FreeRTOS
    1.  [Firmware Stack Layer](Operating_System/freertos/firmware_stack.md)
    2.  IPC Mechanism
    3.  Scheduler
12. Linux
    1.  [Kernel Source Code All Versions](https://elixir.bootlin.com/linux/v4.9.242/source)
    2.  [Linux Device Model](./Operating_System/Linux/Linux_device_model.md)
    3.  [System Call](Operating_System/Linux/syscall.md)
    4.  Synchronization
    5.  Device Tree
    6.  Sysfs
    7.  [Boot Loader](Operating_System/Linux/boot_loader.md)
    8.  [Linux Booting Process](Operating_System/Linux/booting.md)

### I. Computer Architecture
1.  Pipeline
2.  Cache coherence
3.  DMA
4.  TCM
5.  TLB
6.  NUMA
7.  Memory translation

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
1.  JTAG
2.  Trace
3.  [GDB](http://beej.us/guide/bggdb/)
4.  Sysfs
5.  printk

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
1.  Network OSI layer
    1.  Application layer
        1.  [DHCP](Network/dhcp.md)
        2.  PTP
        3.  HTTP
        4.  DNS
    2.  Transport layer
        1.  TCP/UDP
    3.  [Network layer](Network/network_layer.md)
        1.  IP
        2.  ICMP
        3.  NAT
    4.  Link layer
        1.  PPP
        2.  ARP
    5.  Physical layer
        1.  Ethernet
2.  LTE
3.  Wifi
4.  BLE
5.  Bluetooth
6.  [Router/switch](Network/router_switch.md)
7.  Multicast
8.  TSN/AVB

## Interview Questions

### [A.  LeetCode Questions for Embedded Developers](Interview/Algorithm/LeetCode_for_Embedded_Developer.md)

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
   2. Interrupt Interview Questions
   3. Synchronization Interview Questions
   4. [General OS questions](https://practice.geeksforgeeks.org/tag-page.php?tag=Operating%20Systems)
   5. [Linux](https://practice.geeksforgeeks.org/tag-page.php?tag=Linux)
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

