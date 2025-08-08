# Cache-Aware Programming

## 📋 Table of Contents
- [Overview](#-overview)
- [Cache Architecture](#-cache-architecture)
- [Cache Line Optimization](#-cache-line-optimization)
- [Memory Access Patterns](#-memory-access-patterns)
- [False Sharing Prevention](#-false-sharing-prevention)
- [Cache Prefetching](#-cache-prefetching)
- [Cache Flushing and Invalidation](#-cache-flushing-and-invalidation)
- [Multi-core Cache Considerations](#-multi-core-cache-considerations)
- [Performance Profiling](#-performance-profiling)
- [Common Pitfalls](#-common-pitfalls)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

Cache-aware programming optimizes code to work efficiently with CPU cache memory, improving performance by reducing cache misses and maximizing cache utilization. In embedded systems, understanding cache behavior is crucial for real-time performance and power efficiency.

## 🏗️ Cache Architecture

### Cache Hierarchy
```c
// Cache configuration for ARM Cortex-M7
typedef struct {
    uint32_t l1_data_size;      // L1 Data Cache size
    uint32_t l1_instruction_size; // L1 Instruction Cache size
    uint32_t l2_size;            // L2 Cache size
    uint32_t cache_line_size;    // Cache line size
    uint32_t associativity;      // Cache associativity
} cache_config_t;

cache_config_t get_cache_config(void) {
    cache_config_t config = {0};
    
    // Read cache configuration from CPU
    uint32_t ctr = __builtin_arm_mrc(15, 0, 0, 0, 1);
    
    config.cache_line_size = 4 << ((ctr >> 16) & 0xF);
    config.l1_data_size = 4 << ((ctr >> 6) & 0x7);
    config.l1_instruction_size = 4 << ((ctr >> 0) & 0x7);
    
    return config;
}
```

### Cache Line Structure
```c
// Cache line aligned data structure
#define CACHE_LINE_SIZE 64

typedef struct {
    uint8_t data[CACHE_LINE_SIZE];
} __attribute__((aligned(CACHE_LINE_SIZE))) cache_line_t;

// Array of cache-line aligned data
typedef struct {
    cache_line_t lines[100];
} cache_aligned_array_t;

// Ensure data fits in cache lines
typedef struct {
    uint32_t value1;
    uint32_t value2;
    char padding[CACHE_LINE_SIZE - 8];  // Padding to next cache line
} __attribute__((aligned(CACHE_LINE_SIZE))) separated_data_t;
```

## 🔧 Cache Line Optimization

### Data Structure Alignment
```c
// Optimize data structures for cache
typedef struct {
    uint32_t frequently_accessed;  // Hot data
    uint32_t rarely_accessed;      // Cold data
    char padding[CACHE_LINE_SIZE - 8];  // Separate to different cache lines
} __attribute__((aligned(CACHE_LINE_SIZE))) hot_cold_separated_t;

// Array of structures (AoS) vs Structure of arrays (SoA)
// AoS - may cause cache misses
typedef struct {
    uint32_t x, y, z;
} point_aos_t;

point_aos_t points_aos[1000];  // Array of structures

// SoA - better cache locality
typedef struct {
    uint32_t x[1000];
    uint32_t y[1000];
    uint32_t z[1000];
} points_soa_t;

points_soa_t points_soa;  // Structure of arrays
```

### Cache Line Padding
```c
// Prevent false sharing with padding
typedef struct {
    uint32_t counter1;
    char padding1[CACHE_LINE_SIZE - 4];
    uint32_t counter2;
    char padding2[CACHE_LINE_SIZE - 4];
} __attribute__((aligned(CACHE_LINE_SIZE))) padded_counters_t;

// Multi-core safe counters
typedef struct {
    uint32_t core1_counter;
    char padding1[CACHE_LINE_SIZE - 4];
    uint32_t core2_counter;
    char padding2[CACHE_LINE_SIZE - 4];
    uint32_t core3_counter;
    char padding3[CACHE_LINE_SIZE - 4];
    uint32_t core4_counter;
    char padding4[CACHE_LINE_SIZE - 4];
} __attribute__((aligned(CACHE_LINE_SIZE))) multi_core_counters_t;
```

## 🎯 Memory Access Patterns

### Sequential Access Optimization
```c
// Optimize sequential memory access
void optimized_sequential_access(uint32_t* data, size_t size) {
    // Process data in cache-line sized chunks
    size_t cache_line_elements = CACHE_LINE_SIZE / sizeof(uint32_t);
    
    for (size_t i = 0; i < size; i += cache_line_elements) {
        size_t chunk_size = (i + cache_line_elements > size) ? 
                           (size - i) : cache_line_elements;
        
        // Process chunk - all elements likely in cache
        for (size_t j = 0; j < chunk_size; j++) {
            data[i + j] *= 2;  // Sequential access
        }
    }
}

// Cache-friendly matrix access
void cache_friendly_matrix_multiply(float* a, float* b, float* c, int n) {
    // Access matrices in row-major order for better cache performance
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += a[i * n + k] * b[k * n + j];  // Row-major access
            }
            c[i * n + j] = sum;
        }
    }
}
```

### Random Access Optimization
```c
// Optimize random access patterns
typedef struct {
    uint32_t key;
    uint32_t value;
} key_value_t;

// Use hash table for random access
typedef struct {
    key_value_t* table;
    size_t size;
    size_t capacity;
} cache_optimized_hash_table_t;

cache_optimized_hash_table_t* create_cache_optimized_hash_table(size_t capacity) {
    cache_optimized_hash_table_t* table = malloc(sizeof(cache_optimized_hash_table_t));
    if (!table) return NULL;
    
    // Align table to cache line
    size_t aligned_size = ((capacity * sizeof(key_value_t) + CACHE_LINE_SIZE - 1) / 
                          CACHE_LINE_SIZE) * CACHE_LINE_SIZE;
    
    table->table = aligned_alloc(CACHE_LINE_SIZE, aligned_size);
    table->capacity = capacity;
    table->size = 0;
    
    return table;
}
```

## 🚫 False Sharing Prevention

### Multi-core Data Separation
```c
// Prevent false sharing between cores
typedef struct {
    uint32_t core_id;
    uint32_t local_data;
    char padding[CACHE_LINE_SIZE - 8];  // Padding to next cache line
} __attribute__((aligned(CACHE_LINE_SIZE))) per_core_data_t;

per_core_data_t* create_per_core_data_array(int num_cores) {
    size_t total_size = num_cores * sizeof(per_core_data_t);
    per_core_data_t* array = aligned_alloc(CACHE_LINE_SIZE, total_size);
    
    if (array) {
        for (int i = 0; i < num_cores; i++) {
            array[i].core_id = i;
            array[i].local_data = 0;
        }
    }
    
    return array;
}

// Thread-local storage with cache alignment
typedef struct {
    uint32_t thread_id;
    uint32_t local_counter;
    uint32_t local_sum;
    char padding[CACHE_LINE_SIZE - 12];
} __attribute__((aligned(CACHE_LINE_SIZE))) thread_local_storage_t;
```

### Lock-free Data Structures
```c
// Lock-free queue with cache optimization
typedef struct {
    uint32_t head;
    char padding1[CACHE_LINE_SIZE - 4];
    uint32_t tail;
    char padding2[CACHE_LINE_SIZE - 4];
    uint32_t data[QUEUE_SIZE];
} __attribute__((aligned(CACHE_LINE_SIZE))) lock_free_queue_t;

bool lock_free_queue_push(lock_free_queue_t* queue, uint32_t value) {
    uint32_t tail = queue->tail;
    uint32_t next_tail = (tail + 1) % QUEUE_SIZE;
    
    if (next_tail == queue->head) {
        return false;  // Queue full
    }
    
    queue->data[tail] = value;
    queue->tail = next_tail;
    return true;
}

bool lock_free_queue_pop(lock_free_queue_t* queue, uint32_t* value) {
    uint32_t head = queue->head;
    
    if (head == queue->tail) {
        return false;  // Queue empty
    }
    
    *value = queue->data[head];
    queue->head = (head + 1) % QUEUE_SIZE;
    return true;
}
```

## 🔄 Cache Prefetching

### Manual Prefetching
```c
// Manual cache prefetching
void prefetch_optimized_loop(uint32_t* data, size_t size) {
    size_t prefetch_distance = 4;  // Prefetch 4 cache lines ahead
    
    for (size_t i = 0; i < size; i++) {
        // Prefetch next cache line
        if (i + prefetch_distance < size) {
            __builtin_prefetch(&data[i + prefetch_distance], 0, 3);
        }
        
        // Process current element
        data[i] *= 2;
    }
}

// Prefetch for linked list traversal
typedef struct node {
    uint32_t data;
    struct node* next;
} node_t;

void prefetch_linked_list_traversal(node_t* head) {
    node_t* current = head;
    
    while (current) {
        // Prefetch next node
        if (current->next) {
            __builtin_prefetch(current->next, 0, 3);
        }
        
        // Process current node
        process_node(current);
        current = current->next;
    }
}
```

### Compiler Prefetching
```c
// Use compiler hints for prefetching
void compiler_prefetch_loop(uint32_t* data, size_t size) {
    #pragma unroll(4)
    for (size_t i = 0; i < size; i++) {
        // Compiler may generate prefetch instructions
        data[i] = data[i] * 2 + 1;
    }
}

// Loop with prefetch hints
void hinted_prefetch_loop(uint32_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        // Hint that this access pattern is sequential
        __builtin_expect(i < size - 1, 1);
        data[i] = data[i] * 2;
    }
}
```

## 🧹 Cache Flushing and Invalidation

### Cache Maintenance Operations
```c
// Cache flush and invalidate functions
void flush_data_cache(void) {
    // Flush entire data cache
    __builtin___clear_cache((char*)0, (char*)0xFFFFFFFF);
}

void invalidate_data_cache(void) {
    // Invalidate entire data cache
    __builtin___clear_cache((char*)0, (char*)0xFFFFFFFF);
}

void flush_cache_range(void* start, void* end) {
    // Flush specific memory range
    __builtin___clear_cache((char*)start, (char*)end);
}

// DMA-safe memory operations
void* dma_safe_allocate(size_t size) {
    void* ptr = aligned_alloc(CACHE_LINE_SIZE, size);
    if (ptr) {
        // Flush cache for DMA
        flush_cache_range(ptr, (char*)ptr + size);
    }
    return ptr;
}

void dma_safe_free(void* ptr, size_t size) {
    if (ptr) {
        // Invalidate cache after DMA
        invalidate_cache_range(ptr, (char*)ptr + size);
        free(ptr);
    }
}
```

### Cache Coherency
```c
// Ensure cache coherency between cores
typedef struct {
    volatile uint32_t flag;
    char padding[CACHE_LINE_SIZE - 4];
} __attribute__((aligned(CACHE_LINE_SIZE))) cache_coherent_flag_t;

void set_flag_with_coherency(cache_coherent_flag_t* flag) {
    flag->flag = 1;
    // Ensure write is visible to other cores
    __sync_synchronize();
}

bool check_flag_with_coherency(cache_coherent_flag_t* flag) {
    // Ensure we read the latest value
    __sync_synchronize();
    return flag->flag != 0;
}
```

## 🔄 Multi-core Cache Considerations

### Cache Line Sharing
```c
// Shared data structure with cache optimization
typedef struct {
    uint32_t shared_counter;
    char padding[CACHE_LINE_SIZE - 4];
} __attribute__((aligned(CACHE_LINE_SIZE))) shared_counter_t;

shared_counter_t* create_shared_counter(void) {
    return aligned_alloc(CACHE_LINE_SIZE, sizeof(shared_counter_t));
}

void atomic_increment_shared_counter(shared_counter_t* counter) {
    // Use atomic operation to avoid cache line bouncing
    __sync_fetch_and_add(&counter->shared_counter, 1);
}
```

### NUMA-aware Programming
```c
// NUMA-aware memory allocation
typedef struct {
    uint32_t numa_node;
    void* local_memory;
    size_t local_size;
} numa_aware_allocator_t;

numa_aware_allocator_t* create_numa_aware_allocator(uint32_t numa_node) {
    numa_aware_allocator_t* allocator = malloc(sizeof(numa_aware_allocator_t));
    if (!allocator) return NULL;
    
    allocator->numa_node = numa_node;
    // Allocate memory on specific NUMA node
    allocator->local_memory = numa_alloc_onnode(1024, numa_node);
    allocator->local_size = 1024;
    
    return allocator;
}

void* numa_aware_allocate(numa_aware_allocator_t* allocator, size_t size) {
    // Allocate on specific NUMA node
    return numa_alloc_onnode(size, allocator->numa_node);
}
```

## 📊 Performance Profiling

### Cache Performance Monitoring
```c
// Cache performance counters
typedef struct {
    uint64_t cache_misses;
    uint64_t cache_hits;
    uint64_t cache_accesses;
    float hit_ratio;
} cache_performance_t;

cache_performance_t* create_cache_performance_monitor(void) {
    cache_performance_t* monitor = malloc(sizeof(cache_performance_t));
    if (monitor) {
        monitor->cache_misses = 0;
        monitor->cache_hits = 0;
        monitor->cache_accesses = 0;
        monitor->hit_ratio = 0.0f;
    }
    return monitor;
}

void update_cache_performance(cache_performance_t* monitor, bool hit) {
    monitor->cache_accesses++;
    if (hit) {
        monitor->cache_hits++;
    } else {
        monitor->cache_misses++;
    }
    
    monitor->hit_ratio = (float)monitor->cache_hits / monitor->cache_accesses;
}

void report_cache_performance(cache_performance_t* monitor) {
    printf("Cache Performance:\n");
    printf("  Hits: %lu\n", monitor->cache_hits);
    printf("  Misses: %lu\n", monitor->cache_misses);
    printf("  Hit Ratio: %.2f%%\n", monitor->hit_ratio * 100);
}
```

### Cache Miss Analysis
```c
// Analyze cache miss patterns
typedef struct {
    uint32_t* miss_addresses;
    size_t miss_count;
    size_t capacity;
} cache_miss_analyzer_t;

cache_miss_analyzer_t* create_cache_miss_analyzer(size_t capacity) {
    cache_miss_analyzer_t* analyzer = malloc(sizeof(cache_miss_analyzer_t));
    if (!analyzer) return NULL;
    
    analyzer->miss_addresses = malloc(capacity * sizeof(uint32_t));
    analyzer->miss_count = 0;
    analyzer->capacity = capacity;
    
    return analyzer;
}

void record_cache_miss(cache_miss_analyzer_t* analyzer, uint32_t address) {
    if (analyzer->miss_count < analyzer->capacity) {
        analyzer->miss_addresses[analyzer->miss_count++] = address;
    }
}

void analyze_cache_misses(cache_miss_analyzer_t* analyzer) {
    printf("Cache Miss Analysis:\n");
    printf("  Total Misses: %zu\n", analyzer->miss_count);
    
    // Analyze patterns
    uint32_t sequential_misses = 0;
    uint32_t random_misses = 0;
    
    for (size_t i = 1; i < analyzer->miss_count; i++) {
        uint32_t diff = analyzer->miss_addresses[i] - analyzer->miss_addresses[i-1];
        if (diff <= CACHE_LINE_SIZE) {
            sequential_misses++;
        } else {
            random_misses++;
        }
    }
    
    printf("  Sequential Misses: %u\n", sequential_misses);
    printf("  Random Misses: %u\n", random_misses);
}
```

## ⚠️ Common Pitfalls

### 1. Cache Line Crossing
```c
// WRONG: Data structure crossing cache lines
typedef struct {
    uint32_t data[16];  // 64 bytes - fits in cache line
    uint32_t extra_data; // May cause cache line crossing
} unaligned_struct_t;

// CORRECT: Cache line aligned structure
typedef struct {
    uint32_t data[16];
    char padding[CACHE_LINE_SIZE - 64];  // Ensure alignment
} __attribute__((aligned(CACHE_LINE_SIZE))) aligned_struct_t;
```

### 2. False Sharing
```c
// WRONG: Shared variables without padding
typedef struct {
    uint32_t counter1;
    uint32_t counter2;  // May be in same cache line
} shared_counters_t;

// CORRECT: Separate cache lines
typedef struct {
    uint32_t counter1;
    char padding[CACHE_LINE_SIZE - 4];
    uint32_t counter2;
    char padding2[CACHE_LINE_SIZE - 4];
} __attribute__((aligned(CACHE_LINE_SIZE))) separated_counters_t;
```

### 3. Inefficient Memory Access Patterns
```c
// WRONG: Column-major access in row-major array
void inefficient_matrix_access(float* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[j * n + i] *= 2;  // Column-major access
        }
    }
}

// CORRECT: Row-major access
void efficient_matrix_access(float* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] *= 2;  // Row-major access
        }
    }
}
```

## ✅ Best Practices

### 1. Cache Line Alignment
```c
// Always align data structures to cache lines
#define CACHE_ALIGN __attribute__((aligned(CACHE_LINE_SIZE)))

typedef struct {
    uint32_t data[16];
} CACHE_ALIGN cache_aligned_data_t;

// Use alignment for arrays
cache_aligned_data_t* create_aligned_array(size_t count) {
    return aligned_alloc(CACHE_LINE_SIZE, count * sizeof(cache_aligned_data_t));
}
```

### 2. Memory Access Optimization
```c
// Optimize memory access patterns
void cache_optimized_copy(void* dst, const void* src, size_t size) {
    size_t cache_line_elements = CACHE_LINE_SIZE / sizeof(uint32_t);
    uint32_t* d32 = (uint32_t*)dst;
    const uint32_t* s32 = (const uint32_t*)src;
    
    // Copy in cache-line sized chunks
    for (size_t i = 0; i < size / sizeof(uint32_t); i += cache_line_elements) {
        size_t chunk_size = cache_line_elements;
        if (i + chunk_size > size / sizeof(uint32_t)) {
            chunk_size = size / sizeof(uint32_t) - i;
        }
        
        // Copy chunk
        for (size_t j = 0; j < chunk_size; j++) {
            d32[i + j] = s32[i + j];
        }
    }
}
```

### 3. Cache Performance Monitoring
```c
// Monitor cache performance in development
#ifdef DEBUG
    #define MONITOR_CACHE_ACCESS(addr) \
        do { \
            record_cache_access(addr); \
        } while(0)
#else
    #define MONITOR_CACHE_ACCESS(addr) ((void)0)
#endif

void cache_aware_function(uint32_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        MONITOR_CACHE_ACCESS(&data[i]);
        data[i] *= 2;
    }
}
```

### 4. Cache-friendly Data Structures
```c
// Design cache-friendly data structures
typedef struct {
    uint32_t frequently_accessed[16];  // Hot data in first cache line
    uint32_t rarely_accessed[16];      // Cold data in second cache line
} CACHE_ALIGN hot_cold_data_t;

// Use SoA for better cache performance
typedef struct {
    uint32_t x[1000];
    uint32_t y[1000];
    uint32_t z[1000];
} structure_of_arrays_t;
```

## 🎯 Interview Questions

### Basic Questions
1. **What is cache-aware programming and why is it important?**
   - Optimizes code to work efficiently with CPU cache
   - Important for performance, especially in embedded systems

2. **What is a cache line and how does it affect performance?**
   - Cache line: smallest unit of cache transfer
   - Affects performance through cache misses and false sharing

3. **How can you prevent false sharing?**
   - Use cache line padding
   - Separate frequently accessed data
   - Align data structures to cache lines

### Advanced Questions
1. **How would you optimize a matrix multiplication for cache performance?**
   - Use cache-friendly access patterns
   - Implement blocking/tiling
   - Consider cache line size

2. **What are the trade-offs between AoS and SoA?**
   - AoS: Better for random access to all fields
   - SoA: Better for sequential access to specific fields

3. **How would you implement cache-aware memory allocation?**
   - Align allocations to cache lines
   - Use appropriate allocation sizes
   - Consider cache hierarchy

## 📚 Additional Resources

### Standards and Documentation
- **ARM Architecture Reference**: Cache specifications
- **CPU Cache Documentation**: Cache behavior and optimization
- **Performance Analysis Tools**: Cache profiling

### Related Topics
- **[Memory Pool Allocation](./Memory_Pool_Allocation.md)** - Efficient memory management
- **[Performance Optimization](./performance_optimization.md)** - General optimization techniques
- **[Multi-core Programming](./Multi_core_Programming.md)** - Cache considerations in multi-core
- **[DMA Buffer Management](./DMA_Buffer_Management.md)** - Cache coherency with DMA

### Tools and Libraries
- **Perf**: Linux performance analysis
- **Cachegrind**: Cache simulation and profiling
- **Custom cache monitors**: Embedded-specific solutions

---

**Next Topic:** [DMA Buffer Management](./DMA_Buffer_Management.md) → [Memory-Mapped I/O](./Memory_Mapped_IO.md)
