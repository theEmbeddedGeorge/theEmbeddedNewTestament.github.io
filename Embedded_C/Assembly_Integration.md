# Assembly Integration for Embedded Systems

> **Integrating assembly code with C for low-level hardware control and optimization**

## 📋 **Table of Contents**
- [Overview](#overview)
- [Inline Assembly](#inline-assembly)
- [Calling Conventions](#calling-conventions)
- [ARM Assembly](#arm-assembly)
- [Hardware Access](#hardware-access)
- [Performance Optimization](#performance-optimization)
- [Cross-Platform Assembly](#cross-platform-assembly)
- [Common Pitfalls](#common-pitfalls)
- [Best Practices](#best-practices)
- [Interview Questions](#interview-questions)

---

## 🎯 **Overview**

Assembly integration is essential in embedded systems for:
- **Direct hardware control** - Access to specific CPU instructions
- **Performance optimization** - Hand-tuned critical code sections
- **Interrupt handling** - Low-level interrupt service routines
- **System initialization** - Boot code and startup sequences
- **Real-time constraints** - Predictable execution timing

### **Key Concepts**
- **Inline assembly** - Assembly code embedded in C functions
- **Calling conventions** - How functions pass parameters and return values
- **Register allocation** - Managing CPU registers in assembly
- **Memory barriers** - Controlling memory access ordering
- **Interrupt context** - Special considerations for ISRs

---

## 🔧 **Inline Assembly**

### **Basic Inline Assembly**

#### **Simple Inline Assembly**
```c
// Basic inline assembly syntax
void simple_assembly_example(void) {
    __asm volatile (
        "mov r0, #42\n"        // Load immediate value 42 into r0
        "add r0, r0, #10\n"    // Add 10 to r0
        :                       // No output operands
        :                       // No input operands
        : "r0"                 // Clobbered registers
    );
}

// Assembly with input/output operands
uint32_t add_with_assembly(uint32_t a, uint32_t b) {
    uint32_t result;
    
    __asm volatile (
        "add %0, %1, %2\n"     // Add r1 and r2, store in r0
        : "=r" (result)        // Output operand
        : "r" (a), "r" (b)    // Input operands
        :                       // No clobbered registers
    );
    
    return result;
}
```

#### **Assembly with Constraints**
```c
// Different constraint types
void constraint_examples(void) {
    uint32_t value = 42;
    uint32_t result;
    
    // Register constraint
    __asm volatile (
        "mov %0, %1\n"
        : "=r" (result)        // Output in register
        : "r" (value)          // Input in register
    );
    
    // Memory constraint
    __asm volatile (
        "ldr %0, [%1]\n"       // Load from memory
        : "=r" (result)        // Output in register
        : "m" (value)          // Input in memory
    );
    
    // Immediate constraint
    __asm volatile (
        "add %0, %1, #10\n"    // Add immediate
        : "=r" (result)        // Output in register
        : "r" (value), "I" (10) // Input register and immediate
    );
}
```

### **Advanced Inline Assembly**

#### **Multiple Instructions**
```c
// Complex assembly block
uint32_t complex_operation(uint32_t a, uint32_t b) {
    uint32_t result;
    
    __asm volatile (
        "add %0, %1, %2\n"     // Add a and b
        "lsl %0, %0, #2\n"     // Left shift by 2 (multiply by 4)
        "sub %0, %0, #1\n"     // Subtract 1
        : "=r" (result)        // Output
        : "r" (a), "r" (b)    // Inputs
        :                       // No clobbers
    );
    
    return result;
}

// Assembly with labels
void assembly_with_labels(void) {
    uint32_t result;
    
    __asm volatile (
        "mov r0, #0\n"         // Initialize counter
        "1:\n"                 // Label 1
        "add r0, r0, #1\n"     // Increment counter
        "cmp r0, #10\n"        // Compare with 10
        "blt 1b\n"             // Branch if less than (back to label 1)
        "mov %0, r0\n"         // Move result to output
        : "=r" (result)        // Output
        :                       // No inputs
        : "r0"                 // Clobbered register
    );
}
```

#### **Assembly with Clobbers**
```c
// Assembly that modifies registers
void assembly_with_clobbers(uint32_t* data, uint32_t length) {
    __asm volatile (
        "mov r0, %0\n"         // Load data pointer
        "mov r1, %1\n"         // Load length
        "mov r2, #0\n"         // Initialize sum
        "1:\n"                 // Loop start
        "ldr r3, [r0], #4\n"   // Load word and increment pointer
        "add r2, r2, r3\n"     // Add to sum
        "subs r1, r1, #1\n"    // Decrement counter
        "bne 1b\n"             // Branch if not zero
        "mov %0, r2\n"         // Store result
        : "=r" (*data)         // Output (store in first element)
        : "r" (data), "r" (length) // Inputs
        : "r0", "r1", "r2", "r3", "memory" // Clobbered registers and memory
    );
}
```

---

## 📞 **Calling Conventions**

### **ARM Calling Convention**

#### **Parameter Passing**
```c
// ARM AAPCS (ARM Architecture Procedure Call Standard)
// Parameters are passed in r0-r3, additional parameters on stack
uint32_t function_with_parameters(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    // a is in r0, b is in r1, c is in r2, d is in r3
    return a + b + c + d;
}

// Function with many parameters
uint32_t many_parameters(uint32_t a, uint32_t b, uint32_t c, uint32_t d, 
                        uint32_t e, uint32_t f) {
    // a-d in r0-r3, e-f on stack
    return a + b + c + d + e + f;
}
```

#### **Return Value Convention**
```c
// Return values in r0 (32-bit) or r0-r1 (64-bit)
uint32_t return_32bit(void) {
    return 42;  // Returned in r0
}

uint64_t return_64bit(void) {
    return 0x123456789ABCDEF0;  // Returned in r0-r1
}

// Structure return (passed by reference)
typedef struct {
    uint32_t a, b, c;
} large_struct_t;

large_struct_t return_struct(void) {
    large_struct_t result = {1, 2, 3};
    return result;  // Caller provides space, address in r0
}
```

### **Assembly Function Calls**

#### **Calling C from Assembly**
```c
// C function declaration
extern uint32_t c_function(uint32_t a, uint32_t b);

// Assembly function that calls C
uint32_t assembly_calls_c(uint32_t x, uint32_t y) {
    uint32_t result;
    
    __asm volatile (
        "mov r0, %1\n"         // First parameter
        "mov r1, %2\n"         // Second parameter
        "bl c_function\n"       // Branch and link to C function
        "mov %0, r0\n"         // Get return value
        : "=r" (result)        // Output
        : "r" (x), "r" (y)    // Inputs
        : "r0", "r1", "lr"     // Clobbered registers
    );
    
    return result;
}
```

#### **Calling Assembly from C**
```c
// Assembly function declaration
extern uint32_t assembly_function(uint32_t a, uint32_t b);

// C function that calls assembly
uint32_t c_calls_assembly(uint32_t x, uint32_t y) {
    return assembly_function(x, y);
}

// Assembly function implementation (in separate .s file)
/*
.global assembly_function
assembly_function:
    add r0, r0, r1    // Add parameters
    bx lr             // Return
*/
```

---

## 🏗️ **ARM Assembly**

### **Basic ARM Instructions**

#### **Data Processing Instructions**
```c
// Arithmetic operations
void arithmetic_operations(uint32_t a, uint32_t b) {
    uint32_t result;
    
    __asm volatile (
        "add %0, %1, %2\n"     // Addition
        : "=r" (result)
        : "r" (a), "r" (b)
    );
    
    __asm volatile (
        "sub %0, %1, %2\n"     // Subtraction
        : "=r" (result)
        : "r" (a), "r" (b)
    );
    
    __asm volatile (
        "mul %0, %1, %2\n"     // Multiplication
        : "=r" (result)
        : "r" (a), "r" (b)
    );
}

// Logical operations
void logical_operations(uint32_t a, uint32_t b) {
    uint32_t result;
    
    __asm volatile (
        "and %0, %1, %2\n"     // AND
        : "=r" (result)
        : "r" (a), "r" (b)
    );
    
    __asm volatile (
        "orr %0, %1, %2\n"     // OR
        : "=r" (result)
        : "r" (a), "r" (b)
    );
    
    __asm volatile (
        "eor %0, %1, %2\n"     // XOR
        : "=r" (result)
        : "r" (a), "r" (b)
    );
}
```

#### **Branch Instructions**
```c
// Conditional branches
void conditional_branches(uint32_t a, uint32_t b) {
    uint32_t result;
    
    __asm volatile (
        "cmp %1, %2\n"         // Compare a and b
        "movgt %0, #1\n"       // Move 1 if greater than
        "movle %0, #0\n"       // Move 0 if less than or equal
        : "=r" (result)
        : "r" (a), "r" (b)
    );
}

// Loop with assembly
void assembly_loop(uint32_t count) {
    __asm volatile (
        "mov r0, %0\n"         // Load count
        "1:\n"                 // Loop label
        "subs r0, r0, #1\n"    // Decrement and set flags
        "bne 1b\n"             // Branch if not zero
        :                       // No outputs
        : "r" (count)          // Input
        : "r0"                 // Clobbered register
    );
}
```

### **Memory Operations**

#### **Load and Store Instructions**
```c
// Memory access operations
void memory_operations(uint32_t* data, uint32_t value) {
    __asm volatile (
        "str %1, [%0]\n"       // Store word
        :                       // No outputs
        : "r" (data), "r" (value) // Inputs
        : "memory"             // Memory clobber
    );
    
    __asm volatile (
        "ldr r0, [%0]\n"       // Load word
        "add r0, r0, #1\n"     // Add 1
        "str r0, [%0]\n"       // Store back
        :                       // No outputs
        : "r" (data)           // Input
        : "r0", "memory"       // Clobbered register and memory
    );
}

// Multiple memory operations
void multiple_memory_ops(uint32_t* dest, const uint32_t* src, uint32_t count) {
    __asm volatile (
        "mov r0, %0\n"         // Destination pointer
        "mov r1, %1\n"         // Source pointer
        "mov r2, %2\n"         // Count
        "1:\n"                 // Loop start
        "ldr r3, [r1], #4\n"   // Load and increment source
        "str r3, [r0], #4\n"   // Store and increment destination
        "subs r2, r2, #1\n"    // Decrement count
        "bne 1b\n"             // Branch if not zero
        :                       // No outputs
        : "r" (dest), "r" (src), "r" (count) // Inputs
        : "r0", "r1", "r2", "r3", "memory"   // Clobbers
    );
}
```

---

## 🔧 **Hardware Access**

### **Register Access**

#### **Special Register Operations**
```c
// Access to special registers
uint32_t read_control_register(void) {
    uint32_t value;
    
    __asm volatile (
        "mrs %0, control\n"    // Move from special register
        : "=r" (value)         // Output
        :                       // No inputs
    );
    
    return value;
}

void write_control_register(uint32_t value) {
    __asm volatile (
        "msr control, %0\n"    // Move to special register
        :                       // No outputs
        : "r" (value)          // Input
        : "memory"             // Memory barrier
    );
}

// Read/write other special registers
uint32_t read_primask(void) {
    uint32_t value;
    __asm volatile ("mrs %0, primask" : "=r" (value));
    return value;
}

void write_primask(uint32_t value) {
    __asm volatile ("msr primask, %0" : : "r" (value) : "memory");
}
```

#### **Interrupt Control**
```c
// Enable/disable interrupts
void enable_interrupts(void) {
    __asm volatile (
        "cpsie i\n"            // Enable interrupts
        :                       // No outputs
        :                       // No inputs
        : "memory"             // Memory barrier
    );
}

void disable_interrupts(void) {
    __asm volatile (
        "cpsid i\n"            // Disable interrupts
        :                       // No outputs
        :                       // No inputs
        : "memory"             // Memory barrier
    );
}

// Wait for interrupt/event
void wait_for_interrupt(void) {
    __asm volatile (
        "wfi\n"                // Wait for interrupt
        :                       // No outputs
        :                       // No inputs
        : "memory"             // Memory barrier
    );
}

void wait_for_event(void) {
    __asm volatile (
        "wfe\n"                // Wait for event
        :                       // No outputs
        :                       // No inputs
        : "memory"             // Memory barrier
    );
}
```

### **Hardware Register Access**

#### **Memory-Mapped I/O**
```c
// Hardware register access
void write_hardware_register(uint32_t address, uint32_t value) {
    __asm volatile (
        "str %1, [%0]\n"       // Store to hardware register
        :                       // No outputs
        : "r" (address), "r" (value) // Inputs
        : "memory"             // Memory barrier
    );
}

uint32_t read_hardware_register(uint32_t address) {
    uint32_t value;
    
    __asm volatile (
        "ldr %0, [%1]\n"       // Load from hardware register
        : "=r" (value)         // Output
        : "r" (address)        // Input
        : "memory"             // Memory barrier
    );
    
    return value;
}

// Atomic hardware operations
void atomic_set_bits(uint32_t address, uint32_t mask) {
    __asm volatile (
        "ldr r0, [%0]\n"       // Load current value
        "orr r0, r0, %1\n"     // Set bits
        "str r0, [%0]\n"       // Store back
        :                       // No outputs
        : "r" (address), "r" (mask) // Inputs
        : "r0", "memory"       // Clobbered register and memory
    );
}
```

---

## ⚡ **Performance Optimization**

### **Critical Path Optimization**

#### **Hand-Optimized Loops**
```c
// Optimized memory copy
void optimized_memcpy(uint8_t* dest, const uint8_t* src, uint32_t length) {
    // Copy 4 bytes at a time for better performance
    uint32_t word_count = length / 4;
    uint32_t remainder = length % 4;
    
    __asm volatile (
        "mov r0, %0\n"         // Destination
        "mov r1, %1\n"         // Source
        "mov r2, %2\n"         // Word count
        "1:\n"                 // Loop start
        "ldr r3, [r1], #4\n"   // Load word
        "str r3, [r0], #4\n"   // Store word
        "subs r2, r2, #1\n"    // Decrement counter
        "bne 1b\n"             // Branch if not zero
        :                       // No outputs
        : "r" (dest), "r" (src), "r" (word_count) // Inputs
        : "r0", "r1", "r2", "r3", "memory"        // Clobbers
    );
    
    // Handle remaining bytes
    src += word_count * 4;
    dest += word_count * 4;
    for (uint32_t i = 0; i < remainder; i++) {
        dest[i] = src[i];
    }
}
```

#### **Branch Prediction**
```c
// Optimized conditional code
uint32_t optimized_conditional(uint32_t a, uint32_t b) {
    uint32_t result;
    
    __asm volatile (
        "cmp %1, %2\n"         // Compare a and b
        "movgt %0, %1\n"       // If a > b, result = a
        "movle %0, %2\n"       // If a <= b, result = b
        : "=r" (result)        // Output
        : "r" (a), "r" (b)    // Inputs
    );
    
    return result;
}
```

### **Cache Optimization**

#### **Cache-Friendly Operations**
```c
// Prefetch data for cache optimization
void prefetch_data(const uint8_t* data, uint32_t length) {
    __asm volatile (
        "mov r0, %0\n"         // Data pointer
        "mov r1, %1\n"         // Length
        "1:\n"                 // Loop start
        "pld [r0, #32]\n"      // Prefetch data 32 bytes ahead
        "add r0, r0, #4\n"     // Increment pointer
        "subs r1, r1, #1\n"    // Decrement counter
        "bne 1b\n"             // Branch if not zero
        :                       // No outputs
        : "r" (data), "r" (length) // Inputs
        : "r0", "r1"           // Clobbered registers
    );
}
```

---

## 🔄 **Cross-Platform Assembly**

### **Architecture Detection**

#### **Platform-Specific Assembly**
```c
// Cross-platform assembly macros
#ifdef __arm__
    #define ENABLE_INTERRUPTS() __asm volatile ("cpsie i" : : : "memory")
    #define DISABLE_INTERRUPTS() __asm volatile ("cpsid i" : : : "memory")
    #define WAIT_FOR_INTERRUPT() __asm volatile ("wfi" : : : "memory")
#elif defined(__x86_64__) || defined(__i386__)
    #define ENABLE_INTERRUPTS() __asm volatile ("sti" : : : "memory")
    #define DISABLE_INTERRUPTS() __asm volatile ("cli" : : : "memory")
    #define WAIT_FOR_INTERRUPT() __asm volatile ("hlt" : : : "memory")
#else
    #define ENABLE_INTERRUPTS()  // No-op for unsupported platforms
    #define DISABLE_INTERRUPTS() // No-op for unsupported platforms
    #define WAIT_FOR_INTERRUPT() // No-op for unsupported platforms
#endif

// Usage
void safe_critical_section(void) {
    DISABLE_INTERRUPTS();
    // Critical section code
    ENABLE_INTERRUPTS();
}
```

### **Portable Assembly Functions**

#### **Cross-Platform Intrinsics**
```c
// Portable assembly wrapper functions
typedef struct {
    void (*enable_interrupts)(void);
    void (*disable_interrupts)(void);
    void (*wait_for_interrupt)(void);
    uint32_t (*read_register)(uint32_t address);
    void (*write_register)(uint32_t address, uint32_t value);
} assembly_functions_t;

assembly_functions_t get_assembly_functions(void) {
    assembly_functions_t funcs;
    
#ifdef __arm__
    funcs.enable_interrupts = enable_interrupts_arm;
    funcs.disable_interrupts = disable_interrupts_arm;
    funcs.wait_for_interrupt = wait_for_interrupt_arm;
    funcs.read_register = read_hardware_register;
    funcs.write_register = write_hardware_register;
#elif defined(__x86_64__) || defined(__i386__)
    funcs.enable_interrupts = enable_interrupts_x86;
    funcs.disable_interrupts = disable_interrupts_x86;
    funcs.wait_for_interrupt = wait_for_interrupt_x86;
    funcs.read_register = read_hardware_register_x86;
    funcs.write_register = write_hardware_register_x86;
#else
    // Fallback implementations
    funcs.enable_interrupts = enable_interrupts_fallback;
    funcs.disable_interrupts = disable_interrupts_fallback;
    funcs.wait_for_interrupt = wait_for_interrupt_fallback;
    funcs.read_register = read_hardware_register_fallback;
    funcs.write_register = write_hardware_register_fallback;
#endif
    
    return funcs;
}
```

---

## ⚠️ **Common Pitfalls**

### **Assembly Errors**

#### **Incorrect Constraints**
```c
// ❌ Bad: Incorrect constraint usage
void bad_constraints(void) {
    uint32_t value = 42;
    uint32_t result;
    
    __asm volatile (
        "mov %0, %1\n"
        : "=r" (result)        // Output constraint
        : "r" (value)          // Input constraint
        // Missing clobber list for modified registers
    );
}

// ✅ Good: Proper constraints and clobbers
void good_constraints(void) {
    uint32_t value = 42;
    uint32_t result;
    
    __asm volatile (
        "mov %0, %1\n"
        : "=r" (result)        // Output constraint
        : "r" (value)          // Input constraint
        :                       // No clobbers needed
    );
}
```

#### **Memory Barrier Issues**
```c
// ❌ Bad: Missing memory barriers
volatile uint32_t flag = 0;
uint32_t data = 0;

void bad_producer(void) {
    data = 42;
    // Missing memory barrier!
    flag = 1;
}

// ✅ Good: Proper memory barriers
void good_producer(void) {
    data = 42;
    __asm volatile ("dmb st" : : : "memory");  // Write barrier
    flag = 1;
}
```

### **Register Allocation Issues**

#### **Register Conflicts**
```c
// ❌ Bad: Register conflicts
void bad_register_usage(void) {
    uint32_t a = 1, b = 2, c = 3;
    uint32_t result;
    
    __asm volatile (
        "add r0, %1, %2\n"     // Uses r0
        "add %0, r0, %3\n"     // Uses r0 again (conflict!)
        : "=r" (result)
        : "r" (a), "r" (b), "r" (c)
        : "r0"                 // Declare r0 as clobbered
    );
}

// ✅ Good: Proper register usage
void good_register_usage(void) {
    uint32_t a = 1, b = 2, c = 3;
    uint32_t result;
    
    __asm volatile (
        "add r0, %1, %2\n"     // Uses r0
        "add %0, r0, %3\n"     // Uses r0 again
        : "=r" (result)
        : "r" (a), "r" (b), "r" (c)
        : "r0"                 // Properly declare clobbered register
    );
}
```

---

## ✅ **Best Practices**

### **Assembly Code Organization**

#### **Modular Assembly Functions**
```c
// ✅ Good: Well-organized assembly functions
// hardware_access.h
extern void enable_interrupts(void);
extern void disable_interrupts(void);
extern uint32_t read_hardware_register(uint32_t address);
extern void write_hardware_register(uint32_t address, uint32_t value);

// hardware_access.c
void enable_interrupts(void) {
    __asm volatile ("cpsie i" : : : "memory");
}

void disable_interrupts(void) {
    __asm volatile ("cpsid i" : : : "memory");
}

uint32_t read_hardware_register(uint32_t address) {
    uint32_t value;
    __asm volatile ("ldr %0, [%1]" : "=r" (value) : "r" (address) : "memory");
    return value;
}

void write_hardware_register(uint32_t address, uint32_t value) {
    __asm volatile ("str %1, [%0]" : : "r" (address), "r" (value) : "memory");
}
```

#### **Inline Assembly Best Practices**
```c
// ✅ Good: Clear and documented inline assembly
uint32_t optimized_bit_count(uint32_t value) {
    uint32_t count;
    
    // Use built-in function when available, fallback to assembly
#ifdef __GNUC__
    count = __builtin_popcount(value);
#else
    // Assembly implementation
    __asm volatile (
        "mov %0, #0\n"         // Initialize count
        "1:\n"                 // Loop label
        "tst %1, %1\n"         // Test if value is zero
        "beq 2f\n"             // Branch if zero
        "sub %1, %1, #1\n"     // value = value & (value - 1)
        "and %1, %1, %1\n"     // Clear least significant bit
        "add %0, %0, #1\n"     // Increment count
        "b 1b\n"               // Branch back to loop
        "2:\n"                 // End label
        : "=r" (count), "=r" (value) // Outputs
        : "1" (value)          // Input (same as output)
        :                       // No clobbers
    );
#endif
    
    return count;
}
```

### **Error Handling**

#### **Assembly Error Checking**
```c
// ✅ Good: Error checking in assembly functions
bool safe_hardware_write(uint32_t address, uint32_t value) {
    // Check if address is valid
    if (address < 0x40000000 || address > 0x4FFFFFFF) {
        return false;  // Invalid address
    }
    
    // Perform write with error checking
    __asm volatile (
        "str %1, [%0]\n"       // Store value
        :                       // No outputs
        : "r" (address), "r" (value) // Inputs
        : "memory"             // Memory barrier
    );
    
    return true;
}

// Verify write operation
bool verify_hardware_write(uint32_t address, uint32_t expected_value) {
    uint32_t read_value = read_hardware_register(address);
    return read_value == expected_value;
}
```

---

## 🎯 **Interview Questions**

### **Basic Assembly Concepts**
1. **What is inline assembly?**
   - Assembly code embedded in C functions
   - Allows direct access to CPU instructions
   - Provides performance optimization opportunities

2. **What are calling conventions?**
   - Rules for passing parameters between functions
   - Register usage and stack management
   - Return value handling

3. **What's the difference between inline assembly and separate assembly files?**
   - Inline: Embedded in C, easier integration
   - Separate files: Pure assembly, better for complex routines

### **ARM Assembly**
4. **What are the main ARM registers?**
   - r0-r12: General purpose registers
   - r13 (sp): Stack pointer
   - r14 (lr): Link register
   - r15 (pc): Program counter

5. **How do you pass parameters in ARM assembly?**
   - First 4 parameters in r0-r3
   - Additional parameters on stack
   - Return value in r0 (32-bit) or r0-r1 (64-bit)

6. **What are the main ARM instruction types?**
   - Data processing: add, sub, mul, and, orr, eor
   - Memory: ldr, str, ldm, stm
   - Branch: b, bl, bx, blx
   - Special: mrs, msr

### **Performance and Optimization**
7. **How do you optimize assembly code?**
   - Minimize memory accesses
   - Use appropriate instructions
   - Optimize loops and branches
   - Consider cache behavior

8. **What are memory barriers and when do you use them?**
   - Control memory access ordering
   - Used in multi-threaded code
   - Ensure proper synchronization
   - Prevent compiler/hardware reordering

9. **How do you handle interrupts in assembly?**
   - Save/restore registers
   - Use appropriate instructions (cpsie/cpsid)
   - Minimize interrupt latency
   - Avoid blocking operations

### **Advanced Topics**
10. **How do you implement atomic operations in assembly?**
    - Use atomic instructions (ldrex/strex)
    - Implement compare-and-swap
    - Use memory barriers appropriately
    - Consider lock-free algorithms

11. **What are the trade-offs of using assembly?**
    - Pros: Performance, hardware access, control
    - Cons: Portability, maintainability, complexity
    - Balance with project requirements

12. **How do you debug assembly code?**
    - Use debugger with assembly support
    - Add debug symbols
    - Use step-by-step execution
    - Monitor register values

---

## 📚 **Additional Resources**

- [ARM Assembly Reference](https://developer.arm.com/documentation/dui0473/m/arm-and-thumb-instructions)
- [GCC Inline Assembly](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html)
- [ARM AAPCS](https://developer.arm.com/documentation/ihi0042/latest/)

---

**Next Topic:** [Memory Models](./Memory_Models.md) → [Advanced Memory Management](./Advanced_Memory_Management.md)
