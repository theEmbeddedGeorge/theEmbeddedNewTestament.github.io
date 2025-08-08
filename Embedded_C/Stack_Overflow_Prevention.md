# Stack Overflow Prevention

## 📋 Table of Contents
- [Overview](#-overview)
- [Stack Memory Basics](#-stack-memory-basics)
- [Stack Overflow Causes](#-stack-overflow-causes)
- [Stack Size Analysis](#-stack-size-analysis)
- [Static Analysis Tools](#-static-analysis-tools)
- [Dynamic Stack Monitoring](#-dynamic-stack-monitoring)
- [Prevention Techniques](#-prevention-techniques)
- [Real-time Stack Protection](#-real-time-stack-protection)
- [Common Pitfalls](#-common-pitfalls)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

Stack overflow occurs when the call stack exceeds its allocated memory space. In embedded systems, this can cause unpredictable behavior, system crashes, or security vulnerabilities. This guide covers techniques to prevent and detect stack overflow conditions.

## 🔧 Stack Memory Basics

### Stack Layout
```c
// Typical stack layout in embedded systems
typedef struct {
    uint8_t stack[STACK_SIZE];     // Stack memory
    uint8_t guard_zone[GUARD_SIZE]; // Stack guard zone
    size_t stack_pointer;           // Current stack pointer
    size_t max_usage;               // Maximum stack usage
} stack_monitor_t;

#define STACK_SIZE 2048
#define GUARD_SIZE 64
```

### Stack Frame Analysis
```c
// Analyze stack frame size
void analyze_stack_frame() {
    int local_var1 = 10;           // 4 bytes
    char local_var2[100];          // 100 bytes
    double local_var3 = 3.14;      // 8 bytes
    // Total: 112 bytes + alignment padding
    
    printf("Estimated stack frame size: ~120 bytes\n");
}

// Recursive function with stack analysis
int recursive_function(int n) {
    int local_var = n * 2;         // 4 bytes per call
    char buffer[50];               // 50 bytes per call
    
    if (n <= 0) return 0;
    
    // Stack usage: ~60 bytes per recursive call
    // Maximum depth: STACK_SIZE / 60 ≈ 34 calls
    return recursive_function(n - 1) + local_var;
}
```

## 🚨 Stack Overflow Causes

### 1. Deep Recursion
```c
// WRONG: Unbounded recursion
int factorial_recursive(int n) {
    if (n <= 1) return 1;
    return n * factorial_recursive(n - 1);  // Stack overflow for large n
}

// CORRECT: Tail recursion or iteration
int factorial_iterative(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// CORRECT: Tail recursion (compiler can optimize)
int factorial_tail_recursive(int n, int acc) {
    if (n <= 1) return acc;
    return factorial_tail_recursive(n - 1, n * acc);
}
```

### 2. Large Local Arrays
```c
// WRONG: Large stack allocation
void large_stack_array() {
    char buffer[10000];  // 10KB on stack - may overflow
    // Use buffer...
}

// CORRECT: Use heap or static allocation
void safe_large_buffer() {
    char* buffer = malloc(10000);  // Heap allocation
    if (buffer) {
        // Use buffer...
        free(buffer);
    }
}

// CORRECT: Static allocation
void static_large_buffer() {
    static char buffer[10000];  // Static allocation
    // Use buffer...
}
```

### 3. Function Call Chains
```c
// WRONG: Deep call chain
void function_a() { function_b(); }
void function_b() { function_c(); }
void function_c() { function_d(); }
// ... many more levels
void function_z() { 
    char buffer[1000];  // Large stack usage at deep level
    // Use buffer...
}

// CORRECT: Flatten call chain
void flattened_function() {
    // Combine logic to reduce call depth
    char buffer[1000];
    // All logic in one function
}
```

## 📊 Stack Size Analysis

### Static Stack Analysis
```c
// Calculate stack usage statically
typedef struct {
    const char* function_name;
    size_t stack_usage;
    size_t max_depth;
} stack_analysis_t;

stack_analysis_t analyze_function_stack(const char* func_name) {
    stack_analysis_t analysis = {0};
    analysis.function_name = func_name;
    
    // This would be done by static analysis tools
    // For demonstration, we'll estimate
    if (strcmp(func_name, "main") == 0) {
        analysis.stack_usage = 256;  // Estimated stack usage
        analysis.max_depth = 1;
    } else if (strcmp(func_name, "recursive_function") == 0) {
        analysis.stack_usage = 60;   // Per call
        analysis.max_depth = 30;     // Maximum safe depth
    }
    
    return analysis;
}
```

### Dynamic Stack Monitoring
```c
// Monitor stack usage at runtime
typedef struct {
    void* stack_start;
    void* stack_end;
    size_t max_usage;
    size_t current_usage;
} stack_monitor_t;

stack_monitor_t* create_stack_monitor(void* stack_start, size_t stack_size) {
    stack_monitor_t* monitor = malloc(sizeof(stack_monitor_t));
    if (!monitor) return NULL;
    
    monitor->stack_start = stack_start;
    monitor->stack_end = (char*)stack_start + stack_size;
    monitor->max_usage = 0;
    monitor->current_usage = 0;
    
    return monitor;
}

size_t get_stack_usage(stack_monitor_t* monitor) {
    void* current_sp;
    asm volatile ("mov %0, sp" : "=r" (current_sp));
    
    size_t usage = (char*)monitor->stack_end - (char*)current_sp;
    
    if (usage > monitor->max_usage) {
        monitor->max_usage = usage;
    }
    
    monitor->current_usage = usage;
    return usage;
}

void check_stack_overflow(stack_monitor_t* monitor) {
    size_t usage = get_stack_usage(monitor);
    size_t total_size = (char*)monitor->stack_end - (char*)monitor->stack_start;
    
    if (usage > total_size * 0.8) {  // 80% threshold
        printf("WARNING: High stack usage: %zu/%zu bytes (%.1f%%)\n",
               usage, total_size, (float)usage / total_size * 100);
    }
    
    if (usage >= total_size) {
        printf("ERROR: Stack overflow detected!\n");
        // Handle overflow - system reset, error handling, etc.
    }
}
```

## 🔍 Static Analysis Tools

### Compiler Stack Analysis
```c
// GCC stack usage analysis
// Compile with: gcc -fstack-usage -Wstack-usage=1024

void function_with_stack_analysis() {
    char buffer[500];  // Compiler will warn if stack usage > 1024
    // Use buffer...
}

// Stack usage report from compiler:
// function_with_stack_analysis: 500 dynamic, 0 static
```

### Custom Stack Analyzer
```c
// Simple static analysis for stack usage
typedef struct {
    const char* function;
    size_t estimated_usage;
    bool has_recursion;
    int max_depth;
} stack_analyzer_t;

stack_analyzer_t analyze_stack_pattern(const char* code) {
    stack_analyzer_t analysis = {0};
    
    // This would parse the code and analyze:
    // - Local variable sizes
    // - Array allocations
    // - Recursive calls
    // - Function call depth
    
    return analysis;
}
```

## 🔍 Dynamic Stack Monitoring

### Stack Canary Protection
```c
// Stack canary for overflow detection
typedef struct {
    uint32_t canary;
    char data[100];
    uint32_t canary_end;
} protected_stack_frame_t;

void function_with_canary() {
    protected_stack_frame_t frame = {
        .canary = 0xDEADBEEF,
        .canary_end = 0xDEADBEEF
    };
    
    // Use frame.data...
    
    // Check canaries
    if (frame.canary != 0xDEADBEEF || frame.canary_end != 0xDEADBEEF) {
        printf("STACK OVERFLOW DETECTED!\n");
        // Handle overflow
    }
}
```

### Stack Guard Zone
```c
// Stack guard zone monitoring
#define STACK_GUARD_SIZE 64
#define STACK_GUARD_PATTERN 0xAA

typedef struct {
    uint8_t guard_zone[STACK_GUARD_SIZE];
    uint8_t stack_data[STACK_SIZE];
} guarded_stack_t;

guarded_stack_t* create_guarded_stack() {
    guarded_stack_t* stack = malloc(sizeof(guarded_stack_t));
    if (!stack) return NULL;
    
    // Initialize guard zone
    for (int i = 0; i < STACK_GUARD_SIZE; i++) {
        stack->guard_zone[i] = STACK_GUARD_PATTERN;
    }
    
    return stack;
}

bool check_guard_zone(guarded_stack_t* stack) {
    for (int i = 0; i < STACK_GUARD_SIZE; i++) {
        if (stack->guard_zone[i] != STACK_GUARD_PATTERN) {
            printf("GUARD ZONE CORRUPTED - STACK OVERFLOW!\n");
            return false;
        }
    }
    return true;
}
```

## 🛡️ Prevention Techniques

### 1. Stack Size Configuration
```c
// Configure stack size based on analysis
#define MAIN_STACK_SIZE 2048
#define TASK_STACK_SIZE 1024
#define ISR_STACK_SIZE 512

typedef struct {
    uint8_t stack[MAIN_STACK_SIZE];
    size_t max_usage;
} main_stack_t;

// Stack size validation
bool validate_stack_size(size_t required_size, size_t available_size) {
    if (required_size > available_size * 0.8) {  // 80% safety margin
        printf("WARNING: Stack size may be insufficient\n");
        printf("Required: %zu, Available: %zu\n", required_size, available_size);
        return false;
    }
    return true;
}
```

### 2. Recursion Limits
```c
// Safe recursive function with depth limit
#define MAX_RECURSION_DEPTH 100

int safe_recursive_function(int n, int depth) {
    if (depth >= MAX_RECURSION_DEPTH) {
        printf("ERROR: Maximum recursion depth exceeded\n");
        return -1;  // Error condition
    }
    
    if (n <= 1) return 1;
    
    return n * safe_recursive_function(n - 1, depth + 1);
}

// Usage
int result = safe_recursive_function(10, 0);
```

### 3. Dynamic Memory for Large Data
```c
// Use heap for large data structures
typedef struct {
    size_t size;
    void* data;
} dynamic_buffer_t;

dynamic_buffer_t* create_dynamic_buffer(size_t size) {
    dynamic_buffer_t* buffer = malloc(sizeof(dynamic_buffer_t));
    if (!buffer) return NULL;
    
    buffer->size = size;
    buffer->data = malloc(size);
    
    if (!buffer->data) {
        free(buffer);
        return NULL;
    }
    
    return buffer;
}

void destroy_dynamic_buffer(dynamic_buffer_t* buffer) {
    if (buffer) {
        free(buffer->data);
        free(buffer);
    }
}
```

### 4. Function Inlining
```c
// Inline small functions to reduce call stack
static inline int small_function(int a, int b) {
    return a + b;  // Inlined, no stack frame
}

// Use inline for frequently called small functions
static inline void update_counter(uint32_t* counter) {
    (*counter)++;
}
```

## ⏱️ Real-time Stack Protection

### Stack Overflow Handler
```c
// Stack overflow exception handler
void stack_overflow_handler(void) {
    printf("STACK OVERFLOW EXCEPTION!\n");
    
    // Save critical data
    save_critical_data();
    
    // Reset system or restart task
    system_reset();
}

// Install overflow handler
void install_stack_overflow_handler(void) {
    // Set up exception handler for stack overflow
    // Implementation depends on architecture
}
```

### Task Stack Monitoring
```c
// Monitor stack usage in RTOS tasks
typedef struct {
    void* stack_start;
    size_t stack_size;
    size_t min_free_space;
    const char* task_name;
} task_stack_monitor_t;

void monitor_task_stack(task_stack_monitor_t* monitor) {
    void* current_sp;
    asm volatile ("mov %0, sp" : "=r" (current_sp));
    
    size_t used_space = (char*)monitor->stack_start + monitor->stack_size - (char*)current_sp;
    size_t free_space = monitor->stack_size - used_space;
    
    if (free_space < monitor->min_free_space) {
        printf("WARNING: Task '%s' low on stack space: %zu bytes free\n",
               monitor->task_name, free_space);
    }
}
```

### Stack Usage Profiling
```c
// Profile stack usage over time
typedef struct {
    size_t peak_usage;
    size_t current_usage;
    uint32_t sample_count;
    float average_usage;
} stack_profile_t;

void update_stack_profile(stack_profile_t* profile, size_t current_usage) {
    profile->current_usage = current_usage;
    
    if (current_usage > profile->peak_usage) {
        profile->peak_usage = current_usage;
    }
    
    profile->sample_count++;
    profile->average_usage = ((profile->average_usage * (profile->sample_count - 1)) + 
                             current_usage) / profile->sample_count;
}
```

## ⚠️ Common Pitfalls

### 1. Underestimating Stack Usage
```c
// WRONG: Underestimated stack usage
void underestimated_function() {
    char buffer[100];      // 100 bytes
    int array[50];         // 200 bytes
    double values[10];     // 80 bytes
    // Total: ~400 bytes, but compiler may use more
    
    // Function calls add more stack usage
    recursive_function(5);  // Additional stack usage
}

// CORRECT: Conservative estimation
void conservative_function() {
    // Estimate stack usage conservatively
    // Include padding, alignment, function calls
    // Add safety margin
}
```

### 2. Ignoring Compiler Optimizations
```c
// WRONG: Assuming stack usage is predictable
void optimized_function() {
    char buffer[100];
    // Compiler may optimize away unused variables
    // Stack usage may be less than expected
}

// CORRECT: Use stack analysis tools
void analyzed_function() {
    char buffer[100];
    // Use tools to verify actual stack usage
    // Don't rely on manual calculations
}
```

### 3. Not Considering Interrupt Context
```c
// WRONG: Using large stack in ISR
void large_isr_function(void) {
    char buffer[1000];  // Large stack usage in ISR
    // ISRs have limited stack space
}

// CORRECT: Minimal stack usage in ISR
void minimal_isr_function(void) {
    // Use static variables or global buffers
    static char buffer[1000];  // Static allocation
    // Or use heap allocation if necessary
}
```

## ✅ Best Practices

### 1. Stack Size Planning
```c
// Plan stack sizes based on analysis
#define STACK_SIZE_PLANNING

#ifdef STACK_SIZE_PLANNING
    #define MAIN_STACK_SIZE 2048    // Based on analysis
    #define TASK1_STACK_SIZE 1024   // Task 1 requirements
    #define TASK2_STACK_SIZE 512    // Task 2 requirements
    #define ISR_STACK_SIZE 256      // ISR requirements
#endif

// Validate stack sizes at compile time
static_assert(MAIN_STACK_SIZE >= 2048, "Main stack too small");
static_assert(TASK1_STACK_SIZE >= 1024, "Task1 stack too small");
```

### 2. Stack Usage Monitoring
```c
// Monitor stack usage in development
#ifdef DEBUG
    #define MONITOR_STACK_USAGE() \
        do { \
            size_t usage = get_current_stack_usage(); \
            if (usage > STACK_SIZE * 0.8) { \
                printf("High stack usage: %zu bytes\n", usage); \
            } \
        } while(0)
#else
    #define MONITOR_STACK_USAGE() ((void)0)
#endif

void function_with_monitoring() {
    MONITOR_STACK_USAGE();
    // Function code...
    MONITOR_STACK_USAGE();
}
```

### 3. Stack Overflow Detection
```c
// Implement stack overflow detection
#define STACK_OVERFLOW_DETECTION

#ifdef STACK_OVERFLOW_DETECTION
    #define CHECK_STACK_OVERFLOW() \
        do { \
            if (is_stack_overflow()) { \
                handle_stack_overflow(); \
            } \
        } while(0)
#else
    #define CHECK_STACK_OVERFLOW() ((void)0)
#endif

bool is_stack_overflow(void) {
    // Implementation depends on architecture
    // Check stack pointer bounds
    return false;  // Placeholder
}

void handle_stack_overflow(void) {
    printf("STACK OVERFLOW DETECTED!\n");
    // Implement recovery mechanism
}
```

### 4. Stack Usage Documentation
```c
// Document stack usage for functions
/**
 * @brief Process data with known stack usage
 * @param data Input data
 * @return Processed result
 * 
 * Stack Usage: ~200 bytes
 * - Local variables: 150 bytes
 * - Function calls: 50 bytes
 * - Safety margin: 100 bytes
 * Total: 300 bytes recommended
 */
int process_data_with_documentation(int data) {
    char buffer[100];      // 100 bytes
    int temp[10];          // 40 bytes
    double result;         // 8 bytes
    // Additional overhead: ~50 bytes
    
    // Process data...
    return (int)result;
}
```

## 🎯 Interview Questions

### Basic Questions
1. **What is stack overflow and why is it dangerous in embedded systems?**
   - Stack overflow: call stack exceeds allocated memory
   - Dangerous: unpredictable behavior, system crashes, security vulnerabilities

2. **How can you prevent stack overflow?**
   - Limit recursion depth
   - Use heap for large allocations
   - Monitor stack usage
   - Configure appropriate stack sizes

3. **What tools can help detect stack overflow?**
   - Static analysis tools
   - Stack canaries
   - Runtime monitoring
   - Compiler warnings

### Advanced Questions
1. **How would you implement stack overflow detection in an embedded system?**
   - Stack canaries
   - Guard zones
   - Stack pointer bounds checking
   - Exception handlers

2. **What are the trade-offs between stack and heap allocation?**
   - Stack: faster, automatic cleanup, limited size
   - Heap: larger size, manual management, fragmentation risk

3. **How would you analyze stack usage in a real-time system?**
   - Static analysis for worst-case stack usage
   - Runtime monitoring for actual usage
   - Stack profiling tools

## 📚 Additional Resources

### Standards and Documentation
- **C Standard**: Stack behavior specification
- **ARM Architecture Reference**: Stack management
- **RTOS Documentation**: Task stack management

### Related Topics
- **[Memory Protection](./Memory_Protection.md)** - Memory safety mechanisms
- **[Memory Pool Allocation](./Memory_Pool_Allocation.md)** - Efficient memory management
- **[Cache-Aware Programming](./Cache_Aware_Programming.md)** - Memory access patterns
- **[Real-time Systems](./Real_Time_Systems.md)** - RTOS stack management

### Tools and Libraries
- **GCC Stack Usage Analysis**: `-fstack-usage`
- **Valgrind**: Stack overflow detection
- **Custom stack monitors**: Embedded-specific solutions

---

**Next Topic:** [Memory Protection](./Memory_Protection.md) → [Cache-Aware Programming](./Cache_Aware_Programming.md)
