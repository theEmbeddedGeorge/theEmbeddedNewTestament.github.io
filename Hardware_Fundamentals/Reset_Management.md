# 🔄 Reset Management

> **Mastering System Reset Mechanisms and Recovery Strategies**  
> Learn to implement robust reset management, handle different reset sources, and ensure reliable system startup

---

## 📋 **Table of Contents**

- [Overview](#overview)
- [Reset Types](#reset-types)
- [Reset Sources](#reset-sources)
- [Reset Configuration](#reset-configuration)
- [Reset Detection](#reset-detection)
- [System Initialization](#system-initialization)
- [Reset Recovery](#reset-recovery)
- [Best Practices](#best-practices)
- [Common Pitfalls](#common-pitfalls)
- [Examples](#examples)
- [Interview Questions](#interview-questions)

---

## 🎯 **Overview**

Reset management is crucial for embedded systems to ensure reliable startup, handle system failures, and maintain system integrity. Understanding reset mechanisms helps design robust systems that can recover from various failure conditions.

### Concept: Know every reset cause and make resets informative

Track POR, BOR, watchdog, software reset, and pin reset. Preserve reset reason across boot and expose it for diagnostics.

### Minimal example
```c
uint32_t reset_reason_read(void){ return RCC->CSR & REASON_MASK; }
void reset_reason_clear(void){ RCC->CSR |= RCC_CSR_RMVF; }
```

### Takeaways
- Clear flags early; log to non-volatile memory if needed.
- Define a boot flow that handles partial initialization after soft resets.
- Provide a user-visible diagnostic (LED pattern, log) of reset cause.

---

## 🧪 Guided Labs
1) Reset cause logging
- Implement a system that logs and displays the cause of the last reset.

2) Reset flag handling
- Test different reset scenarios and verify flag handling.

## ✅ Check Yourself
- How do you distinguish between different types of resets?
- When should you use a soft reset vs a hard reset?

## 🔗 Cross-links
- `Hardware_Fundamentals/Watchdog_Timers.md` for watchdog resets
- `Embedded_C/Type_Qualifiers.md` for volatile access

### **Key Concepts**
- **Reset Sources** - Different conditions that trigger system reset
- **Reset Configuration** - Setting up reset behavior and timing
- **System Initialization** - Proper startup sequence after reset
- **Reset Recovery** - Handling different reset scenarios

---

## 🔄 **Reset Types**

### **1. Power-On Reset (POR)**
Triggered when power is first applied to the system.

```c
// Power-on reset configuration
typedef struct {
    uint32_t por_delay_ms;      // Delay before system startup
    bool por_voltage_monitor;    // Enable voltage monitoring
    float por_voltage_threshold; // Minimum voltage threshold
    bool por_brownout_detect;    // Enable brownout detection
} por_config_t;

// Power-on reset status
typedef struct {
    bool por_occurred;           // POR flag
    uint32_t por_timestamp;      // When POR occurred
    float por_voltage;           // Voltage at POR
    uint8_t por_source;          // POR source (main, backup, etc.)
} por_status_t;
```

### **2. Watchdog Reset**
Triggered by watchdog timer timeout.

```c
// Watchdog reset configuration
typedef struct {
    uint32_t wdt_timeout_ms;     // Watchdog timeout period
    bool wdt_enabled;            // Enable watchdog
    bool wdt_window_mode;        // Enable window mode
    uint32_t wdt_window_start;   // Window start time
    uint32_t wdt_window_end;     // Window end time
} wdt_reset_config_t;

// Watchdog reset status
typedef struct {
    bool wdt_reset_occurred;     // WDT reset flag
    uint32_t wdt_reset_count;    // Number of WDT resets
    uint32_t last_wdt_feed;      // Last watchdog feed time
    uint8_t wdt_reset_source;    // WDT reset source
} wdt_reset_status_t;
```

### **3. Software Reset**
Triggered by software command or system request.

```c
// Software reset types
typedef enum {
    SW_RESET_SYSTEM,            // Full system reset
    SW_RESET_PERIPHERAL,        // Peripheral reset only
    SW_RESET_WARM,              // Warm reset (preserve some state)
    SW_RESET_COLD               // Cold reset (full initialization)
} software_reset_type_t;

// Software reset configuration
typedef struct {
    software_reset_type_t reset_type;
    bool preserve_memory;        // Preserve memory contents
    bool preserve_registers;     // Preserve register state
    uint32_t reset_delay_ms;     // Delay before reset
} sw_reset_config_t;
```

### **4. External Reset**
Triggered by external signal or hardware.

```c
// External reset configuration
typedef struct {
    uint8_t reset_pin;          // Reset pin number
    bool reset_pin_active_low;  // Active low reset pin
    uint32_t reset_debounce_ms; // Debounce time
    bool reset_pin_pullup;      // Enable pull-up resistor
} external_reset_config_t;

// External reset status
typedef struct {
    bool ext_reset_occurred;     // External reset flag
    uint32_t ext_reset_count;    // Number of external resets
    uint32_t last_ext_reset;     // Last external reset time
} external_reset_status_t;
```

---

## 🎯 **Reset Sources**

### **1. Hardware Reset Sources**

```c
// Hardware reset sources
typedef enum {
    RESET_SOURCE_POR,           // Power-on reset
    RESET_SOURCE_PIN,           // Reset pin
    RESET_SOURCE_WDT,           // Watchdog timer
    RESET_SOURCE_BOD,           // Brownout detector
    RESET_SOURCE_LOCKUP,        // Lockup reset
    RESET_SOURCE_SYSRESET,      // System reset request
    RESET_SOURCE_SOFTWARE,      // Software reset
    RESET_SOURCE_UNKNOWN        // Unknown reset source
} reset_source_t;

// Reset source detection
reset_source_t detect_reset_source(void) {
    uint32_t reset_flags = RCC->CSR;
    
    if (reset_flags & RCC_CSR_PORRSTF) {
        return RESET_SOURCE_POR;
    } else if (reset_flags & RCC_CSR_PINRSTF) {
        return RESET_SOURCE_PIN;
    } else if (reset_flags & RCC_CSR_WWDGRSTF) {
        return RESET_SOURCE_WDT;
    } else if (reset_flags & RCC_CSR_BORRSTF) {
        return RESET_SOURCE_BOD;
    } else if (reset_flags & RCC_CSR_SFTRSTF) {
        return RESET_SOURCE_SOFTWARE;
    } else {
        return RESET_SOURCE_UNKNOWN;
    }
}
```

### **2. Reset Source Configuration**

```c
// Configure reset sources
void configure_reset_sources(void) {
    // Enable brownout detector
    PWR->CR |= PWR_CR_BODEN;
    
    // Configure reset pin
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = RESET_PIN;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RESET_PORT, &gpio_init);
    
    // Enable reset pin interrupt
    HAL_NVIC_SetPriority(RESET_PIN_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RESET_PIN_IRQn);
}
```

---

## ⚙️ **Reset Configuration**

### **1. Reset Timing Configuration**

```c
// Reset timing configuration
typedef struct {
    uint32_t por_delay_ms;      // Power-on reset delay
    uint32_t reset_debounce_ms; // Reset pin debounce
    uint32_t wdt_timeout_ms;    // Watchdog timeout
    uint32_t brownout_delay_ms; // Brownout detection delay
} reset_timing_config_t;

// Configure reset timing
void configure_reset_timing(reset_timing_config_t *config) {
    // Configure power-on reset delay
    if (config->por_delay_ms > 0) {
        // Set POR delay timer
        TIM_HandleTypeDef htim_por;
        htim_por.Instance = TIM2;
        htim_por.Init.Prescaler = 8399; // 84MHz / 8400 = 10kHz
        htim_por.Init.Period = config->por_delay_ms * 10;
        HAL_TIM_Base_Init(&htim_por);
    }
    
    // Configure watchdog timeout
    if (config->wdt_timeout_ms > 0) {
        IWDG_HandleTypeDef hiwdg;
        hiwdg.Instance = IWDG;
        hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
        hiwdg.Init.Reload = (config->wdt_timeout_ms * 40000) / 256;
        HAL_IWDG_Init(&hiwdg);
    }
}
```

### **2. Reset Behavior Configuration**

```c
// Reset behavior configuration
typedef struct {
    bool preserve_ram;          // Preserve RAM contents
    bool preserve_registers;    // Preserve register state
    bool preserve_peripherals;  // Preserve peripheral state
    bool clear_flags;           // Clear reset flags
    bool enable_debug;          // Enable debug after reset
} reset_behavior_config_t;

// Configure reset behavior
void configure_reset_behavior(reset_behavior_config_t *config) {
    if (config->preserve_ram) {
        // Configure RAM retention
        PWR->CR |= PWR_CR_DBP;
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    }
    
    if (config->clear_flags) {
        // Clear reset flags
        RCC->CSR |= RCC_CSR_RMVF;
    }
    
    if (config->enable_debug) {
        // Enable debug after reset
        DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP | DBGMCU_CR_DBG_STOP | DBGMCU_CR_DBG_STANDBY;
    }
}
```

---

## 🔍 **Reset Detection**

### **1. Reset Flag Detection**

```c
// Reset flag structure
typedef struct {
    bool por_flag;              // Power-on reset flag
    bool pin_flag;              // Reset pin flag
    bool wdt_flag;              // Watchdog reset flag
    bool bod_flag;              // Brownout reset flag
    bool software_flag;         // Software reset flag
    bool lockup_flag;           // Lockup reset flag
    uint32_t reset_count;       // Total reset count
} reset_flags_t;

// Detect reset flags
reset_flags_t detect_reset_flags(void) {
    reset_flags_t flags = {0};
    uint32_t reset_flags = RCC->CSR;
    
    flags.por_flag = (reset_flags & RCC_CSR_PORRSTF) != 0;
    flags.pin_flag = (reset_flags & RCC_CSR_PINRSTF) != 0;
    flags.wdt_flag = (reset_flags & RCC_CSR_WWDGRSTF) != 0;
    flags.bod_flag = (reset_flags & RCC_CSR_BORRSTF) != 0;
    flags.software_flag = (reset_flags & RCC_CSR_SFTRSTF) != 0;
    flags.lockup_flag = (reset_flags & RCC_CSR_LOCKUPRSTF) != 0;
    
    return flags;
}
```

### **2. Reset Source Logging**

```c
// Reset log entry
typedef struct {
    reset_source_t source;      // Reset source
    uint32_t timestamp;         // Reset timestamp
    uint32_t reset_count;       // Reset count
    uint8_t additional_info[16]; // Additional information
} reset_log_entry_t;

// Log reset event
void log_reset_event(reset_source_t source) {
    reset_log_entry_t log_entry;
    log_entry.source = source;
    log_entry.timestamp = HAL_GetTick();
    log_entry.reset_count = get_reset_count() + 1;
    
    // Store in non-volatile memory
    store_reset_log(&log_entry);
}
```

---

## 🚀 **System Initialization**

### **1. Reset Handler**

```c
// Reset handler
void reset_handler(void) {
    // Clear reset flags
    RCC->CSR |= RCC_CSR_RMVF;
    
    // Initialize system
    system_init();
    
    // Jump to main application
    main();
}

// System initialization
void system_init(void) {
    // Initialize clock system
    clock_init();
    
    // Initialize GPIO
    gpio_init();
    
    // Initialize peripherals
    peripheral_init();
    
    // Initialize watchdog
    watchdog_init();
    
    // Initialize reset management
    reset_management_init();
}
```

### **2. Post-Reset Initialization**

```c
// Post-reset initialization
void post_reset_init(void) {
    reset_source_t source = detect_reset_source();
    
    switch (source) {
        case RESET_SOURCE_POR:
            // Full system initialization
            full_system_init();
            break;
            
        case RESET_SOURCE_WDT:
            // Recover from watchdog reset
            watchdog_recovery();
            break;
            
        case RESET_SOURCE_SOFTWARE:
            // Software reset recovery
            software_reset_recovery();
            break;
            
        default:
            // Unknown reset source
            unknown_reset_recovery();
            break;
    }
}
```

---

## 🔄 **Reset Recovery**

### **1. Watchdog Reset Recovery**

```c
// Watchdog reset recovery
void watchdog_recovery(void) {
    // Check system health
    if (check_system_health()) {
        // System is healthy, continue normal operation
        log_system_event("WDT reset - system recovered");
    } else {
        // System is unhealthy, perform recovery
        perform_system_recovery();
    }
    
    // Reset watchdog
    feed_watchdog();
}

// System health check
bool check_system_health(void) {
    // Check critical system parameters
    if (check_voltage_levels() && check_temperature() && check_memory_integrity()) {
        return true;
    }
    return false;
}
```

### **2. Software Reset Recovery**

```c
// Software reset recovery
void software_reset_recovery(void) {
    // Restore system state if needed
    if (was_state_preserved()) {
        restore_system_state();
    }
    
    // Reinitialize necessary components
    reinitialize_components();
    
    // Continue normal operation
    log_system_event("Software reset - recovery completed");
}
```

---

## ✅ **Best Practices**

### **1. Reset Design Principles**

- **Always detect reset source** - Know why the system reset
- **Implement proper initialization sequence** - Ensure reliable startup
- **Handle all reset scenarios** - Don't ignore any reset sources
- **Log reset events** - Track system behavior over time
- **Implement recovery mechanisms** - Handle reset scenarios gracefully

### **2. Reset Configuration**

```c
// Best practice reset configuration
void configure_reset_best_practices(void) {
    // Enable all reset sources
    enable_all_reset_sources();
    
    // Configure appropriate timeouts
    configure_reset_timeouts();
    
    // Set up reset logging
    setup_reset_logging();
    
    // Configure recovery mechanisms
    configure_recovery_mechanisms();
}
```

---

## ⚠️ **Common Pitfalls**

### **1. Reset-Related Issues**

- **Not clearing reset flags** - Can cause confusion about reset source
- **Incomplete initialization** - System may not start properly
- **Ignoring reset sources** - Missing important system information
- **No recovery mechanism** - System may get stuck after reset
- **Incorrect timing** - Reset may occur too early or late

### **2. Debugging Reset Issues**

```c
// Debug reset issues
void debug_reset_issues(void) {
    reset_flags_t flags = detect_reset_flags();
    
    if (flags.wdt_flag) {
        // Check watchdog configuration
        check_watchdog_config();
    }
    
    if (flags.bod_flag) {
        // Check power supply
        check_power_supply();
    }
    
    if (flags.software_flag) {
        // Check software reset logic
        check_software_reset_logic();
    }
}
```

---

## 💡 **Examples**

### **1. Complete Reset Management System**

```c
// Reset management system
typedef struct {
    reset_timing_config_t timing;
    reset_behavior_config_t behavior;
    reset_flags_t flags;
    uint32_t reset_count;
} reset_management_system_t;

// Initialize reset management
void init_reset_management(reset_management_system_t *rms) {
    // Configure timing
    configure_reset_timing(&rms->timing);
    
    // Configure behavior
    configure_reset_behavior(&rms->behavior);
    
    // Detect reset flags
    rms->flags = detect_reset_flags();
    
    // Log reset event
    log_reset_event(detect_reset_source());
    
    // Perform post-reset initialization
    post_reset_init();
}
```

### **2. Reset Monitoring System**

```c
// Reset monitoring system
void reset_monitoring_task(void) {
    static uint32_t last_reset_count = 0;
    uint32_t current_reset_count = get_reset_count();
    
    if (current_reset_count != last_reset_count) {
        // Reset count changed, log event
        log_reset_event(detect_reset_source());
        last_reset_count = current_reset_count;
    }
    
    // Check for excessive resets
    if (current_reset_count > MAX_RESET_COUNT) {
        // Too many resets, take action
        handle_excessive_resets();
    }
}
```

---

## 🎯 **Interview Questions**

### **Basic Questions**
1. **What are the different types of reset in embedded systems?**
   - Power-on reset, watchdog reset, software reset, external reset

2. **How do you detect the source of a reset?**
   - Check reset flags in RCC->CSR register

3. **What is the difference between warm reset and cold reset?**
   - Warm reset preserves some state, cold reset is full initialization

### **Intermediate Questions**
4. **How would you implement a reset recovery mechanism?**
   - Detect reset source, check system health, perform appropriate recovery

5. **What are the considerations for reset timing?**
   - Power stabilization, debouncing, initialization sequence

6. **How do you handle watchdog resets?**
   - Check system health, perform recovery if needed, reset watchdog

### **Advanced Questions**
7. **How would you design a robust reset management system?**
   - Multiple reset sources, proper logging, recovery mechanisms

8. **What are the challenges in reset management for multi-core systems?**
   - Core synchronization, shared resource management

9. **How do you implement reset in a real-time system?**
   - Minimal latency, predictable timing, proper state management

---

## 🔗 **Related Topics**

- **[Interrupts and Exceptions](./Interrupts_Exceptions.md)** - Interrupt handling and exception management
- **[Watchdog Timers](./Watchdog_Timers.md)** - System monitoring and recovery
- **[Power Management](./Power_Management.md)** - Power modes and management
- **[Clock Management](./Clock_Management.md)** - System clock configuration
- **[Hardware Abstraction Layer](./Hardware_Abstraction_Layer.md)** - Porting code between MCUs

---

## 📚 **Resources**

### **Documentation**
- [ARM Cortex-M Reset and Clock Control](https://developer.arm.com/documentation/dui0552/a/cortex-m3-peripherals/system-control-block/reset-control)
- [STM32 Reset and Clock Control](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)

### **Books**
- "Embedded Systems: Introduction to ARM Cortex-M Microcontrollers" by Jonathan Valvano
- "Making Embedded Systems" by Elecia White

### **Online Resources**
- [Embedded.com - Reset Management](https://www.embedded.com/)
- [ARM Developer - Reset and Initialization](https://developer.arm.com/)
