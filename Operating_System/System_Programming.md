> ## 🚀 Practice & deep-dive on EmbeddedInterviewLab
>
> Get these OS / Linux concepts as community-ranked interview questions with model answers, plus interactive deep-dive guides.
>
> 👉 **[Browse Embedded Linux & OS questions →](https://embeddedinterviewlab.com/questions/domain/embedded-linux?utm_source=github&utm_medium=referral&utm_campaign=kb_cta&utm_content=operating_system)** &nbsp;·&nbsp; **[Read the in-depth guide →](https://embeddedinterviewlab.com/topics/linux-userspace-basics?utm_source=github&utm_medium=referral&utm_campaign=kb_topic&utm_content=operating_system)**

---

# System Programming

> **Mastering the Interface Between Applications and the Operating System**  
> Understanding POSIX APIs, system calls, and signal handling for robust embedded applications

---

## 📋 **Table of Contents**

- [System Programming Fundamentals](#system-programming-fundamentals)
- [POSIX APIs and Standards](#posix-apis-and-standards)
- [System Call Interface](#system-call-interface)
- [Signal Handling](#signal-handling)
- [File and I/O Operations](#file-and-io-operations)
- [Process and Thread Control](#process-and-thread-control)
- [Memory Management APIs](#memory-management-apis)
- [Advanced System Programming](#advanced-system-programming)

---

## 🏗️ **System Programming Fundamentals**

### **What is System Programming?**

System programming involves writing software that directly interfaces with the operating system kernel to access hardware resources and system services. Unlike application programming, system programming operates at a lower level, requiring deep understanding of the operating system's architecture and behavior.

**The Role of System Programming in Embedded Systems:**

- **Hardware Access**: Direct control over hardware resources
- **Performance Optimization**: Bypass high-level abstractions for efficiency
- **System Integration**: Interface with kernel services and drivers
- **Resource Management**: Control memory, CPU, and I/O allocation
- **Real-time Requirements**: Meet strict timing and response requirements

#### **System Programming vs. Application Programming**

Understanding the distinction between system and application programming is crucial for embedded development:

**Application Programming:**
- **Level**: User space, high-level abstractions
- **Focus**: Business logic, user interface, data processing
- **Safety**: Protected by kernel, cannot crash system
- **Performance**: Moderate, with kernel overhead
- **Complexity**: Lower, with rich library support

**System Programming:**
- **Level**: Kernel space or privileged user space
- **Focus**: System services, hardware control, performance
- **Safety**: Can affect system stability, requires careful design
- **Performance**: High, with direct hardware access
- **Complexity**: Higher, with manual resource management

```
┌─────────────────────────────────────┐
│         Application Layer           │ ← High-level abstractions
│         (User programs)             │   - Libraries and frameworks
│                                    │   - Business logic
├─────────────────────────────────────┤
│         System Programming          │ ← Low-level system interface
│         (System calls, APIs)        │   - Kernel services
│                                    │   - Hardware abstraction
├─────────────────────────────────────┤
│         Operating System            │ ← Kernel and drivers
│         (Kernel space)              │   - Resource management
│                                    │   - Hardware control
├─────────────────────────────────────┤
│         Hardware Layer              │ ← Physical hardware
│         (CPU, memory, I/O)          │   - Raw hardware resources
└─────────────────────────────────────┘
```

#### **System Programming Philosophy**

System programming follows the **efficiency and reliability principle**—write code that is both performant and robust, with careful attention to resource management and error handling.

**Design Principles:**

- **Efficiency**: Minimize overhead and maximize performance
- **Reliability**: Handle errors gracefully and predictably
- **Portability**: Write code that works across different systems
- **Maintainability**: Create clear, well-documented interfaces
- **Security**: Follow security best practices and principles

---

## 📚 **POSIX APIs and Standards**

### **Portable Operating System Interface**

POSIX (Portable Operating System Interface) defines a standard set of APIs that ensure application portability across different Unix-like operating systems. Understanding POSIX is essential for writing portable embedded applications.

#### **POSIX Standard Philosophy**

POSIX follows the **portability principle**—define interfaces that work consistently across different operating systems while allowing for system-specific optimizations and extensions.

**POSIX Design Goals:**

- **Portability**: Code works on different POSIX-compliant systems
- **Consistency**: Similar functions work the same way everywhere
- **Efficiency**: APIs are designed for performance
- **Extensibility**: Systems can add features while maintaining compatibility
- **Backward Compatibility**: Old code continues to work

#### **Core POSIX Standards**

**POSIX.1 (Core Services):**
- **File Operations**: Open, read, write, close files
- **Process Management**: Create, terminate, wait for processes
- **Signals**: Handle asynchronous events
- **Memory Management**: Allocate and manage memory
- **Inter-process Communication**: Pipes, message queues, shared memory

**POSIX.1b (Real-time Extensions):**
- **Real-time Scheduling**: Priority-based scheduling policies
- **Timers**: High-precision timers and clocks
- **Synchronous I/O**: Blocking I/O operations
- **Memory Locking**: Lock memory in physical RAM
- **Message Queues**: Real-time message passing

**POSIX.1c (Threads):**
- **Thread Creation**: Create and manage threads
- **Thread Synchronization**: Mutexes, condition variables, semaphores
- **Thread-specific Data**: Per-thread storage
- **Thread Cancellation**: Graceful thread termination
- **Thread Scheduling**: Thread-level scheduling control

#### **POSIX Compliance Levels**

Different systems provide varying levels of POSIX compliance:

```
┌─────────────────────────────────────┐
│         Full POSIX Compliance      │ ← Complete standard support
│         (Linux, FreeBSD)           │   - All required APIs
│                                    │   - Optional extensions
├─────────────────────────────────────┤
│         Partial POSIX Compliance   │ ← Core functionality only
│         (macOS, Windows Subsystem) │   - Essential APIs
│                                    │   - Some extensions missing
├─────────────────────────────────────┤
│         Minimal POSIX Support      │ ← Basic compatibility
│         (Embedded RTOS)            │   - Core APIs only
│                                    │   - Limited functionality
└─────────────────────────────────────┘
```

---

## 🔧 **System Call Interface**

### **The Bridge Between User and Kernel Space**

System calls provide the fundamental mechanism by which user-space applications request services from the kernel. Understanding how system calls work is essential for efficient system programming.

#### **System Call Architecture**

System calls follow a well-defined process that involves several architectural layers:

```
User Application
       │
       ▼
   Library Function (e.g., printf)
       │
       ▼
   System Call Wrapper
       │
       ▼
   System Call Number (in register)
       │
       ▼
   Kernel Entry Point
       │
       ▼
   System Call Handler
       │
       ▼
   Kernel Service Function
       │
       ▼
   Return to User Space
```

**System Call Flow:**

1. **User Preparation**: Application places system call number and arguments in registers
2. **Trap Instruction**: Special instruction triggers transition to kernel mode
3. **Kernel Entry**: Kernel saves user context and switches to kernel mode
4. **Parameter Validation**: Kernel validates all arguments for safety
5. **Service Execution**: Kernel performs the requested operation
6. **Context Restoration**: Kernel restores user context and returns result

#### **Common System Call Categories**

**Process Management:**
- **`fork()`**: Create new process
- **`execve()`**: Execute program
- **`wait()`**: Wait for child process
- **`exit()`**: Terminate process
- **`getpid()`**: Get process ID

**File Operations:**
- **`open()`**: Open file or device
- **`read()`**: Read data from file
- **`write()`**: Write data to file
- **`close()`**: Close file descriptor
- **`lseek()`**: Change file position

**Memory Management:**
- **`brk()`**: Change data segment size
- **`mmap()`**: Map file or device to memory
- **`munmap()`**: Unmap memory region
- **`mprotect()`**: Change memory protection

**Inter-process Communication:**
- **`pipe()`**: Create pipe for IPC
- **`shmget()`**: Get shared memory segment
- **`msgget()`**: Get message queue
- **`semget()`**: Get semaphore set

#### **System Call Performance Considerations**

System calls have overhead that can impact performance in embedded systems:

**Overhead Sources:**
- **Mode Switch**: User to kernel mode transition
- **Context Save/Restore**: Save and restore CPU registers
- **Parameter Validation**: Kernel validates all arguments
- **Memory Access**: Cross-user/kernel boundary data access
- **Scheduling**: Potential context switch during system call

**Optimization Strategies:**
- **Batch Operations**: Combine multiple operations into single system call
- **Memory Mapping**: Use `mmap()` for large I/O operations
- **Direct I/O**: Bypass kernel buffering when appropriate
- **System Call Batching**: Use `io_uring` or similar mechanisms
- **Avoid Unnecessary Calls**: Cache results and minimize calls

---

## ⚡ **Signal Handling**

### **Managing Asynchronous Events**

Signals provide a mechanism for the kernel to notify processes of asynchronous events. Understanding signal handling is crucial for building robust embedded applications that can respond to external events and system conditions.

#### **Signal Philosophy**

Signal handling follows the **asynchronous event principle**—provide a mechanism for processes to respond to events that occur outside their normal execution flow.

**Signal Design Goals:**

- **Asynchronous**: Handle events that occur at unpredictable times
- **Efficient**: Minimal overhead for signal delivery
- **Flexible**: Allow processes to customize signal handling
- **Reliable**: Ensure signals are delivered to intended processes
- **Safe**: Prevent signal handling from corrupting process state

#### **Signal Types and Categories**

**Standard Signals (1-31):**
- **Termination Signals**: SIGTERM, SIGKILL, SIGQUIT
- **Error Signals**: SIGSEGV, SIGBUS, SIGFPE, SIGILL
- **Control Signals**: SIGINT, SIGTSTP, SIGCONT
- **Alarm Signals**: SIGALRM, SIGVTALRM, SIGPROF
- **User Signals**: SIGUSR1, SIGUSR2

**Real-time Signals (32-63):**
- **Extended Range**: Additional signal numbers for applications
- **Queue Support**: Multiple signals can be queued
- **Priority**: Higher priority than standard signals
- **Custom Use**: Application-defined purposes

#### **Signal Handling Implementation**

Signal handling involves setting up signal handlers and managing signal delivery:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// Global variables for signal handling
volatile sig_atomic_t signal_received = 0;
volatile sig_atomic_t graceful_shutdown = 0;

// Signal handler for graceful shutdown
void signal_handler(int sig) {
    switch (sig) {
        case SIGINT:
            printf("Received SIGINT (Ctrl+C)\n");
            graceful_shutdown = 1;
            break;
            
        case SIGTERM:
            printf("Received SIGTERM\n");
            graceful_shutdown = 1;
            break;
            
        case SIGUSR1:
            printf("Received SIGUSR1\n");
            signal_received = 1;
            break;
            
        case SIGALRM:
            printf("Received SIGALRM (timeout)\n");
            signal_received = 1;
            break;
            
        default:
            printf("Received signal %d\n", sig);
            break;
    }
}

// Set up signal handling
int setup_signal_handling(void) {
    struct sigaction sa;
    
    // Initialize signal action structure
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;  // Restart interrupted system calls
    
    // Set up signal handlers
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Failed to set SIGINT handler");
        return -1;
    }
    
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Failed to set SIGTERM handler");
        return -1;
    }
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Failed to set SIGUSR1 handler");
        return -1;
    }
    
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("Failed to set SIGALRM handler");
        return -1;
    }
    
    printf("Signal handlers set up successfully\n");
    return 0;
}

// Main application loop
int main() {
    // Set up signal handling
    if (setup_signal_handling() == -1) {
        exit(1);
    }
    
    printf("Application started (PID: %d)\n", getpid());
    printf("Send SIGUSR1 to trigger action, SIGINT/SIGTERM to exit\n");
    
    // Main application loop
    while (!graceful_shutdown) {
        // Check for signals
        if (signal_received) {
            printf("Processing signal...\n");
            signal_received = 0;
            
            // Perform signal-specific actions
            // This could involve updating state, logging, etc.
        }
        
        // Simulate application work
        sleep(1);
    }
    
    printf("Graceful shutdown initiated\n");
    
    // Cleanup code here
    printf("Application terminated\n");
    
    return 0;
}
```

**Signal Handling Best Practices:**

- **Keep Handlers Simple**: Signal handlers should be minimal and fast
- **Use Volatile Variables**: Mark signal flags as volatile for proper access
- **Avoid Complex Operations**: Don't call non-reentrant functions in handlers
- **Handle All Signals**: Set up handlers for all signals your application uses
- **Graceful Shutdown**: Implement proper cleanup in signal handlers

---

## 📁 **File and I/O Operations**

### **Managing Data Input and Output**

File and I/O operations form the foundation of most system programming tasks. Understanding how to efficiently read from and write to files, devices, and network connections is essential for embedded applications.

#### **I/O Philosophy**

I/O operations follow the **efficiency and reliability principle**—provide fast, reliable access to data while handling errors gracefully and managing system resources efficiently.

**I/O Design Goals:**

- **Performance**: Minimize I/O overhead and maximize throughput
- **Reliability**: Ensure data integrity and handle errors gracefully
- **Efficiency**: Use appropriate I/O methods for different use cases
- **Portability**: Work consistently across different systems
- **Scalability**: Handle varying data sizes and I/O patterns

#### **File Descriptor Management**

File descriptors are the fundamental abstraction for I/O operations in Unix-like systems:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

// File descriptor management example
int manage_file_descriptors(void) {
    int fd1, fd2, fd3;
    char buffer[1024];
    ssize_t bytes_read, bytes_written;
    
    // Open source file for reading
    fd1 = open("source.txt", O_RDONLY);
    if (fd1 == -1) {
        perror("Failed to open source.txt");
        return -1;
    }
    
    // Open destination file for writing
    fd2 = open("destination.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 == -1) {
        perror("Failed to open destination.txt");
        close(fd1);
        return -1;
    }
    
    // Open log file for appending
    fd3 = open("operation.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd3 == -1) {
        perror("Failed to open operation.log");
        close(fd1);
        close(fd2);
        return -1;
    }
    
    // Copy data from source to destination
    while ((bytes_read = read(fd1, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(fd2, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Write failed");
            break;
        }
        
        // Log operation
        dprintf(fd3, "Copied %zd bytes\n", bytes_written);
    }
    
    if (bytes_read == -1) {
        perror("Read failed");
    }
    
    // Close all file descriptors
    close(fd1);
    close(fd2);
    close(fd3);
    
    return 0;
}
```

**File Descriptor Best Practices:**

- **Check Return Values**: Always check return values from I/O functions
- **Close Descriptors**: Ensure all file descriptors are properly closed
- **Error Handling**: Handle I/O errors gracefully
- **Resource Limits**: Be aware of system limits on open files
- **Descriptor Leaks**: Avoid leaving file descriptors open

#### **Advanced I/O Techniques**

**Memory-Mapped I/O:**
```c
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int memory_mapped_io_example(void) {
    int fd;
    struct stat sb;
    char *mapped_data;
    
    // Open file
    fd = open("large_file.txt", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return -1;
    }
    
    // Get file size
    if (fstat(fd, &sb) == -1) {
        perror("Failed to get file stats");
        close(fd);
        return -1;
    }
    
    // Map file to memory
    mapped_data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_data == MAP_FAILED) {
        perror("Failed to map file");
        close(fd);
        return -1;
    }
    
    // Process data in memory
    printf("File mapped to memory, size: %ld bytes\n", sb.st_size);
    
    // Unmap and close
    munmap(mapped_data, sb.st_size);
    close(fd);
    
    return 0;
}
```

**Non-blocking I/O:**
```c
#include <fcntl.h>

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("Failed to get file flags");
        return -1;
    }
    
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("Failed to set non-blocking mode");
        return -1;
    }
    
    return 0;
}
```

---

## 🔄 **Process and Thread Control**

### **Managing Execution Contexts**

Process and thread control involves creating, managing, and coordinating multiple execution contexts. This is essential for building embedded applications that need to handle multiple tasks simultaneously.

#### **Process Control Philosophy**

Process control follows the **isolation and coordination principle**—provide separate execution contexts for different tasks while enabling controlled communication and resource sharing.

**Process Control Goals:**

- **Isolation**: Separate processes cannot interfere with each other
- **Resource Management**: Efficient allocation and sharing of system resources
- **Communication**: Enable controlled inter-process communication
- **Scheduling**: Coordinate execution of multiple processes
- **Security**: Control access to system resources

#### **Process Creation and Management**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// Process creation and management example
int process_management_example(void) {
    pid_t pid1, pid2;
    int status1, status2;
    
    printf("Parent process (PID: %d) starting\n", getpid());
    
    // Create first child process
    pid1 = fork();
    if (pid1 == -1) {
        perror("First fork failed");
        return -1;
    }
    
    if (pid1 == 0) {
        // First child process
        printf("First child (PID: %d) created\n", getpid());
        
        // Simulate work
        sleep(2);
        printf("First child completing\n");
        exit(10);  // Exit with status 10
    }
    
    // Create second child process
    pid2 = fork();
    if (pid2 == -1) {
        perror("Second fork failed");
        wait(&status1);  // Wait for first child
        return -1;
    }
    
    if (pid2 == 0) {
        // Second child process
        printf("Second child (PID: %d) created\n", getpid());
        
        // Simulate work
        sleep(1);
        printf("Second child completing\n");
        exit(20);  // Exit with status 20
    }
    
    // Parent process waits for both children
    printf("Parent waiting for children to complete\n");
    
    waitpid(pid1, &status1, 0);
    printf("First child (PID: %d) completed with status: %d\n", 
           pid1, WEXITSTATUS(status1));
    
    waitpid(pid2, &status2, 0);
    printf("Second child (PID: %d) completed with status: %d\n", 
           pid2, WEXITSTATUS(status2));
    
    printf("All children completed\n");
    return 0;
}
```

#### **Thread Creation and Management**

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Thread data structure
struct thread_data {
    int thread_id;
    int work_duration;
    pthread_mutex_t *mutex;
};

// Thread function
void *thread_function(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    
    printf("Thread %d starting\n", data->thread_id);
    
    // Simulate work
    sleep(data->work_duration);
    
    // Critical section
    pthread_mutex_lock(data->mutex);
    printf("Thread %d in critical section\n", data->thread_id);
    sleep(1);  // Simulate critical work
    pthread_mutex_unlock(data->mutex);
    
    printf("Thread %d completing\n", data->thread_id);
    
    return (void *)(long)(data->thread_id * 100);
}

// Thread management example
int thread_management_example(void) {
    pthread_t threads[3];
    struct thread_data thread_data[3];
    pthread_mutex_t mutex;
    void *thread_result;
    int i;
    
    // Initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Failed to initialize mutex");
        return -1;
    }
    
    // Create threads
    for (i = 0; i < 3; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].work_duration = i + 1;
        thread_data[i].mutex = &mutex;
        
        if (pthread_create(&threads[i], NULL, thread_function, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            return -1;
        }
    }
    
    // Wait for threads to complete
    for (i = 0; i < 3; i++) {
        if (pthread_join(threads[i], &thread_result) != 0) {
            perror("Failed to join thread");
            return -1;
        }
        
        printf("Thread %d returned: %ld\n", i, (long)thread_result);
    }
    
    // Clean up mutex
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
```

---

## 💾 **Memory Management APIs**

### **Controlling Memory Allocation and Access**

Memory management APIs provide control over how memory is allocated, managed, and protected. Understanding these APIs is crucial for building efficient and reliable embedded applications.

#### **Memory Management Philosophy**

Memory management follows the **efficiency and safety principle**—provide fast, reliable memory allocation while preventing memory-related errors and optimizing for system performance.

**Memory Management Goals:**

- **Efficiency**: Minimize memory allocation overhead
- **Safety**: Prevent memory leaks and buffer overflows
- **Performance**: Optimize memory access patterns
- **Flexibility**: Support various allocation strategies
- **Reliability**: Handle allocation failures gracefully

#### **Dynamic Memory Allocation**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Memory allocation with error handling
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed: %s\n", strerror(errno));
        exit(1);
    }
    return ptr;
}

// Memory allocation example
int memory_allocation_example(void) {
    int *int_array;
    char *string_buffer;
    struct {
        int id;
        char name[64];
        double value;
    } *data_array;
    
    // Allocate integer array
    int_array = safe_malloc(100 * sizeof(int));
    for (int i = 0; i < 100; i++) {
        int_array[i] = i * i;
    }
    
    // Allocate string buffer
    string_buffer = safe_malloc(1024);
    strcpy(string_buffer, "Hello, World!");
    
    // Allocate data structure array
    data_array = safe_malloc(50 * sizeof(*data_array));
    for (int i = 0; i < 50; i++) {
        data_array[i].id = i;
        snprintf(data_array[i].name, sizeof(data_array[i].name), "Item_%d", i);
        data_array[i].value = i * 1.5;
    }
    
    // Use allocated memory
    printf("Integer array[50] = %d\n", int_array[50]);
    printf("String buffer: %s\n", string_buffer);
    printf("Data array[25]: id=%d, name=%s, value=%.1f\n",
           data_array[25].id, data_array[25].name, data_array[25].value);
    
    // Free allocated memory
    free(int_array);
    free(string_buffer);
    free(data_array);
    
    return 0;
}
```

#### **Memory Mapping and Protection**

```c
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Memory mapping example
int memory_mapping_example(void) {
    int fd;
    char *mapped_memory;
    size_t page_size = getpagesize();
    size_t map_size = page_size * 2;  // Map 2 pages
    
    // Create temporary file for mapping
    fd = open("/tmp/memory_map", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to create temporary file");
        return -1;
    }
    
    // Extend file to desired size
    if (ftruncate(fd, map_size) == -1) {
        perror("Failed to extend file");
        close(fd);
        return -1;
    }
    
    // Map file to memory
    mapped_memory = mmap(NULL, map_size, PROT_READ | PROT_WRITE, 
                        MAP_SHARED, fd, 0);
    if (mapped_memory == MAP_FAILED) {
        perror("Failed to map memory");
        close(fd);
        return -1;
    }
    
    // Write data to mapped memory
    strcpy(mapped_memory, "Hello from mapped memory!");
    printf("Wrote to mapped memory: %s\n", mapped_memory);
    
    // Change memory protection to read-only
    if (mprotect(mapped_memory, map_size, PROT_READ) == -1) {
        perror("Failed to change memory protection");
    } else {
        printf("Memory protection changed to read-only\n");
        
        // Try to write (this should fail)
        strcpy(mapped_memory, "This should fail");
        printf("Read from memory: %s\n", mapped_memory);
    }
    
    // Unmap and cleanup
    munmap(mapped_memory, map_size);
    close(fd);
    unlink("/tmp/memory_map");
    
    return 0;
}
```

---

## 🚀 **Advanced System Programming**

### **Beyond Basic System Interfaces**

Advanced system programming involves sophisticated techniques for optimizing performance, managing complex resources, and building robust systems. These techniques are essential for high-performance embedded applications.

#### **Advanced I/O Techniques**

**Asynchronous I/O with `io_uring`:**
```c
#include <linux/io_uring.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>

// Simplified io_uring example (Linux-specific)
int io_uring_example(void) {
    struct io_uring ring;
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;
    int fd, ret;
    
    // Initialize io_uring
    ret = io_uring_queue_init(32, &ring, 0);
    if (ret < 0) {
        fprintf(stderr, "Failed to initialize io_uring: %s\n", strerror(-ret));
        return -1;
    }
    
    // Open file for I/O
    fd = open("test_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        io_uring_queue_exit(&ring);
        return -1;
    }
    
    // Submit write request
    sqe = io_uring_get_sqe(&ring);
    if (!sqe) {
        fprintf(stderr, "Failed to get SQE\n");
        close(fd);
        io_uring_queue_exit(&ring);
        return -1;
    }
    
    char write_data[] = "Hello, io_uring!";
    io_uring_prep_write(sqe, fd, write_data, strlen(write_data), 0);
    
    // Submit the request
    ret = io_uring_submit(&ring);
    if (ret < 0) {
        fprintf(stderr, "Failed to submit request: %s\n", strerror(-ret));
        close(fd);
        io_uring_queue_exit(&ring);
        return -1;
    }
    
    // Wait for completion
    ret = io_uring_wait_cqe(&ring, &cqe);
    if (ret < 0) {
        fprintf(stderr, "Failed to wait for completion: %s\n", strerror(-ret));
        close(fd);
        io_uring_queue_exit(&ring);
        return -1;
    }
    
    printf("Write completed: %d bytes\n", cqe->res);
    
    // Mark completion as seen
    io_uring_cqe_seen(&ring, cqe);
    
    // Cleanup
    close(fd);
    io_uring_queue_exit(&ring);
    
    return 0;
}
```

#### **Process and Thread Synchronization**

**Advanced synchronization with futexes:**
```c
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>

// Futex-based mutex implementation
struct futex_mutex {
    int32_t value;
};

// Futex system call wrapper
int futex(int *uaddr, int op, int val, const struct timespec *timeout,
          int *uaddr2, int val3) {
    return syscall(SYS_futex, uaddr, op, val, timeout, uaddr2, val3);
}

// Initialize futex mutex
void futex_mutex_init(struct futex_mutex *mutex) {
    mutex->value = 0;
}

// Lock futex mutex
void futex_mutex_lock(struct futex_mutex *mutex) {
    int32_t expected = 0;
    
    // Try to acquire lock
    while (!__sync_bool_compare_and_swap(&mutex->value, expected, 1)) {
        // Wait for lock to become available
        futex(&mutex->value, FUTEX_WAIT, 1, NULL, NULL, 0);
        expected = 0;
    }
}

// Unlock futex mutex
void futex_mutex_unlock(struct futex_mutex *mutex) {
    int32_t expected = 1;
    
    // Release lock
    if (__sync_bool_compare_and_swap(&mutex->value, expected, 0)) {
        // Wake up waiting threads
        futex(&mutex->value, FUTEX_WAKE, 1, NULL, NULL, 0);
    }
}
```

---

## 🎯 **Conclusion**

System programming provides the foundation for building robust, efficient embedded applications that directly interface with the operating system. Understanding POSIX APIs, system calls, and signal handling is essential for creating applications that can efficiently manage system resources and respond to external events.

**Key Takeaways:**

- **POSIX standards** ensure portability across different systems
- **System calls** provide controlled access to kernel services
- **Signal handling** enables response to asynchronous events
- **I/O operations** form the foundation of data processing
- **Process and thread control** enables concurrent execution
- **Memory management APIs** provide control over resource allocation
- **Advanced techniques** optimize performance and reliability

**The Path Forward:**

As embedded systems become more complex and require more sophisticated operating system interaction, the importance of system programming skills will only increase. Modern systems continue to evolve, providing new APIs and techniques that enable more powerful and efficient embedded applications.

**Remember**: System programming is not just about making system calls—it's about understanding how the operating system works, how to efficiently manage system resources, and how to build applications that can reliably interact with the underlying system. The skills you develop here will serve you throughout your embedded systems career, enabling you to create robust, efficient, and maintainable systems.
