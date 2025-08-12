# 🐕 Watchdog Timers

> **System Monitoring and Recovery Mechanisms for Reliable Embedded Systems**  
> Learn to implement watchdog timers for system health monitoring, fault detection, and automatic recovery

---

## 📋 **Table of Contents**

- [Overview](#overview)
- [Watchdog Types](#watchdog-types)
- [Watchdog Configuration](#watchdog-configuration)
- [System Monitoring](#system-monitoring)
- [Recovery Mechanisms](#recovery-mechanisms)
- [Watchdog Implementation](#watchdog-implementation)
- [Best Practices](#best-practices)
- [Common Pitfalls](#common-pitfalls)
- [Examples](#examples)
- [Interview Questions](#interview-questions)

---

## 🎯 **Overview**

Watchdog timers are essential safety mechanisms that monitor system health and automatically reset the system if it becomes unresponsive or enters an error state. They are critical for reliable embedded systems, especially in safety-critical applications.

### Concept: Last line of defense; pet with intent

The watchdog should be serviced only after critical system health checks pass. Use windowed watchdogs where available to catch “too-fast” faults.

### Minimal example
```c
void watchdog_kick_if_ok(void){
  if (tasks_alive() && comms_ok() && stacks_ok()) IWDG->KR = 0xAAAA; // refresh
}
```

### Takeaways
- Kick from a low-priority task that runs only if the system is healthy.
- Log resets and watchdog reasons; expose them at boot.
- Configure timeouts based on worst-case execution + jitter margins.

---

## 🧪 Guided Labs
1) Watchdog pet timing
- Measure the time between watchdog pets; observe system behavior when petting too early/late.

2) Reset cause logging
- Implement a system that logs and displays the cause of the last reset.

## ✅ Check Yourself
- How do you determine the optimal watchdog timeout for your application?
- When should you use an external watchdog instead of the built-in one?

## 🔗 Cross-links
- `Hardware_Fundamentals/Reset_Management.md` for reset handling
- `Embedded_C/Type_Qualifiers.md` for volatile access

### **Key Concepts**
- **System Monitoring** - Continuous health checking and fault detection
- **Timeout Period** - Maximum time between watchdog feeds
- **Recovery Mechanisms** - Automatic system reset and recovery
- **Fault Detection** - Identifying system failures and errors

---

## 🔄 **Watchdog Types**

### **1. Hardware Watchdog**
Independent hardware timer that operates independently of the main CPU.

```c
// Hardware watchdog configuration
typedef struct {
    uint32_t timeout_ms;          // Timeout period in milliseconds
    uint32_t window_start_ms;     // Window start time (for windowed watchdog)
    uint32_t window_end_ms;       // Window end time
    bool window_mode;             // Enable windowed mode
    void (*pre_reset_callback)(void); // Callback before reset
} hw_watchdog_config_t;

// Hardware watchdog registers
typedef struct {
    volatile uint32_t CR;         // Control register
    volatile uint32_t SR;         // Status register
    volatile uint32_t PR;         // Prescaler register
    volatile uint32_t KR;         // Key register
} IWDG_TypeDef;
```

### **2. Software Watchdog**
Software-based monitoring that runs as a task or timer interrupt.

```c
// Software watchdog structure
typedef struct {
    uint32_t timeout_ms;          // Timeout period
    uint32_t last_feed_time;      // Last feed timestamp
    bool enabled;                 // Watchdog enabled flag
    uint8_t task_id;              // Monitored task ID
    void (*timeout_handler)(void); // Timeout handler function
} sw_watchdog_t;

// Software watchdog configuration
typedef struct {
    uint32_t check_interval_ms;   // How often to check
    uint32_t timeout_ms;          // Timeout period
    bool auto_reset;              // Auto reset on timeout
} sw_watchdog_config_t;
```

### **3. Windowed Watchdog**
Hardware watchdog with a time window for feeding.

```c
// Windowed watchdog configuration
typedef struct {
    uint32_t window_start_ms;     // Start of feeding window
    uint32_t window_end_ms;       // End of feeding window
    uint32_t timeout_ms;          // Total timeout period
    bool early_warning;           // Enable early warning interrupt
} windowed_watchdog_config_t;
```

---

## ⚙️ **Watchdog Configuration**

### **1. Hardware Watchdog Setup**

```c
// Initialize hardware watchdog
void init_hardware_watchdog(uint32_t timeout_ms) {
    // Enable IWDG clock
    RCC->CSR |= RCC_CSR_LSION;
    
    // Wait for LSI to be ready
    while (!(RCC->CSR & RCC_CSR_LSIRDY));
    
    // Configure prescaler (LSI = 40kHz)
    // Prescaler options: 4, 8, 16, 32, 64, 128, 256
    uint32_t prescaler = 256; // 40kHz / 256 = 156.25 Hz
    IWDG->PR = (prescaler >> 2) - 1;
    
    // Calculate reload value
    uint32_t reload = (timeout_ms * 156) / 1000; // 156.25 Hz
    IWDG->RLR = reload;
    
    // Enable watchdog
    IWDG->KR = 0xCCCC;
    
    // Start watchdog
    IWDG->KR = 0xAAAA;
}

// Feed hardware watchdog
void feed_hardware_watchdog(void) {
    // Write key to reload counter
    IWDG->KR = 0xAAAA;
}
```

### **2. Software Watchdog Setup**

```c
// Initialize software watchdog
void init_software_watchdog(sw_watchdog_config_t *config) {
    // Allocate watchdog structure
    sw_watchdog_t *watchdog = malloc(sizeof(sw_watchdog_t));
    
    if (watchdog) {
        watchdog->timeout_ms = config->timeout_ms;
        watchdog->last_feed_time = get_system_tick();
        watchdog->enabled = true;
        watchdog->timeout_handler = config->timeout_handler;
        
        // Start watchdog task
        create_watchdog_task(watchdog, config->check_interval_ms);
    }
}

// Software watchdog task
void watchdog_task(void *pvParameters) {
    sw_watchdog_t *watchdog = (sw_watchdog_t *)pvParameters;
    
    while (watchdog->enabled) {
        uint32_t current_time = get_system_tick();
        
        // Check if timeout has occurred
        if (current_time - watchdog->last_feed_time > watchdog->timeout_ms) {
            // Timeout occurred
            if (watchdog->timeout_handler) {
                watchdog->timeout_handler();
            }
            
            // Reset system if auto-reset enabled
            if (watchdog->auto_reset) {
                system_reset();
            }
        }
        
        // Wait for next check
        vTaskDelay(pdMS_TO_TICKS(100)); // Check every 100ms
    }
}
```

### **3. Windowed Watchdog Setup**

```c
// Initialize windowed watchdog
void init_windowed_watchdog(windowed_watchdog_config_t *config) {
    // Enable WWDG clock
    RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;
    
    // Configure window
    uint32_t window_value = (config->window_start_ms * 1000) / 122; // 122 Hz
    uint32_t timeout_value = (config->timeout_ms * 1000) / 122;
    
    // Configure WWDG
    WWDG->CR = WWDG_CR_WDGA | WWDG_CR_T | timeout_value;
    WWDG->CFR = WWDG_CFR_WDGA | WWDG_CFR_W | window_value;
    
    // Enable early warning interrupt if requested
    if (config->early_warning) {
        WWDG->CFR |= WWDG_CFR_EWI;
        NVIC_EnableIRQ(WWDG_IRQn);
    }
}
```

---

## 🔍 **System Monitoring**

### **1. Task Monitoring**

```c
// Task monitoring structure
typedef struct {
    uint8_t task_id;
    uint32_t last_heartbeat;
    uint32_t timeout_ms;
    bool active;
} task_monitor_t;

task_monitor_t task_monitors[MAX_TASKS];
volatile uint32_t task_heartbeats[MAX_TASKS];

// Register task for monitoring
void register_task_monitoring(uint8_t task_id, uint32_t timeout_ms) {
    if (task_id < MAX_TASKS) {
        task_monitors[task_id].task_id = task_id;
        task_monitors[task_id].timeout_ms = timeout_ms;
        task_monitors[task_id].active = true;
        task_monitors[task_id].last_heartbeat = get_system_tick();
    }
}

// Task heartbeat function
void task_heartbeat(uint8_t task_id) {
    if (task_id < MAX_TASKS) {
        task_heartbeats[task_id] = get_system_tick();
        task_monitors[task_id].last_heartbeat = task_heartbeats[task_id];
    }
}

// Check task health
bool check_task_health(uint8_t task_id) {
    if (task_id >= MAX_TASKS || !task_monitors[task_id].active) {
        return false;
    }
    
    uint32_t current_time = get_system_tick();
    uint32_t time_since_heartbeat = current_time - task_monitors[task_id].last_heartbeat;
    
    return (time_since_heartbeat < task_monitors[task_id].timeout_ms);
}
```

### **2. System Health Monitoring**

```c
// System health monitoring
typedef struct {
    uint32_t cpu_usage;
    uint32_t memory_usage;
    uint32_t stack_usage;
    bool watchdog_active;
    uint32_t error_count;
} system_health_t;

volatile system_health_t system_health;

// Monitor system health
void monitor_system_health(void) {
    // Check CPU usage
    system_health.cpu_usage = get_cpu_usage();
    
    // Check memory usage
    system_health.memory_usage = get_memory_usage();
    
    // Check stack usage
    system_health.stack_usage = get_stack_usage();
    
    // Check watchdog status
    system_health.watchdog_active = is_watchdog_active();
    
    // Check for errors
    if (system_health.error_count > MAX_ERRORS) {
        // Too many errors, trigger recovery
        trigger_system_recovery();
    }
}

// System health check
bool is_system_healthy(void) {
    return (system_health.cpu_usage < MAX_CPU_USAGE &&
            system_health.memory_usage < MAX_MEMORY_USAGE &&
            system_health.stack_usage < MAX_STACK_USAGE &&
            system_health.watchdog_active);
}
```

### **3. Application-Specific Monitoring**

```c
// Application monitoring structure
typedef struct {
    uint32_t last_communication;
    uint32_t last_sensor_read;
    uint32_t last_actuator_update;
    bool communication_ok;
    bool sensor_ok;
    bool actuator_ok;
} application_health_t;

volatile application_health_t app_health;

// Monitor application health
void monitor_application_health(void) {
    uint32_t current_time = get_system_tick();
    
    // Check communication
    if (current_time - app_health.last_communication > COMM_TIMEOUT_MS) {
        app_health.communication_ok = false;
    }
    
    // Check sensor readings
    if (current_time - app_health.last_sensor_read > SENSOR_TIMEOUT_MS) {
        app_health.sensor_ok = false;
    }
    
    // Check actuator updates
    if (current_time - app_health.last_actuator_update > ACTUATOR_TIMEOUT_MS) {
        app_health.actuator_ok = false;
    }
    
    // Trigger recovery if needed
    if (!app_health.communication_ok || !app_health.sensor_ok || !app_health.actuator_ok) {
        trigger_application_recovery();
    }
}
```

---

## 🔄 **Recovery Mechanisms**

### **1. System Reset**

```c
// System reset function
void system_reset(void) {
    // Save critical data before reset
    save_critical_data();
    
    // Disable interrupts
    __disable_irq();
    
    // Trigger system reset
    SCB->AIRCR = SCB_AIRCR_VECTKEY | SCB_AIRCR_SYSRESETREQ;
    
    // Wait for reset
    while (1) {
        __NOP();
    }
}

// Save critical data before reset
void save_critical_data(void) {
    // Save to backup registers or non-volatile memory
    RTC->BKP0R = get_system_uptime();
    RTC->BKP1R = get_error_count();
    RTC->BKP2R = get_last_error_code();
    
    // Save to flash if needed
    save_to_flash_backup();
}
```

### **2. Graceful Recovery**

```c
// Graceful recovery mechanism
typedef enum {
    RECOVERY_NONE,
    RECOVERY_RESTART_TASK,
    RECOVERY_RESTART_APPLICATION,
    RECOVERY_SYSTEM_RESET
} recovery_level_t;

// Recovery handler
void handle_system_recovery(recovery_level_t level) {
    switch (level) {
        case RECOVERY_RESTART_TASK:
            restart_failed_task();
            break;
            
        case RECOVERY_RESTART_APPLICATION:
            restart_application();
            break;
            
        case RECOVERY_SYSTEM_RESET:
            system_reset();
            break;
            
        default:
            // No recovery needed
            break;
    }
}

// Restart failed task
void restart_failed_task(void) {
    // Stop the failed task
    vTaskSuspend(failed_task_handle);
    
    // Clear task state
    clear_task_state();
    
    // Restart the task
    vTaskResume(failed_task_handle);
}
```

### **3. Error Logging and Reporting**

```c
// Error logging structure
typedef struct {
    uint32_t timestamp;
    uint8_t error_code;
    uint8_t error_source;
    uint32_t additional_data;
} error_log_entry_t;

#define MAX_ERROR_LOG_ENTRIES 100
error_log_entry_t error_log[MAX_ERROR_LOG_ENTRIES];
volatile uint8_t error_log_index = 0;

// Log error
void log_error(uint8_t error_code, uint8_t error_source, uint32_t additional_data) {
    error_log_entry_t *entry = &error_log[error_log_index];
    
    entry->timestamp = get_system_tick();
    entry->error_code = error_code;
    entry->error_source = error_source;
    entry->additional_data = additional_data;
    
    // Increment index
    error_log_index = (error_log_index + 1) % MAX_ERROR_LOG_ENTRIES;
    
    // Increment error count
    system_health.error_count++;
}

// Get error log
void get_error_log(error_log_entry_t *log_buffer, uint8_t *count) {
    uint8_t i;
    uint8_t log_count = 0;
    
    for (i = 0; i < MAX_ERROR_LOG_ENTRIES; i++) {
        uint8_t index = (error_log_index - 1 - i + MAX_ERROR_LOG_ENTRIES) % MAX_ERROR_LOG_ENTRIES;
        
        if (error_log[index].timestamp != 0) {
            log_buffer[log_count] = error_log[index];
            log_count++;
        }
    }
    
    *count = log_count;
}
```

---

## 🎯 **Watchdog Implementation**

### **1. Complete Watchdog System**

```c
// Complete watchdog system
typedef struct {
    hw_watchdog_config_t hw_config;
    sw_watchdog_config_t sw_config;
    bool hw_watchdog_enabled;
    bool sw_watchdog_enabled;
    uint32_t last_feed_time;
    void (*pre_reset_callback)(void);
} watchdog_system_t;

watchdog_system_t watchdog_system;

// Initialize complete watchdog system
void init_watchdog_system(watchdog_system_t *system) {
    // Initialize hardware watchdog
    if (system->hw_watchdog_enabled) {
        init_hardware_watchdog(system->hw_config.timeout_ms);
    }
    
    // Initialize software watchdog
    if (system->sw_watchdog_enabled) {
        init_software_watchdog(&system->sw_config);
    }
    
    // Initialize monitoring
    init_system_monitoring();
    
    // Set initial feed time
    system->last_feed_time = get_system_tick();
}

// Feed watchdog system
void feed_watchdog_system(watchdog_system_t *system) {
    uint32_t current_time = get_system_tick();
    
    // Check if system is healthy
    if (!is_system_healthy()) {
        // System is not healthy, don't feed watchdog
        return;
    }
    
    // Feed hardware watchdog
    if (system->hw_watchdog_enabled) {
        feed_hardware_watchdog();
    }
    
    // Update software watchdog
    if (system->sw_watchdog_enabled) {
        system->last_feed_time = current_time;
    }
    
    // Update task heartbeats
    update_task_heartbeats();
}
```

### **2. Watchdog Task**

```c
// Watchdog monitoring task
void watchdog_monitor_task(void *pvParameters) {
    watchdog_system_t *system = (watchdog_system_t *)pvParameters;
    
    while (1) {
        // Check system health
        if (!is_system_healthy()) {
            // System is not healthy, trigger recovery
            if (system->pre_reset_callback) {
                system->pre_reset_callback();
            }
            
            // Wait for recovery or reset
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
        
        // Feed watchdog if system is healthy
        feed_watchdog_system(system);
        
        // Wait for next check
        vTaskDelay(pdMS_TO_TICKS(100)); // Check every 100ms
    }
}

// Create watchdog monitoring task
void create_watchdog_monitor_task(watchdog_system_t *system) {
    xTaskCreate(watchdog_monitor_task,
                "WatchdogMonitor",
                WATCHDOG_TASK_STACK_SIZE,
                system,
                WATCHDOG_TASK_PRIORITY,
                NULL);
}
```

---

## 🎯 **Best Practices**

### **1. Watchdog Design Guidelines**

```c
// Watchdog design checklist
/*
    □ Set appropriate timeout values
    □ Implement proper feeding mechanism
    □ Monitor critical system components
    □ Handle recovery gracefully
    □ Log errors and events
    □ Test watchdog functionality
    □ Document watchdog behavior
    □ Consider power consumption
    □ Implement early warning
    □ Handle watchdog during development
*/

// Good watchdog implementation
void good_watchdog_implementation(void) {
    // Set reasonable timeout (not too short, not too long)
    uint32_t timeout_ms = 5000; // 5 seconds
    
    // Initialize watchdog
    init_hardware_watchdog(timeout_ms);
    
    // Start monitoring task
    create_watchdog_monitor_task();
    
    // Regular feeding in main loop
    while (1) {
        // Process system tasks
        process_system_tasks();
        
        // Feed watchdog if system is healthy
        if (is_system_healthy()) {
            feed_hardware_watchdog();
        }
        
        // Wait for next cycle
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

### **2. Timeout Selection**

```c
// Timeout selection guidelines
void select_watchdog_timeout(void) {
    /*
        Timeout selection considerations:
        
        1. Application requirements:
           - Real-time constraints
           - Safety requirements
           - User experience
        
        2. System characteristics:
           - Processing time
           - Communication delays
           - Sensor response time
        
        3. Typical values:
           - Simple systems: 1-5 seconds
           - Complex systems: 5-30 seconds
           - Safety-critical: 100ms-1s
    */
    
    // Example timeout selection
    uint32_t timeout_ms;
    
    if (is_safety_critical_system()) {
        timeout_ms = 500; // 500ms for safety-critical
    } else if (is_real_time_system()) {
        timeout_ms = 2000; // 2s for real-time
    } else {
        timeout_ms = 10000; // 10s for general purpose
    }
}
```

---

## ⚠️ **Common Pitfalls**

### **1. Incorrect Timeout Values**

```c
// WRONG: Too short timeout
void bad_short_timeout(void) {
    // 100ms is too short for most applications
    init_hardware_watchdog(100);
}

// CORRECT: Appropriate timeout
void good_timeout(void) {
    // 5 seconds is reasonable for most applications
    init_hardware_watchdog(5000);
}
```

### **2. Missing Watchdog Feeding**

```c
// WRONG: No watchdog feeding
void bad_no_feeding(void) {
    while (1) {
        process_tasks();
        // Missing: feed_watchdog();
    }
}

// CORRECT: Regular watchdog feeding
void good_feeding(void) {
    while (1) {
        process_tasks();
        
        // Feed watchdog regularly
        if (is_system_healthy()) {
            feed_watchdog();
        }
    }
}
```

### **3. Improper Recovery Handling**

```c
// WRONG: No recovery mechanism
void bad_no_recovery(void) {
    if (system_failed()) {
        // Just wait, no recovery
        while (1) {
            __NOP();
        }
    }
}

// CORRECT: Proper recovery
void good_recovery(void) {
    if (system_failed()) {
        // Log error
        log_error(ERROR_SYSTEM_FAILURE, SOURCE_WATCHDOG, 0);
        
        // Attempt recovery
        if (!attempt_recovery()) {
            // If recovery fails, reset system
            system_reset();
        }
    }
}
```

---

## 💡 **Examples**

### **1. Simple Hardware Watchdog**

```c
// Simple hardware watchdog implementation
#define WATCHDOG_TIMEOUT_MS 5000

void init_simple_watchdog(void) {
    // Enable IWDG clock
    RCC->CSR |= RCC_CSR_LSION;
    
    // Wait for LSI ready
    while (!(RCC->CSR & RCC_CSR_LSIRDY));
    
    // Configure IWDG (40kHz LSI, 256 prescaler = 156.25 Hz)
    IWDG->PR = 6; // 256 prescaler
    IWDG->RLR = 781; // 5 second timeout (781 * 1/156.25 = 5s)
    
    // Enable watchdog
    IWDG->KR = 0xCCCC;
    IWDG->KR = 0xAAAA;
}

void feed_watchdog(void) {
    IWDG->KR = 0xAAAA;
}

// Main loop with watchdog
void main_loop(void) {
    init_simple_watchdog();
    
    while (1) {
        // Process application tasks
        process_application();
        
        // Feed watchdog
        feed_watchdog();
        
        // Small delay
        delay_ms(100);
    }
}
```

### **2. Advanced Watchdog with Monitoring**

```c
// Advanced watchdog with system monitoring
typedef struct {
    uint32_t task_heartbeats[MAX_TASKS];
    uint32_t last_communication;
    uint32_t last_sensor_read;
    bool system_healthy;
} system_monitor_t;

volatile system_monitor_t system_monitor;

void advanced_watchdog_init(void) {
    // Initialize hardware watchdog
    init_hardware_watchdog(10000); // 10 second timeout
    
    // Initialize system monitor
    memset(&system_monitor, 0, sizeof(system_monitor_t));
    system_monitor.system_healthy = true;
    
    // Start monitoring task
    xTaskCreate(monitor_task, "Monitor", 512, NULL, 3, NULL);
}

void monitor_task(void *pvParameters) {
    while (1) {
        uint32_t current_time = get_system_tick();
        
        // Check task heartbeats
        for (int i = 0; i < MAX_TASKS; i++) {
            if (current_time - system_monitor.task_heartbeats[i] > TASK_TIMEOUT_MS) {
                system_monitor.system_healthy = false;
                log_error(ERROR_TASK_TIMEOUT, i, current_time);
            }
        }
        
        // Check communication
        if (current_time - system_monitor.last_communication > COMM_TIMEOUT_MS) {
            system_monitor.system_healthy = false;
            log_error(ERROR_COMM_TIMEOUT, 0, current_time);
        }
        
        // Check sensors
        if (current_time - system_monitor.last_sensor_read > SENSOR_TIMEOUT_MS) {
            system_monitor.system_healthy = false;
            log_error(ERROR_SENSOR_TIMEOUT, 0, current_time);
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void feed_advanced_watchdog(void) {
    if (system_monitor.system_healthy) {
        feed_hardware_watchdog();
    }
}
```

### **3. Watchdog with Recovery Mechanisms**

```c
// Watchdog with recovery mechanisms
typedef enum {
    RECOVERY_NONE,
    RECOVERY_RESTART_TASK,
    RECOVERY_RESTART_APPLICATION,
    RECOVERY_SYSTEM_RESET
} recovery_level_t;

void watchdog_with_recovery(void) {
    // Initialize watchdog
    init_hardware_watchdog(5000);
    
    // Initialize recovery system
    init_recovery_system();
    
    while (1) {
        // Check system health
        if (!is_system_healthy()) {
            recovery_level_t level = determine_recovery_level();
            handle_recovery(level);
        }
        
        // Feed watchdog if healthy
        if (is_system_healthy()) {
            feed_hardware_watchdog();
        }
        
        // Process tasks
        process_tasks();
        
        delay_ms(100);
    }
}

recovery_level_t determine_recovery_level(void) {
    if (get_error_count() > MAX_ERRORS) {
        return RECOVERY_SYSTEM_RESET;
    } else if (get_task_failure_count() > 0) {
        return RECOVERY_RESTART_TASK;
    } else {
        return RECOVERY_RESTART_APPLICATION;
    }
}

void handle_recovery(recovery_level_t level) {
    switch (level) {
        case RECOVERY_RESTART_TASK:
            restart_failed_tasks();
            break;
        case RECOVERY_RESTART_APPLICATION:
            restart_application();
            break;
        case RECOVERY_SYSTEM_RESET:
            system_reset();
            break;
        default:
            break;
    }
}
```

---

## 🎯 **Interview Questions**

### **Basic Questions**
1. **What is a watchdog timer and why is it important?**
   - Hardware/software timer that monitors system health
   - Automatically resets system if it becomes unresponsive
   - Critical for reliable embedded systems

2. **What are the different types of watchdog timers?**
   - Hardware watchdog: Independent hardware timer
   - Software watchdog: Software-based monitoring
   - Windowed watchdog: Time window for feeding

3. **How do you configure a watchdog timer?**
   - Set timeout period based on application requirements
   - Configure prescaler and reload values
   - Enable watchdog and start monitoring

### **Advanced Questions**
4. **How do you implement system monitoring with watchdog timers?**
   - Monitor task heartbeats
   - Check system health metrics
   - Implement error logging and reporting

5. **What are the best practices for watchdog timer design?**
   - Set appropriate timeout values
   - Implement proper feeding mechanism
   - Handle recovery gracefully
   - Test watchdog functionality

6. **How do you handle watchdog timers during development and debugging?**
   - Disable during development if needed
   - Use longer timeouts for debugging
   - Implement development mode

### **Practical Questions**
7. **Design a watchdog system for a safety-critical application.**
   ```c
   void safety_critical_watchdog(void) {
       // Short timeout for safety-critical
       init_hardware_watchdog(500); // 500ms
       
       // Monitor critical functions
       while (1) {
           if (is_safety_system_healthy()) {
               feed_watchdog();
           } else {
               // Immediate reset for safety
               system_reset();
           }
           
           process_safety_tasks();
       }
   }
   ```

8. **Implement a watchdog with multiple recovery levels.**
   ```c
   void multi_level_recovery_watchdog(void) {
       init_watchdog_system();
       
       while (1) {
           if (!is_system_healthy()) {
               recovery_level_t level = assess_system_health();
               execute_recovery(level);
           }
           
           feed_watchdog();
           process_tasks();
       }
   }
   ```

---

## 🔗 **Related Topics**

- **[External Interrupts](./External_Interrupts.md)** - Edge/level triggered interrupts, debouncing
- **[Interrupts and Exceptions](./Interrupts_Exceptions.md)** - Interrupt handling, ISR design, interrupt latency
- **[Power Management](./Power_Management.md)** - Sleep modes, wake-up sources, power consumption optimization
- **[System Integration](./../System_Integration/README.md)** - System development, firmware updates, error handling

---

## 📚 **Resources**

### **Books**
- "Making Embedded Systems" by Elecia White
- "Programming Embedded Systems" by Michael Barr
- "Real-Time Systems" by Jane W. S. Liu

### **Online Resources**
- [ARM Cortex-M Watchdog Timers](https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/watchdog-timers)
- [STM32 Independent Watchdog](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)

---

**Next Topic:** [Interrupts and Exceptions](./Interrupts_Exceptions.md) → [Power Management](./Power_Management.md)
