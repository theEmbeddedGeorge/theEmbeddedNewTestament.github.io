# Aligned Memory Allocation

## 📋 Table of Contents
- [Overview](#-overview)
- [Memory Alignment Concepts](#-memory-alignment-concepts)
- [Alignment Requirements](#-alignment-requirements)
- [Aligned Allocation Techniques](#-aligned-allocation-techniques)
- [Hardware-Specific Alignment](#-hardware-specific-alignment)
- [Performance Considerations](#-performance-considerations)
- [Common Pitfalls](#-common-pitfalls)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

Aligned memory allocation is crucial in embedded systems where hardware has specific alignment requirements for optimal performance and correct operation. This guide covers techniques for allocating memory with specific alignment constraints.

## 🔧 Memory Alignment Concepts

### What is Memory Alignment?
Memory alignment refers to placing data at memory addresses that are multiples of specific values (alignment boundaries).

```c
// Example: 4-byte alignment
struct aligned_data {
    uint32_t value;  // Requires 4-byte alignment
    uint8_t flag;    // Can be at any address
} __attribute__((aligned(4)));
```

### Why Alignment Matters in Embedded Systems
- **Performance**: Misaligned access can cause multiple memory cycles
- **Hardware Requirements**: Some peripherals require specific alignment
- **Cache Efficiency**: Aligned data improves cache performance
- **DMA Requirements**: DMA transfers often require specific alignment

## 📏 Alignment Requirements

### Common Alignment Requirements
```c
// Different alignment requirements
#define ALIGN_1   1   // No special alignment
#define ALIGN_2   2   // 2-byte alignment
#define ALIGN_4   4   // 4-byte alignment
#define ALIGN_8   8   // 8-byte alignment
#define ALIGN_16  16  // 16-byte alignment (cache line)
#define ALIGN_32  32  // 32-byte alignment (AVX)
#define ALIGN_64  64  // 64-byte alignment (AVX-512)
```

### ARM Architecture Alignment
```c
// ARM-specific alignment requirements
#ifdef __ARM_ARCH_7A__
    #define ARM_ALIGN 8   // ARMv7-A typically 8-byte aligned
#elif defined(__ARM_ARCH_8A__)
    #define ARM_ALIGN 16  // ARMv8-A often 16-byte aligned
#endif
```

## 🛠️ Aligned Allocation Techniques

### 1. Using `aligned_alloc()` (C11)
```c
#include <stdlib.h>

void* aligned_malloc_example() {
    // Allocate 1024 bytes with 16-byte alignment
    void* ptr = aligned_alloc(16, 1024);
    if (ptr == NULL) {
        // Handle allocation failure
        return NULL;
    }
    return ptr;
}
```

### 2. Using `posix_memalign()`
```c
#include <stdlib.h>

int posix_aligned_alloc_example() {
    void* ptr;
    int result = posix_memalign(&ptr, 16, 1024);
    if (result != 0) {
        // Handle error
        return -1;
    }
    // Use ptr...
    free(ptr);
    return 0;
}
```

### 3. Manual Alignment Calculation
```c
#include <stdint.h>

void* manual_aligned_alloc(size_t alignment, size_t size) {
    // Calculate required padding
    size_t padding = alignment - 1;
    size_t total_size = size + padding;
    
    // Allocate extra space
    void* raw_ptr = malloc(total_size);
    if (raw_ptr == NULL) {
        return NULL;
    }
    
    // Calculate aligned address
    uintptr_t raw_addr = (uintptr_t)raw_ptr;
    uintptr_t aligned_addr = (raw_addr + padding) & ~padding;
    
    return (void*)aligned_addr;
}
```

### 4. Using Compiler Attributes
```c
// GCC/Clang aligned attribute
struct __attribute__((aligned(16))) aligned_struct {
    uint32_t data[4];
    uint8_t flags;
};

// Allocate aligned structure
aligned_struct* create_aligned_struct() {
    return (aligned_struct*)malloc(sizeof(aligned_struct));
}
```

## 🔧 Hardware-Specific Alignment

### DMA Buffer Alignment
```c
// DMA buffer with cache line alignment
#define DMA_ALIGNMENT 64  // Cache line size

typedef struct {
    uint8_t buffer[1024];
} __attribute__((aligned(DMA_ALIGNMENT))) dma_buffer_t;

dma_buffer_t* allocate_dma_buffer() {
    dma_buffer_t* buffer = (dma_buffer_t*)aligned_alloc(
        DMA_ALIGNMENT, 
        sizeof(dma_buffer_t)
    );
    
    if (buffer) {
        // Ensure buffer is cache-line aligned for DMA
        // Flush cache if necessary
        __builtin___clear_cache((char*)buffer, 
                               (char*)buffer + sizeof(dma_buffer_t));
    }
    
    return buffer;
}
```

### SIMD Vector Alignment
```c
// SIMD vector alignment for ARM NEON
#ifdef __ARM_NEON
    #include <arm_neon.h>
    
    typedef struct {
        float32x4_t vector_data[4];  // 16-byte aligned
    } __attribute__((aligned(16))) neon_buffer_t;
    
    neon_buffer_t* allocate_neon_buffer() {
        return (neon_buffer_t*)aligned_alloc(16, sizeof(neon_buffer_t));
    }
#endif
```

### Peripheral Register Alignment
```c
// Hardware register structure alignment
typedef struct {
    volatile uint32_t control;    // 0x00
    volatile uint32_t status;     // 0x04
    volatile uint32_t data;       // 0x08
    volatile uint32_t reserved;   // 0x0C
} __attribute__((aligned(4))) peripheral_regs_t;

// Map peripheral registers
peripheral_regs_t* map_peripheral(uintptr_t base_addr) {
    // Ensure base address is 4-byte aligned
    if (base_addr & 0x3) {
        return NULL;  // Invalid alignment
    }
    return (peripheral_regs_t*)base_addr;
}
```

## ⚡ Performance Considerations

### Cache Line Alignment
```c
// Cache line aligned data structure
#define CACHE_LINE_SIZE 64

typedef struct {
    uint32_t data[16];  // 64 bytes
} __attribute__((aligned(CACHE_LINE_SIZE))) cache_aligned_data_t;

// Avoid false sharing in multi-core systems
typedef struct {
    uint32_t core1_data[16];
    char padding[CACHE_LINE_SIZE - 64];  // Padding to next cache line
    uint32_t core2_data[16];
} __attribute__((aligned(CACHE_LINE_SIZE))) multi_core_data_t;
```

### Memory Access Patterns
```c
// Optimized memory access with alignment
void aligned_memory_copy(void* dst, const void* src, size_t size) {
    // Ensure both pointers are 8-byte aligned
    if (((uintptr_t)dst & 0x7) == 0 && ((uintptr_t)src & 0x7) == 0) {
        // Use 64-bit transfers
        uint64_t* d64 = (uint64_t*)dst;
        const uint64_t* s64 = (const uint64_t*)src;
        size_t count = size / 8;
        
        for (size_t i = 0; i < count; i++) {
            d64[i] = s64[i];
        }
        
        // Handle remaining bytes
        uint8_t* d8 = (uint8_t*)(d64 + count);
        const uint8_t* s8 = (const uint8_t*)(s64 + count);
        for (size_t i = 0; i < (size % 8); i++) {
            d8[i] = s8[i];
        }
    } else {
        // Fallback to byte-by-byte copy
        memcpy(dst, src, size);
    }
}
```

## ⚠️ Common Pitfalls

### 1. Incorrect Alignment Calculation
```c
// WRONG: This doesn't guarantee alignment
void* wrong_aligned_alloc(size_t alignment, size_t size) {
    return malloc(size + alignment);  // Wrong approach
}

// CORRECT: Proper alignment calculation
void* correct_aligned_alloc(size_t alignment, size_t size) {
    size_t padding = alignment - 1;
    size_t total_size = size + padding;
    void* raw_ptr = malloc(total_size);
    if (!raw_ptr) return NULL;
    
    uintptr_t raw_addr = (uintptr_t)raw_ptr;
    uintptr_t aligned_addr = (raw_addr + padding) & ~padding;
    return (void*)aligned_addr;
}
```

### 2. Forgetting to Free Aligned Memory
```c
// WRONG: Using free() with aligned_alloc()
void* ptr = aligned_alloc(16, 1024);
// ... use ptr ...
free(ptr);  // May work but not guaranteed

// CORRECT: Use appropriate free function
void* ptr = aligned_alloc(16, 1024);
// ... use ptr ...
free(ptr);  // aligned_alloc uses standard free
```

### 3. Misaligned Structure Members
```c
// WRONG: Packed structure with alignment requirements
struct __attribute__((packed)) misaligned_struct {
    uint8_t flag;
    uint32_t data;  // May be misaligned
};

// CORRECT: Consider alignment in packed structures
struct __attribute__((packed)) correct_struct {
    uint8_t flag;
    uint8_t padding[3];  // Manual padding for alignment
    uint32_t data;
};
```

## ✅ Best Practices

### 1. Use Standard Library Functions
```c
// Prefer standard functions when available
void* allocate_aligned(size_t alignment, size_t size) {
    #if __STDC_VERSION__ >= 201112L
        return aligned_alloc(alignment, size);
    #else
        // Fallback implementation
        return manual_aligned_alloc(alignment, size);
    #endif
}
```

### 2. Validate Alignment Requirements
```c
bool is_valid_alignment(size_t alignment) {
    // Alignment must be power of 2
    return (alignment != 0) && ((alignment & (alignment - 1)) == 0);
}

void* safe_aligned_alloc(size_t alignment, size_t size) {
    if (!is_valid_alignment(alignment)) {
        return NULL;
    }
    return aligned_alloc(alignment, size);
}
```

### 3. Consider Memory Pool for Frequent Allocations
```c
typedef struct {
    void* pool;
    size_t alignment;
    size_t block_size;
    size_t total_blocks;
    size_t used_blocks;
} aligned_memory_pool_t;

aligned_memory_pool_t* create_aligned_pool(size_t alignment, 
                                          size_t block_size, 
                                          size_t num_blocks) {
    aligned_memory_pool_t* pool = malloc(sizeof(aligned_memory_pool_t));
    if (!pool) return NULL;
    
    pool->alignment = alignment;
    pool->block_size = block_size;
    pool->total_blocks = num_blocks;
    pool->used_blocks = 0;
    
    pool->pool = aligned_alloc(alignment, block_size * num_blocks);
    if (!pool->pool) {
        free(pool);
        return NULL;
    }
    
    return pool;
}
```

### 4. Debug Alignment Issues
```c
#include <assert.h>

void debug_alignment(void* ptr, size_t alignment) {
    uintptr_t addr = (uintptr_t)ptr;
    assert((addr % alignment) == 0);
    printf("Pointer %p is %zu-byte aligned\n", ptr, alignment);
}

// Usage
void* ptr = aligned_alloc(16, 1024);
debug_alignment(ptr, 16);
```

## 🎯 Interview Questions

### Basic Questions
1. **What is memory alignment and why is it important in embedded systems?**
   - Memory alignment places data at addresses that are multiples of specific values
   - Important for performance, hardware requirements, and cache efficiency

2. **How would you allocate memory with 16-byte alignment?**
   ```c
   void* ptr = aligned_alloc(16, size);
   // or
   void* ptr;
   posix_memalign(&ptr, 16, size);
   ```

3. **What happens if you access misaligned data on ARM?**
   - May cause alignment faults
   - Performance degradation due to multiple memory accesses
   - Hardware exceptions on strict alignment architectures

### Advanced Questions
1. **How would you implement a memory pool with specific alignment?**
   - Pre-allocate aligned memory blocks
   - Track free/used blocks
   - Ensure all allocations maintain alignment

2. **What are the trade-offs between different alignment sizes?**
   - Larger alignment: Better performance, more memory waste
   - Smaller alignment: Less waste, potential performance impact

3. **How would you handle alignment in a cross-platform embedded system?**
   - Use conditional compilation for different architectures
   - Implement alignment detection at runtime
   - Use portable alignment macros

## 📚 Additional Resources

### Standards and Documentation
- **C11 Standard**: `aligned_alloc()` specification
- **POSIX**: `posix_memalign()` documentation
- **ARM Architecture Reference Manual**: Alignment requirements
- **GCC Documentation**: `__attribute__((aligned))`

### Related Topics
- **[Memory Pool Allocation](./Memory_Pool_Allocation.md)** - Efficient memory management
- **[Structure Alignment](./Structure_Alignment.md)** - Data structure alignment
- **[DMA Buffer Management](./DMA_Buffer_Management.md)** - DMA-specific alignment
- **[Cache-Aware Programming](./Cache_Aware_Programming.md)** - Cache line alignment

### Tools and Libraries
- **Valgrind**: Memory alignment checking
- **AddressSanitizer**: Alignment violation detection
- **GCC/Clang**: Alignment attributes and built-ins

---

**Next Topic:** [Memory Fragmentation](./Memory_Fragmentation.md) → [Memory Leak Detection](./Memory_Leak_Detection.md)
