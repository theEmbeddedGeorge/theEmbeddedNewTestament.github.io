# Memory Pool Allocation in Embedded Systems

## 📋 Table of Contents
- [Overview](#-overview)
- [Pool Design](#-pool-design)
- [Fixed-Size Pools](#-fixed-size-pools)
- [Variable-Size Pools](#-variable-size-pools)
- [Multi-Pool Systems](#-multi-pool-systems)
- [Thread Safety](#-thread-safety)
- [Performance Optimization](#-performance-optimization)
- [Common Pitfalls](#-common-pitfalls)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

Memory pool allocation is a critical technique for embedded systems where predictable memory usage and fast allocation/deallocation are essential. Pools eliminate fragmentation and provide deterministic performance.

### Key Concepts for Embedded Development
- **Deterministic allocation** - Predictable allocation time
- **No fragmentation** - Fixed-size blocks prevent fragmentation
- **Fast operations** - O(1) allocation and deallocation
- **Memory safety** - Bounds checking and overflow protection

## 🏗️ Pool Design

### Basic Pool Structure
```c
// Memory pool block header
typedef struct pool_block {
    struct pool_block* next;  // Next free block
    uint8_t data[];          // Flexible array member
} pool_block_t;

// Memory pool structure
typedef struct {
    uint8_t* pool_start;     // Start of pool memory
    size_t pool_size;        // Total pool size
    size_t block_size;       // Size of each block
    size_t block_count;      // Number of blocks
    pool_block_t* free_list; // List of free blocks
    size_t free_count;       // Number of free blocks
    uint8_t initialized;     // Pool initialization flag
} memory_pool_t;

// Pool statistics
typedef struct {
    size_t total_blocks;
    size_t used_blocks;
    size_t free_blocks;
    size_t peak_usage;
    size_t allocation_count;
    size_t deallocation_count;
} pool_stats_t;
```

### Pool Initialization
```c
// Initialize memory pool
int pool_init(memory_pool_t* pool, size_t block_size, size_t block_count) {
    if (pool == NULL || block_size == 0 || block_count == 0) {
        return -1;  // Invalid parameters
    }
    
    // Calculate total pool size
    size_t total_size = block_count * block_size;
    
    // Allocate pool memory
    pool->pool_start = malloc(total_size);
    if (pool->pool_start == NULL) {
        return -2;  // Memory allocation failed
    }
    
    // Initialize pool structure
    pool->pool_size = total_size;
    pool->block_size = block_size;
    pool->block_count = block_count;
    pool->free_count = block_count;
    pool->initialized = 1;
    
    // Initialize free list
    pool->free_list = (pool_block_t*)pool->pool_start;
    
    // Link all blocks in free list
    pool_block_t* current = pool->free_list;
    for (size_t i = 0; i < block_count - 1; i++) {
        current->next = (pool_block_t*)((uint8_t*)current + block_size);
        current = current->next;
    }
    current->next = NULL;  // Last block points to NULL
    
    return 0;  // Success
}

// Destroy memory pool
void pool_destroy(memory_pool_t* pool) {
    if (pool != NULL && pool->initialized) {
        if (pool->pool_start != NULL) {
            free(pool->pool_start);
            pool->pool_start = NULL;
        }
        pool->initialized = 0;
    }
}
```

## 🔧 Fixed-Size Pools

### Basic Fixed-Size Pool Operations
```c
// Allocate block from pool
void* pool_alloc(memory_pool_t* pool) {
    if (pool == NULL || !pool->initialized) {
        return NULL;
    }
    
    if (pool->free_count == 0) {
        return NULL;  // Pool exhausted
    }
    
    // Get first free block
    pool_block_t* block = pool->free_list;
    pool->free_list = block->next;
    pool->free_count--;
    
    return &block->data;  // Return data area
}

// Free block back to pool
void pool_free(memory_pool_t* pool, void* ptr) {
    if (pool == NULL || !pool->initialized || ptr == NULL) {
        return;
    }
    
    // Calculate block address
    pool_block_t* block = (pool_block_t*)((uint8_t*)ptr - offsetof(pool_block_t, data));
    
    // Validate block is within pool bounds
    if ((uint8_t*)block < pool->pool_start || 
        (uint8_t*)block >= pool->pool_start + pool->pool_size) {
        return;  // Invalid pointer
    }
    
    // Add to free list
    block->next = pool->free_list;
    pool->free_list = block;
    pool->free_count++;
}

// Get pool statistics
pool_stats_t pool_get_stats(memory_pool_t* pool) {
    pool_stats_t stats = {0};
    
    if (pool != NULL && pool->initialized) {
        stats.total_blocks = pool->block_count;
        stats.free_blocks = pool->free_count;
        stats.used_blocks = pool->block_count - pool->free_count;
    }
    
    return stats;
}
```

### Multiple Fixed-Size Pools
```c
// Pool configuration for different block sizes
typedef struct {
    size_t block_size;
    size_t block_count;
    memory_pool_t pool;
} pool_config_t;

// Multi-pool system
typedef struct {
    pool_config_t* configs;
    size_t config_count;
    uint8_t initialized;
} multi_pool_t;

// Initialize multi-pool system
int multi_pool_init(multi_pool_t* mp, pool_config_t* configs, size_t count) {
    if (mp == NULL || configs == NULL || count == 0) {
        return -1;
    }
    
    mp->configs = configs;
    mp->config_count = count;
    
    // Initialize each pool
    for (size_t i = 0; i < count; i++) {
        int result = pool_init(&configs[i].pool, 
                              configs[i].block_size, 
                              configs[i].block_count);
        if (result != 0) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                pool_destroy(&configs[j].pool);
            }
            return result;
        }
    }
    
    mp->initialized = 1;
    return 0;
}

// Allocate from appropriate pool
void* multi_pool_alloc(multi_pool_t* mp, size_t size) {
    if (mp == NULL || !mp->initialized) {
        return NULL;
    }
    
    // Find appropriate pool for size
    for (size_t i = 0; i < mp->config_count; i++) {
        if (size <= mp->configs[i].block_size) {
            return pool_alloc(&mp->configs[i].pool);
        }
    }
    
    return NULL;  // No suitable pool found
}
```

## 📊 Variable-Size Pools

### Buddy System Implementation
```c
// Buddy system for variable-size allocations
typedef struct buddy_block {
    struct buddy_block* next;
    size_t size;
    uint8_t used;
} buddy_block_t;

typedef struct {
    uint8_t* pool_start;
    size_t pool_size;
    size_t min_block_size;
    buddy_block_t* free_lists[32];  // Free lists for different sizes
    uint8_t initialized;
} buddy_pool_t;

// Initialize buddy pool
int buddy_pool_init(buddy_pool_t* pool, size_t total_size, size_t min_block) {
    if (pool == NULL || total_size == 0 || min_block == 0) {
        return -1;
    }
    
    pool->pool_start = malloc(total_size);
    if (pool->pool_start == NULL) {
        return -2;
    }
    
    pool->pool_size = total_size;
    pool->min_block_size = min_block;
    pool->initialized = 1;
    
    // Initialize free lists
    for (int i = 0; i < 32; i++) {
        pool->free_lists[i] = NULL;
    }
    
    // Add initial block to appropriate free list
    size_t block_size = total_size;
    int list_index = 0;
    
    while (block_size >= min_block) {
        if (block_size == min_block) {
            buddy_block_t* block = (buddy_block_t*)pool->pool_start;
            block->next = pool->free_lists[list_index];
            block->size = block_size;
            block->used = 0;
            pool->free_lists[list_index] = block;
            break;
        }
        block_size /= 2;
        list_index++;
    }
    
    return 0;
}

// Allocate from buddy pool
void* buddy_pool_alloc(buddy_pool_t* pool, size_t size) {
    if (pool == NULL || !pool->initialized || size == 0) {
        return NULL;
    }
    
    // Find appropriate block size
    size_t block_size = pool->min_block_size;
    int list_index = 0;
    
    while (block_size < size && list_index < 31) {
        block_size *= 2;
        list_index++;
    }
    
    // Find free block
    buddy_block_t* block = pool->free_lists[list_index];
    if (block == NULL) {
        return NULL;  // No free blocks of this size
    }
    
    // Remove from free list
    pool->free_lists[list_index] = block->next;
    block->used = 1;
    
    return (uint8_t*)block + sizeof(buddy_block_t);
}
```

## 🔄 Multi-Pool Systems

### Hierarchical Pool System
```c
// Pool hierarchy for different allocation patterns
typedef enum {
    POOL_CRITICAL,    // Critical real-time allocations
    POOL_NORMAL,      // Normal allocations
    POOL_LARGE,       // Large allocations
    POOL_TEMPORARY    // Temporary allocations
} pool_priority_t;

typedef struct {
    memory_pool_t pools[4];  // One pool per priority
    pool_stats_t stats[4];
    uint8_t initialized;
} hierarchical_pool_t;

// Initialize hierarchical pool
int hierarchical_pool_init(hierarchical_pool_t* hp) {
    if (hp == NULL) {
        return -1;
    }
    
    // Critical pool: small, fast blocks
    pool_init(&hp->pools[POOL_CRITICAL], 32, 64);
    
    // Normal pool: medium blocks
    pool_init(&hp->pools[POOL_NORMAL], 128, 32);
    
    // Large pool: large blocks
    pool_init(&hp->pools[POOL_LARGE], 1024, 8);
    
    // Temporary pool: temporary allocations
    pool_init(&hp->pools[POOL_TEMPORARY], 256, 16);
    
    hp->initialized = 1;
    return 0;
}

// Allocate with priority
void* hierarchical_alloc(hierarchical_pool_t* hp, size_t size, pool_priority_t priority) {
    if (hp == NULL || !hp->initialized) {
        return NULL;
    }
    
    return pool_alloc(&hp->pools[priority]);
}
```

## 🔒 Thread Safety

### Thread-Safe Pool Implementation
```c
#include <pthread.h>

typedef struct {
    memory_pool_t pool;
    pthread_mutex_t mutex;
    uint8_t thread_safe;
} thread_safe_pool_t;

// Initialize thread-safe pool
int thread_safe_pool_init(thread_safe_pool_t* tsp, size_t block_size, size_t block_count) {
    if (tsp == NULL) {
        return -1;
    }
    
    int result = pool_init(&tsp->pool, block_size, block_count);
    if (result != 0) {
        return result;
    }
    
    if (pthread_mutex_init(&tsp->mutex, NULL) != 0) {
        pool_destroy(&tsp->pool);
        return -2;
    }
    
    tsp->thread_safe = 1;
    return 0;
}

// Thread-safe allocation
void* thread_safe_alloc(thread_safe_pool_t* tsp) {
    if (tsp == NULL || !tsp->thread_safe) {
        return NULL;
    }
    
    pthread_mutex_lock(&tsp->mutex);
    void* result = pool_alloc(&tsp->pool);
    pthread_mutex_unlock(&tsp->mutex);
    
    return result;
}

// Thread-safe deallocation
void thread_safe_free(thread_safe_pool_t* tsp, void* ptr) {
    if (tsp == NULL || !tsp->thread_safe) {
        return;
    }
    
    pthread_mutex_lock(&tsp->mutex);
    pool_free(&tsp->pool, ptr);
    pthread_mutex_unlock(&tsp->mutex);
}
```

## ⚡ Performance Optimization

### Lock-Free Pool Implementation
```c
#include <stdatomic.h>

typedef struct lock_free_block {
    struct lock_free_block* next;
    uint8_t data[];
} lock_free_block_t;

typedef struct {
    uint8_t* pool_start;
    size_t block_size;
    size_t block_count;
    _Atomic(lock_free_block_t*) free_list;
    _Atomic(size_t) free_count;
} lock_free_pool_t;

// Lock-free allocation
void* lock_free_alloc(lock_free_pool_t* pool) {
    lock_free_block_t* old_head;
    lock_free_block_t* new_head;
    
    do {
        old_head = atomic_load(&pool->free_list);
        if (old_head == NULL) {
            return NULL;  // Pool exhausted
        }
        new_head = old_head->next;
    } while (!atomic_compare_exchange_weak(&pool->free_list, &old_head, new_head));
    
    atomic_fetch_sub(&pool->free_count, 1);
    return &old_head->data;
}

// Lock-free deallocation
void lock_free_free(lock_free_pool_t* pool, void* ptr) {
    lock_free_block_t* block = (lock_free_block_t*)((uint8_t*)ptr - offsetof(lock_free_block_t, data));
    lock_free_block_t* old_head;
    
    do {
        old_head = atomic_load(&pool->free_list);
        block->next = old_head;
    } while (!atomic_compare_exchange_weak(&pool->free_list, &old_head, block));
    
    atomic_fetch_add(&pool->free_count, 1);
}
```

### Memory Alignment Optimization
```c
// Aligned pool allocation
typedef struct {
    uint8_t* pool_start;
    size_t pool_size;
    size_t block_size;
    size_t alignment;
    size_t block_count;
    void** free_list;
    size_t free_count;
} aligned_pool_t;

// Initialize aligned pool
int aligned_pool_init(aligned_pool_t* pool, size_t block_size, size_t block_count, size_t alignment) {
    if (pool == NULL || block_size == 0 || block_count == 0) {
        return -1;
    }
    
    // Calculate aligned block size
    size_t aligned_block_size = (block_size + alignment - 1) & ~(alignment - 1);
    size_t total_size = aligned_block_size * block_count;
    
    // Allocate aligned memory
    if (posix_memalign(&pool->pool_start, alignment, total_size) != 0) {
        return -2;
    }
    
    pool->pool_size = total_size;
    pool->block_size = aligned_block_size;
    pool->alignment = alignment;
    pool->block_count = block_count;
    pool->free_count = block_count;
    
    // Initialize free list
    pool->free_list = malloc(block_count * sizeof(void*));
    if (pool->free_list == NULL) {
        free(pool->pool_start);
        return -3;
    }
    
    // Link blocks
    for (size_t i = 0; i < block_count; i++) {
        pool->free_list[i] = pool->pool_start + (i * aligned_block_size);
    }
    
    return 0;
}
```

## ⚠️ Common Pitfalls

### Memory Corruption
```c
// BAD: No bounds checking
void* unsafe_pool_alloc(memory_pool_t* pool) {
    pool_block_t* block = pool->free_list;
    pool->free_list = block->next;
    return &block->data;  // No validation
}

// GOOD: Bounds checking
void* safe_pool_alloc(memory_pool_t* pool) {
    if (pool == NULL || !pool->initialized) {
        return NULL;
    }
    
    if (pool->free_count == 0) {
        return NULL;
    }
    
    pool_block_t* block = pool->free_list;
    pool->free_list = block->next;
    pool->free_count--;
    
    return &block->data;
}
```

### Double Free
```c
// BAD: No double-free protection
void unsafe_pool_free(memory_pool_t* pool, void* ptr) {
    pool_block_t* block = (pool_block_t*)((uint8_t*)ptr - offsetof(pool_block_t, data));
    block->next = pool->free_list;
    pool->free_list = block;
}

// GOOD: Double-free detection
void safe_pool_free(memory_pool_t* pool, void* ptr) {
    if (pool == NULL || ptr == NULL) {
        return;
    }
    
    pool_block_t* block = (pool_block_t*)((uint8_t*)ptr - offsetof(pool_block_t, data));
    
    // Check if block is already in free list
    pool_block_t* current = pool->free_list;
    while (current != NULL) {
        if (current == block) {
            return;  // Already freed
        }
        current = current->next;
    }
    
    block->next = pool->free_list;
    pool->free_list = block;
    pool->free_count++;
}
```

## ✅ Best Practices

### Pool Management Guidelines
```c
// 1. Choose appropriate block sizes
#define SMALL_BLOCK_SIZE   32   // For small objects
#define MEDIUM_BLOCK_SIZE  128  // For medium objects
#define LARGE_BLOCK_SIZE   1024 // For large objects

// 2. Monitor pool usage
void monitor_pool_usage(memory_pool_t* pool) {
    pool_stats_t stats = pool_get_stats(pool);
    float usage_percent = (float)stats.used_blocks / stats.total_blocks * 100;
    
    if (usage_percent > 90) {
        // Pool nearly full - take action
        log_warning("Pool usage: %.1f%%", usage_percent);
    }
}

// 3. Use multiple pools for different sizes
memory_pool_t small_pool;
memory_pool_t medium_pool;
memory_pool_t large_pool;

// 4. Implement pool validation
int validate_pool(memory_pool_t* pool) {
    if (pool == NULL || !pool->initialized) {
        return -1;
    }
    
    // Check free list integrity
    size_t free_count = 0;
    pool_block_t* current = pool->free_list;
    
    while (current != NULL) {
        free_count++;
        current = current->next;
    }
    
    return (free_count == pool->free_count) ? 0 : -1;
}
```

### Embedded-Specific Patterns
```c
// Critical section pool for real-time systems
typedef struct {
    memory_pool_t pool;
    uint32_t allocation_time;
    uint32_t max_allocation_time;
} critical_pool_t;

// Allocate with timing constraints
void* critical_alloc(critical_pool_t* cp) {
    uint32_t start_time = get_system_time();
    
    void* result = pool_alloc(&cp->pool);
    
    uint32_t allocation_time = get_system_time() - start_time;
    if (allocation_time > cp->max_allocation_time) {
        cp->max_allocation_time = allocation_time;
    }
    
    return result;
}

// Memory pool for interrupt handlers
static memory_pool_t interrupt_pool;
static uint8_t interrupt_pool_initialized = 0;

// Initialize interrupt pool
void init_interrupt_pool(void) {
    if (!interrupt_pool_initialized) {
        pool_init(&interrupt_pool, 64, 16);
        interrupt_pool_initialized = 1;
    }
}

// Interrupt-safe allocation
void* interrupt_alloc(void) {
    if (!interrupt_pool_initialized) {
        return NULL;
    }
    
    // Disable interrupts during allocation
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    
    void* result = pool_alloc(&interrupt_pool);
    
    // Restore interrupt state
    if (!primask) {
        __enable_irq();
    }
    
    return result;
}
```

## 🎯 Interview Questions

### Basic Concepts
1. **What are the advantages of memory pools over malloc/free?**
   - Deterministic allocation time
   - No fragmentation
   - Better cache locality
   - Predictable memory usage

2. **How do you prevent fragmentation in memory pools?**
   - Fixed-size blocks
   - No splitting or coalescing
   - Separate pools for different sizes

3. **What are the trade-offs between different pool implementations?**
   - Simple pools: Fast, limited flexibility
   - Buddy systems: Variable size, more complex
   - Multi-pools: Better utilization, more overhead

### Advanced Topics
1. **How would you implement a thread-safe memory pool?**
   - Use mutexes for synchronization
   - Implement lock-free algorithms
   - Consider per-thread pools

2. **How do you handle pool exhaustion in embedded systems?**
   - Monitor pool usage
   - Implement fallback strategies
   - Use multiple pools with different priorities

3. **What are the performance characteristics of memory pools?**
   - O(1) allocation and deallocation
   - No fragmentation overhead
   - Better cache performance
   - Predictable timing

## 📚 Additional Resources

- **Books**: "Making Embedded Systems" by Elecia White
- **Standards**: MISRA C guidelines for memory management
- **Tools**: Memory profilers, static analysis tools
- **Documentation**: Real-time memory management guides

**Next Topic:** [Aligned Memory Allocation](./Aligned_Memory_Allocation.md) → [Memory Fragmentation](./Memory_Fragmentation.md)
