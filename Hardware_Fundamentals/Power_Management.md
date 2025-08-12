# 🔋 Power Management

> **Optimizing Power Consumption for Battery-Powered and Energy-Efficient Embedded Systems**  
> Learn to implement sleep modes, wake-up sources, and power optimization techniques

---

## 📋 **Table of Contents**

- [Overview](#overview)
- [Power Modes](#power-modes)
- [Sleep Modes](#sleep-modes)
- [Wake-up Sources](#wake-up-sources)
- [Power Optimization](#power-optimization)
- [Clock Management](#clock-management)
- [Peripheral Power Management](#peripheral-power-management)
- [Battery Management](#battery-management)
- [Power Monitoring](#power-monitoring)
- [Best Practices](#best-practices)
- [Common Pitfalls](#common-pitfalls)
- [Examples](#examples)
- [Interview Questions](#interview-questions)

---

## 🎯 **Overview**

Power management is critical for battery-powered embedded systems and energy-efficient applications. Effective power management extends battery life, reduces heat generation, and enables portable and IoT devices.

### Concept: Budget power per state and per wakeup

Power is owned by your state machine: define active/idle/sleep states with known currents and wakeup sources. Measure, don’t guess.

### Minimal example
```c
typedef enum { RUN, IDLE, SLEEP } pm_state_t;
void enter_idle(void){ /* reduce clocks, gate peripherals */ }
void enter_sleep(void){ /* tickless idle, stop clocks, enable wakeups */ }
```

### Takeaways
- Use tickless idle when latency budget allows; verify wake sources.
- Gate unused clocks/peripherals; disable pull-ups that leak.
- Quantify energy/event (uC per sensor read) to compare designs.

---

## 🧪 Guided Labs
1) Power state measurement
- Measure current consumption in different power states using a multimeter or power analyzer.

2) Wake-up source testing
- Test different wake-up sources and measure wake-up time and power consumption.

## ✅ Check Yourself
- How do you calculate the total power budget for your system?
- When should you use deep sleep vs light sleep modes?

## 🔗 Cross-links
- `Hardware_Fundamentals/Clock_Management.md` for clock gating
- `Hardware_Fundamentals/Watchdog_Timers.md` for wake-up sources

### **Key Concepts**
- **Sleep Modes** - Different power states for energy conservation
- **Wake-up Sources** - Events that bring system out of sleep
- **Power Optimization** - Techniques to minimize power consumption
- **Battery Management** - Monitoring and optimizing battery usage

---

## 🔄 **Power Modes**

### **1. Active Mode**
Full system operation with all peripherals enabled.

```c
// Active mode configuration
typedef struct {
    uint32_t cpu_frequency;      // CPU frequency in Hz
    bool peripherals_enabled;    // All peripherals enabled
    uint32_t power_consumption;  // Power consumption in mW
} active_mode_config_t;

// Active mode power consumption
void configure_active_mode(active_mode_config_t *config) {
    // Set CPU frequency
    set_cpu_frequency(config->cpu_frequency);
    
    // Enable all required peripherals
    if (config->peripherals_enabled) {
        enable_all_peripherals();
    }
    
    // Monitor power consumption
    monitor_power_consumption();
}
```

### **2. Sleep Mode**
Reduced power consumption with some peripherals disabled.

```c
// Sleep mode configuration
typedef struct {
    sleep_mode_t mode;           // Sleep mode type
    uint32_t wake_up_time;       // Wake-up time in ms
    wake_up_source_t sources;    // Wake-up sources
    bool peripherals_disabled;   // Disable unused peripherals
} sleep_mode_config_t;

// Sleep mode types
typedef enum {
    SLEEP_MODE_LIGHT,    // Light sleep - CPU stopped, peripherals active
    SLEEP_MODE_DEEP,     // Deep sleep - CPU and most peripherals stopped
    SLEEP_MODE_STANDBY,  // Standby - Only backup domain active
    SLEEP_MODE_HIBERNATE // Hibernate - Only RTC active
} sleep_mode_t;
```

### **3. Power State Transitions**

```c
// Power state transition
typedef enum {
    POWER_STATE_ACTIVE,
    POWER_STATE_SLEEP,
    POWER_STATE_DEEP_SLEEP,
    POWER_STATE_STANDBY
} power_state_t;

// Power state management
typedef struct {
    power_state_t current_state;
    power_state_t target_state;
    uint32_t transition_time;
    bool transition_in_progress;
} power_state_manager_t;

// Transition to power state
void transition_to_power_state(power_state_t target_state) {
    power_state_manager_t *pm = get_power_state_manager();
    
    if (pm->current_state != target_state) {
        // Prepare for transition
        prepare_power_transition(target_state);
        
        // Execute transition
        execute_power_transition(target_state);
        
        // Update state
        pm->current_state = target_state;
    }
}
```

---

## 😴 **Sleep Modes**

### **1. Light Sleep Mode**
CPU stopped but peripherals and memory remain active.

```c
// Light sleep mode implementation
void enter_light_sleep(void) {
    // Save current state
    save_system_state();
    
    // Disable CPU
    __WFI(); // Wait for interrupt
    
    // Restore state after wake-up
    restore_system_state();
}

// Light sleep configuration
void configure_light_sleep(void) {
    // Configure wake-up sources
    configure_wake_up_sources();
    
    // Set sleep mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    
    // Enable sleep mode
    __enable_irq();
}
```

### **2. Deep Sleep Mode**
CPU and most peripherals stopped, only essential functions active.

```c
// Deep sleep mode implementation
void enter_deep_sleep(void) {
    // Save critical data
    save_critical_data();
    
    // Disable unused peripherals
    disable_unused_peripherals();
    
    // Configure deep sleep
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    
    // Enter deep sleep
    __WFI();
    
    // Restore after wake-up
    restore_critical_data();
    enable_peripherals();
}

// Deep sleep configuration
void configure_deep_sleep(void) {
    // Configure wake-up sources
    configure_deep_sleep_wake_up();
    
    // Set deep sleep mode
    PWR->CR |= PWR_CR_LPDS;
    
    // Configure voltage scaling
    PWR->CR |= PWR_CR_VOS;
}
```

### **3. Standby Mode**
Only backup domain and RTC active, all other functions stopped.

```c
// Standby mode implementation
void enter_standby_mode(void) {
    // Save essential data to backup registers
    save_to_backup_registers();
    
    // Configure standby mode
    PWR->CR |= PWR_CR_CWUF;
    PWR->CR |= PWR_CR_PDDS;
    
    // Enter standby
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
    
    // System will reset after wake-up
}

// Standby mode configuration
void configure_standby_mode(void) {
    // Configure RTC as wake-up source
    configure_rtc_wake_up();
    
    // Enable backup domain
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;
}
```

---

## 🔔 **Wake-up Sources**

### **1. External Interrupts**

```c
// External interrupt wake-up configuration
typedef struct {
    uint8_t pin;
    edge_type_t edge;
    bool enabled;
} external_wake_up_config_t;

// Configure external wake-up
void configure_external_wake_up(external_wake_up_config_t *config) {
    // Configure GPIO as input
    configure_gpio_input(config->pin);
    
    // Configure interrupt
    configure_external_interrupt(config->pin, config->edge);
    
    // Enable wake-up capability
    EXTI->IMR |= (1 << config->pin);
    
    // Enable in NVIC
    NVIC_EnableIRQ(EXTI0_IRQn + config->pin);
}

// External wake-up handler
void external_wake_up_handler(void) {
    // Clear wake-up flag
    PWR->CR |= PWR_CR_CWUF;
    
    // Process wake-up event
    process_wake_up_event();
}
```

### **2. Timer Wake-up**

```c
// Timer wake-up configuration
typedef struct {
    uint32_t wake_up_time_ms;
    timer_type_t timer_type;
    bool enabled;
} timer_wake_up_config_t;

// Configure timer wake-up
void configure_timer_wake_up(timer_wake_up_config_t *config) {
    // Configure timer
    configure_timer(config->timer_type, config->wake_up_time_ms);
    
    // Enable timer interrupt
    enable_timer_interrupt(config->timer_type);
    
    // Configure as wake-up source
    configure_timer_wake_up_source(config->timer_type);
}

// Timer wake-up handler
void timer_wake_up_handler(void) {
    // Clear timer interrupt
    clear_timer_interrupt();
    
    // Process timer wake-up
    process_timer_wake_up();
}
```

### **3. RTC Wake-up**

```c
// RTC wake-up configuration
typedef struct {
    uint32_t wake_up_time;
    rtc_wake_up_source_t source;
    bool enabled;
} rtc_wake_up_config_t;

// Configure RTC wake-up
void configure_rtc_wake_up(rtc_wake_up_config_t *config) {
    // Configure RTC
    configure_rtc();
    
    // Set wake-up time
    set_rtc_wake_up_time(config->wake_up_time);
    
    // Enable RTC wake-up
    RTC->CR |= RTC_CR_WUTE;
    
    // Enable RTC interrupt
    NVIC_EnableIRQ(RTC_IRQn);
}

// RTC wake-up handler
void rtc_wake_up_handler(void) {
    // Clear RTC wake-up flag
    RTC->ISR &= ~RTC_ISR_WUTF;
    
    // Process RTC wake-up
    process_rtc_wake_up();
}
```

---

## ⚡ **Power Optimization**

### **1. CPU Power Optimization**

```c
// CPU power optimization
typedef struct {
    uint32_t frequency;
    voltage_scale_t voltage;
    bool dynamic_scaling;
} cpu_power_config_t;

// Configure CPU power
void configure_cpu_power(cpu_power_config_t *config) {
    // Set voltage scaling
    set_voltage_scaling(config->voltage);
    
    // Set CPU frequency
    set_cpu_frequency(config->frequency);
    
    // Enable dynamic frequency scaling
    if (config->dynamic_scaling) {
        enable_dynamic_frequency_scaling();
    }
}

// Dynamic frequency scaling
void enable_dynamic_frequency_scaling(void) {
    // Monitor CPU load
    uint32_t cpu_load = get_cpu_load();
    
    if (cpu_load < 30) {
        // Reduce frequency for low load
        set_cpu_frequency(CPU_FREQ_LOW);
    } else if (cpu_load > 80) {
        // Increase frequency for high load
        set_cpu_frequency(CPU_FREQ_HIGH);
    }
}
```

### **2. Peripheral Power Optimization**

```c
// Peripheral power management
typedef struct {
    peripheral_type_t peripheral;
    bool enabled;
    uint32_t power_consumption;
} peripheral_power_config_t;

// Disable unused peripherals
void disable_unused_peripherals(void) {
    // Disable unused UARTs
    if (!uart1_used) {
        disable_peripheral(UART1);
    }
    
    // Disable unused timers
    if (!timer1_used) {
        disable_peripheral(TIM1);
    }
    
    // Disable unused ADCs
    if (!adc1_used) {
        disable_peripheral(ADC1);
    }
}

// Enable peripheral only when needed
void enable_peripheral_on_demand(peripheral_type_t peripheral) {
    // Enable peripheral
    enable_peripheral(peripheral);
    
    // Use peripheral
    use_peripheral(peripheral);
    
    // Disable peripheral after use
    disable_peripheral(peripheral);
}
```

### **3. Memory Power Optimization**

```c
// Memory power optimization
typedef struct {
    bool flash_power_down;
    bool sram_retention;
    bool cache_enabled;
} memory_power_config_t;

// Configure memory power
void configure_memory_power(memory_power_config_t *config) {
    if (config->flash_power_down) {
        // Power down flash when not in use
        power_down_flash();
    }
    
    if (config->sram_retention) {
        // Enable SRAM retention in sleep mode
        enable_sram_retention();
    }
    
    if (config->cache_enabled) {
        // Enable cache for better performance
        enable_cache();
    }
}
```

---

## ⏰ **Clock Management**

### **1. Clock Configuration**

```c
// Clock configuration
typedef struct {
    uint32_t system_clock;
    uint32_t peripheral_clock;
    bool pll_enabled;
    clock_source_t source;
} clock_config_t;

// Configure system clock
void configure_system_clock(clock_config_t *config) {
    // Configure PLL if enabled
    if (config->pll_enabled) {
        configure_pll(config->system_clock);
    }
    
    // Set system clock
    set_system_clock(config->system_clock);
    
    // Configure peripheral clocks
    configure_peripheral_clocks(config->peripheral_clock);
}

// Dynamic clock scaling
void dynamic_clock_scaling(void) {
    uint32_t current_load = get_system_load();
    
    if (current_load < 20) {
        // Low load - reduce clock frequency
        set_system_clock(SYSTEM_CLOCK_LOW);
    } else if (current_load > 80) {
        // High load - increase clock frequency
        set_system_clock(SYSTEM_CLOCK_HIGH);
    }
}
```

### **2. Clock Gating**

```c
// Clock gating for power saving
void enable_clock_gating(void) {
    // Gate unused peripheral clocks
    RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);
    RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN | RCC_APB1ENR_TIM3EN);
    RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN | RCC_APB2ENR_TIM1EN);
}

// Enable clock only when needed
void enable_peripheral_clock(peripheral_type_t peripheral) {
    switch (peripheral) {
        case PERIPHERAL_UART1:
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            break;
        case PERIPHERAL_TIM1:
            RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
            break;
        case PERIPHERAL_ADC1:
            RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
            break;
    }
}
```

---

## 🔋 **Battery Management**

### **1. Battery Monitoring**

```c
// Battery monitoring
typedef struct {
    uint32_t voltage;
    uint32_t capacity;
    uint32_t remaining;
    battery_status_t status;
} battery_info_t;

// Battery status
typedef enum {
    BATTERY_STATUS_UNKNOWN,
    BATTERY_STATUS_CHARGING,
    BATTERY_STATUS_DISCHARGING,
    BATTERY_STATUS_FULL,
    BATTERY_STATUS_LOW,
    BATTERY_STATUS_CRITICAL
} battery_status_t;

// Monitor battery
void monitor_battery(void) {
    battery_info_t battery;
    
    // Read battery voltage
    battery.voltage = read_battery_voltage();
    
    // Calculate remaining capacity
    battery.remaining = calculate_battery_capacity(battery.voltage);
    
    // Update battery status
    battery.status = get_battery_status(battery.voltage);
    
    // Handle low battery
    if (battery.status == BATTERY_STATUS_CRITICAL) {
        handle_critical_battery();
    }
}
```

### **2. Battery Optimization**

```c
// Battery optimization strategies
void optimize_for_battery_life(void) {
    // Reduce CPU frequency
    set_cpu_frequency(CPU_FREQ_LOW);
    
    // Disable unused peripherals
    disable_unused_peripherals();
    
    // Enable sleep modes
    enable_sleep_modes();
    
    // Optimize communication
    optimize_communication_power();
    
    // Reduce sensor sampling rate
    reduce_sensor_sampling_rate();
}

// Critical battery handling
void handle_critical_battery(void) {
    // Save critical data
    save_critical_data();
    
    // Enter deep sleep mode
    enter_deep_sleep();
    
    // Configure wake-up only for critical events
    configure_critical_wake_up_sources();
}
```

---

## 📊 **Power Monitoring**

### **1. Power Consumption Monitoring**

```c
// Power consumption monitoring
typedef struct {
    uint32_t current_consumption;
    uint32_t average_consumption;
    uint32_t peak_consumption;
    uint32_t total_energy;
} power_consumption_t;

// Monitor power consumption
void monitor_power_consumption(void) {
    power_consumption_t power;
    
    // Read current consumption
    power.current_consumption = read_current_consumption();
    
    // Update average consumption
    update_average_consumption(power.current_consumption);
    
    // Check for peak consumption
    if (power.current_consumption > power.peak_consumption) {
        power.peak_consumption = power.current_consumption;
    }
    
    // Calculate total energy
    power.total_energy += power.current_consumption;
    
    // Log power consumption
    log_power_consumption(&power);
}
```

### **2. Power Profiling**

```c
// Power profiling
typedef struct {
    uint32_t timestamp;
    power_state_t state;
    uint32_t consumption;
    uint32_t duration;
} power_profile_entry_t;

// Power profiling
void profile_power_consumption(void) {
    static power_profile_entry_t profile[MAX_PROFILE_ENTRIES];
    static uint8_t profile_index = 0;
    
    // Record power profile entry
    profile[profile_index].timestamp = get_system_tick();
    profile[profile_index].state = get_current_power_state();
    profile[profile_index].consumption = read_current_consumption();
    profile[profile_index].duration = calculate_duration();
    
    // Increment index
    profile_index = (profile_index + 1) % MAX_PROFILE_ENTRIES;
    
    // Analyze power profile
    analyze_power_profile(profile, profile_index);
}
```

---

## 🎯 **Best Practices**

### **1. Power Management Guidelines**

```c
// Power management checklist
/*
    □ Use appropriate sleep modes
    □ Disable unused peripherals
    □ Optimize clock frequencies
    □ Implement dynamic power scaling
    □ Monitor power consumption
    □ Handle battery management
    □ Use efficient wake-up sources
    □ Optimize communication protocols
    □ Implement power-aware scheduling
    □ Test power consumption
*/

// Good power management example
void good_power_management(void) {
    // Configure power management
    configure_power_management();
    
    // Main loop with power optimization
    while (1) {
        // Process tasks
        process_tasks();
        
        // Check if system can sleep
        if (can_enter_sleep_mode()) {
            // Enter sleep mode
            enter_sleep_mode();
        }
        
        // Monitor power consumption
        monitor_power_consumption();
    }
}
```

### **2. Sleep Mode Guidelines**

```c
// Sleep mode checklist
/*
    □ Choose appropriate sleep mode
    □ Configure wake-up sources
    □ Save critical data
    □ Disable unused peripherals
    □ Handle wake-up events
    □ Restore system state
    □ Monitor sleep duration
    □ Test sleep mode functionality
    □ Document sleep behavior
    □ Consider safety requirements
*/

// Good sleep mode implementation
void good_sleep_mode(void) {
    // Save system state
    save_system_state();
    
    // Configure wake-up sources
    configure_wake_up_sources();
    
    // Enter sleep mode
    enter_sleep_mode();
    
    // Restore system state after wake-up
    restore_system_state();
    
    // Process wake-up events
    process_wake_up_events();
}
```

---

## ⚠️ **Common Pitfalls**

### **1. Inefficient Sleep Modes**

```c
// WRONG: Not using sleep modes
void bad_no_sleep(void) {
    while (1) {
        // Process tasks
        process_tasks();
        
        // Always active - wastes power
        delay_ms(100);
    }
}

// CORRECT: Using sleep modes
void good_sleep_usage(void) {
    while (1) {
        // Process tasks
        process_tasks();
        
        // Enter sleep mode when idle
        if (is_system_idle()) {
            enter_sleep_mode();
        }
    }
}
```

### **2. Unused Peripherals**

```c
// WRONG: Not disabling unused peripherals
void bad_unused_peripherals(void) {
    // Enable all peripherals
    enable_all_peripherals();
    
    // Use only some peripherals
    use_some_peripherals();
    
    // Leave unused peripherals enabled
}

// CORRECT: Disable unused peripherals
void good_peripheral_management(void) {
    // Enable only needed peripherals
    enable_needed_peripherals();
    
    // Use peripherals
    use_peripherals();
    
    // Disable when not needed
    disable_unused_peripherals();
}
```

### **3. Inefficient Clock Management**

```c
// WRONG: Fixed high frequency
void bad_fixed_clock(void) {
    // Always use high frequency
    set_cpu_frequency(CPU_FREQ_HIGH);
    
    // Process tasks
    process_tasks();
}

// CORRECT: Dynamic clock scaling
void good_clock_management(void) {
    // Scale clock based on load
    uint32_t load = get_cpu_load();
    
    if (load < 30) {
        set_cpu_frequency(CPU_FREQ_LOW);
    } else if (load > 80) {
        set_cpu_frequency(CPU_FREQ_HIGH);
    }
    
    // Process tasks
    process_tasks();
}
```

---

## 💡 **Examples**

### **1. Simple Power Management**

```c
// Simple power management implementation
void simple_power_management(void) {
    // Configure power management
    configure_power_management();
    
    while (1) {
        // Process application tasks
        process_application_tasks();
        
        // Check if system can sleep
        if (is_system_idle()) {
            // Enter light sleep
            enter_light_sleep();
        }
        
        // Monitor battery
        monitor_battery();
    }
}

// System idle check
bool is_system_idle(void) {
    // Check if no tasks are pending
    if (task_queue_empty() && !communication_pending()) {
        return true;
    }
    
    return false;
}
```

### **2. Advanced Power Management**

```c
// Advanced power management with multiple modes
typedef enum {
    POWER_MODE_ACTIVE,
    POWER_MODE_LIGHT_SLEEP,
    POWER_MODE_DEEP_SLEEP,
    POWER_MODE_STANDBY
} power_mode_t;

void advanced_power_management(void) {
    power_mode_t current_mode = POWER_MODE_ACTIVE;
    
    while (1) {
        // Determine optimal power mode
        power_mode_t optimal_mode = determine_optimal_power_mode();
        
        // Transition to optimal mode
        if (optimal_mode != current_mode) {
            transition_to_power_mode(optimal_mode);
            current_mode = optimal_mode;
        }
        
        // Process tasks based on mode
        switch (current_mode) {
            case POWER_MODE_ACTIVE:
                process_active_tasks();
                break;
            case POWER_MODE_LIGHT_SLEEP:
                process_light_sleep_tasks();
                break;
            case POWER_MODE_DEEP_SLEEP:
                process_deep_sleep_tasks();
                break;
            case POWER_MODE_STANDBY:
                process_standby_tasks();
                break;
        }
    }
}

// Determine optimal power mode
power_mode_t determine_optimal_power_mode(void) {
    uint32_t battery_level = get_battery_level();
    uint32_t system_load = get_system_load();
    
    if (battery_level < 20) {
        return POWER_MODE_DEEP_SLEEP;
    } else if (system_load < 10) {
        return POWER_MODE_LIGHT_SLEEP;
    } else {
        return POWER_MODE_ACTIVE;
    }
}
```

### **3. Battery-Optimized System**

```c
// Battery-optimized system
void battery_optimized_system(void) {
    // Configure for battery operation
    configure_battery_optimization();
    
    while (1) {
        // Monitor battery level
        uint32_t battery_level = get_battery_level();
        
        if (battery_level < 10) {
            // Critical battery - enter deep sleep
            enter_critical_battery_mode();
        } else if (battery_level < 30) {
            // Low battery - reduce power consumption
            enter_low_battery_mode();
        } else {
            // Normal battery - standard operation
            enter_normal_mode();
        }
        
        // Process tasks based on battery level
        process_tasks_based_on_battery(battery_level);
    }
}

// Critical battery mode
void enter_critical_battery_mode(void) {
    // Disable non-essential peripherals
    disable_non_essential_peripherals();
    
    // Reduce CPU frequency
    set_cpu_frequency(CPU_FREQ_MIN);
    
    // Enter deep sleep with minimal wake-up sources
    configure_minimal_wake_up_sources();
    enter_deep_sleep();
}
```

---

## 🎯 **Interview Questions**

### **Basic Questions**
1. **What are the different sleep modes in embedded systems?**
   - Light sleep: CPU stopped, peripherals active
   - Deep sleep: CPU and most peripherals stopped
   - Standby: Only backup domain active
   - Hibernate: Only RTC active

2. **How do you implement power management in embedded systems?**
   - Use appropriate sleep modes
   - Disable unused peripherals
   - Optimize clock frequencies
   - Monitor power consumption

3. **What are the common wake-up sources?**
   - External interrupts
   - Timer interrupts
   - RTC alarms
   - Communication events

### **Advanced Questions**
4. **How do you optimize power consumption for battery-powered devices?**
   - Implement dynamic power scaling
   - Use efficient sleep modes
   - Optimize communication protocols
   - Monitor and manage battery usage

5. **What are the trade-offs in power management?**
   - Performance vs power consumption
   - Response time vs sleep duration
   - Functionality vs battery life
   - Cost vs power efficiency

6. **How do you handle power management in real-time systems?**
   - Ensure timing requirements are met
   - Use appropriate wake-up sources
   - Balance power savings with responsiveness
   - Test power management thoroughly

### **Practical Questions**
7. **Design a power management system for an IoT device.**
   ```c
   void iot_power_management(void) {
       // Configure for IoT operation
       configure_iot_power_management();
       
       while (1) {
           // Process IoT tasks
           process_iot_tasks();
           
           // Check for communication
           if (communication_needed()) {
               enable_communication();
               send_data();
               disable_communication();
           }
           
           // Enter sleep mode
           enter_sleep_mode();
       }
   }
   ```

8. **Implement a battery monitoring system.**
   ```c
   void battery_monitoring_system(void) {
       // Configure battery monitoring
       configure_battery_monitoring();
       
       while (1) {
           // Monitor battery
           uint32_t battery_level = get_battery_level();
           
           // Handle different battery levels
           if (battery_level < 10) {
               handle_critical_battery();
           } else if (battery_level < 30) {
               handle_low_battery();
           }
           
           // Log battery status
           log_battery_status(battery_level);
           
           // Sleep for monitoring interval
           delay_ms(BATTERY_MONITOR_INTERVAL);
       }
   }
   ```

---

## 🔗 **Related Topics**

- **[External Interrupts](./External_Interrupts.md)** - Edge/level triggered interrupts, debouncing
- **[Watchdog Timers](./Watchdog_Timers.md)** - System monitoring and recovery mechanisms
- **[Interrupts and Exceptions](./Interrupts_Exceptions.md)** - Interrupt handling, ISR design, interrupt latency
- **[Clock Management](./Clock_Management.md)** - System clock configuration, PLL setup

---

## 📚 **Resources**

### **Books**
- "Making Embedded Systems" by Elecia White
- "Programming Embedded Systems" by Michael Barr
- "Real-Time Systems" by Jane W. S. Liu

### **Online Resources**
- [ARM Cortex-M Power Management](https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/power-management)
- [STM32 Power Management](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)

---

**Next Topic:** [Clock Management](./Clock_Management.md) → [Reset Management](./Reset_Management.md)
