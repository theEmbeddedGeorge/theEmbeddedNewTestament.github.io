# Memory Management in Embedded Systems

## 📋 Table of Contents
- [Overview](#-overview)
- [Memory Types](#-memory-types)
- [Stack vs Heap](#-stack-vs-heap)
- [Memory Allocation](#-memory-allocation)
- [Memory Deallocation](#-memory-deallocation)
- [Memory Safety](#-memory-safety)
- [Common Pitfalls](#-common-pitfalls)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

Memory management is critical in embedded systems where resources are limited and reliability is paramount. Understanding how memory is allocated, used, and freed is essential for writing efficient and safe embedded code.

### Key Concepts for Embedded Development
- **Deterministic allocation** - Predictable memory usage patterns
- **Memory safety** - Preventing buffer overflows and memory leaks
- **Resource constraints** - Working within limited RAM
- **Real-time requirements** - Avoiding dynamic allocation in critical paths

## 🔢 Memory Types

### Static Memory
```c
// Global variables - allocated at compile time
static uint8_t global_buffer[1024];
static const char* const_string = "Hello World";

// Static local variables - persist between function calls
void function() {
    static int counter = 0;  // Initialized once, persists
    counter++;
}
```

### Stack Memory
```c
void stack_example() {
    int local_var = 42;           // Stack allocated
    uint8_t buffer[256];          // Stack array
    struct sensor_data data;       // Stack structure
    
    // Stack memory is automatically freed when function returns
}
```

### Heap Memory
```c
#include <stdlib.h>

void heap_example() {
    // Dynamic allocation
    uint8_t* buffer = malloc(1024);
    if (buffer == NULL) {
        // Handle allocation failure
        return;
    }
    
    // Use buffer...
    
    // Must be explicitly freed
    free(buffer);
    buffer = NULL;  // Prevent use-after-free
}
```

## 🏗️ Stack vs Heap

### Stack Characteristics
```c
// Stack allocation is fast and deterministic
void stack_operations() {
    uint32_t stack_var = 0x12345678;
    uint8_t stack_array[64];
    
    // NOTE: In C, automatic (stack) variables are NOT automatically initialized.
    // Memory is automatically freed
    // No fragmentation issues
}
```

### Heap Characteristics
```c
// Heap allocation is flexible but requires management
void heap_operations() {
    // Allocate memory
    void* ptr1 = malloc(100);
    void* ptr2 = malloc(200);
    
    // Use memory...
    
    // Freeing order does not need to be reverse; fragmentation behavior
    // depends on the allocator. Prefer fixed-size pools to avoid fragmentation.
    free(ptr1);
    free(ptr2);
}
```

### Memory Layout
```c
// Typical embedded system memory layout
/*
High Address
    ┌─────────────────┐
    │     Stack       │ ← Grows downward
    ├─────────────────┤
    │     Heap        │ ← Grows upward
    ├─────────────────┤
    │     .bss        │ ← Uninitialized data
    ├─────────────────┤
    │     .data       │ ← Initialized data
    ├─────────────────┤
    │     .text       │ ← Code
    └─────────────────┘
Low Address
*/
```

## 📊 Memory Allocation

### Static Allocation
```c
// Pre-allocated buffers for embedded systems
#define BUFFER_SIZE 1024
#define MAX_SENSORS 8

// Static allocation - no runtime overhead
static uint8_t sensor_buffer[BUFFER_SIZE];
static struct sensor_data sensors[MAX_SENSORS];

// Memory pools for fixed-size allocations
#define POOL_SIZE 16
#define POOL_COUNT 32

typedef struct {
    uint8_t data[POOL_SIZE];
    uint8_t used;
} memory_pool_t;

static memory_pool_t memory_pools[POOL_COUNT];
```

### Dynamic Allocation
```c
// Safe dynamic allocation with error checking
void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        // Log error or handle gracefully
        return NULL;
    }
    return ptr;
}

// Allocate with alignment
// NOTE: posix_memalign is POSIX-specific and often unavailable on bare-metal.
// Use it only on hosted POSIX targets. For bare-metal, prefer statically
// aligned storage or a custom pool.
#if defined(__unix__) || defined(__APPLE__)
void* aligned_malloc(size_t size, size_t alignment) {
    void* ptr = NULL;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        return NULL;
    }
    return ptr;
}
#endif

// Statically aligned buffer (portable approach for bare-metal)
#if defined(__GNUC__) || defined(__clang__)
__attribute__((aligned(32))) static uint8_t dma_buffer[1024];
#elif defined(_MSC_VER)
__declspec(align(32)) static uint8_t dma_buffer[1024];
#endif
```

### Memory Pool Implementation
```c
typedef struct {
    uint8_t* pool;
    size_t pool_size;
    size_t block_size;
    uint8_t* free_list;
    size_t free_count;
} mem_pool_t;

// Initialize memory pool
int mem_pool_init(mem_pool_t* pool, size_t block_size, size_t block_count) {
    pool->block_size = block_size;
    pool->pool_size = block_size * block_count;
    pool->pool = malloc(pool->pool_size);
    
    if (pool->pool == NULL) {
        return -1;
    }
    
    // Initialize free list
    pool->free_list = pool->pool;
    pool->free_count = block_count;
    
    // Link blocks in free list
    uint8_t* current = pool->pool;
    for (size_t i = 0; i < block_count - 1; i++) {
        *(uint8_t**)current = current + block_size;
        current += block_size;
    }
    *(uint8_t**)current = NULL;
    
    return 0;
}

// Allocate from pool
void* mem_pool_alloc(mem_pool_t* pool) {
    if (pool->free_count == 0) {
        return NULL;  // Pool exhausted
    }
    
    uint8_t* block = pool->free_list;
    pool->free_list = *(uint8_t**)block;
    pool->free_count--;
    
    return block;
}

// Free to pool
void mem_pool_free(mem_pool_t* pool, void* ptr) {
    if (ptr == NULL) return;
    
    // Add to free list
    *(uint8_t**)ptr = pool->free_list;
    pool->free_list = ptr;
    pool->free_count++;
}
```

## 🗑️ Memory Deallocation

### Safe Deallocation Patterns
```c
// Always check for NULL before freeing
void safe_free(void** ptr) {
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;  // Prevent use-after-free
    }
}

// Example usage
void cleanup_example() {
    uint8_t* buffer = malloc(1024);
    // Use buffer...
    
    safe_free((void**)&buffer);
    // buffer is now NULL
}
```

### Memory Leak Prevention
```c
// Track allocations in debug builds
#ifdef DEBUG
static size_t total_allocated = 0;
static size_t peak_allocated = 0;

void* debug_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr != NULL) {
        total_allocated += size;
        if (total_allocated > peak_allocated) {
            peak_allocated = total_allocated;
        }
    }
    return ptr;
}

void debug_free(void* ptr) {
    if (ptr != NULL) {
        // Note: This is simplified - real implementation would track sizes
        free(ptr);
    }
}
#endif
```

## 🛡️ Memory Safety

### Buffer Overflow Prevention
```c
// Safe string operations
void safe_strcpy(char* dest, const char* src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return;
    }
    
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';  // Always null-terminate
}

// Safe array access
#define SAFE_ARRAY_ACCESS(array, index, size) \
    ((index) < (size) ? &(array)[index] : NULL)

// Usage
void safe_array_example() {
    uint8_t buffer[64];
    uint8_t* element = SAFE_ARRAY_ACCESS(buffer, 32, 64);
    if (element != NULL) {
        *element = 42;
    }
}
```

### Memory Initialization
```c
// Initialize memory to known state
void secure_memset(void* ptr, int value, size_t size) {
    volatile uint8_t* p = (volatile uint8_t*)ptr;
    for (size_t i = 0; i < size; i++) {
        p[i] = (uint8_t)value;
    }
}

// Clear sensitive data
void clear_sensitive_data(uint8_t* data, size_t size) {
    secure_memset(data, 0, size);
}

// NOTE: Using a volatile write loop is a common technique to prevent the
// compiler from optimizing the clear, but the C standard does not fully
// guarantee it. Where available, prefer a conforming API such as memset_s
// (C11 Annex K, optional) or compiler-specific intrinsics/pragmas.
#if defined(__STDC_LIB_EXT1__)
void clear_sensitive_data_portable(uint8_t* data, rsize_t size) {
    memset_s(data, size, 0, size);
}
#endif

> Platform note: On freestanding/bare‑metal targets, `malloc`/`free` may be
> unavailable or undesirable in real-time paths. Favor static allocation and
> memory pools for predictability.
```

## ⚠️ Common Pitfalls

### Memory Leaks
```c
// BAD: Memory leak
void memory_leak_example() {
    uint8_t* buffer = malloc(1024);
    // Use buffer...
    // Forgot to free - memory leak!
}

// GOOD: Proper cleanup
void proper_cleanup_example() {
    uint8_t* buffer = malloc(1024);
    if (buffer == NULL) {
        return;
    }
    
    // Use buffer...
    
    free(buffer);
    buffer = NULL;
}
```

### Use-After-Free
```c
// BAD: Use after free
void use_after_free_example() {
    uint8_t* buffer = malloc(1024);
    free(buffer);
    buffer[0] = 42;  // Undefined behavior!
}

// GOOD: Set pointer to NULL after free
void safe_free_example() {
    uint8_t* buffer = malloc(1024);
    free(buffer);
    buffer = NULL;  // Prevents accidental use
}
```

### Stack Overflow
```c
// BAD: Large stack allocation
void stack_overflow_example() {
    uint8_t large_buffer[8192];  // May cause stack overflow
    // Use buffer...
}

// GOOD: Use heap for large allocations
void safe_large_allocation() {
    uint8_t* buffer = malloc(8192);
    if (buffer != NULL) {
        // Use buffer...
        free(buffer);
    }
}
```

## ✅ Best Practices

### Memory Management Guidelines
```c
// 1. Always check allocation success
void* ptr = malloc(size);
if (ptr == NULL) {
    // Handle allocation failure
    return ERROR_CODE;
}

// 2. Use const for read-only data
const uint8_t* read_only_data = get_sensor_data();

// 3. Initialize variables
uint8_t buffer[64] = {0};  // Zero-initialize

// 4. Use appropriate data types
uint8_t small_value = 42;      // 0-255
uint16_t medium_value = 1000;  // 0-65535
uint32_t large_value = 1000000; // 0-4294967295

// 5. Check bounds before access
if (index < array_size) {
    array[index] = value;
}
```

### Embedded-Specific Patterns
```c
// Memory pool for fixed-size allocations
#define SENSOR_DATA_SIZE 32
#define MAX_SENSORS 16

static uint8_t sensor_pool[SENSOR_DATA_SIZE * MAX_SENSORS];
static bool pool_used[MAX_SENSORS] = {false};

uint8_t* allocate_sensor_buffer() {
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (!pool_used[i]) {
            pool_used[i] = true;
            return &sensor_pool[i * SENSOR_DATA_SIZE];
        }
    }
    return NULL;  // No free slots
}

void free_sensor_buffer(uint8_t* buffer) {
    if (buffer == NULL) return;
    
    // Calculate index from pointer
    size_t index = (buffer - sensor_pool) / SENSOR_DATA_SIZE;
    if (index < MAX_SENSORS) {
        pool_used[index] = false;
    }
}
```

## 🎯 Interview Questions

### Basic Concepts
1. **What is the difference between stack and heap memory?**
   - Stack: Automatic allocation/deallocation, LIFO, limited size
   - Heap: Manual allocation/deallocation, flexible size, potential fragmentation

2. **How do you prevent memory leaks in embedded systems?**
   - Use static allocation when possible
   - Always free allocated memory
   - Use memory pools for fixed-size allocations
   - Set pointers to NULL after freeing

3. **What is memory fragmentation and how do you prevent it?**
   - Fragmentation occurs when free memory is scattered in small chunks
   - Prevention: Use memory pools, allocate similar-sized blocks together

### Advanced Topics
1. **How would you implement a memory pool for embedded systems?**
   - Pre-allocate fixed-size blocks
   - Maintain a free list
   - O(1) allocation and deallocation
   - No fragmentation

2. **What are the trade-offs between static and dynamic allocation?**
   - Static: Predictable, no runtime overhead, limited flexibility
   - Dynamic: Flexible, runtime overhead, potential fragmentation

3. **How do you handle memory allocation failures in embedded systems?**
   - Check return values
   - Implement graceful degradation
   - Use static allocation for critical components
   - Monitor memory usage

## 📚 Additional Resources

- **Books**: "Making Embedded Systems" by Elecia White
- **Standards**: MISRA C guidelines for memory management
- **Tools**: Valgrind, AddressSanitizer for memory debugging
- **Documentation**: ARM Cortex-M memory model documentation

**Next Topic:** [Pointers and Memory Addresses](./Pointers_Memory_Addresses.md) → [Type Qualifiers](./Type_Qualifiers.md)
