# 🚀 Embedded Software Learning Platform

> **The Ultimate Guide for Embedded Software Engineers**  
> From fundamentals to advanced concepts, interview preparation to hands-on projects

[![GitHub stars](https://img.shields.io/github/stars/theEmbeddedGeorge/theEmbeddedGeorge.github.io?style=social)](https://github.com/theEmbeddedGeorge/theEmbeddedGeorge.github.io)
[![GitHub forks](https://img.shields.io/github/forks/theEmbeddedGeorge/theEmbeddedGeorge.github.io?style=social)](https://github.com/theEmbeddedGeorge/theEmbeddedGeorge.github.io)
[![GitHub issues](https://img.shields.io/github/issues/theEmbeddedGeorge/theEmbeddedGeorge.github.io)](https://github.com/theEmbeddedGeorge/theEmbeddedGeorge.github.io/issues)

---

## 📚 **Learning Paths**

### 🟢 **Beginner Path (0-6 months)**
*For those new to embedded systems or transitioning from other fields*

#### **Phase 1: Programming Fundamentals (4 weeks)**
- [ ] **C Programming Basics**
  - [C Overview](https://embeddedworlddevelopers.blogspot.com/p/c-language.html)
  - [C/C++ Preprocessor Directives](https://www.tenouk.com/Module10.html)
  - [C/C++ Type Specifiers](https://www.tenouk.com/Module11.html)
  - [Structure Member Alignment, Padding and Data Packing](https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/)
  - [Memory Alignment](https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/)

- [ ] **Basic Data Structures**
  - [Arrays and Linked Lists](Data_Struct_Implementation/queue/README.md)
  - [Stacks and Queues](Data_Struct_Implementation/stack/README.md)
  - [Hash Tables](Data_Struct_Implementation/hashTable/README.md)

#### **Phase 2: Embedded Basics (4 weeks)**
- [ ] **Hardware Fundamentals**
  - [GPIO and Basic I/O](HW_Module/Flash_Storage.md)
  - [Timers and Interrupts](Operating_System/Interrupt.md)
  - [UART Communication](Bus_Protocol/uart.md)

- [ ] **Basic Algorithms**
  - [Bubble Sort](Data_Struct_Implementation/bubbleSort/bubbleSort.md)
  - [Insertion Sort](Data_Struct_Implementation/insertionSort/insertionSort.md)
  - [Binary Search](Data_Struct_Implementation/BST/README.md)

#### **Phase 3: System Concepts (4 weeks)**
- [ ] **Operating System Basics**
  - [Process Management](Operating_System/Process/Process.md)
  - [Memory Management](Operating_System/virtual_memory.md)
  - [Interrupts and Exceptions](Operating_System/Interrupt.md)

### 🟡 **Intermediate Path (6-12 months)**
*For those with basic embedded knowledge*

#### **Phase 1: Advanced Programming (6 weeks)**
- [ ] **Advanced C/C++**
  - [Inline Assembly](https://wiki.osdev.org/Inline_Assembly)
  - [Concurrency and Threading](Data_Struct_Implementation/concurrency/)
  - [Memory Management](Data_Struct_Implementation/memoryPoolAllocator/README.md)

- [ ] **Data Structures & Algorithms**
  - [Binary Search Trees](Data_Struct_Implementation/BST/README.md)
  - [Heaps and Priority Queues](Data_Struct_Implementation/binaryHeap/README.md)
  - [Ring Buffers](Data_Struct_Implementation/circularRingBuffer/README.md)

#### **Phase 2: Communication Protocols (4 weeks)**
- [ ] **Serial Communication**
  - [I2C Protocol](Bus_Protocol/i2c.md)
  - [SPI Protocol](Bus_Protocol/spi.md)
  - [UART/RS232](Bus_Protocol/uart.md)

- [ ] **Network Protocols**
  - [TCP/UDP](Network/tcp_udp_ip.md)
  - [Ethernet](Network/ethernet.md)
  - [ARP/MAC](Network/ARP.md)

#### **Phase 3: System Design (6 weeks)**
- [ ] **Real-Time Systems**
  - [FreeRTOS Basics](Operating_System/freertos/firmware_stack.md)
  - [Scheduling Algorithms](Operating_System/Process/Process_thread_scheduling.md)
  - [State Machines](Data_Struct_Implementation/stateMachine/README.md)

### 🔴 **Advanced Path (12+ months)**
*For experienced embedded engineers*

#### **Phase 1: Computer Architecture (6 weeks)**
- [ ] **ARM Architecture**
  - [ARM Overview](https://wiki.osdev.org/ARM_Overview)
  - [ARM Assembly](https://azeria-labs.com/writing-arm-assembly-part-1/)
  - [Memory Hierarchy](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf)

- [ ] **Advanced Memory**
  - [Cache Memory](https://www.geeksforgeeks.org/cache-memory-in-computer-organization/)
  - [DMA](Computer_architecture/dma.md)
  - [TLB](Computer_architecture/tlb.md)

#### **Phase 2: System Design (8 weeks)**
- [ ] **Distributed Systems**
  - [System Design Basics](Interview/SystemDesign/systemDesign.md)
  - [Load Balancing](Interview/SystemDesign/loadBalancing.md)
  - [Caching Strategies](Interview/SystemDesign/caching.md)

- [ ] **Embedded Security**
  - [TPM 2.0](Security/TPM.md)
  - [ARM TrustZone](https://embeddedsecurity.io/)
  - [Encryption Algorithms](Security/)

---

## 🎯 **Quick Navigation**

### **📖 Core Concepts**
| Topic | Difficulty | Status |
|-------|------------|--------|
| [C/C++ Programming](#a-cc-coding-specific) | 🟢 Beginner | ✅ Complete |
| [Data Structures & Algorithms](#c-data-structures--implementation) | 🟡 Intermediate | ✅ Complete |
| [Bus Protocols](#f-bus) | 🟡 Intermediate | ✅ Complete |
| [Operating Systems](#h-operating-systems) | 🔴 Advanced | ✅ Complete |
| [Computer Architecture](#i-computer-architecture) | 🔴 Advanced | ✅ Complete |

### **🔧 Practical Implementation**
| Implementation | Language | Status |
|----------------|----------|--------|
| [State Machine](Data_Struct_Implementation/stateMachine/README.md) | C | ✅ Complete |
| [Ring Buffer](Data_Struct_Implementation/circularRingBuffer/README.md) | C | ✅ Complete |
| [Hash Table](Data_Struct_Implementation/hashTable/README.md) | C | ✅ Complete |
| [Binary Search Tree](Data_Struct_Implementation/BST/README.md) | C | ✅ Complete |
| [Memory Pool Allocator](Data_Struct_Implementation/memoryPoolAllocator/README.md) | C | ✅ Complete |

### **💼 Interview Preparation**
| Category | Questions | Status |
|----------|-----------|--------|
| [LeetCode for Embedded](Interview/Algorithm/LeetCode_for_Embedded_Developer.md) | 50+ | ✅ Complete |
| [Concept Questions](Interview/Concept/Concept_questions.md) | 200+ | ✅ Complete |
| [System Design](Interview/SystemDesign/) | 20+ | ✅ Complete |
| [Company-Specific](Interview/Company/) | 10+ companies | ✅ Complete |

---

## 📋 **Detailed Topics**

### **A. C/C++ Programming** 🟢
<details>
<summary>Click to expand</summary>

#### **Core Concepts**
1. [C Overview](https://embeddedworlddevelopers.blogspot.com/p/c-language.html)
2. [C++ Overview](https://embeddedworlddevelopers.blogspot.com/p/c-language_12.html)
3. [Tenouk's Ultimate C/C++ Tutorials](https://www.tenouk.com/Sitemap.html)
4. [C/C++ Preprocessor Directives](https://www.tenouk.com/Module10.html)
5. [C/C++ Type Specifiers](https://www.tenouk.com/Module11.html)
6. [Structure Member Alignment, Padding and Data Packing](https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/)
7. [Memory Alignment](https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/)

#### **Advanced Topics**
8. [The Function Stack](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow2a.html)
9. [C/C++ Compile, Link and Running Programs](https://www.tenouk.com/ModuleW.html)
10. [C Storage Class, Scope and Memory allocation](https://www.tenouk.com/ModuleZ.html)
11. [C/C++ Storage Classes: const, volatile, local and global](https://www.tenouk.com/Module20.html)
12. [Inline Assembly](https://wiki.osdev.org/Inline_Assembly)
13. [Buffer Overflow](https://www.tenouk.com/Bufferoverflowc/Bufferoverflow1.html)

#### **Modern C++**
14. [C++11 Q/A](https://www.stroustrup.com/C++11FAQ.html#specific-aims)
15. [C++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
16. [C++ STL Cheat Sheet](Resources/STL_Cheatsheet.pdf)

#### **Concurrency**
17. [C/C++ MULTI-THREADED programming with Pthread](https://www.bogotobogo.com/cplusplus/multithreading_pthread.php)
18. [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
19. [OpenMP Programming](https://computing.llnl.gov/tutorials/openMP/)

</details>

### **B. Data Structures & Implementation** 🟡
<details>
<summary>Click to expand</summary>

#### **Core Data Structures**
1. [Finite State Machine](Data_Struct_Implementation/stateMachine/README.md)
2. [Ring Buffer](Data_Struct_Implementation/circularRingBuffer/README.md)
3. [Hash Table](Data_Struct_Implementation/hashTable/README.md)
4. [Binary Search Tree](Data_Struct_Implementation/BST/README.md)
5. [Heap (Priority Queue)](Data_Struct_Implementation/binaryHeap/README.md)
6. [Stack](Data_Struct_Implementation/stack/README.md)
7. [Queue](Data_Struct_Implementation/queue/README.md)

#### **Memory Management**
8. [Memory Pool Allocation](Data_Struct_Implementation/memoryPoolAllocator/README.md)
9. [Aligned Malloc](Data_Struct_Implementation/alignedMalloc/README.md)
10. [Safe memcpy (memmove)](Data_Struct_Implementation/memcpy_memmove/README.md)

#### **Algorithms**
11. [Bubble Sort](Data_Struct_Implementation/bubbleSort/bubbleSort.md)
12. [Merge Sort](Data_Struct_Implementation/mergeSort/mergeSort.md)
13. [Quick Sort](Data_Struct_Implementation/quickSort/quicksort.md)
14. [Heap Sort](Data_Struct_Implementation/heapSort/heapSort.md)
15. [Insertion Sort](Data_Struct_Implementation/insertionSort/insertionSort.md)

#### **Bit Manipulation**
16. [Reverse Bits](Data_Struct_Implementation/BitsManipulation/reverseBits.md)
17. [Count bits with lookup table](Data_Struct_Implementation/BitsManipulation/countBitsLookUpTable.md)
18. [Swap odd and even bits](Data_Struct_Implementation/BitsManipulation/swapOddEvenBIts.md)

#### **Concurrency**
19. [Producer and Consumer Problem](Data_Struct_Implementation/concurrency/BoundedQueue.md)
20. [Reader Writer Problem](Data_Struct_Implementation/concurrency/ReaderWritter.md)
21. [FizzBuzz MultiThread](Data_Struct_Implementation/concurrency/FizzBuzz.md)
22. [Building H2O](Data_Struct_Implementation/concurrency/BuildingH2O.md)

</details>

### **C. Bus Protocols** 🟡
<details>
<summary>Click to expand</summary>

#### **Serial Communication**
1. [Serial Communication Overview](Bus_Protocol/Serial_communication.md)
2. [UART Protocol](Bus_Protocol/uart.md)
3. [RS232/RS422/RS485](Bus_Protocol/RS232_RS422_RS485.md)

#### **Synchronous Protocols**
4. [SPI Protocol](Bus_Protocol/spi.md)
5. [I2C Protocol](Bus_Protocol/i2c.md)
6. CAN Protocol (Coming Soon)
7. USB Protocol (Coming Soon)

#### **High-Speed Protocols**
8. PCIe (Coming Soon)

</details>

### **D. Operating Systems** 🔴
<details>
<summary>Click to expand</summary>

#### **Core Concepts**
1. [Interrupts and Exceptions](Operating_System/Interrupt.md)
2. [Exception Handling (ARM64)](Operating_System/Exception_ARM64.md)
3. [Process Management](Operating_System/Process/Process.md)
4. [Threads and Concurrency](Operating_System/Process/Threads.md)
5. [Scheduling](Operating_System/Process/Process_thread_scheduling.md)

#### **Memory Management**
6. [Virtual Memory](Operating_System/virtual_memory.md)
7. [Memory Management Reference](https://www.memorymanagement.org/index.html)

#### **Linux**
8. [Linux Device Model](Operating_System/Linux/Linux_device_model.md)
9. [Linux Booting Process](Operating_System/Linux/booting.md)
10. [System Calls](Operating_System/Linux/syscall.md)
11. [Boot Loader](Operating_System/Linux/boot_loader.md)

#### **RTOS**
12. [FreeRTOS Firmware Stack](Operating_System/freertos/firmware_stack.md)
13. [FreeRTOS API Reference](https://www.freertos.org/a00106.html)

</details>

### **E. Computer Architecture** 🔴
<details>
<summary>Click to expand</summary>

#### **ARM Architecture**
1. [ARM Overview](https://wiki.osdev.org/ARM_Overview)
2. [ARM Assembly](https://azeria-labs.com/writing-arm-assembly-part-1/)
3. [ARM Paging](https://wiki.osdev.org/ARM_Paging)

#### **Memory Systems**
4. [Memory Hierarchy](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf)
5. [Cache Memory](https://www.geeksforgeeks.org/cache-memory-in-computer-organization/)
6. [Cache Coherence](https://www.geeksforgeeks.org/cache-coherence/)
7. [DMA](Computer_architecture/dma.md)
8. [TLB](Computer_architecture/tlb.md)

#### **Hardware Modules**
9. [Flash Storage](HW_Module/Flash_Storage.md)
10. [STM32 MCU](http://www.emcu.it/STM32.html)

</details>

### **F. Networking** 🟡
<details>
<summary>Click to expand</summary>

#### **Network Layers**
1. [Network OSI Layer](https://www.geeksforgeeks.org/layers-of-osi-model/)
2. [TCP/UDP](Network/tcp_udp_ip.md)
3. [IP Protocol](Network/ip.md)
4. [ICMP](Network/icmp.md)
5. [ARP/MAC](Network/ARP.md)
6. [Ethernet](Network/ethernet.md)

#### **Advanced Networking**
7. [DHCP](Network/dhcp.md)
8. [LTE](Network/LTE.md)
9. [Multicast](Network/Multicast.md)
10. [TSN/AVB](Network/AVB_TSN.md)

</details>

### **G. Interview Preparation** 🎯
<details>
<summary>Click to expand</summary>

#### **Coding Questions**
1. [LeetCode for Embedded Developers](Interview/Algorithm/LeetCode_for_Embedded_Developer.md)
2. [Advanced LeetCode Questions](Interview/Algorithm/LeetCode_for_embedded_advanced.md)
3. [Linked List Problems](Interview/Algorithm/linked_list.md)
4. [String Problems](Interview/Algorithm/string.md)
5. [Array Problems](Interview/Algorithm/Array.md)

#### **Concept Questions**
6. [Common Embedded Interview Questions](Interview/Concept/Common_embedded_interview.md)
7. [I2C Interview Questions](Interview/Concept/I2C_interview_questions.md)
8. [SPI Interview Questions](Interview/Concept/SPI_interview_questions.md)
9. [UART Interview Questions](Interview/Concept/UART_interview_questions.md)

#### **System Design**
10. [System Design Approach](Interview/SystemDesign/systemDesign.md)
11. [Load Balancing](Interview/SystemDesign/loadBalancing.md)
12. [Caching Strategies](Interview/SystemDesign/caching.md)
13. [Consistent Hashing](Interview/SystemDesign/consistentHashing.md)

#### **Company-Specific**
14. [Tesla](Interview/Company/tesla.md)
15. [Google](https://www.geeksforgeeks.org/google-interview-preparation/)
16. [Qualcomm](Interview/Company/qualcomm.md)
17. [Nvidia](Interview/Company/Nvidia.md)
18. [Apple](Interview/Company/apple.md)

</details>

---

## 🛠️ **Hands-On Projects**

### **Raspberry Pi Projects**
1. [Learning OS Development with Raspberry Pi](https://github.com/s-matyukevich/raspberry-pi-os)
2. [Raspberry Pi Multicore Programming](https://github.com/LdB-ECM/Raspberry-Pi)
3. [David Welch's Tutorials](https://github.com/dwelch67/raspberrypi)
4. [Cambridge OS Tutorials](https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/)
5. [Bare Metal Programming on Raspberry Pi 3](https://github.com/bztsrc/raspi3-tutorial)

---

## 📚 **Recommended Books**

### **Networking**
- Unix Network Programming, volumes 1-2 by W. Richard Stevens
- Internetworking with TCP/IP, volume I by Douglas E. Comer
- TCP/IP Illustrated, volumes 1-3 by W. Richard Stevens and Gary R. Wright
- TCP/IP Network Administration by Craig Hunt
- Advanced Programming in the UNIX Environment by W. Richard Stevens
- Computer Networking - A Top-Down Approach

---

## 🔗 **Useful Resources**

### **Community & References**
- [Awesome Embedded Systems](https://github.com/embedded-boston/awesome-embedded-systems)
- [Awesome C](https://github.com/kozross/awesome-c)
- [Embedded Systems Guide](https://github.com/Embedded-Systems-Guide)
- [The Linux Documentation Project](https://tldp.org/guides.html)
- [OSDev.org](https://wiki.osdev.org/Main_Page)
- [Memory Management References](https://www.memorymanagement.org/)

### **Interview Resources**
- [Embedded Software Interview Archive](https://docs.google.com/document/d/1uW030FMfBxKLxXz-eIwyzlMJdERN5DMEwtUnJMYsF-g/edit)
- [Embedded Noob Engineer Guide](https://drive.google.com/file/d/0B8K8lEdVL_HUZHgwd0t5Zmc1d3c/view?usp=sharing)
- [Awesome Embedded Interview](https://docs.google.com/document/d/18HMyd-lFu1hWiixFLS2Pc7-SgyzDDqitzXbfAnUVeBE/mobilebasic)

---

## 🤝 **Contributing**

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for details.

### **How to Contribute**
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

---

## 📄 **License**

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 **Acknowledgments**

- All contributors who have helped build this comprehensive learning platform
- The embedded systems community for sharing knowledge and resources
- Open source projects that have inspired and supported this work

---

**⭐ Star this repository if you found it helpful!**