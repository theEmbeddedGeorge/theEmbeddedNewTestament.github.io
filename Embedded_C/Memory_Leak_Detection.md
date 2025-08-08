# Memory Leak Detection

## 📋 Table of Contents
- [Overview](#-overview)
- [Types of Memory Leaks](#-types-of-memory-leaks)
- [Static Analysis Tools](#-static-analysis-tools)
- [Dynamic Analysis Tools](#-dynamic-analysis-tools)
- [Custom Memory Tracking](#-custom-memory-tracking)
- [Embedded-Specific Detection](#-embedded-specific-detection)
- [Real-time Leak Detection](#-real-time-leak-detection)
- [Common Leak Patterns](#-common-leak-patterns)
- [Prevention Strategies](#-prevention-strategies)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

Memory leaks occur when allocated memory is not properly deallocated, leading to gradual memory exhaustion. In embedded systems, memory leaks can be catastrophic due to limited resources and long uptimes.

## 🔍 Types of Memory Leaks

### Classic Memory Leaks
```c
// Classic memory leak - allocated but never freed
void classic_memory_leak() {
    void* ptr = malloc(1024);
    // Use ptr...
    // Forgot to call free(ptr) - LEAK!
}

// Memory leak in error path
void leak_in_error_path() {
    void* ptr1 = malloc(512);
    if (ptr1 == NULL) return;  // Early return without cleanup
    
    void* ptr2 = malloc(256);
    if (ptr2 == NULL) {
        // Forgot to free ptr1 - LEAK!
        return;
    }
    
    // Use both pointers...
    free(ptr1);
    free(ptr2);
}
```

### Resource Leaks
```c
// File handle leak
void file_handle_leak() {
    FILE* file = fopen("data.txt", "r");
    if (file == NULL) return;
    
    // Process file...
    // Forgot to call fclose(file) - RESOURCE LEAK!
}

// Mutex leak
#include <pthread.h>

void mutex_leak() {
    pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mutex, NULL);
    
    // Use mutex...
    // Forgot to call pthread_mutex_destroy() and free() - LEAK!
}
```

### Circular Reference Leaks
```c
typedef struct node {
    int data;
    struct node* next;
} node_t;

void circular_reference_leak() {
    node_t* head = malloc(sizeof(node_t));
    node_t* tail = malloc(sizeof(node_t));
    
    head->next = tail;
    tail->next = head;  // Circular reference
    
    // If we only free head, tail becomes unreachable
    free(head);
    // tail is now leaked!
}
```

## 🔧 Static Analysis Tools

### Compiler Warnings
```c
// Enable all warnings for memory leak detection
// gcc -Wall -Wextra -Werror -fsanitize=address

void potential_leak_function() {
    void* ptr = malloc(100);
    // Compiler warning: variable 'ptr' set but not used
    // This could indicate a leak
}
```

### Static Analysis with Clang
```c
// clang --analyze -Xanalyzer -analyzer-output=text

void analyzed_function() {
    void* ptr = malloc(100);
    if (some_condition()) {
        free(ptr);
        return;
    }
    // Static analyzer detects: ptr is not freed in this path
}
```

### Custom Static Analysis
```c
// Simple static analysis for common patterns
#include <regex.h>

void check_for_malloc_without_free(const char* source_code) {
    regex_t malloc_regex, free_regex;
    regcomp(&malloc_regex, "malloc\\s*\\(", REG_EXTENDED);
    regcomp(&free_regex, "free\\s*\\(", REG_EXTENDED);
    
    // Count malloc and free calls
    // If malloc_count > free_count, potential leak
}
```

## 🔍 Dynamic Analysis Tools

### AddressSanitizer (ASan)
```c
// Compile with: gcc -fsanitize=address -g

void asan_demo() {
    void* ptr = malloc(100);
    // Use ptr...
    // If we forget to free(ptr), ASan will report leak
    
    // ASan also detects:
    // - Buffer overflows
    // - Use-after-free
    // - Double-free
}

// ASan output example:
// ==12345==ERROR: LeakSanitizer: detected memory leaks
// ==12345==SUMMARY: AddressSanitizer: 1 byte(s) leaked in 1 allocation(s)
```

### Valgrind
```c
// Run with: valgrind --leak-check=full --show-leak-kinds=all

void valgrind_demo() {
    void* ptr = malloc(100);
    // Use ptr...
    // Valgrind will report:
    // ==12345== 100 bytes in 1 blocks are definitely lost
    // ==12345==    at 0x4C2AB80: malloc (in /usr/lib/valgrind/vgpreload_memcheck.so)
    // ==12345==    by 0x400544: valgrind_demo (demo.c:5)
}
```

### Custom Memory Tracker
```c
typedef struct {
    void* ptr;
    size_t size;
    const char* file;
    int line;
    bool freed;
} allocation_record_t;

typedef struct {
    allocation_record_t* records;
    size_t count;
    size_t capacity;
} memory_tracker_t;

memory_tracker_t* create_memory_tracker(size_t initial_capacity) {
    memory_tracker_t* tracker = malloc(sizeof(memory_tracker_t));
    if (!tracker) return NULL;
    
    tracker->records = calloc(initial_capacity, sizeof(allocation_record_t));
    tracker->count = 0;
    tracker->capacity = initial_capacity;
    
    return tracker;
}

void* tracked_malloc(size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    if (ptr) {
        // Add to tracker
        allocation_record_t record = {
            .ptr = ptr,
            .size = size,
            .file = file,
            .line = line,
            .freed = false
        };
        add_allocation_record(&record);
    }
    return ptr;
}

void tracked_free(void* ptr) {
    if (ptr) {
        // Mark as freed in tracker
        mark_as_freed(ptr);
        free(ptr);
    }
}
```

## 🔧 Embedded-Specific Detection

### Memory Pool Leak Detection
```c
typedef struct {
    void* pool;
    size_t block_size;
    size_t total_blocks;
    bool* allocated_blocks;
    size_t allocated_count;
} leak_detecting_pool_t;

leak_detecting_pool_t* create_leak_detecting_pool(size_t block_size, 
                                                  size_t num_blocks) {
    leak_detecting_pool_t* pool = malloc(sizeof(leak_detecting_pool_t));
    if (!pool) return NULL;
    
    pool->block_size = block_size;
    pool->total_blocks = num_blocks;
    pool->allocated_count = 0;
    
    pool->pool = malloc(block_size * num_blocks);
    pool->allocated_blocks = calloc(num_blocks, sizeof(bool));
    
    return pool;
}

void* pool_allocate_with_tracking(leak_detecting_pool_t* pool) {
    for (size_t i = 0; i < pool->total_blocks; i++) {
        if (!pool->allocated_blocks[i]) {
            pool->allocated_blocks[i] = true;
            pool->allocated_count++;
            return (char*)pool->pool + (i * pool->block_size);
        }
    }
    return NULL;
}

void pool_free_with_tracking(leak_detecting_pool_t* pool, void* ptr) {
    size_t block_index = ((char*)ptr - (char*)pool->pool) / pool->block_size;
    if (block_index < pool->total_blocks && pool->allocated_blocks[block_index]) {
        pool->allocated_blocks[block_index] = false;
        pool->allocated_count--;
    }
}

void report_pool_leaks(leak_detecting_pool_t* pool) {
    if (pool->allocated_count > 0) {
        printf("MEMORY LEAK: %zu blocks not freed in pool\n", 
               pool->allocated_count);
        
        for (size_t i = 0; i < pool->total_blocks; i++) {
            if (pool->allocated_blocks[i]) {
                printf("  Block %zu still allocated\n", i);
            }
        }
    }
}
```

### Stack-Based Memory Tracking
```c
// Track allocations on stack for embedded systems
#define MAX_STACK_ALLOCATIONS 100

typedef struct {
    void* ptr;
    const char* function;
    int line;
} stack_allocation_t;

static stack_allocation_t allocation_stack[MAX_STACK_ALLOCATIONS];
static int stack_top = 0;

void* stack_tracked_malloc(size_t size, const char* function, int line) {
    void* ptr = malloc(size);
    if (ptr && stack_top < MAX_STACK_ALLOCATIONS) {
        allocation_stack[stack_top].ptr = ptr;
        allocation_stack[stack_top].function = function;
        allocation_stack[stack_top].line = line;
        stack_top++;
    }
    return ptr;
}

void stack_tracked_free(void* ptr) {
    if (ptr) {
        // Find and remove from stack
        for (int i = 0; i < stack_top; i++) {
            if (allocation_stack[i].ptr == ptr) {
                // Remove by shifting
                for (int j = i; j < stack_top - 1; j++) {
                    allocation_stack[j] = allocation_stack[j + 1];
                }
                stack_top--;
                break;
            }
        }
        free(ptr);
    }
}

void check_stack_leaks() {
    if (stack_top > 0) {
        printf("STACK LEAKS DETECTED: %d allocations not freed\n", stack_top);
        for (int i = 0; i < stack_top; i++) {
            printf("  %s:%d - %p\n", 
                   allocation_stack[i].function,
                   allocation_stack[i].line,
                   allocation_stack[i].ptr);
        }
    }
}
```

## ⏱️ Real-time Leak Detection

### Lightweight Leak Monitor
```c
typedef struct {
    uint32_t total_allocations;
    uint32_t total_frees;
    uint32_t current_allocated;
    uint32_t peak_allocated;
} rt_leak_monitor_t;

static rt_leak_monitor_t leak_monitor = {0};

void* rt_tracked_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr) {
        leak_monitor.total_allocations++;
        leak_monitor.current_allocated++;
        
        if (leak_monitor.current_allocated > leak_monitor.peak_allocated) {
            leak_monitor.peak_allocated = leak_monitor.current_allocated;
        }
    }
    return ptr;
}

void rt_tracked_free(void* ptr) {
    if (ptr) {
        leak_monitor.total_frees++;
        leak_monitor.current_allocated--;
        free(ptr);
    }
}

void report_rt_leaks() {
    printf("Allocations: %u, Frees: %u, Current: %u, Peak: %u\n",
           leak_monitor.total_allocations,
           leak_monitor.total_frees,
           leak_monitor.current_allocated,
           leak_monitor.peak_allocated);
    
    if (leak_monitor.current_allocated > 0) {
        printf("POTENTIAL LEAK: %u blocks not freed\n", 
               leak_monitor.current_allocated);
    }
}
```

### Periodic Leak Check
```c
// Check for leaks periodically in real-time systems
void periodic_leak_check() {
    static uint32_t check_counter = 0;
    check_counter++;
    
    if (check_counter % 10000 == 0) {  // Check every 10k allocations
        uint32_t leak_count = leak_monitor.total_allocations - leak_monitor.total_frees;
        
        if (leak_count > 100) {  // Threshold for leak detection
            printf("WARNING: Potential memory leak detected\n");
            report_rt_leaks();
        }
    }
}
```

## 🚨 Common Leak Patterns

### 1. Exception/Error Path Leaks
```c
// WRONG: Leak in error path
void* allocate_with_error_leak() {
    void* ptr1 = malloc(100);
    if (!ptr1) return NULL;
    
    void* ptr2 = malloc(200);
    if (!ptr2) {
        // LEAK: ptr1 not freed
        return NULL;
    }
    
    // Use both pointers...
    free(ptr1);
    free(ptr2);
    return ptr2;
}

// CORRECT: Proper cleanup
void* allocate_with_cleanup() {
    void* ptr1 = malloc(100);
    if (!ptr1) return NULL;
    
    void* ptr2 = malloc(200);
    if (!ptr2) {
        free(ptr1);  // Clean up before returning
        return NULL;
    }
    
    // Use both pointers...
    free(ptr1);
    free(ptr2);
    return ptr2;
}
```

### 2. Loop Leaks
```c
// WRONG: Leak in loop
void loop_leak() {
    for (int i = 0; i < 1000; i++) {
        void* ptr = malloc(100);
        // Use ptr...
        // Forgot to free(ptr) - LEAK!
    }
}

// CORRECT: Free in loop
void loop_no_leak() {
    for (int i = 0; i < 1000; i++) {
        void* ptr = malloc(100);
        // Use ptr...
        free(ptr);  // Free before next iteration
    }
}
```

### 3. Function Exit Leaks
```c
// WRONG: Multiple exit points without cleanup
void* function_with_exit_leaks() {
    void* ptr = malloc(100);
    
    if (condition1()) {
        return ptr;  // LEAK: ptr not freed
    }
    
    if (condition2()) {
        return NULL;  // LEAK: ptr not freed
    }
    
    // Use ptr...
    free(ptr);
    return NULL;
}

// CORRECT: Single exit point or proper cleanup
void* function_with_cleanup() {
    void* ptr = malloc(100);
    if (!ptr) return NULL;
    
    void* result = NULL;
    
    if (condition1()) {
        result = ptr;
    } else if (condition2()) {
        free(ptr);
        result = NULL;
    } else {
        // Use ptr...
        free(ptr);
        result = NULL;
    }
    
    return result;
}
```

## 🛡️ Prevention Strategies

### 1. RAII Pattern in C
```c
typedef struct {
    void* ptr;
    void (*cleanup)(void*);
} raii_wrapper_t;

raii_wrapper_t* create_raii_wrapper(void* ptr, void (*cleanup)(void*)) {
    raii_wrapper_t* wrapper = malloc(sizeof(raii_wrapper_t));
    if (wrapper) {
        wrapper->ptr = ptr;
        wrapper->cleanup = cleanup;
    }
    return wrapper;
}

void destroy_raii_wrapper(raii_wrapper_t* wrapper) {
    if (wrapper && wrapper->cleanup) {
        wrapper->cleanup(wrapper->ptr);
    }
    free(wrapper);
}

// Usage
void example_raii() {
    raii_wrapper_t* wrapper = create_raii_wrapper(
        malloc(100), 
        free
    );
    
    // Use wrapper->ptr...
    
    destroy_raii_wrapper(wrapper);  // Automatically frees ptr
}
```

### 2. Memory Pool with Automatic Cleanup
```c
typedef struct {
    void* pool;
    size_t block_size;
    size_t total_blocks;
    bool* allocated_blocks;
} auto_cleanup_pool_t;

auto_cleanup_pool_t* create_auto_cleanup_pool(size_t block_size, 
                                              size_t num_blocks) {
    auto_cleanup_pool_t* pool = malloc(sizeof(auto_cleanup_pool_t));
    if (!pool) return NULL;
    
    pool->block_size = block_size;
    pool->total_blocks = num_blocks;
    pool->pool = malloc(block_size * num_blocks);
    pool->allocated_blocks = calloc(num_blocks, sizeof(bool));
    
    return pool;
}

void destroy_auto_cleanup_pool(auto_cleanup_pool_t* pool) {
    if (pool) {
        // Check for leaks before destroying
        size_t leak_count = 0;
        for (size_t i = 0; i < pool->total_blocks; i++) {
            if (pool->allocated_blocks[i]) {
                leak_count++;
            }
        }
        
        if (leak_count > 0) {
            printf("WARNING: %zu blocks leaked in pool\n", leak_count);
        }
        
        free(pool->pool);
        free(pool->allocated_blocks);
        free(pool);
    }
}
```

### 3. Smart Pointer Pattern
```c
typedef struct {
    void* ptr;
    int ref_count;
} smart_ptr_t;

smart_ptr_t* create_smart_ptr(void* ptr) {
    smart_ptr_t* smart = malloc(sizeof(smart_ptr_t));
    if (smart) {
        smart->ptr = ptr;
        smart->ref_count = 1;
    }
    return smart;
}

smart_ptr_t* smart_ptr_ref(smart_ptr_t* smart) {
    if (smart) {
        smart->ref_count++;
    }
    return smart;
}

void smart_ptr_unref(smart_ptr_t* smart) {
    if (smart) {
        smart->ref_count--;
        if (smart->ref_count <= 0) {
            free(smart->ptr);
            free(smart);
        }
    }
}
```

## ✅ Best Practices

### 1. Use Consistent Allocation/Deallocation Patterns
```c
// Define allocation patterns
#define ALLOCATE_AND_CHECK(ptr, size) \
    do { \
        ptr = malloc(size); \
        if (!ptr) { \
            fprintf(stderr, "Allocation failed at %s:%d\n", __FILE__, __LINE__); \
            return NULL; \
        } \
    } while(0)

#define FREE_AND_NULL(ptr) \
    do { \
        if (ptr) { \
            free(ptr); \
            ptr = NULL; \
        } \
    } while(0)

// Usage
void* safe_allocation_example() {
    void* ptr1 = NULL, *ptr2 = NULL;
    
    ALLOCATE_AND_CHECK(ptr1, 100);
    ALLOCATE_AND_CHECK(ptr2, 200);
    
    // Use pointers...
    
    FREE_AND_NULL(ptr2);
    FREE_AND_NULL(ptr1);
    return NULL;
}
```

### 2. Implement Memory Leak Detection in Debug Builds
```c
#ifdef DEBUG
    #define MALLOC(size) debug_malloc(size, __FILE__, __LINE__)
    #define FREE(ptr) debug_free(ptr, __FILE__, __LINE__)
#else
    #define MALLOC(size) malloc(size)
    #define FREE(ptr) free(ptr)
#endif

void* debug_malloc(size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    if (ptr) {
        printf("DEBUG: malloc(%zu) at %s:%d -> %p\n", size, file, line, ptr);
        add_to_debug_tracker(ptr, size, file, line);
    }
    return ptr;
}

void debug_free(void* ptr, const char* file, int line) {
    if (ptr) {
        printf("DEBUG: free(%p) at %s:%d\n", ptr, file, line);
        remove_from_debug_tracker(ptr);
        free(ptr);
    }
}
```

### 3. Regular Memory Audits
```c
void perform_memory_audit() {
    struct mallinfo info = mallinfo();
    
    printf("Memory Audit:\n");
    printf("  Total allocated: %d bytes\n", info.uordblks);
    printf("  Total free: %d bytes\n", info.fordblks);
    printf("  Largest free block: %d bytes\n", info.mxordblk);
    printf("  Number of free blocks: %d\n", info.ordblks);
    
    // Check for potential leaks
    if (info.uordblks > 0 && info.fordblks < 1000) {
        printf("WARNING: Low free memory - potential leak?\n");
    }
}
```

## 🎯 Interview Questions

### Basic Questions
1. **What is a memory leak and why is it problematic in embedded systems?**
   - Memory leak: allocated memory not properly freed
   - Problematic in embedded: limited resources, long uptimes, no virtual memory

2. **How would you detect memory leaks?**
   - Static analysis tools (compiler warnings, static analyzers)
   - Dynamic analysis tools (Valgrind, AddressSanitizer)
   - Custom memory tracking

3. **What are common causes of memory leaks?**
   - Forgetting to free allocated memory
   - Error paths without cleanup
   - Circular references
   - Exception handling without cleanup

### Advanced Questions
1. **How would you implement a memory leak detector for embedded systems?**
   - Lightweight tracking with minimal overhead
   - Stack-based allocation tracking
   - Periodic leak checks

2. **What strategies can prevent memory leaks?**
   - RAII patterns
   - Smart pointers
   - Memory pools
   - Consistent allocation/deallocation patterns

3. **How would you handle memory leaks in real-time systems?**
   - Pre-allocate memory pools
   - Use static allocation where possible
   - Implement lightweight leak detection

## 📚 Additional Resources

### Standards and Documentation
- **C Standard Library**: Memory management functions
- **Valgrind Documentation**: Memory error detection
- **AddressSanitizer**: Runtime memory error detector

### Related Topics
- **[Memory Pool Allocation](./Memory_Pool_Allocation.md)** - Efficient memory management
- **[Memory Fragmentation](./Memory_Fragmentation.md)** - Memory fragmentation issues
- **[Stack Overflow Prevention](./Stack_Overflow_Prevention.md)** - Stack management
- **[Memory Protection](./Memory_Protection.md)** - Memory safety

### Tools and Libraries
- **Valgrind**: Comprehensive memory analysis
- **AddressSanitizer**: Fast memory error detection
- **Custom memory trackers**: Embedded-specific solutions

---

**Next Topic:** [Stack Overflow Prevention](./Stack_Overflow_Prevention.md) → [Memory Protection](./Memory_Protection.md)
