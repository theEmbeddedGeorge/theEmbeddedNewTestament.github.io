> ## 🚀 Practice & deep-dive on EmbeddedInterviewLab
>
> Get these OS / Linux concepts as community-ranked interview questions with model answers, plus interactive deep-dive guides.
>
> 👉 **[Browse Embedded Linux & OS questions →](https://embeddedinterviewlab.com/questions/domain/embedded-linux?utm_source=github&utm_medium=referral&utm_campaign=kb_cta&utm_content=operating_system)** &nbsp;·&nbsp; **[Browse the Embedded Linux guides →](https://embeddedinterviewlab.com/categories/embedded-linux?utm_source=github&utm_medium=referral&utm_campaign=kb_domain&utm_content=operating_system)**

---

# Process Management

> **The Foundation of Multitasking in Linux**  
> Understanding how the operating system manages multiple programs and coordinates their execution

---

## 📋 **Table of Contents**

- [Process Fundamentals](#process-fundamentals)
- [Process Creation and Lifecycle](#process-creation-and-lifecycle)
- [Process Scheduling](#process-scheduling)
- [Inter-Process Communication](#inter-process-communication)
- [Process Synchronization](#process-synchronization)
- [Process States and Transitions](#process-states-and-transitions)
- [Advanced Process Management](#advanced-process-management)

---

## 🏗️ **Process Fundamentals**

### **What is a Process?**

A process is the fundamental unit of execution in Linux—it's a running instance of a program that has its own memory space, execution context, and system resources. Think of a process as a "container" that holds everything needed to run a program: code, data, stack, heap, file descriptors, and more.

**The Process Abstraction:**

- **Isolation**: Each process runs in its own virtual address space
- **Resources**: Processes have dedicated system resources (CPU time, memory, I/O)
- **Identity**: Each process has a unique Process ID (PID)
- **State**: Processes can be in various states (running, waiting, stopped)
- **Hierarchy**: Processes form a tree structure with parent-child relationships

#### **Process vs. Program: Understanding the Distinction**

The relationship between programs and processes is fundamental to understanding how Linux works:

**Program (Static):**
- **Definition**: A file containing executable code and data
- **Storage**: Stored on disk as a binary file
- **Content**: Machine instructions, static data, symbol tables
- **State**: Inactive until executed

**Process (Dynamic):**
- **Definition**: An executing instance of a program
- **Storage**: Exists in memory with dynamic state
- **Content**: Code, data, stack, heap, registers, file descriptors
- **State**: Active, with changing execution context

```
Program File (on disk)
       │
       ▼
   Load into Memory
       │
       ▼
   Create Process
       │
       ▼
   Allocate Resources
       │
       ▼
   Begin Execution
```

#### **Process Memory Layout**

Every process has a well-defined memory layout that the kernel manages:

```
┌─────────────────────────────────────┐
│         Stack                       │ ← Grows downward
│  (local variables, function calls) │   - Function call frames
│                                    │   - Local variables
│                                    │   - Return addresses
├─────────────────────────────────────┤
│         ↑                          │
│         │                          │
│         │                          │
│         │                          │
│         │                          │
├─────────────────────────────────────┤
│         Heap                       │ ← Grows upward
│     (dynamic allocations)          │   - malloc() allocations
│                                    │   - Dynamic data structures
├─────────────────────────────────────┤
│        Global/Static Data          │ ← Fixed size
│      (global variables, etc.)      │   - Global variables
│                                    │   - Static variables
├─────────────────────────────────────┤
│           Code                      │ ← Read-only
│        (program instructions)      │   - Machine instructions
│                                    │   - Constants
└─────────────────────────────────────┘
```

**Memory Management Principles:**

- **Stack**: Automatically managed, grows with function calls
- **Heap**: Manually managed, grows with dynamic allocations
- **Data**: Fixed size, initialized at program start
- **Code**: Read-only, shared between processes when possible

---

## 🔄 **Process Creation and Lifecycle**

### **How Processes Come to Life**

Process creation in Linux involves several sophisticated steps that transform a program file into an executing process. This process, known as "forking," creates a copy of the parent process that can then execute different code or the same code with different data.

#### **The Fork Philosophy**

The `fork()` system call embodies the **copy-on-write principle**—it creates the illusion of copying an entire process while actually sharing most of the memory until one process modifies it. This optimization is crucial for performance in embedded systems.

**Fork Design Principles:**

- **Efficiency**: Minimize memory copying during process creation
- **Flexibility**: Allow processes to diverge after creation
- **Reliability**: Ensure clean separation between parent and child
- **Performance**: Optimize for the common case of immediate exec

#### **Process Creation Flow**

```
Parent Process
      │
      ▼
   fork() System Call
      │
      ▼
   Kernel Creates Process Descriptor
      │
      ▼
   Allocate New PID
      │
      ▼
   Copy Parent's Memory Descriptors
      │
      ▼
   Set Up Child-Specific Data
      │
      ▼
   Return to Both Processes
      │
      ▼
   Parent: Child PID, Child: 0
```

**Fork Implementation Details:**

1. **Process Descriptor**: Kernel allocates a new `task_struct`
2. **Memory Mapping**: Child gets copy of parent's memory descriptors
3. **File Descriptors**: Child inherits open files and directories
4. **Signal Handlers**: Child inherits signal handling configuration
5. **Working Directory**: Child inherits current working directory

#### **Basic Process Creation Example**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    
    printf("Parent process starting (PID: %d)\n", getpid());
    
    // Create a child process
    pid = fork();
    
    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child process created (PID: %d, Parent PID: %d)\n", 
               getpid(), getppid());
        
        // Child can execute different code
        printf("Child process executing...\n");
        sleep(2);
        printf("Child process finishing\n");
        exit(0);
    } else {
        // Parent process
        printf("Parent process continuing (Child PID: %d)\n", pid);
        
        // Wait for child to complete
        int status;
        wait(&status);
        printf("Child process completed with status: %d\n", WEXITSTATUS(status));
    }
    
    return 0;
}
```

**Key Concepts in Process Creation:**

- **Return Value**: `fork()` returns different values to parent and child
- **Memory Sharing**: Parent and child initially share memory pages
- **Copy-on-Write**: Memory is only copied when one process writes to it
- **Resource Inheritance**: Child inherits most parent resources
- **PID Assignment**: Child gets a new, unique process ID

---

## ⏱️ **Process Scheduling**

### **Managing CPU Time Among Processes**

Process scheduling is the mechanism by which the Linux kernel determines which process should run on the CPU at any given time. The scheduler must balance several competing goals: fairness, responsiveness, throughput, and resource utilization.

#### **Scheduling Philosophy**

Linux scheduling follows the **fairness principle**—all processes should get a reasonable share of CPU time while maintaining system responsiveness. The scheduler adapts to different types of workloads and system requirements.

**Scheduling Goals:**

- **Fairness**: Ensure all processes get reasonable CPU time
- **Responsiveness**: Minimize response time for interactive processes
- **Throughput**: Maximize overall system performance
- **Efficiency**: Minimize scheduling overhead
- **Predictability**: Provide consistent behavior for real-time processes

#### **Linux Scheduler Architecture**

The Linux scheduler operates on multiple levels:

```
┌─────────────────────────────────────┐
│         User Processes              │ ← User space
├─────────────────────────────────────┤
│         System Call Interface      │ ← Boundary
├─────────────────────────────────────┤
│         Scheduler Core             │ ← Kernel space
│         (CFS - Completely Fair)    │
├─────────────────────────────────────┤
│         CPU Scheduler              │ ← Hardware level
│         (Run queue management)     │
└─────────────────────────────────────┘
```

**Scheduler Components:**

- **CFS (Completely Fair Scheduler)**: Main scheduler for normal processes
- **Real-time Scheduler**: Handles real-time processes with strict priorities
- **Load Balancer**: Distributes processes across multiple CPUs
- **Sleep/Wake Logic**: Manages processes waiting for events

#### **Scheduling Policies**

Linux supports several scheduling policies, each designed for specific use cases:

**SCHED_OTHER (Normal Scheduling):**
- **Purpose**: Default policy for most processes
- **Algorithm**: Completely Fair Scheduler (CFS)
- **Characteristics**: Time-sharing, dynamic priorities
- **Use Case**: General applications, user programs

**SCHED_FIFO (Real-time First-In-First-Out):**
- **Purpose**: Real-time processes that run until completion
- **Algorithm**: Priority-based, no time quantum
- **Characteristics**: Preemptive, no yielding
- **Use Case**: Hard real-time applications

**SCHED_RR (Real-time Round-Robin):**
- **Purpose**: Real-time processes with time quantum limits
- **Algorithm**: Priority-based with time slicing
- **Characteristics**: Preemptive, fair sharing among same priority
- **Use Case**: Soft real-time applications

#### **Scheduling Policy Example**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/resource.h>

int main() {
    int policy;
    struct sched_param param;
    
    // Get current scheduling policy
    policy = sched_getscheduler(0);
    printf("Current scheduling policy: ");
    
    switch (policy) {
        case SCHED_OTHER:
            printf("SCHED_OTHER (normal)\n");
            break;
        case SCHED_FIFO:
            printf("SCHED_FIFO (real-time)\n");
            break;
        case SCHED_RR:
            printf("SCHED_RR (round-robin real-time)\n");
            break;
        default:
            printf("Unknown\n");
    }
    
    // Get current priority
    if (sched_getparam(0, &param) == 0) {
        printf("Current priority: %d\n", param.sched_priority);
    }
    
    // Set real-time scheduling policy (requires root privileges)
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == 0) {
        printf("Successfully set to SCHED_FIFO with priority 50\n");
    } else {
        printf("Failed to set real-time scheduling (may need root privileges)\n");
    }
    
    return 0;
}
```

**Scheduling Priority Management:**

- **Nice Values**: Range from -20 (highest priority) to +19 (lowest priority)
- **Real-time Priorities**: Range from 1 (lowest) to 99 (highest)
- **Dynamic Adjustment**: Scheduler adjusts priorities based on behavior
- **Priority Inheritance**: Prevents priority inversion problems

---

## 📡 **Inter-Process Communication**

### **Sharing Data and Coordinating Execution**

Inter-process communication (IPC) mechanisms allow processes to exchange data, synchronize their execution, and coordinate access to shared resources. Linux provides several IPC mechanisms, each designed for specific use cases and performance requirements.

#### **IPC Design Philosophy**

IPC mechanisms follow the **abstraction principle**—they provide simple, consistent interfaces that hide the complexity of inter-process communication. The choice of mechanism depends on the specific requirements of the application.

**IPC Selection Criteria:**

- **Performance**: How fast does data need to be transferred?
- **Reliability**: How critical is data integrity?
- **Complexity**: How sophisticated does the communication need to be?
- **Persistence**: How long should the communication channel exist?
- **Security**: How much isolation is required between processes?

#### **IPC Mechanism Overview**

```
┌─────────────────────────────────────┐
│         User Applications           │
├─────────────────────────────────────┤
│         IPC Mechanisms              │
│  ┌─────────┬─────────┬─────────┐   │
│  │  Pipes  │ Shared  │Message │   │
│  │         │ Memory  │Queues  │   │
│  └─────────┴─────────┴─────────┘   │
├─────────────────────────────────────┤
│         Kernel Support              │
│  (System calls, memory management) │
└─────────────────────────────────────┘
```

**IPC Mechanism Types:**

- **Pipes**: Simple unidirectional byte streams
- **FIFOs**: Named pipes for unrelated processes
- **Shared Memory**: High-performance data sharing
- **Message Queues**: Structured message passing
- **Sockets**: Network-based communication
- **Signals**: Asynchronous event notification

#### **Pipes: Simple Data Flow**

Pipes provide the simplest form of IPC, creating a unidirectional communication channel between related processes:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[256];
    
    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
    
    pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process - writes to pipe
        close(pipefd[0]); // Close read end
        
        const char *message = "Hello from child process!";
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);
        
        printf("Child sent message\n");
        exit(0);
    } else {
        // Parent process - reads from pipe
        close(pipefd[1]); // Close write end
        
        int bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        if (bytes_read > 0) {
            printf("Parent received: %s\n", buffer);
        }
        
        close(pipefd[0]);
        wait(NULL);
    }
    
    return 0;
}
```

**Pipe Characteristics:**

- **Unidirectional**: Data flows in one direction only
- **Related Processes**: Only parent-child or related processes can use
- **Automatic Synchronization**: Kernel handles blocking and buffering
- **Byte Stream**: No message boundaries, just continuous data
- **Kernel Buffering**: Data is buffered in kernel memory

#### **Shared Memory: High-Performance Data Sharing**

Shared memory provides the fastest IPC mechanism by allowing multiple processes to access the same region of physical memory:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    key_t key = ftok("/tmp", 'A');
    int shmid;
    char *shared_memory;
    
    // Create shared memory segment
    shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Shared memory creation failed");
        exit(1);
    }
    
    // Attach shared memory to process address space
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (char *)-1) {
        perror("Shared memory attachment failed");
        exit(1);
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process - writes to shared memory
        strcpy(shared_memory, "Hello from child via shared memory!");
        printf("Child wrote to shared memory\n");
        
        // Detach shared memory
        shmdt(shared_memory);
        exit(0);
    } else {
        // Parent process - reads from shared memory
        wait(NULL);
        
        printf("Parent read from shared memory: %s\n", shared_memory);
        
        // Detach shared memory
        shmdt(shared_memory);
        
        // Remove shared memory segment
        shmctl(shmid, IPC_RMID, NULL);
    }
    
    return 0;
}
```

**Shared Memory Characteristics:**

- **Highest Performance**: No data copying, direct memory access
- **No Kernel Overhead**: Once established, no system calls needed
- **Requires Synchronization**: Processes must coordinate access
- **Memory Mapping**: Appears as normal memory in process address space
- **System-Wide**: Can be accessed by any process with proper permissions

---

## 🔒 **Process Synchronization**

### **Coordinating Access to Shared Resources**

Process synchronization mechanisms ensure that multiple processes can coordinate their execution and access shared resources safely. Linux provides several synchronization primitives that can be used across process boundaries.

#### **Synchronization Philosophy**

Synchronization follows the **safety principle**—ensure that shared resources are accessed safely while maintaining system performance and avoiding deadlocks.

**Synchronization Goals:**

- **Safety**: Prevent race conditions and data corruption
- **Liveness**: Ensure processes can make progress
- **Performance**: Minimize synchronization overhead
- **Simplicity**: Use the simplest mechanism that meets requirements
- **Reliability**: Handle failures and edge cases gracefully

#### **Synchronization Mechanisms**

Linux provides several IPC-based synchronization mechanisms:

**Semaphores:**
- **Purpose**: Resource counting and mutual exclusion
- **Characteristics**: Atomic operations, can sleep
- **Use Case**: Resource pools, producer-consumer patterns
- **Implementation**: System V semaphores, POSIX semaphores

**File Locks:**
- **Purpose**: File-based mutual exclusion
- **Characteristics**: Advisory or mandatory, process-wide
- **Use Case**: File access coordination, database locking
- **Implementation**: `flock()`, `fcntl()` with F_SETLK

**Condition Variables:**
- **Purpose**: Wait for specific conditions
- **Characteristics**: Can sleep, efficient waiting
- **Use Case**: Producer-consumer, barrier synchronization
- **Implementation**: POSIX condition variables

#### **Semaphore Implementation Example**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

int main() {
    key_t key = ftok("/tmp", 'C');
    int semid;
    
    // Create semaphore set with one semaphore
    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Semaphore creation failed");
        exit(1);
    }
    
    // Initialize semaphore to 1 (binary semaphore for mutual exclusion)
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;
    
    argument.val = 1;
    if (semctl(semid, 0, SETVAL, argument) == -1) {
        perror("Semaphore initialization failed");
        exit(1);
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        struct sembuf operation = {0, -1, 0}; // Wait (decrement)
        
        printf("Child waiting for semaphore...\n");
        if (semop(semid, &operation, 1) == -1) {
            perror("Child: Semaphore wait failed");
            exit(1);
        }
        
        printf("Child acquired semaphore\n");
        sleep(2);
        
        operation.sem_op = 1; // Signal (increment)
        if (semop(semid, &operation, 1) == -1) {
            perror("Child: Semaphore signal failed");
            exit(1);
        }
        
        printf("Child released semaphore\n");
        exit(0);
    } else {
        // Parent process
        struct sembuf operation = {0, -1, 0}; // Wait (decrement)
        
        printf("Parent waiting for semaphore...\n");
        if (semop(semid, &operation, 1) == -1) {
            perror("Parent: Semaphore wait failed");
            exit(1);
        }
        
        printf("Parent acquired semaphore\n");
        sleep(1);
        
        operation.sem_op = 1; // Signal (increment)
        if (semop(semid, &operation, 1) == -1) {
            perror("Parent: Semaphore signal failed");
            exit(1);
        }
        
        printf("Parent released semaphore\n");
        wait(NULL);
        
        // Remove semaphore set
        semctl(semid, 0, IPC_RMID);
    }
    
    return 0;
}
```

**Semaphore Operations:**

- **Wait (P)**: Decrement semaphore, block if zero
- **Signal (V)**: Increment semaphore, wake waiting processes
- **Atomic**: Operations are indivisible
- **Blocking**: Processes can wait for semaphore availability
- **Counting**: Can represent multiple available resources

---

## 🔄 **Process States and Transitions**

### **Understanding the Process Lifecycle**

Processes in Linux can exist in several states, each representing a different phase of their lifecycle. Understanding these states is crucial for effective process management and debugging.

#### **Process State Philosophy**

Process states represent the **resource availability model**—processes move between states based on their resource requirements and system availability. The kernel manages these transitions to optimize system performance.

**State Management Goals:**

- **Efficiency**: Minimize CPU idle time
- **Fairness**: Ensure all processes get CPU time
- **Responsiveness**: Minimize response time for interactive processes
- **Resource Utilization**: Make efficient use of available resources
- **Predictability**: Provide consistent behavior for real-time processes

#### **Process State Diagram**

```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Created   │───▶│  Runnable   │───▶│   Running   │
│   (New)     │    │ (Ready)     │    │ (Active)    │
└─────────────┘    └─────────────┘    └─────────────┘
                           ▲                │
                           │                ▼
                    ┌─────────────┐    ┌─────────────┐
                    │  Sleeping   │◀───│  Blocked    │
                    │ (Waiting)   │    │ (I/O, etc.) │
                    └─────────────┘    └─────────────┘
                           │                │
                           ▼                ▼
                    ┌─────────────┐    ┌─────────────┐
                    │   Stopped   │    │   Zombie    │
                    │(Suspended)  │    │ (Exited)    │
                    └─────────────┘    └─────────────┘
```

**Process States Explained:**

- **Created**: Process is being initialized
- **Runnable**: Process is ready to run, waiting for CPU
- **Running**: Process is currently executing on CPU
- **Sleeping**: Process is waiting for an event (I/O, signal, etc.)
- **Stopped**: Process has been suspended by a signal
- **Zombie**: Process has completed but exit status not collected

#### **State Transition Example**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void signal_handler(int sig) {
    if (sig == SIGUSR1) {
        printf("Process %d received SIGUSR1\n", getpid());
    }
}

int main() {
    signal(SIGUSR1, signal_handler);
    
    printf("Parent process (PID: %d) starting\n", getpid());
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) created\n", getpid());
        
        // Child waits for signal (Sleeping state)
        printf("Child waiting for signal...\n");
        pause();
        
        printf("Child continuing after signal\n");
        exit(0);
    } else {
        // Parent process
        printf("Parent continuing (Child PID: %d)\n", pid);
        
        sleep(1);
        
        // Send signal to child (wakes it from Sleeping state)
        printf("Parent sending SIGUSR1 to child\n");
        kill(pid, SIGUSR1);
        
        // Wait for child to complete
        int status;
        wait(&status);
        printf("Child completed with status: %d\n", WEXITSTATUS(status));
    }
    
    return 0;
}
```

**State Transition Triggers:**

- **Fork**: Created → Runnable
- **Scheduler**: Runnable ↔ Running
- **I/O Request**: Running → Sleeping
- **I/O Completion**: Sleeping → Runnable
- **Signal (SIGSTOP)**: Running → Stopped
- **Signal (SIGCONT)**: Stopped → Runnable
- **Exit**: Running → Zombie
- **Wait**: Zombie → Terminated

---

## 🚀 **Advanced Process Management**

### **Beyond Basic Process Operations**

Advanced process management involves sophisticated techniques for monitoring, controlling, and optimizing process behavior. These techniques are essential for building robust, high-performance embedded systems.

#### **Process Monitoring Philosophy**

Process monitoring follows the **observability principle**—make system behavior visible and understandable so that problems can be identified and resolved quickly.

**Monitoring Goals:**

- **Visibility**: Understand what processes are doing
- **Performance**: Identify bottlenecks and optimization opportunities
- **Debugging**: Quickly locate and resolve problems
- **Capacity Planning**: Understand resource requirements
- **Security**: Detect unauthorized or suspicious activity

#### **Process Information Gathering**

Linux provides several mechanisms for gathering process information:

**`/proc` Filesystem:**
- **Purpose**: Virtual filesystem providing process information
- **Access**: Read files in `/proc/<pid>/` directories
- **Information**: Memory usage, file descriptors, environment, etc.
- **Real-time**: Information is current when accessed

**System Calls:**
- **`getpid()`**: Get current process ID
- **`getppid()`**: Get parent process ID
- **`getuid()`**: Get user ID
- **`getgid()`**: Get group ID

**Library Functions:**
- **`ps` command**: Process status information
- **`top` command**: Real-time process monitoring
- **`strace` command**: System call tracing

#### **Process Control Example**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>

void print_process_info(const char *label) {
    printf("\n=== %s ===\n", label);
    printf("PID: %d\n", getpid());
    printf("Parent PID: %d\n", getppid());
    printf("User ID: %d\n", getuid());
    printf("Group ID: %d\n", getgid());
    
    // Get process priority
    int priority = getpriority(PRIO_PROCESS, 0);
    printf("Priority: %d\n", priority);
    
    // Get resource usage
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        printf("User CPU time: %ld.%06ld seconds\n", 
               usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
        printf("System CPU time: %ld.%06ld seconds\n", 
               usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
        printf("Page faults: %ld\n", usage.ru_majflt);
    }
}

void signal_handler(int sig) {
    printf("Process %d received signal %d\n", getpid(), sig);
    
    if (sig == SIGUSR1) {
        printf("Continuing execution...\n");
    } else if (sig == SIGTERM) {
        printf("Terminating gracefully...\n");
        exit(0);
    }
}

int main() {
    // Set up signal handlers
    signal(SIGUSR1, signal_handler);
    signal(SIGTERM, signal_handler);
    
    print_process_info("Parent Process");
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        print_process_info("Child Process");
        
        // Set different priority
        setpriority(PRIO_PROCESS, 0, 10);
        printf("Child set priority to 10\n");
        
        // Wait for signals
        while (1) {
            printf("Child waiting for signals...\n");
            sleep(5);
        }
    } else {
        // Parent process
        printf("Parent continuing (Child PID: %d)\n", pid);
        
        sleep(2);
        
        // Send signals to child
        printf("Parent sending SIGUSR1 to child\n");
        kill(pid, SIGUSR1);
        
        sleep(2);
        
        printf("Parent sending SIGTERM to child\n");
        kill(pid, SIGTERM);
        
        // Wait for child to terminate
        int status;
        wait(&status);
        printf("Child terminated with status: %d\n", WEXITSTATUS(status));
    }
    
    return 0;
}
```

**Advanced Process Control Features:**

- **Priority Management**: Adjust process scheduling priority
- **Resource Limits**: Set limits on memory, CPU, file descriptors
- **Signal Handling**: Customize response to system events
- **Process Groups**: Organize processes into logical groups
- **Session Management**: Control terminal and job control

---

## 🎯 **Conclusion**

Process management in Linux provides a sophisticated and flexible system for creating, scheduling, and coordinating multiple processes. The system balances performance, resource efficiency, and system stability while providing powerful IPC mechanisms for process communication and synchronization.

**Key Takeaways:**

- **Process abstraction** provides isolation and resource management
- **Fork-exec model** enables efficient process creation and program execution
- **Scheduling policies** balance fairness, responsiveness, and throughput
- **IPC mechanisms** provide flexible inter-process communication
- **Synchronization primitives** ensure safe resource sharing
- **Process states** reflect resource availability and system load
- **Advanced monitoring** enables performance optimization and debugging

**The Path Forward:**

As embedded systems become more complex and require more sophisticated multitasking capabilities, the importance of understanding process management will only increase. Linux continues to evolve its process management system, providing new features and optimizations that enable more powerful and efficient embedded applications.

The future of process management lies in the development of more sophisticated scheduling algorithms, better resource management, and more efficient IPC mechanisms. By embracing these developments and applying process management principles systematically, developers can build embedded systems that effectively utilize the operating system's multitasking capabilities while maintaining system stability and performance.

**Remember**: Process management is not just about creating processes—it's about understanding how processes interact, communicate, and compete for resources. The skills you develop here will serve you throughout your embedded systems career, enabling you to build robust, efficient, and maintainable systems.
