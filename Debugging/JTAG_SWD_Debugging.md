# JTAG/SWD Debugging

## Table of Contents
- [Overview](#overview)
- [Key Concepts](#key-concepts)
- [Core Concepts](#core-concepts)
- [Implementation](#implementation)
- [Advanced Techniques](#advanced-techniques)
- [Common Pitfalls](#common-pitfalls)
- [Best Practices](#best-practices)
- [Interview Questions](#interview-questions)

## Overview

JTAG (Joint Test Action Group) and SWD (Serial Wire Debug) are industry-standard hardware debugging interfaces that provide direct access to embedded system internals. These protocols enable real-time debugging, memory inspection, and hardware validation without disrupting normal system operation.

**Key Benefits:**
- Non-intrusive debugging with minimal performance impact
- Real-time access to CPU registers and memory
- Hardware breakpoint support for complex debugging scenarios
- Standardized interface across multiple vendors and architectures

## Key Concepts

### JTAG Fundamentals
JTAG operates on a 4-wire interface (TMS, TCK, TDI, TDO) plus optional TRST, creating a state machine that can access internal scan chains. Each device in the scan chain has a unique ID and can be individually addressed for debugging operations.

**Scan Chain Architecture:**
```
Host ←→ JTAG Interface ←→ Target Device 1 ←→ Target Device 2 ←→ ...
      TMS/TCK/TDI/TDO    TMS/TCK/TDI/TDO   TMS/TCK/TDI/TDO
```

### SWD Fundamentals
SWD is ARM's simplified 2-wire debug interface that reduces pin count while maintaining full debugging capabilities. It uses SWDIO (bidirectional data) and SWCLK (clock) to communicate with ARM Cortex-M processors.

**SWD Interface:**
```
Host ←→ SWD Interface ←→ ARM Cortex-M
      SWDIO/SWCLK      SWDIO/SWCLK
```

### Debug Access Port (DAP)
Both protocols access the Debug Access Port, which provides:
- CPU register access (R0-R15, PSR, special registers)
- Memory read/write operations
- Breakpoint and watchpoint management
- Core control (halt, step, resume)

## Core Concepts

### State Machine Operation
JTAG uses a complex state machine with 16 states, while SWD uses a simpler packet-based protocol. Understanding these state transitions is crucial for reliable debugging.

**JTAG State Transitions:**
```
Test-Logic-Reset ←→ Run-Test/Idle ←→ Select-DR-Scan ←→ Capture-DR ←→ Shift-DR ←→ Exit1-DR ←→ Update-DR
       ↑                ↑                ↑              ↑            ↑           ↑           ↑
       └────────────────┴────────────────┴──────────────┴────────────┴───────────┴───────────┘
```

### Memory Access Patterns
Debug interfaces support different memory access patterns:
- **Word-aligned access**: Most efficient for 32-bit processors
- **Byte access**: Required for 8-bit operations and unaligned data
- **Burst transfers**: Optimized for sequential memory operations

### Breakpoint Types
- **Hardware breakpoints**: Limited in number but very fast
- **Software breakpoints**: Unlimited but require memory modification
- **Conditional breakpoints**: Trigger only under specific conditions

## Implementation

### Basic JTAG Implementation
```c
// JTAG interface structure
typedef struct {
    uint32_t tms_pin;
    uint32_t tck_pin;
    uint32_t tdi_pin;
    uint32_t tdo_pin;
    uint32_t trst_pin;
    uint32_t srst_pin;
} jtag_interface_t;

// JTAG state machine
typedef enum {
    TEST_LOGIC_RESET,
    RUN_TEST_IDLE,
    SELECT_DR_SCAN,
    CAPTURE_DR,
    SHIFT_DR,
    EXIT1_DR,
    UPDATE_DR,
    SELECT_IR_SCAN,
    CAPTURE_IR,
    SHIFT_IR,
    EXIT1_IR,
    UPDATE_IR
} jtag_state_t;

// Initialize JTAG interface
void jtag_init(jtag_interface_t *jtag) {
    // Configure GPIO pins
    gpio_set_mode(jtag->tms_pin, GPIO_MODE_OUTPUT);
    gpio_set_mode(jtag->tck_pin, GPIO_MODE_OUTPUT);
    gpio_set_mode(jtag->tdi_pin, GPIO_MODE_OUTPUT);
    gpio_set_mode(jtag->tdo_pin, GPIO_MODE_INPUT);
    gpio_set_mode(jtag->trst_pin, GPIO_MODE_OUTPUT);
    gpio_set_mode(jtag->srst_pin, GPIO_MODE_OUTPUT);
    
    // Reset to known state
    jtag_reset(jtag);
}

// JTAG state transition
void jtag_state_transition(jtag_interface_t *jtag, jtag_state_t target_state) {
    // Navigate through state machine to reach target
    // Implementation depends on current state and target
}
```

### SWD Implementation
```c
// SWD interface structure
typedef struct {
    uint32_t swdio_pin;
    uint32_t swclk_pin;
    uint32_t swdio_dir;  // Direction control
} swd_interface_t;

// SWD packet types
typedef enum {
    SWD_READ_AP = 0x05,
    SWD_WRITE_AP = 0x01,
    SWD_READ_DP = 0x04,
    SWD_WRITE_DP = 0x00
} swd_packet_type_t;

// SWD packet structure
typedef struct {
    uint8_t start;
    uint8_t apndp;
    uint8_t a[2];
    uint8_t parity;
    uint8_t stop;
    uint8_t park;
} swd_packet_t;

// Initialize SWD interface
void swd_init(swd_interface_t *swd) {
    gpio_set_mode(swd->swdio_pin, GPIO_MODE_OUTPUT);
    gpio_set_mode(swd->swclk_pin, GPIO_MODE_OUTPUT);
    gpio_set_mode(swd->swdio_dir, GPIO_MODE_OUTPUT);
    
    // Generate SWD reset sequence
    swd_reset_sequence(swd);
}

// Send SWD packet
uint32_t swd_send_packet(swd_interface_t *swd, swd_packet_t *packet) {
    uint32_t ack;
    
    // Set direction to output
    gpio_write(swd->swdio_dir, 1);
    
    // Send packet bits
    for (int i = 0; i < 8; i++) {
        gpio_write(swd->swdio_pin, (packet->start >> i) & 1);
        gpio_write(swd->swclk_pin, 1);
        gpio_write(swd->swclk_pin, 0);
    }
    
    // Switch to input for ACK
    gpio_write(swd->swdio_dir, 0);
    
    // Read ACK
    ack = 0;
    for (int i = 0; i < 3; i++) {
        gpio_write(swd->swclk_pin, 1);
        ack |= (gpio_read(swd->swdio_pin) << i);
        gpio_write(swd->swclk_pin, 0);
    }
    
    return ack;
}
```

### Debug Session Management
```c
// Debug session structure
typedef struct {
    uint32_t target_id;
    uint32_t core_id;
    bool is_halted;
    uint32_t breakpoint_count;
    uint32_t watchpoint_count;
} debug_session_t;

// Initialize debug session
debug_session_t* debug_session_init(uint32_t target_id) {
    debug_session_t *session = malloc(sizeof(debug_session_t));
    if (session) {
        session->target_id = target_id;
        session->core_id = 0;
        session->is_halted = false;
        session->breakpoint_count = 0;
        session->watchpoint_count = 0;
    }
    return session;
}

// Halt target
bool debug_halt_target(debug_session_t *session) {
    // Send halt command through debug interface
    if (send_halt_command(session->target_id)) {
        session->is_halted = true;
        return true;
    }
    return false;
}

// Read CPU register
uint32_t debug_read_register(debug_session_t *session, uint32_t reg_num) {
    if (!session->is_halted) {
        return 0xFFFFFFFF; // Error: target not halted
    }
    
    // Read register through debug interface
    return read_register_value(session->target_id, reg_num);
}
```

## Advanced Techniques

### Multi-Core Debugging
```c
// Multi-core debug session
typedef struct {
    uint32_t target_id;
    uint32_t core_count;
    debug_session_t *cores;
    bool all_halted;
} multi_core_debug_t;

// Halt all cores
bool debug_halt_all_cores(multi_core_debug_t *multi_core) {
    bool success = true;
    
    for (uint32_t i = 0; i < multi_core->core_count; i++) {
        if (!debug_halt_target(&multi_core->cores[i])) {
            success = false;
        }
    }
    
    multi_core->all_halted = success;
    return success;
}

// Synchronized stepping across cores
bool debug_step_all_cores(multi_core_debug_t *multi_core) {
    if (!multi_core->all_halted) {
        return false;
    }
    
    // Step all cores simultaneously
    for (uint32_t i = 0; i < multi_core->core_count; i++) {
        step_core(&multi_core->cores[i]);
    }
    
    return true;
}
```

### Conditional Breakpoints
```c
// Conditional breakpoint structure
typedef struct {
    uint32_t address;
    uint32_t condition_type;
    uint32_t condition_value;
    uint32_t hit_count;
    bool enabled;
} conditional_breakpoint_t;

// Condition types
typedef enum {
    CONDITION_EQUAL,
    CONDITION_NOT_EQUAL,
    CONDITION_GREATER_THAN,
    CONDITION_LESS_THAN,
    CONDITION_BIT_SET,
    CONDITION_BIT_CLEAR
} breakpoint_condition_t;

// Check breakpoint condition
bool check_breakpoint_condition(conditional_breakpoint_t *bp, uint32_t value) {
    if (!bp->enabled) {
        return false;
    }
    
    switch (bp->condition_type) {
        case CONDITION_EQUAL:
            return (value == bp->condition_value);
        case CONDITION_NOT_EQUAL:
            return (value != bp->condition_value);
        case CONDITION_GREATER_THAN:
            return (value > bp->condition_value);
        case CONDITION_LESS_THAN:
            return (value < bp->condition_value);
        case CONDITION_BIT_SET:
            return (value & bp->condition_value) != 0;
        case CONDITION_BIT_CLEAR:
            return (value & bp->condition_value) == 0;
        default:
            return false;
    }
}
```

### Memory Access Optimization
```c
// Memory access optimization
typedef struct {
    uint32_t base_address;
    uint32_t size;
    uint32_t access_count;
    uint32_t cache_hits;
    uint32_t cache_misses;
} memory_cache_t;

// Optimized memory read
uint32_t debug_read_memory_optimized(debug_session_t *session, 
                                    uint32_t address, 
                                    memory_cache_t *cache) {
    // Check cache first
    if (address >= cache->base_address && 
        address < cache->base_address + cache->size) {
        cache->cache_hits++;
        return read_cached_memory(address);
    }
    
    cache->cache_misses++;
    cache->access_count++;
    
    // Update cache with new region
    update_memory_cache(cache, address);
    
    return read_memory_through_debug(address);
}
```

## Common Pitfalls

### Timing Issues
- **Clock frequency mismatches**: Ensure debug interface clock matches target requirements
- **Setup/hold time violations**: Respect timing specifications for reliable communication
- **Clock jitter**: Use stable clock sources to prevent communication errors

### Power Management
- **Target power sequencing**: Debug interface may require specific power-up sequence
- **Voltage level compatibility**: Ensure interface voltage matches target requirements
- **Power consumption**: Debug interface adds power overhead during operation

### Connection Problems
- **Pin configuration**: Incorrect GPIO setup can prevent communication
- **Cable quality**: Poor quality cables can cause intermittent failures
- **Ground connections**: Missing or poor ground connections cause signal integrity issues

## Best Practices

### Interface Configuration
1. **Always verify pin assignments** before initializing debug interface
2. **Use pull-up/pull-down resistors** for floating inputs
3. **Implement proper reset sequences** for reliable initialization
4. **Validate timing requirements** against target specifications

### Debug Session Management
1. **Check target state** before attempting operations
2. **Implement timeout mechanisms** for all debug operations
3. **Use proper error handling** for failed operations
4. **Maintain session consistency** across multiple operations

### Performance Optimization
1. **Batch memory operations** when possible
2. **Use hardware breakpoints** for frequently hit conditions
3. **Implement caching** for repeated memory access
4. **Minimize debug overhead** during normal operation

## Interview Questions

### Basic Level
1. **What are the main differences between JTAG and SWD?**
   - JTAG uses 4+ wires, SWD uses 2 wires
   - JTAG has complex state machine, SWD uses packet-based protocol
   - SWD is ARM-specific, JTAG is vendor-agnostic

2. **What is the purpose of the TMS pin in JTAG?**
   - Controls state machine transitions
   - Determines next state based on current state and TMS value
   - Critical for proper JTAG operation

### Intermediate Level
3. **How would you implement a conditional breakpoint system?**
   - Use hardware breakpoints when available
   - Implement software breakpoints with condition checking
   - Consider performance impact of condition evaluation

4. **What are the challenges of debugging a multi-core system?**
   - Synchronizing halt/resume across cores
   - Managing breakpoints across multiple cores
   - Handling inter-core communication during debug

### Advanced Level
5. **How would you optimize memory access through a debug interface?**
   - Implement memory caching for frequently accessed regions
   - Use burst transfers when possible
   - Minimize protocol overhead for large transfers

6. **What considerations are important for production debugging?**
   - Security implications of debug access
   - Performance impact on production systems
   - Remote debugging capabilities and security
