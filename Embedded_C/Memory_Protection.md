# Memory Protection

## 📋 Table of Contents
- [Overview](#-overview)
- [Memory Protection Units (MPU)](#-memory-protection-units-mpu)
- [Memory Management Units (MMU)](#-memory-management-units-mmu)
- [ARM TrustZone](#-arm-trustzone)
- [Memory Access Control](#-memory-access-control)
- [Stack Protection](#-stack-protection)
- [Heap Protection](#-heap-protection)
- [Code Protection](#-code-protection)
- [Real-time Protection](#-real-time-protection)
- [Common Pitfalls](#-common-pitfalls)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

Memory protection mechanisms prevent unauthorized access to memory regions, ensuring system security, stability, and reliability. In embedded systems, memory protection is crucial for isolating tasks, preventing buffer overflows, and maintaining system integrity.

## 🛡️ Memory Protection Units (MPU)

### MPU Configuration
```c
// MPU region configuration
typedef struct {
    uint32_t base_address;
    uint32_t size;
    uint32_t access_permissions;
    uint32_t attributes;
} mpu_region_t;

#define MPU_REGION_PRIVILEGED_RW    0x00000006
#define MPU_REGION_USER_RW          0x00000003
#define MPU_REGION_PRIVILEGED_RO    0x00000002
#define MPU_REGION_USER_RO          0x00000001
#define MPU_REGION_NO_ACCESS        0x00000000

// Configure MPU region
void configure_mpu_region(uint8_t region, mpu_region_t* config) {
    // Disable MPU region
    MPU->RNR = region;
    MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;
    
    // Configure region
    MPU->RBAR = config->base_address | region;
    MPU->RASR = config->size | config->access_permissions | config->attributes;
    
    // Enable MPU
    MPU->CTRL |= MPU_CTRL_ENABLE_Msk;
    __DSB();
    __ISB();
}
```

### Memory Region Protection
```c
// Protect different memory regions
void setup_memory_protection(void) {
    mpu_region_t regions[] = {
        // Flash memory - read-only for user
        {
            .base_address = 0x08000000,
            .size = MPU_REGION_SIZE_1MB,
            .access_permissions = MPU_REGION_USER_RO,
            .attributes = MPU_REGION_CACHEABLE
        },
        // SRAM - read-write for user
        {
            .base_address = 0x20000000,
            .size = MPU_REGION_SIZE_128KB,
            .access_permissions = MPU_REGION_USER_RW,
            .attributes = MPU_REGION_CACHEABLE
        },
        // Peripherals - privileged only
        {
            .base_address = 0x40000000,
            .size = MPU_REGION_SIZE_1MB,
            .access_permissions = MPU_REGION_PRIVILEGED_RW,
            .attributes = MPU_REGION_DEVICE
        }
    };
    
    // Configure all regions
    for (int i = 0; i < sizeof(regions)/sizeof(regions[0]); i++) {
        configure_mpu_region(i, &regions[i]);
    }
}
```

### Task Isolation
```c
// Isolate tasks using MPU
typedef struct {
    uint32_t stack_start;
    uint32_t stack_size;
    uint32_t data_start;
    uint32_t data_size;
    uint8_t priority;
} task_memory_config_t;

void isolate_task_memory(uint8_t task_id, task_memory_config_t* config) {
    mpu_region_t task_region = {
        .base_address = config->stack_start,
        .size = config->stack_size,
        .access_permissions = MPU_REGION_USER_RW,
        .attributes = MPU_REGION_CACHEABLE
    };
    
    // Configure MPU region for task
    configure_mpu_region(task_id, &task_region);
}

// Switch task context with memory protection
void switch_task_context(uint8_t task_id) {
    // Save current task context
    save_current_context();
    
    // Configure MPU for new task
    task_memory_config_t* new_task_config = get_task_config(task_id);
    isolate_task_memory(task_id, new_task_config);
    
    // Restore new task context
    restore_task_context(task_id);
}
```

## 🏗️ Memory Management Units (MMU)

### MMU Configuration
```c
// MMU page table entry
typedef struct {
    uint32_t physical_address;
    uint32_t virtual_address;
    uint32_t page_size;
    uint32_t permissions;
    uint32_t attributes;
} mmu_page_entry_t;

#define MMU_PAGE_4KB     0x00000000
#define MMU_PAGE_64KB    0x00000001
#define MMU_PAGE_1MB     0x00000002
#define MMU_PAGE_16MB    0x00000003

#define MMU_PERM_NONE    0x00000000
#define MMU_PERM_RO      0x00000001
#define MMU_PERM_RW      0x00000003

// Configure MMU page
void configure_mmu_page(mmu_page_entry_t* entry) {
    // Calculate page table entry
    uint32_t pte = (entry->physical_address & 0xFFFFF000) |
                   (entry->page_size << 2) |
                   (entry->permissions << 1) |
                   0x00000001;  // Valid bit
    
    // Write to page table
    uint32_t* page_table = get_page_table_base();
    uint32_t page_index = entry->virtual_address >> 12;
    page_table[page_index] = pte;
    
    // Invalidate TLB
    __asm volatile ("mcr p15, 0, %0, c8, c7, 0" : : "r" (0));
}
```

### Virtual Memory Mapping
```c
// Map virtual to physical memory
void map_virtual_memory(uint32_t virtual_addr, uint32_t physical_addr, 
                       uint32_t size, uint32_t permissions) {
    mmu_page_entry_t entry = {
        .physical_address = physical_addr,
        .virtual_address = virtual_addr,
        .page_size = MMU_PAGE_4KB,
        .permissions = permissions,
        .attributes = 0
    };
    
    // Map each page
    for (uint32_t offset = 0; offset < size; offset += 4096) {
        entry.virtual_address = virtual_addr + offset;
        entry.physical_address = physical_addr + offset;
        configure_mmu_page(&entry);
    }
}

// Setup virtual memory layout
void setup_virtual_memory(void) {
    // Map kernel space
    map_virtual_memory(0xC0000000, 0x08000000, 0x1000000, MMU_PERM_RO);
    
    // Map user space
    map_virtual_memory(0x00000000, 0x20000000, 0x100000, MMU_PERM_RW);
    
    // Map device space
    map_virtual_memory(0x40000000, 0x40000000, 0x1000000, MMU_PERM_RW);
}
```

## 🔒 ARM TrustZone

### TrustZone Configuration
```c
// TrustZone security state
typedef enum {
    SECURE_STATE = 0,
    NON_SECURE_STATE = 1
} trustzone_state_t;

// Configure TrustZone
void configure_trustzone(void) {
    // Configure secure and non-secure regions
    SAU->CTRL = SAU_CTRL_ENABLE_Msk;
    
    // Region 0: Secure flash
    SAU->RNR = 0;
    SAU->RBAR = 0x08000000;  // Flash base
    SAU->RLAR = 0x080FFFFF | SAU_RLAR_ENABLE_Msk;
    
    // Region 1: Non-secure RAM
    SAU->RNR = 1;
    SAU->RBAR = 0x20000000;  // RAM base
    SAU->RLAR = 0x2001FFFF | SAU_RLAR_ENABLE_Msk;
    
    // Enable TrustZone
    SCB->AIRCR = SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIVDEFENA_Msk;
}

// Switch to secure state
void enter_secure_state(void) {
    __asm volatile (
        "smc #0\n"
        : : : "r0", "r1", "r2", "r3", "r12", "lr"
    );
}

// Switch to non-secure state
void enter_non_secure_state(void) {
    __asm volatile (
        "bxns lr\n"
    );
}
```

### Secure Service Interface
```c
// Secure service call
typedef struct {
    uint32_t service_id;
    uint32_t parameters[4];
    uint32_t return_value;
} secure_service_t;

uint32_t call_secure_service(uint32_t service_id, uint32_t* params) {
    secure_service_t service = {
        .service_id = service_id,
        .parameters = {params[0], params[1], params[2], params[3]}
    };
    
    // Call secure service
    __asm volatile (
        "mov r0, %0\n"
        "smc #1\n"
        : : "r" (&service) : "r0", "r1", "r2", "r3"
    );
    
    return service.return_value;
}

// Example secure service
uint32_t secure_crypto_service(uint32_t operation, uint32_t* data) {
    uint32_t params[4] = {operation, (uint32_t)data, 0, 0};
    return call_secure_service(0x1001, params);
}
```

## 🔐 Memory Access Control

### Access Permission Checking
```c
// Memory access permissions
typedef enum {
    ACCESS_NONE = 0,
    ACCESS_READ = 1,
    ACCESS_WRITE = 2,
    ACCESS_EXECUTE = 4,
    ACCESS_ALL = 7
} memory_access_t;

typedef struct {
    uint32_t start_address;
    uint32_t end_address;
    memory_access_t permissions;
    bool is_secure;
} memory_region_t;

// Check memory access permissions
bool check_memory_access(uint32_t address, memory_access_t access) {
    memory_region_t* regions = get_memory_regions();
    int region_count = get_region_count();
    
    for (int i = 0; i < region_count; i++) {
        if (address >= regions[i].start_address && 
            address <= regions[i].end_address) {
            
            // Check if current state matches region security
            bool current_secure = is_secure_state();
            if (regions[i].is_secure != current_secure) {
                return false;  // Security state mismatch
            }
            
            // Check permissions
            return (regions[i].permissions & access) == access;
        }
    }
    
    return false;  // Address not in any region
}

// Memory access wrapper
void* secure_memory_access(uint32_t address, memory_access_t access) {
    if (!check_memory_access(address, access)) {
        // Trigger memory protection fault
        trigger_memory_fault(address, access);
        return NULL;
    }
    
    return (void*)address;
}
```

### Buffer Overflow Protection
```c
// Protected buffer with bounds checking
typedef struct {
    uint8_t* data;
    size_t size;
    uint8_t* guard_zone;
    size_t guard_size;
} protected_buffer_t;

protected_buffer_t* create_protected_buffer(size_t size) {
    protected_buffer_t* buffer = malloc(sizeof(protected_buffer_t));
    if (!buffer) return NULL;
    
    // Allocate guard zones and data
    size_t guard_size = 64;  // 64-byte guard zones
    buffer->guard_size = guard_size;
    buffer->size = size;
    
    buffer->guard_zone = malloc(guard_size * 2 + size);
    if (!buffer->guard_zone) {
        free(buffer);
        return NULL;
    }
    
    buffer->data = buffer->guard_zone + guard_size;
    
    // Initialize guard zones
    memset(buffer->guard_zone, 0xAA, guard_size);
    memset(buffer->data + size, 0xAA, guard_size);
    
    return buffer;
}

bool check_buffer_integrity(protected_buffer_t* buffer) {
    // Check guard zones
    for (size_t i = 0; i < buffer->guard_size; i++) {
        if (buffer->guard_zone[i] != 0xAA) {
            return false;  // Lower guard zone corrupted
        }
        if (buffer->data[buffer->size + i] != 0xAA) {
            return false;  // Upper guard zone corrupted
        }
    }
    return true;
}

void destroy_protected_buffer(protected_buffer_t* buffer) {
    if (buffer) {
        if (!check_buffer_integrity(buffer)) {
            printf("WARNING: Buffer overflow detected!\n");
        }
        free(buffer->guard_zone);
        free(buffer);
    }
}
```

## 🛡️ Stack Protection

### Stack Canary Implementation
```c
// Stack canary for overflow detection
typedef struct {
    uint32_t canary;
    uint8_t data[];
} stack_frame_t;

// Generate random canary
uint32_t generate_canary(void) {
    // Use hardware random number generator if available
    return 0xDEADBEEF;  // For demonstration
}

// Create protected stack frame
stack_frame_t* create_protected_stack_frame(size_t data_size) {
    stack_frame_t* frame = malloc(sizeof(stack_frame_t) + data_size);
    if (!frame) return NULL;
    
    frame->canary = generate_canary();
    return frame;
}

// Check stack frame integrity
bool check_stack_integrity(stack_frame_t* frame) {
    return frame->canary == generate_canary();
}

// Function with stack protection
void function_with_stack_protection(void) {
    stack_frame_t* frame = create_protected_stack_frame(100);
    if (!frame) return;
    
    // Use frame->data...
    
    if (!check_stack_integrity(frame)) {
        printf("STACK OVERFLOW DETECTED!\n");
        // Handle overflow
    }
    
    free(frame);
}
```

### Stack Pointer Validation
```c
// Validate stack pointer bounds
typedef struct {
    uint32_t stack_start;
    uint32_t stack_end;
    uint32_t current_sp;
} stack_validator_t;

bool validate_stack_pointer(stack_validator_t* validator) {
    uint32_t sp;
    __asm volatile ("mov %0, sp" : "=r" (sp));
    
    validator->current_sp = sp;
    
    return (sp >= validator->stack_start && sp <= validator->stack_end);
}

// Stack overflow detection
void check_stack_overflow(stack_validator_t* validator) {
    if (!validate_stack_pointer(validator)) {
        printf("STACK POINTER OUT OF BOUNDS!\n");
        // Handle stack overflow
        handle_stack_overflow();
    }
}
```

## 🗄️ Heap Protection

### Heap Corruption Detection
```c
// Protected heap allocation
typedef struct {
    uint32_t magic_start;
    size_t size;
    uint8_t data[];
} protected_heap_block_t;

#define HEAP_MAGIC_START 0xDEADBEEF
#define HEAP_MAGIC_END   0xCAFEBABE

void* protected_malloc(size_t size) {
    protected_heap_block_t* block = malloc(sizeof(protected_heap_block_t) + 
                                         size + sizeof(uint32_t));
    if (!block) return NULL;
    
    block->magic_start = HEAP_MAGIC_START;
    block->size = size;
    
    // Add end magic
    uint32_t* end_magic = (uint32_t*)(block->data + size);
    *end_magic = HEAP_MAGIC_END;
    
    return block->data;
}

bool check_heap_block_integrity(void* ptr) {
    protected_heap_block_t* block = (protected_heap_block_t*)((char*)ptr - 
                                                             offsetof(protected_heap_block_t, data));
    
    // Check start magic
    if (block->magic_start != HEAP_MAGIC_START) {
        return false;
    }
    
    // Check end magic
    uint32_t* end_magic = (uint32_t*)(block->data + block->size);
    if (*end_magic != HEAP_MAGIC_END) {
        return false;
    }
    
    return true;
}

void protected_free(void* ptr) {
    if (!check_heap_block_integrity(ptr)) {
        printf("HEAP CORRUPTION DETECTED!\n");
        // Handle corruption
        return;
    }
    
    protected_heap_block_t* block = (protected_heap_block_t*)((char*)ptr - 
                                                             offsetof(protected_heap_block_t, data));
    free(block);
}
```

## 🔒 Code Protection

### Code Integrity Checking
```c
// Code integrity verification
typedef struct {
    uint32_t start_address;
    uint32_t size;
    uint32_t checksum;
} code_region_t;

uint32_t calculate_checksum(uint32_t start, uint32_t size) {
    uint32_t checksum = 0;
    uint32_t* ptr = (uint32_t*)start;
    
    for (uint32_t i = 0; i < size / 4; i++) {
        checksum ^= ptr[i];
    }
    
    return checksum;
}

bool verify_code_integrity(code_region_t* region) {
    uint32_t current_checksum = calculate_checksum(region->start_address, 
                                                  region->size);
    return current_checksum == region->checksum;
}

// Verify all code regions
void verify_all_code_regions(void) {
    code_region_t* regions = get_code_regions();
    int region_count = get_code_region_count();
    
    for (int i = 0; i < region_count; i++) {
        if (!verify_code_integrity(&regions[i])) {
            printf("CODE INTEGRITY VIOLATION IN REGION %d!\n", i);
            // Handle code corruption
            handle_code_corruption();
        }
    }
}
```

### Execute-Only Memory
```c
// Configure execute-only memory regions
void configure_execute_only_memory(uint32_t start, uint32_t size) {
    mpu_region_t region = {
        .base_address = start,
        .size = size,
        .access_permissions = MPU_REGION_EXECUTE_ONLY,
        .attributes = MPU_REGION_CACHEABLE
    };
    
    configure_mpu_region(0, &region);
}

// Protect code from read access
void protect_code_from_reading(uint32_t code_start, uint32_t code_size) {
    // Configure MPU to make code execute-only
    configure_execute_only_memory(code_start, code_size);
}
```

## ⏱️ Real-time Protection

### Memory Protection Fault Handler
```c
// Memory protection fault handler
void memory_protection_fault_handler(void) {
    uint32_t fault_address;
    uint32_t fault_type;
    
    // Read fault information
    __asm volatile (
        "mrc p15, 0, %0, c6, c0, 0\n"  // Read FAR
        "mrc p15, 0, %1, c5, c0, 0\n"  // Read FSR
        : "=r" (fault_address), "=r" (fault_type)
    );
    
    printf("MEMORY PROTECTION FAULT!\n");
    printf("Address: 0x%08X\n", fault_address);
    printf("Type: 0x%08X\n", fault_type);
    
    // Handle fault based on type
    if (fault_type & 0x1) {
        // Data abort
        handle_data_abort(fault_address);
    } else {
        // Instruction abort
        handle_instruction_abort(fault_address);
    }
}

void handle_data_abort(uint32_t address) {
    // Check if it's a recoverable fault
    if (is_recoverable_fault(address)) {
        // Attempt recovery
        attempt_fault_recovery(address);
    } else {
        // System reset or error handling
        system_reset();
    }
}
```

### Protected Critical Sections
```c
// Memory protection for critical sections
typedef struct {
    uint32_t original_permissions;
    bool protection_enabled;
} critical_section_protection_t;

void enter_critical_section_protection(critical_section_protection_t* protection) {
    // Save current memory permissions
    protection->original_permissions = get_current_memory_permissions();
    
    // Enable strict memory protection
    enable_strict_memory_protection();
    protection->protection_enabled = true;
}

void exit_critical_section_protection(critical_section_protection_t* protection) {
    if (protection->protection_enabled) {
        // Restore original permissions
        set_memory_permissions(protection->original_permissions);
        protection->protection_enabled = false;
    }
}

// Usage example
void critical_function(void) {
    critical_section_protection_t protection;
    
    enter_critical_section_protection(&protection);
    
    // Critical code with enhanced protection
    perform_critical_operation();
    
    exit_critical_section_protection(&protection);
}
```

## ⚠️ Common Pitfalls

### 1. Incorrect MPU Configuration
```c
// WRONG: Overlapping MPU regions
void incorrect_mpu_config(void) {
    mpu_region_t region1 = {0x20000000, 0x10000, MPU_REGION_USER_RW, 0};
    mpu_region_t region2 = {0x20008000, 0x10000, MPU_REGION_USER_RW, 0};
    // Regions overlap - undefined behavior
    
    configure_mpu_region(0, &region1);
    configure_mpu_region(1, &region2);
}

// CORRECT: Non-overlapping regions
void correct_mpu_config(void) {
    mpu_region_t region1 = {0x20000000, 0x8000, MPU_REGION_USER_RW, 0};
    mpu_region_t region2 = {0x20008000, 0x8000, MPU_REGION_USER_RW, 0};
    // Regions don't overlap
    
    configure_mpu_region(0, &region1);
    configure_mpu_region(1, &region2);
}
```

### 2. Ignoring Memory Alignment
```c
// WRONG: Unaligned memory access
void unaligned_access(void) {
    uint8_t buffer[100];
    uint32_t* ptr = (uint32_t*)(buffer + 1);  // Unaligned pointer
    *ptr = 0x12345678;  // May cause fault on some architectures
}

// CORRECT: Aligned memory access
void aligned_access(void) {
    uint8_t buffer[100];
    uint32_t* ptr = (uint32_t*)((uintptr_t)(buffer + 1) & ~0x3);  // Align
    *ptr = 0x12345678;
}
```

### 3. Not Handling Protection Faults
```c
// WRONG: No fault handling
void unprotected_function(void) {
    uint8_t* ptr = (uint8_t*)0x40000000;  // Peripheral address
    *ptr = 0xFF;  // May cause protection fault
    // No fault handler - system may crash
}

// CORRECT: Proper fault handling
void protected_function(void) {
    // Install fault handler
    install_memory_fault_handler();
    
    uint8_t* ptr = (uint8_t*)0x40000000;
    *ptr = 0xFF;  // Fault will be handled gracefully
}
```

## ✅ Best Practices

### 1. Memory Protection Configuration
```c
// Configure memory protection at startup
void initialize_memory_protection(void) {
    // Disable MPU during configuration
    MPU->CTRL = 0;
    
    // Configure regions
    setup_memory_regions();
    
    // Enable MPU
    MPU->CTRL = MPU_CTRL_ENABLE_Msk;
    
    // Enable fault handlers
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
    
    // Set fault handler
    NVIC_SetPriority(MemoryManagement_IRQn, 0);
    NVIC_EnableIRQ(MemoryManagement_IRQn);
}
```

### 2. Memory Protection Monitoring
```c
// Monitor memory protection violations
typedef struct {
    uint32_t fault_count;
    uint32_t last_fault_address;
    uint32_t last_fault_type;
} memory_protection_monitor_t;

static memory_protection_monitor_t protection_monitor = {0};

void update_protection_monitor(uint32_t address, uint32_t type) {
    protection_monitor.fault_count++;
    protection_monitor.last_fault_address = address;
    protection_monitor.last_fault_type = type;
    
    if (protection_monitor.fault_count > 10) {
        printf("WARNING: High memory protection fault rate\n");
        // Implement recovery or shutdown
    }
}
```

### 3. Secure Memory Management
```c
// Secure memory allocation
void* secure_malloc(size_t size) {
    // Check if allocation is allowed in current security state
    if (!is_allocation_allowed()) {
        return NULL;
    }
    
    void* ptr = malloc(size);
    if (ptr) {
        // Mark memory as secure if in secure state
        if (is_secure_state()) {
            mark_memory_secure(ptr, size);
        }
    }
    
    return ptr;
}

bool is_allocation_allowed(void) {
    // Check current security state and permissions
    return true;  // Implementation depends on system
}

void mark_memory_secure(void* ptr, size_t size) {
    // Mark memory region as secure in MPU/MMU
    // Implementation depends on hardware
}
```

## 🎯 Interview Questions

### Basic Questions
1. **What is memory protection and why is it important in embedded systems?**
   - Memory protection prevents unauthorized access to memory regions
   - Important for security, stability, and task isolation

2. **What are the differences between MPU and MMU?**
   - MPU: Simple, fixed regions, no virtual memory
   - MMU: Complex, flexible, virtual memory support

3. **How does ARM TrustZone provide security?**
   - Separates secure and non-secure worlds
   - Provides hardware-based security isolation

### Advanced Questions
1. **How would you implement memory protection in a real-time system?**
   - Use MPU for simple protection
   - Configure regions for different tasks
   - Handle protection faults efficiently

2. **What are the trade-offs of different memory protection mechanisms?**
   - MPU: Simple but limited flexibility
   - MMU: Complex but powerful
   - TrustZone: Security but additional complexity

3. **How would you detect and handle memory protection violations?**
   - Install fault handlers
   - Monitor fault rates
   - Implement recovery mechanisms

## 📚 Additional Resources

### Standards and Documentation
- **ARM Architecture Reference Manual**: MPU/MMU specifications
- **ARM TrustZone Documentation**: Security features
- **C Standard**: Memory safety requirements

### Related Topics
- **[Stack Overflow Prevention](./Stack_Overflow_Prevention.md)** - Stack protection
- **[Memory Pool Allocation](./Memory_Pool_Allocation.md)** - Safe memory management
- **[Cache-Aware Programming](./Cache_Aware_Programming.md)** - Memory access patterns
- **[Embedded Security](./embedded_security.md)** - Security considerations

### Tools and Libraries
- **ARM TrustZone**: Hardware security features
- **MPU/MMU configuration tools**: Memory protection setup
- **Memory protection monitors**: Runtime protection verification

---

**Next Topic:** [Cache-Aware Programming](./Cache_Aware_Programming.md) → [DMA Buffer Management](./DMA_Buffer_Management.md)
