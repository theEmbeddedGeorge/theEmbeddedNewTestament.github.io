# Memory Fragmentation

## 📋 Table of Contents
- [Overview](#-overview)
- [Types of Fragmentation](#-types-of-fragmentation)
- [Fragmentation Detection](#-fragmentation-detection)
- [Prevention Strategies](#-prevention-strategies)
- [Defragmentation Techniques](#-defragmentation-techniques)
- [Memory Pool Solutions](#-memory-pool-solutions)
- [Real-time Considerations](#-real-time-considerations)
- [Common Pitfalls](#-common-pitfalls)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

Memory fragmentation occurs when free memory becomes divided into small, non-contiguous blocks, making it difficult to allocate larger chunks. In embedded systems, this can lead to allocation failures even when sufficient total free memory exists.

## 🔍 Types of Fragmentation

### External Fragmentation
External fragmentation occurs when free memory is scattered in small pieces throughout the heap.

```c
// Example of external fragmentation
void external_fragmentation_demo() {
    // Allocate several small blocks
    void* ptr1 = malloc(100);
    void* ptr2 = malloc(100);
    void* ptr3 = malloc(100);
    
    // Free middle block
    free(ptr2);
    
    // Try to allocate larger block
    void* large_ptr = malloc(200);  // May fail despite having 200+ bytes free
    if (large_ptr == NULL) {
        printf("External fragmentation prevented allocation\n");
    }
    
    free(ptr1);
    free(ptr3);
}
```

### Internal Fragmentation
Internal fragmentation occurs when allocated memory is larger than requested due to alignment or allocation granularity.

```c
// Example of internal fragmentation
void internal_fragmentation_demo() {
    // Request 1 byte, but get 8-byte aligned block
    void* ptr = malloc(1);
    
    // On many systems, this allocates 8 or 16 bytes
    // 7-15 bytes are wasted (internal fragmentation)
    
    free(ptr);
}
```

## 🔧 Fragmentation Detection

### Memory Block Tracking
```c
typedef struct {
    void* start;
    size_t size;
    bool is_free;
} memory_block_t;

typedef struct {
    memory_block_t* blocks;
    size_t block_count;
    size_t max_blocks;
} fragmentation_tracker_t;

fragmentation_tracker_t* create_fragmentation_tracker(size_t max_blocks) {
    fragmentation_tracker_t* tracker = malloc(sizeof(fragmentation_tracker_t));
    if (!tracker) return NULL;
    
    tracker->blocks = calloc(max_blocks, sizeof(memory_block_t));
    tracker->block_count = 0;
    tracker->max_blocks = max_blocks;
    
    return tracker;
}

void track_allocation(fragmentation_tracker_t* tracker, void* ptr, size_t size) {
    if (tracker->block_count < tracker->max_blocks) {
        tracker->blocks[tracker->block_count].start = ptr;
        tracker->blocks[tracker->block_count].size = size;
        tracker->blocks[tracker->block_count].is_free = false;
        tracker->block_count++;
    }
}
```

### Fragmentation Analysis
```c
typedef struct {
    size_t total_free;
    size_t largest_free_block;
    size_t free_block_count;
    float fragmentation_ratio;
} fragmentation_stats_t;

fragmentation_stats_t analyze_fragmentation(fragmentation_tracker_t* tracker) {
    fragmentation_stats_t stats = {0};
    
    // Calculate total free memory
    for (size_t i = 0; i < tracker->block_count; i++) {
        if (tracker->blocks[i].is_free) {
            stats.total_free += tracker->blocks[i].size;
            stats.free_block_count++;
            
            if (tracker->blocks[i].size > stats.largest_free_block) {
                stats.largest_free_block = tracker->blocks[i].size;
            }
        }
    }
    
    // Calculate fragmentation ratio
    if (stats.total_free > 0) {
        stats.fragmentation_ratio = (float)stats.largest_free_block / stats.total_free;
    }
    
    return stats;
}
```

### Heap Analysis Tools
```c
#include <malloc.h>

void analyze_heap_fragmentation() {
    struct mallinfo info = mallinfo();
    
    printf("Total allocated space: %d bytes\n", info.uordblks);
    printf("Total free space: %d bytes\n", info.fordblks);
    printf("Largest free block: %d bytes\n", info.mxordblk);
    printf("Number of free blocks: %d\n", info.ordblks);
    
    // Calculate fragmentation
    float fragmentation = 1.0f - ((float)info.mxordblk / info.fordblks);
    printf("Fragmentation ratio: %.2f%%\n", fragmentation * 100);
}
```

## 🛡️ Prevention Strategies

### 1. Memory Pool Allocation
```c
typedef struct {
    void* pool;
    size_t block_size;
    size_t total_blocks;
    bool* free_blocks;
} memory_pool_t;

memory_pool_t* create_memory_pool(size_t block_size, size_t num_blocks) {
    memory_pool_t* pool = malloc(sizeof(memory_pool_t));
    if (!pool) return NULL;
    
    pool->block_size = block_size;
    pool->total_blocks = num_blocks;
    pool->pool = malloc(block_size * num_blocks);
    pool->free_blocks = calloc(num_blocks, sizeof(bool));
    
    if (!pool->pool || !pool->free_blocks) {
        free(pool->pool);
        free(pool->free_blocks);
        free(pool);
        return NULL;
    }
    
    // Mark all blocks as free
    for (size_t i = 0; i < num_blocks; i++) {
        pool->free_blocks[i] = true;
    }
    
    return pool;
}

void* pool_allocate(memory_pool_t* pool) {
    for (size_t i = 0; i < pool->total_blocks; i++) {
        if (pool->free_blocks[i]) {
            pool->free_blocks[i] = false;
            return (char*)pool->pool + (i * pool->block_size);
        }
    }
    return NULL;  // No free blocks
}

void pool_free(memory_pool_t* pool, void* ptr) {
    size_t block_index = ((char*)ptr - (char*)pool->pool) / pool->block_size;
    if (block_index < pool->total_blocks) {
        pool->free_blocks[block_index] = true;
    }
}
```

### 2. Fixed-Size Allocation
```c
// Power-of-2 size classes to reduce fragmentation
#define SIZE_CLASS_8    8
#define SIZE_CLASS_16   16
#define SIZE_CLASS_32   32
#define SIZE_CLASS_64   64
#define SIZE_CLASS_128  128
#define SIZE_CLASS_256  256

size_t get_size_class(size_t requested_size) {
    if (requested_size <= SIZE_CLASS_8) return SIZE_CLASS_8;
    if (requested_size <= SIZE_CLASS_16) return SIZE_CLASS_16;
    if (requested_size <= SIZE_CLASS_32) return SIZE_CLASS_32;
    if (requested_size <= SIZE_CLASS_64) return SIZE_CLASS_64;
    if (requested_size <= SIZE_CLASS_128) return SIZE_CLASS_128;
    if (requested_size <= SIZE_CLASS_256) return SIZE_CLASS_256;
    return requested_size;  // Larger allocations use exact size
}

void* fragmented_aware_malloc(size_t size) {
    size_t actual_size = get_size_class(size);
    return malloc(actual_size);
}
```

### 3. Allocation Patterns
```c
// Allocate objects of similar lifetimes together
typedef struct {
    void* short_lived_objects[100];
    void* long_lived_objects[50];
    size_t short_count;
    size_t long_count;
} lifetime_manager_t;

void* allocate_short_lived(lifetime_manager_t* manager) {
    if (manager->short_count < 100) {
        void* ptr = malloc(64);  // Fixed size for short-lived objects
        manager->short_lived_objects[manager->short_count++] = ptr;
        return ptr;
    }
    return NULL;
}

void cleanup_short_lived(lifetime_manager_t* manager) {
    for (size_t i = 0; i < manager->short_count; i++) {
        free(manager->short_lived_objects[i]);
    }
    manager->short_count = 0;
}
```

## 🔄 Defragmentation Techniques

### 1. Compaction Algorithm
```c
void compact_memory(fragmentation_tracker_t* tracker) {
    // Sort blocks by address
    for (size_t i = 0; i < tracker->block_count - 1; i++) {
        for (size_t j = i + 1; j < tracker->block_count; j++) {
            if (tracker->blocks[i].start > tracker->blocks[j].start) {
                memory_block_t temp = tracker->blocks[i];
                tracker->blocks[i] = tracker->blocks[j];
                tracker->blocks[j] = temp;
            }
        }
    }
    
    // Merge adjacent free blocks
    for (size_t i = 0; i < tracker->block_count - 1; i++) {
        if (tracker->blocks[i].is_free && tracker->blocks[i + 1].is_free) {
            // Merge blocks
            tracker->blocks[i].size += tracker->blocks[i + 1].size;
            
            // Remove second block
            for (size_t j = i + 1; j < tracker->block_count - 1; j++) {
                tracker->blocks[j] = tracker->blocks[j + 1];
            }
            tracker->block_count--;
            i--;  // Recheck this position
        }
    }
}
```

### 2. Garbage Collection Approach
```c
typedef struct {
    void* ptr;
    size_t size;
    bool is_marked;
} gc_object_t;

typedef struct {
    gc_object_t* objects;
    size_t object_count;
    size_t max_objects;
} garbage_collector_t;

void mark_and_sweep(garbage_collector_t* gc) {
    // Mark phase - mark all reachable objects
    for (size_t i = 0; i < gc->object_count; i++) {
        // In real implementation, traverse object graph
        // For simplicity, assume all objects are reachable
        gc->objects[i].is_marked = true;
    }
    
    // Sweep phase - free unmarked objects
    for (size_t i = 0; i < gc->object_count; i++) {
        if (!gc->objects[i].is_marked) {
            free(gc->objects[i].ptr);
            // Remove from objects array
            for (size_t j = i; j < gc->object_count - 1; j++) {
                gc->objects[j] = gc->objects[j + 1];
            }
            gc->object_count--;
            i--;  // Recheck this position
        }
    }
}
```

## ⏱️ Real-time Considerations

### Predictable Allocation
```c
// Pre-allocate memory for real-time tasks
typedef struct {
    void* preallocated_buffers[10];
    size_t buffer_size;
    size_t available_buffers;
} rt_memory_pool_t;

rt_memory_pool_t* create_rt_pool(size_t buffer_size, size_t num_buffers) {
    rt_memory_pool_t* pool = malloc(sizeof(rt_memory_pool_t));
    if (!pool) return NULL;
    
    pool->buffer_size = buffer_size;
    pool->available_buffers = num_buffers;
    
    for (size_t i = 0; i < num_buffers; i++) {
        pool->preallocated_buffers[i] = malloc(buffer_size);
        if (!pool->preallocated_buffers[i]) {
            // Cleanup and return NULL
            for (size_t j = 0; j < i; j++) {
                free(pool->preallocated_buffers[j]);
            }
            free(pool);
            return NULL;
        }
    }
    
    return pool;
}

void* rt_allocate(rt_memory_pool_t* pool) {
    if (pool->available_buffers > 0) {
        return pool->preallocated_buffers[--pool->available_buffers];
    }
    return NULL;  // No buffers available
}
```

### Fragmentation Monitoring
```c
// Monitor fragmentation in real-time systems
typedef struct {
    size_t total_memory;
    size_t free_memory;
    size_t largest_free_block;
    float fragmentation_level;
} rt_memory_monitor_t;

void update_fragmentation_monitor(rt_memory_monitor_t* monitor) {
    struct mallinfo info = mallinfo();
    
    monitor->total_memory = info.uordblks + info.fordblks;
    monitor->free_memory = info.fordblks;
    monitor->largest_free_block = info.mxordblk;
    
    if (monitor->free_memory > 0) {
        monitor->fragmentation_level = 1.0f - 
            ((float)monitor->largest_free_block / monitor->free_memory);
    } else {
        monitor->fragmentation_level = 1.0f;
    }
    
    // Alert if fragmentation is too high
    if (monitor->fragmentation_level > 0.8f) {
        printf("WARNING: High fragmentation detected: %.2f%%\n", 
               monitor->fragmentation_level * 100);
    }
}
```

## ⚠️ Common Pitfalls

### 1. Ignoring Fragmentation in Long-Running Systems
```c
// WRONG: No fragmentation monitoring
void long_running_task() {
    while (1) {
        void* ptr = malloc(100);
        // Use ptr...
        free(ptr);
        // Fragmentation may accumulate over time
    }
}

// CORRECT: Monitor and handle fragmentation
void long_running_task_with_monitoring() {
    rt_memory_monitor_t monitor = {0};
    
    while (1) {
        update_fragmentation_monitor(&monitor);
        
        if (monitor.fragmentation_level > 0.7f) {
            // Trigger defragmentation or use memory pool
            compact_memory_or_use_pool();
        }
        
        void* ptr = malloc(100);
        // Use ptr...
        free(ptr);
    }
}
```

### 2. Not Considering Allocation Patterns
```c
// WRONG: Mixed allocation patterns
void mixed_allocation_pattern() {
    void* large1 = malloc(1024);
    void* small1 = malloc(16);
    void* large2 = malloc(1024);
    void* small2 = malloc(16);
    
    free(large1);
    free(large2);
    // Small blocks fragment the heap
    
    free(small1);
    free(small2);
}

// CORRECT: Group allocations by size
void grouped_allocation_pattern() {
    void* large1 = malloc(1024);
    void* large2 = malloc(1024);
    void* small1 = malloc(16);
    void* small2 = malloc(16);
    
    free(large1);
    free(large2);
    // Large blocks freed together
    
    free(small1);
    free(small2);
}
```

## ✅ Best Practices

### 1. Use Memory Pools for Fixed-Size Allocations
```c
// Create pools for common allocation sizes
typedef struct {
    memory_pool_t* pool_16;
    memory_pool_t* pool_64;
    memory_pool_t* pool_256;
} multi_pool_allocator_t;

multi_pool_allocator_t* create_multi_pool_allocator() {
    multi_pool_allocator_t* allocator = malloc(sizeof(multi_pool_allocator_t));
    if (!allocator) return NULL;
    
    allocator->pool_16 = create_memory_pool(16, 100);
    allocator->pool_64 = create_memory_pool(64, 50);
    allocator->pool_256 = create_memory_pool(256, 20);
    
    return allocator;
}

void* multi_pool_allocate(multi_pool_allocator_t* allocator, size_t size) {
    if (size <= 16) return pool_allocate(allocator->pool_16);
    if (size <= 64) return pool_allocate(allocator->pool_64);
    if (size <= 256) return pool_allocate(allocator->pool_256);
    return malloc(size);  // Fallback to standard malloc
}
```

### 2. Implement Fragmentation Monitoring
```c
// Regular fragmentation checks
void schedule_fragmentation_check() {
    static uint32_t check_counter = 0;
    check_counter++;
    
    if (check_counter % 1000 == 0) {  // Check every 1000 allocations
        fragmentation_stats_t stats = analyze_current_fragmentation();
        
        if (stats.fragmentation_ratio < 0.3f) {
            printf("Low fragmentation: %.2f%%\n", 
                   (1.0f - stats.fragmentation_ratio) * 100);
        } else {
            printf("High fragmentation: %.2f%%\n", 
                   (1.0f - stats.fragmentation_ratio) * 100);
        }
    }
}
```

### 3. Design for Predictable Memory Usage
```c
// Pre-allocate memory for known patterns
typedef struct {
    void* buffers[5];
    size_t buffer_sizes[5];
    bool buffer_used[5];
} predictable_allocator_t;

predictable_allocator_t* create_predictable_allocator() {
    predictable_allocator_t* allocator = malloc(sizeof(predictable_allocator_t));
    if (!allocator) return NULL;
    
    // Pre-allocate common buffer sizes
    allocator->buffer_sizes[0] = 64;
    allocator->buffer_sizes[1] = 128;
    allocator->buffer_sizes[2] = 256;
    allocator->buffer_sizes[3] = 512;
    allocator->buffer_sizes[4] = 1024;
    
    for (int i = 0; i < 5; i++) {
        allocator->buffers[i] = malloc(allocator->buffer_sizes[i]);
        allocator->buffer_used[i] = false;
    }
    
    return allocator;
}
```

## 🎯 Interview Questions

### Basic Questions
1. **What is memory fragmentation and what are its types?**
   - External fragmentation: Free memory scattered in small pieces
   - Internal fragmentation: Wasted space within allocated blocks

2. **How can you detect memory fragmentation?**
   - Track free memory blocks
   - Analyze largest free block vs total free memory
   - Use heap analysis tools

3. **What strategies can prevent fragmentation?**
   - Memory pools for fixed-size allocations
   - Power-of-2 size classes
   - Grouping allocations by lifetime

### Advanced Questions
1. **How would you implement a defragmentation algorithm?**
   - Compact memory by moving allocated blocks
   - Merge adjacent free blocks
   - Update pointers to moved objects

2. **What are the challenges of defragmentation in real-time systems?**
   - Defragmentation can cause unpredictable delays
   - May violate real-time constraints
   - Requires careful scheduling

3. **How would you design a memory allocator that minimizes fragmentation?**
   - Use multiple pools for different size classes
   - Implement compaction when fragmentation is high
   - Monitor fragmentation levels continuously

## 📚 Additional Resources

### Standards and Documentation
- **C Standard Library**: `malloc()`, `free()` behavior
- **POSIX**: Memory management functions
- **Embedded C**: Memory management best practices

### Related Topics
- **[Memory Pool Allocation](./Memory_Pool_Allocation.md)** - Efficient memory pools
- **[Memory Leak Detection](./Memory_Leak_Detection.md)** - Detecting memory leaks
- **[Stack Overflow Prevention](./Stack_Overflow_Prevention.md)** - Stack management
- **[Cache-Aware Programming](./Cache_Aware_Programming.md)** - Memory access patterns

### Tools and Libraries
- **Valgrind**: Memory analysis and fragmentation detection
- **AddressSanitizer**: Memory error detection
- **Custom fragmentation monitors**: Real-time fragmentation tracking

---

**Next Topic:** [Memory Leak Detection](./Memory_Leak_Detection.md) → [Stack Overflow Prevention](./Stack_Overflow_Prevention.md)
