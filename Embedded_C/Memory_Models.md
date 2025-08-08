# Memory Models in Embedded Systems

## 📋 Table of Contents
- [Overview](#-overview)
- [Memory Layout](#-memory-layout)
- [Memory Segments](#-memory-segments)
- [Linker Scripts](#-linker-scripts)
- [Memory Protection](#-memory-protection)
- [Cache Behavior](#-cache-behavior)
- [Memory Ordering](#-memory-ordering)
- [Common Pitfalls](#-common-pitfalls)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

Understanding memory models is crucial for embedded systems programming. Memory layout, segmentation, and access patterns directly impact performance, reliability, and security of embedded applications.

### Key Concepts for Embedded Development
- **Memory segmentation** - Code, data, stack, heap organization
- **Memory protection** - Preventing unauthorized access
- **Cache behavior** - Optimizing memory access patterns
- **Memory ordering** - Ensuring correct execution order

## 🏗️ Memory Layout

### Typical Embedded Memory Layout
```c
/*
High Address
    ┌─────────────────┐
    │     Stack       │ ← Grows downward
    │                 │
    ├─────────────────┤
    │     Heap        │ ← Grows upward
    │                 │
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

### Memory Address Space
```c
// Memory address ranges for ARM Cortex-M
#define FLASH_BASE     0x08000000  // Code memory
#define SRAM_BASE      0x20000000  // Data memory
#define PERIPH_BASE    0x40000000  // Peripheral registers

// Memory sizes
#define FLASH_SIZE     (512 * 1024)  // 512KB
#define SRAM_SIZE      (64 * 1024)   // 64KB
#define STACK_SIZE     (8 * 1024)    // 8KB stack
```

## 📊 Memory Segments

### .text Segment (Code)
```c
// Code segment - contains executable instructions
void function_in_text(void) {
    // This function is stored in .text segment
    uint32_t local_var = 42;
    // Function code here...
}

// Constants in .text segment
const uint8_t lookup_table[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// Function pointers
typedef void (*callback_t)(void);
const callback_t callbacks[] = {function1, function2, function3};
```

### .data Segment (Initialized Data)
```c
// Initialized global variables
uint32_t global_counter = 0;
uint8_t sensor_data[64] = {0xAA, 0xBB, 0xCC, 0xDD};
const char* const_string = "Hello World";

// Initialized static variables
static uint16_t static_var = 0x1234;

// Initialized arrays
uint8_t buffer[1024] = {0};  // Zero-initialized
```

### .bss Segment (Uninitialized Data)
```c
// Uninitialized global variables (zeroed by startup code)
uint32_t uninitialized_var;
uint8_t large_buffer[8192];
static uint16_t static_uninit;

// These variables are automatically zeroed
// No space is used in the binary file
```

### Stack Segment
```c
// Stack variables
void stack_example(void) {
    int local_var = 42;           // Stack allocated
    uint8_t buffer[256];          // Stack array
    struct sensor_data data;       // Stack structure
    
    // Stack grows downward
    // Variables are automatically freed when function returns
}

// Stack overflow detection
void check_stack_usage(void) {
    uint8_t* stack_ptr;
    asm volatile ("mov %0, sp" : "=r" (stack_ptr));
    
    // Calculate stack usage
    uint32_t stack_used = STACK_BASE - (uint32_t)stack_ptr;
    if (stack_used > STACK_SIZE - 1024) {
        // Stack nearly full - take action
    }
}
```

### Heap Segment
```c
// Dynamic memory allocation
void heap_example(void) {
    uint8_t* buffer = malloc(1024);
    if (buffer != NULL) {
        // Use buffer...
        free(buffer);
    }
}

// Heap fragmentation monitoring
typedef struct {
    size_t total_blocks;
    size_t free_blocks;
    size_t largest_free_block;
} heap_stats_t;

heap_stats_t get_heap_stats(void) {
    heap_stats_t stats = {0};
    // Implementation depends on malloc implementation
    return stats;
}
```

## 🔧 Linker Scripts

### Basic Linker Script
```c
/* STM32F4 Linker Script */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
    /* Code section */
    .text : {
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
    } > FLASH
    
    /* Initialized data */
    .data : {
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
    } > SRAM AT> FLASH
    
    /* Uninitialized data */
    .bss : {
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM
}
```

### Custom Sections
```c
// Custom section for critical data
__attribute__((section(".critical_data")))
uint8_t critical_buffer[256];

// Custom section for interrupt vectors
__attribute__((section(".isr_vector")))
void (* const isr_vector_table[])(void) = {
    (void (*)(void))(&_estack),  // Stack pointer
    Reset_Handler,                // Reset handler
    NMI_Handler,                  // NMI handler
    HardFault_Handler,            // Hard fault handler
    // ... more handlers
};

// Custom section for configuration data
__attribute__((section(".config")))
const struct system_config config = {
    .clock_freq = 168000000,
    .uart_baud = 115200,
    .adc_resolution = 12
};
```

## 🛡️ Memory Protection

### MPU Configuration
```c
// Memory Protection Unit setup
typedef struct {
    uint32_t base_address;
    uint32_t size;
    uint32_t access_permissions;
    uint32_t attributes;
} mpu_region_t;

// Configure MPU regions
void configure_mpu(void) {
    // Region 0: Flash (read-only, executable)
    mpu_region_t flash_region = {
        .base_address = 0x08000000,
        .size = 512 * 1024,
        .access_permissions = MPU_AP_PRIV,
        .attributes = MPU_TEX_LEVEL0 | MPU_CACHEABLE
    };
    
    // Region 1: SRAM (read-write, non-executable)
    mpu_region_t sram_region = {
        .base_address = 0x20000000,
        .size = 64 * 1024,
        .access_permissions = MPU_AP_FULL,
        .attributes = MPU_TEX_LEVEL0 | MPU_CACHEABLE
    };
    
    // Apply MPU configuration
    apply_mpu_config(&flash_region, &sram_region);
}
```

### Stack Protection
```c
// Stack canary for overflow detection
uint32_t __stack_chk_guard = 0xDEADBEEF;

void __stack_chk_fail(void) {
    // Stack overflow detected
    // Implement recovery mechanism
    system_reset();
}

// Stack usage monitoring
typedef struct {
    uint32_t stack_base;
    uint32_t stack_size;
    uint32_t watermark;
} stack_monitor_t;

void update_stack_watermark(void) {
    uint8_t* current_sp;
    asm volatile ("mov %0, sp" : "=r" (current_sp));
    
    uint32_t stack_used = stack_monitor.stack_base - (uint32_t)current_sp;
    if (stack_used > stack_monitor.watermark) {
        stack_monitor.watermark = stack_used;
    }
}
```

## ⚡ Cache Behavior

### Cache-Aware Programming
```c
// Cache line size (typically 32 bytes for ARM Cortex-M)
#define CACHE_LINE_SIZE 32

// Align data structures to cache lines
typedef struct {
    uint32_t data[8];  // 32 bytes, fits in one cache line
} __attribute__((aligned(CACHE_LINE_SIZE))) cache_aligned_t;

// Avoid false sharing in multi-core systems
typedef struct {
    uint32_t core1_data;
    uint8_t padding[CACHE_LINE_SIZE - sizeof(uint32_t)];
    uint32_t core2_data;
} __attribute__((aligned(CACHE_LINE_SIZE))) shared_data_t;
```

### Memory Access Patterns
```c
// Good: Sequential access pattern
void sequential_access(uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] = i;  // Sequential access
    }
}

// Bad: Random access pattern
void random_access(uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        size_t random_index = get_random_index();
        buffer[random_index] = i;  // Random access
    }
}

// Better: Block-based access
void block_access(uint8_t* buffer, size_t size) {
    const size_t block_size = CACHE_LINE_SIZE;
    
    for (size_t offset = 0; offset < size; offset += block_size) {
        size_t block_end = (offset + block_size < size) ? 
                          offset + block_size : size;
        
        for (size_t i = offset; i < block_end; i++) {
            buffer[i] = i;  // Access within cache line
        }
    }
}
```

## 🔄 Memory Ordering

### Memory Barriers
```c
// Memory barriers for ARM Cortex-M
void memory_barrier_example(void) {
    uint32_t flag = 0;
    uint32_t data = 42;
    
    // Write data first
    data = 100;
    
    // Memory barrier ensures data is written before flag
    __asm volatile ("dmb" : : : "memory");
    
    // Set flag to indicate data is ready
    flag = 1;
    
    // Memory barrier ensures flag is visible to other cores
    __asm volatile ("dmb" : : : "memory");
}

// Atomic operations
uint32_t atomic_increment(uint32_t* counter) {
    uint32_t old_value;
    
    __asm volatile (
        "ldrex %0, [%1]\n"
        "add %0, %0, #1\n"
        "strex r1, %0, [%1]\n"
        "teq r1, #0\n"
        "bne 1b"
        : "=r" (old_value)
        : "r" (counter)
        : "r1", "cc"
    );
    
    return old_value;
}
```

### Volatile Usage
```c
// Hardware registers must be volatile
volatile uint32_t* const GPIO_ODR = (uint32_t*)0x40020014;

// Shared variables in multi-threaded code
volatile uint32_t shared_flag = 0;

// Interrupt-safe flag checking
uint32_t check_flag_safely(void) {
    uint32_t flag_value;
    
    // Disable interrupts
    __asm volatile ("cpsid i" : : : "memory");
    
    flag_value = shared_flag;
    
    // Re-enable interrupts
    __asm volatile ("cpsie i" : : : "memory");
    
    return flag_value;
}
```

## ⚠️ Common Pitfalls

### Memory Alignment Issues
```c
// BAD: Unaligned access
struct unaligned_struct {
    uint8_t byte;
    uint32_t word;  // May be unaligned
};

// GOOD: Proper alignment
struct aligned_struct {
    uint32_t word;  // Naturally aligned
    uint8_t byte;
    uint8_t padding[3];  // Explicit padding
} __attribute__((packed));

// Check alignment
#define IS_ALIGNED(ptr, align) (((uintptr_t)(ptr) % (align)) == 0)
```

### Stack Overflow
```c
// BAD: Large stack allocation
void large_stack_allocation(void) {
    uint8_t large_buffer[8192];  // May cause stack overflow
    // Use buffer...
}

// GOOD: Use heap for large allocations
void safe_large_allocation(void) {
    uint8_t* buffer = malloc(8192);
    if (buffer != NULL) {
        // Use buffer...
        free(buffer);
    }
}
```

### Memory Leaks
```c
// BAD: Memory leak in loop
void memory_leak_example(void) {
    for (int i = 0; i < 1000; i++) {
        uint8_t* buffer = malloc(1024);
        // Use buffer...
        // Forgot to free - memory leak!
    }
}

// GOOD: Proper cleanup
void proper_cleanup_example(void) {
    for (int i = 0; i < 1000; i++) {
        uint8_t* buffer = malloc(1024);
        if (buffer != NULL) {
            // Use buffer...
            free(buffer);
        }
    }
}
```

## ✅ Best Practices

### Memory Management Guidelines
```c
// 1. Use static allocation when possible
static uint8_t static_buffer[1024];  // No runtime overhead

// 2. Check allocation success
void* ptr = malloc(size);
if (ptr == NULL) {
    // Handle allocation failure
    return ERROR_CODE;
}

// 3. Use appropriate data types
uint8_t small_value = 42;      // 0-255
uint16_t medium_value = 1000;  // 0-65535
uint32_t large_value = 1000000; // 0-4294967295

// 4. Align data structures
typedef struct {
    uint32_t word;
    uint16_t halfword;
    uint8_t byte;
    uint8_t padding;
} __attribute__((aligned(4))) aligned_struct_t;

// 5. Monitor memory usage
void print_memory_usage(void) {
    extern uint32_t _estack;
    extern uint32_t _sstack;
    extern uint32_t _heap_start;
    extern uint32_t _heap_end;
    
    uint32_t stack_size = (uint32_t)&_estack - (uint32_t)&_sstack;
    uint32_t heap_size = (uint32_t)&_heap_end - (uint32_t)&_heap_start;
    
    printf("Stack size: %lu bytes\n", stack_size);
    printf("Heap size: %lu bytes\n", heap_size);
}
```

### Embedded-Specific Patterns
```c
// Memory pool for fixed-size allocations
#define POOL_BLOCK_SIZE 32
#define POOL_BLOCK_COUNT 64

typedef struct {
    uint8_t pool[POOL_BLOCK_SIZE * POOL_BLOCK_COUNT];
    uint8_t used[POOL_BLOCK_COUNT / 8];  // Bitmap for used blocks
} memory_pool_t;

static memory_pool_t system_pool;

// Allocate from pool
void* pool_alloc(void) {
    for (int i = 0; i < POOL_BLOCK_COUNT; i++) {
        int byte_index = i / 8;
        int bit_index = i % 8;
        
        if (!(system_pool.used[byte_index] & (1 << bit_index))) {
            system_pool.used[byte_index] |= (1 << bit_index);
            return &system_pool.pool[i * POOL_BLOCK_SIZE];
        }
    }
    return NULL;  // No free blocks
}

// Free to pool
void pool_free(void* ptr) {
    if (ptr == NULL) return;
    
    uint8_t* block = (uint8_t*)ptr;
    int index = (block - system_pool.pool) / POOL_BLOCK_SIZE;
    
    if (index >= 0 && index < POOL_BLOCK_COUNT) {
        int byte_index = index / 8;
        int bit_index = index % 8;
        system_pool.used[byte_index] &= ~(1 << bit_index);
    }
}
```

## 🎯 Interview Questions

### Basic Concepts
1. **What are the different memory segments in embedded systems?**
   - .text: Code and constants
   - .data: Initialized global variables
   - .bss: Uninitialized global variables
   - Stack: Local variables and function calls
   - Heap: Dynamic memory allocation

2. **How do you prevent stack overflow in embedded systems?**
   - Monitor stack usage
   - Use static allocation for large buffers
   - Implement stack canaries
   - Set appropriate stack size

3. **What is memory alignment and why is it important?**
   - Data alignment for efficient access
   - Hardware requirements for certain operations
   - Cache line alignment for performance

### Advanced Topics
1. **How would you implement memory protection in an embedded system?**
   - Use MPU/MMU
   - Configure memory regions
   - Set access permissions
   - Monitor memory violations

2. **What are the trade-offs between different memory allocation strategies?**
   - Static: Predictable, no fragmentation, limited flexibility
   - Dynamic: Flexible, potential fragmentation, runtime overhead
   - Pool: Fixed-size, no fragmentation, limited sizes

3. **How do you optimize memory access patterns for cache performance?**
   - Sequential access patterns
   - Cache line alignment
   - Avoid false sharing
   - Use appropriate data structures

## 📚 Additional Resources

- **Books**: "Making Embedded Systems" by Elecia White
- **Standards**: ARM Cortex-M programming guide
- **Tools**: Linker script documentation, memory profilers
- **Documentation**: MPU/MMU configuration guides

**Next Topic:** [Assembly Integration](./Assembly_Integration.md) → [Advanced Memory Management](./Advanced_Memory_Management.md)
