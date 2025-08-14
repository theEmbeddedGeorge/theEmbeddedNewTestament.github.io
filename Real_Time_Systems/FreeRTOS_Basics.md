# FreeRTOS Basics

> **Understanding FreeRTOS fundamentals through concepts, not just code. Learn why RTOS matters and how to think about real-time systems.**

## 📋 **Table of Contents**
- [Concept → Why it matters → Minimal example → Try it → Takeaways](#concept--why-it-matters--minimal-example--try-it--takeaways)
- [Core Concepts](#core-concepts)
- [FreeRTOS Architecture](#freertos-architecture)
- [Task Management](#task-management)
- [Synchronization](#synchronization)
- [Configuration](#configuration)
- [Guided Labs](#guided-labs)
- [Check Yourself](#check-yourself)
- [Cross-links](#cross-links)

---

## **Concept → Why it matters → Minimal example → Try it → Takeaways**

**Concept**: FreeRTOS is a real-time operating system that manages multiple tasks by giving each one a slice of CPU time, ensuring critical operations happen when they need to happen.

**Why it matters**: Without an RTOS, you'd have to manually manage timing, priorities, and resource sharing between different parts of your program. This becomes impossible to maintain as complexity grows, and critical operations might miss their deadlines.

**Minimal example**: A simple system with two tasks - one blinking an LED every 100ms, another reading a sensor every 500ms. FreeRTOS ensures both happen reliably without interfering with each other.

**Try it**: Start with a single task that blinks an LED, then add a second task that reads a sensor. Observe how FreeRTOS manages both automatically.

**Takeaways**: FreeRTOS provides predictable timing and resource management, allowing you to focus on what your system should do rather than how to coordinate multiple operations.

---

## 📋 **Quick Reference: Key Facts**

### **FreeRTOS Fundamentals**
- **Real-time**: Guarantees predictable timing, not necessarily fast execution
- **Preemptive**: Higher priority tasks can interrupt lower priority ones
- **Portable**: Works across multiple microcontroller architectures
- **Open source**: MIT license with no royalties or licensing fees
- **Scalable**: Configurable for systems with minimal or complex requirements

### **Core Components**
- **Scheduler**: Manages which task runs when based on priorities
- **Task Manager**: Handles task creation, deletion, and state transitions
- **Memory Manager**: Manages stack allocation and memory pools
- **Timing Services**: Provides delays, timeouts, and periodic execution
- **Communication**: Queues, semaphores, and mutexes for task coordination

### **Task States**
- **Created**: Task exists but not yet scheduled
- **Ready**: Task is ready to run, waiting for CPU time
- **Running**: Task is currently executing on the CPU
- **Blocked**: Task is waiting for something (delay, data, resource)
- **Deleted**: Task has been removed from system

### **Key Configuration Options**
- **configUSE_PREEMPTION**: Enable/disable preemptive scheduling
- **configTICK_RATE_HZ**: System tick frequency (typically 1000Hz)
- **configMAX_PRIORITIES**: Maximum number of task priorities
- **configMINIMAL_STACK_SIZE**: Minimum stack size for tasks
- **configUSE_MUTEXES**: Enable mutex support for resource protection

---

## 🧠 **Core Concepts**

### **What is Real-Time?**

Real-time doesn't mean "fast" - it means **predictable**. A real-time system guarantees that operations complete within their specified time limits.

```
┌─────────────────────────────────────────────────────────────┐
│                    Real-Time vs Non-Real-Time              │
├─────────────────────────────────────────────────────────────┤
│                Real-Time System                            │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐      │
│  │ Task A  │  │ Task B  │  │ Task A  │  │ Task B  │      │
│  │ 100ms   │  │ 500ms   │  │ 100ms   │  │ 500ms   │      │
│  └─────────┘  └─────────┘  └─────────┘  └─────────┘      │
│  ↑           ↑           ↑           ↑                    │
│  0ms        100ms       200ms       300ms                 │
│                                                           │
│                Non-Real-Time System                       │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐      │
│  │ Task A  │  │ Task B  │  │ Task A  │  │ Task B  │      │
│  │ 100ms   │  │ 500ms   │  │ 150ms   │  │ 600ms   │      │
│  └─────────┘  └─────────┘  └─────────┘  └─────────┘      │
│  ↑           ↑           ↑           ↑                    │
│  0ms        100ms       250ms       350ms                 │
│                                                           │
│  ❌ Timing varies - unpredictable!                        │
└─────────────────────────────────────────────────────────────┘
```

### **Why Use an RTOS Instead of Bare Metal?**

**Bare Metal Approach:**
```
┌─────────────────────────────────────────────────────────────┐
│                    Bare Metal Programming                  │
├─────────────────────────────────────────────────────────────┤
│  while(1) {                                               │
│    // Check if it's time to blink LED                     │
│    if (timer_elapsed(100ms)) {                            │
│      toggle_led();                                         │
│      reset_timer();                                        │
│    }                                                       │
│                                                            │
│    // Check if it's time to read sensor                   │
│    if (timer_elapsed(500ms)) {                            │
│      sensor_value = read_sensor();                         │
│      reset_timer();                                        │
│    }                                                       │
│                                                            │
│    // What if we need to add more tasks?                  │
│    // What if priorities change?                          │
│    // What if timing requirements change?                  │
│  }                                                         │
│                                                            │
│  ❌ Becomes unmanageable quickly!                          │
└─────────────────────────────────────────────────────────────┘
```

**FreeRTOS Approach:**
```
┌─────────────────────────────────────────────────────────────┐
│                    FreeRTOS Approach                       │
├─────────────────────────────────────────────────────────────┤
│  // Task 1: Blink LED every 100ms                         │
│  void vBlinkTask(void *pvParameters) {                    │
│    while(1) {                                             │
│      toggle_led();                                         │
│      vTaskDelay(pdMS_TO_TICKS(100));                      │
│    }                                                       │
│  }                                                         │
│                                                            │
│  // Task 2: Read sensor every 500ms                       │
│  void vSensorTask(void *pvParameters) {                    │
│    while(1) {                                             │
│      sensor_value = read_sensor();                         │
│      vTaskDelay(pdMS_TO_TICKS(500));                      │
│    }                                                       │
│  }                                                         │
│                                                            │
│  // FreeRTOS handles the rest automatically!               │
│  ✅ Clean, maintainable, scalable                          │
└─────────────────────────────────────────────────────────────┘
```

---

## 🏗️ **FreeRTOS Architecture**

### **System Overview**

FreeRTOS sits between your application and the hardware, managing resources and timing:

```
┌─────────────────────────────────────────────────────────────┐
│                    FreeRTOS Architecture                    │
├─────────────────────────────────────────────────────────────┤
│                Application Layer                            │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │   Task 1    │  │   Task 2    │  │   Task 3    │        │
│  │ (Blink LED) │  │ (Read Sens) │  │ (Send Data) │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
├─────────────────────────────────────────────────────────────┤
│                FreeRTOS Kernel                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │  Scheduler  │  │  Memory     │  │  Timing     │        │
│  │             │  │  Manager    │  │  Services   │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │   Queues    │  │ Semaphores  │  │   Mutexes   │        │
│  │             │  │             │  │             │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
├─────────────────────────────────────────────────────────────┤
│                Hardware Abstraction Layer                  │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │   Port      │  │   Memory    │  │  Interrupt  │        │
│  │  Layer      │  │   Model     │  │   Handler   │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
├─────────────────────────────────────────────────────────────┤
│                Hardware (MCU)                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │     CPU     │  │    RAM      │  │  Peripherals│        │
│  │             │  │             │  │             │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
└─────────────────────────────────────────────────────────────┘
```

### **Key Components**

**Scheduler**: The brain that decides which task runs when
**Memory Manager**: Handles stack allocation and memory pools
**Timing Services**: Provides delays, timeouts, and periodic execution
**Communication**: Queues, semaphores, and mutexes for task coordination

---

## 📋 **Task Management**

### **What is a Task?**

A task is like a separate program that runs independently. Think of it as a worker with a specific job to do.

```
┌─────────────────────────────────────────────────────────────┐
│                    Task Lifecycle                          │
├─────────────────────────────────────────────────────────────┤
│                                                           │
│  ┌─────────┐    ┌─────────┐    ┌─────────┐                │
│  │ Created │───▶│  Ready  │───▶│ Running │                │
│  └─────────┘    └─────────┘    └────┬────┘                │
│                                      │                     │
│                                      ▼                     │
│  ┌─────────┐    ┌─────────┐    ┌─────────┐                │
│  │Deleted  │◄───│ Blocked │◄───│         │                │
│  └─────────┘    └─────────┘    └─────────┘                │
│                                                           │
│  • Created: Task exists but not yet scheduled             │
│  • Ready: Task is ready to run, waiting for CPU           │
│  • Running: Task is currently executing                   │
│  • Blocked: Task is waiting for something (delay, data)   │
│  • Deleted: Task has been removed from system             │
└─────────────────────────────────────────────────────────────┘
```

### **Task Priorities**

Tasks have priorities - higher priority tasks get CPU time first:

```
┌─────────────────────────────────────────────────────────────┐
│                    Task Priority System                    │
├─────────────────────────────────────────────────────────────┤
│                                                           │
│  Priority 5: Emergency Stop (highest)                    │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ ██████████████████████████████████████████████████ │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                           │
│  Priority 4: Safety Monitoring                           │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ ██████████████████████████████████████████████████ │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                           │
│  Priority 3: Control Loop                                 │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ ██████████████████████████████████████████████████ │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                           │
│  Priority 2: Data Logging                                 │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ ██████████████████████████████████████████████████ │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                           │
│  Priority 1: Status Updates (lowest)                     │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ ██████████████████████████████████████████████████ │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                           │
│  ⚠️  Higher priority tasks can interrupt lower ones!      │
└─────────────────────────────────────────────────────────────┘
```

### **Creating Your First Task**

Here's the minimal code to create a simple task:

```c
#include "FreeRTOS.h"
#include "task.h"

// Task function - this is what the task will do
void vBlinkTask(void *pvParameters) {
    while (1) {
        // Toggle LED (your hardware-specific code here)
        toggle_led();
        
        // Wait for 500ms - FreeRTOS handles the timing
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// In your main function
int main(void) {
    // Create the task
    xTaskCreate(
        vBlinkTask,        // Function to run
        "BlinkTask",       // Task name (for debugging)
        128,               // Stack size in words
        NULL,              // Parameters (none in this case)
        1,                 // Priority (1 = lowest)
        NULL               // Task handle (not needed here)
    );
    
    // Start the FreeRTOS scheduler
    vTaskStartScheduler();
    
    // Should never reach here
    while (1);
}
```

**Key Points:**
- `vTaskDelay()` doesn't block the CPU - it lets other tasks run
- Stack size (128) should be enough for simple tasks
- Priority 1 is the lowest priority
- The task runs forever in the `while(1)` loop

---

## 🔗 **Synchronization**

### **Why Do Tasks Need to Synchronize?**

When multiple tasks share resources (like a sensor or communication bus), they need to coordinate:

```
┌─────────────────────────────────────────────────────────────┐
│                Problem: Resource Conflict                   │
├─────────────────────────────────────────────────────────────┤
│                                                           │
│  Task A: "I want to read the temperature sensor"          │
│  Task B: "I want to read the temperature sensor"          │
│                                                           │
│  ❌ Both try to read at the same time → corrupted data!   │
│                                                           │
│  Solution: Use a mutex (mutual exclusion)                 │
│                                                           │
│  Task A: "I'll take the mutex first"                      │
│  Task B: "I'll wait for the mutex"                        │
│                                                           │
│  ✅ Only one task can access the sensor at a time         │
└─────────────────────────────────────────────────────────────┘
```

### **Basic Synchronization with Mutex**

```c
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Mutex to protect the sensor
SemaphoreHandle_t xSensorMutex;

// Task that reads sensor
void vSensorTask(void *pvParameters) {
    while (1) {
        // Wait for mutex (wait forever if needed)
        if (xSemaphoreTake(xSensorMutex, portMAX_DELAY) == pdTRUE) {
            // We have the mutex - safe to read sensor
            float temperature = read_temperature_sensor();
            
            // Process temperature data
            process_temperature(temperature);
            
            // Give back the mutex so other tasks can use it
            xSemaphoreGive(xSensorMutex);
        }
        
        // Wait before next reading
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Initialize mutex
void vInitializeSystem(void) {
    // Create the mutex
    xSensorMutex = xSemaphoreCreateMutex();
    
    // Create the task
    xTaskCreate(vSensorTask, "Sensor", 128, NULL, 2, NULL);
}
```

---

## ⚙️ **Configuration**

### **Essential Configuration Options**

FreeRTOS is highly configurable. Here are the key settings you need to understand:

```c
// FreeRTOSConfig.h - Key configuration options

// Enable preemptive scheduling (tasks can interrupt each other)
#define configUSE_PREEMPTION                    1

// System tick frequency (how often FreeRTOS checks for task switches)
#define configTICK_RATE_HZ                      1000

// Maximum number of task priorities
#define configMAX_PRIORITIES                    32

// Minimum stack size for tasks
#define configMINIMAL_STACK_SIZE                128

// Enable mutex support
#define configUSE_MUTEXES                       1

// Enable queue support
#define configUSE_QUEUES                        1

// Enable semaphore support
#define configUSE_COUNTING_SEMAPHORES           1

// Check for stack overflow (important for debugging)
#define configCHECK_FOR_STACK_OVERFLOW          2
```

**What These Mean:**
- **Preemption**: Higher priority tasks can interrupt lower ones
- **Tick Rate**: How often FreeRTOS makes scheduling decisions
- **Priorities**: How many different priority levels you can use
- **Stack Size**: Minimum memory each task gets
- **Features**: Which FreeRTOS features are enabled

---

## 🧪 **Guided Labs**

### **Lab 1: Single Task System**
**Objective**: Understand basic task creation and timing.

**Setup**: Create a single task that blinks an LED at a specific frequency.

**Steps**:
1. Create a task function that toggles an LED
2. Use `vTaskDelay()` to control timing
3. Observe consistent timing behavior
4. Measure actual timing with an oscilloscope (if available)

**Expected Outcome**: Understanding that FreeRTOS provides predictable timing.

### **Lab 2: Multi-Task Coordination**
**Objective**: Learn how tasks can work together.

**Setup**: Create two tasks - one blinks LED A every 200ms, another blinks LED B every 300ms.

**Steps**:
1. Create two separate task functions
2. Give them different priorities
3. Observe how both run independently
4. Notice that timing remains consistent for both

**Expected Outcome**: Understanding that multiple tasks can run concurrently without interference.

### **Lab 3: Resource Sharing**
**Objective**: Learn about synchronization and resource protection.

**Setup**: Create two tasks that need to share a resource (like a UART or sensor).

**Steps**:
1. Create a mutex to protect the shared resource
2. Have both tasks try to use the resource
3. Observe how the mutex prevents conflicts
4. Measure the impact on timing

**Expected Outcome**: Understanding why synchronization is necessary and how to implement it.

---

## ✅ **Check Yourself**

### **Understanding Check**
- [ ] Can you explain why real-time doesn't mean "fast"?
- [ ] Do you understand the difference between bare metal and RTOS approaches?
- [ ] Can you explain what a task is and how it differs from a function?
- [ ] Do you understand why tasks need priorities?
- [ ] Can you explain what a mutex is and when to use it?

### **Application Check**
- [ ] Can you create a simple task that runs periodically?
- [ ] Do you know how to set task priorities appropriately?
- [ ] Can you implement basic synchronization between tasks?
- [ ] Do you understand how to configure FreeRTOS for your needs?
- [ ] Can you debug basic task scheduling issues?

### **Analysis Check**
- [ ] Can you analyze when to use FreeRTOS vs bare metal?
- [ ] Do you understand the trade-offs of different configuration options?
- [ ] Can you identify potential priority inversion issues?
- [ ] Do you know how to measure and optimize task performance?
- [ ] Can you design a multi-task system architecture?

---

## 🔗 **Cross-links**

### **Related Topics**
- **[Task Creation and Management](./Task_Creation_Management.md)**: Deep dive into task management
- **[Scheduling Algorithms](./Scheduling_Algorithms.md)**: Understanding how FreeRTOS decides what runs when
- **[Interrupt Handling](./Interrupt_Handling.md)**: How FreeRTOS works with hardware interrupts
- **[Memory Management](./../Embedded_C/Memory_Management.md)**: Understanding memory allocation in RTOS

### **Further Reading**
- **FreeRTOS User Manual**: Official documentation and API reference
- **Real-Time Systems Design**: Understanding real-time principles
- **Embedded Systems Programming**: Practical embedded development
- **RTOS Performance Analysis**: Measuring and optimizing RTOS performance

### **Industry Standards**
- **POSIX Real-Time Extensions**: Standard real-time programming interfaces
- **OSEK/VDX**: Automotive real-time operating system standard
- **ARINC 653**: Avionics real-time operating system standard
- **IEC 61508**: Functional safety of electrical/electronic systems


