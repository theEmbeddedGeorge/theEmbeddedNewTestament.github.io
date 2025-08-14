# Clock Distribution

> **Clock System Architecture for Embedded Systems**  
> From crystal oscillators to advanced clock management - comprehensive clock distribution principles

---

## 📋 **Table of Contents**

- [Clock System Fundamentals](#clock-system-fundamentals)
- [Clock Sources](#clock-sources)
- [Clock Distribution Network](#clock-distribution-network)
- [Clock Management Techniques](#clock-management-techniques)
- [Clock Synchronization](#clock-synchronization)
- [Clock Integrity](#clock-integrity)
- [Power Management](#power-management)
- [Practical Examples](#practical-examples)

---

## 🕐 **Clock System Fundamentals**

### **What is Clock Distribution?**

Clock distribution is the process of generating, distributing, and managing clock signals throughout an embedded system to ensure proper timing and synchronization of all components.

#### **Clock System Functions**

- **Timing reference**: Provide precise timing for digital circuits
- **Synchronization**: Coordinate operations across multiple components
- **Performance control**: Enable dynamic frequency scaling
- **Power management**: Control power consumption through clock gating

#### **Clock System Requirements**

```c
// Clock system specification structure
typedef struct {
    float frequency_mhz;             // Clock frequency
    float jitter_ps;                 // Maximum jitter
    float skew_ps;                   // Maximum skew
    float stability_ppm;             // Frequency stability
    bool requires_pll;               // PLL requirement
    bool requires_clock_gating;      // Clock gating requirement
} clock_system_specs_t;

void analyze_clock_requirements(clock_system_specs_t *specs) {
    // Calculate clock period
    float period_ns = 1000.0f / specs->frequency_mhz;
    
    // Check jitter requirements (typically < 10% of period)
    float max_jitter_ns = period_ns * 0.1f;
    if (specs->jitter_ps > max_jitter_ns * 1000.0f) {
        printf("Warning: Jitter too high for frequency\n");
    }
    
    // Check skew requirements (typically < 5% of period)
    float max_skew_ns = period_ns * 0.05f;
    if (specs->skew_ps > max_skew_ns * 1000.0f) {
        printf("Warning: Skew too high for frequency\n");
    }
}
```

---

## 🔌 **Clock Sources**

### **Crystal Oscillators**

#### **Crystal Characteristics**

```c
// Crystal oscillator specifications
typedef struct {
    float frequency_mhz;             // Fundamental frequency
    float load_capacitance_pf;       // Load capacitance
    float esr_ohms;                  // Equivalent series resistance
    float drive_level_mw;            // Drive level
    float stability_ppm;             // Frequency stability
    char* cut_type;                  // Crystal cut (AT, BT, etc.)
} crystal_specs_t;

void configure_crystal_oscillator(crystal_specs_t *crystal) {
    // Calculate load capacitance for Pierce oscillator
    float c1_c2_pf = crystal->load_capacitance_pf * 2.0f;
    
    // Select standard capacitor values
    float c1_pf = select_standard_capacitor(c1_c2_pf / 2.0f);
    float c2_pf = select_standard_capacitor(c1_c2_pf / 2.0f);
    
    // Calculate actual load capacitance
    float actual_load_pf = (c1_pf * c2_pf) / (c1_pf + c2_pf);
    
    printf("Crystal oscillator configured:\n");
    printf("C1: %.1f pF\n", c1_pf);
    printf("C2: %.1f pF\n", c2_pf);
    printf("Actual load: %.1f pF\n", actual_load_pf);
}

float select_standard_capacitor(float required_pf) {
    // Standard capacitor values (E24 series)
    float standard_values[] = {10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82, 100};
    uint8_t num_values = sizeof(standard_values) / sizeof(standard_values[0]);
    
    // Find closest standard value
    float best_value = standard_values[0];
    float best_error = fabs(required_pf - best_value);
    
    for (uint8_t i = 1; i < num_values; i++) {
        float error = fabs(required_pf - standard_values[i]);
        if (error < best_error) {
            best_error = error;
            best_value = standard_values[i];
        }
    }
    
    return best_value;
}
```

#### **Oscillator Circuit Design**

```c
// Pierce oscillator circuit design
typedef struct {
    float crystal_frequency_mhz;     // Crystal frequency
    float load_capacitance_pf;       // Load capacitance
    float feedback_resistance_ohms;  // Feedback resistance
    float startup_time_ms;           // Startup time
} pierce_oscillator_t;

void design_pierce_oscillator(pierce_oscillator_t *oscillator) {
    // Calculate feedback resistance (typically 1-10 MΩ)
    float feedback_r = 2.2e6f;  // 2.2 MΩ
    
    // Calculate load capacitors
    float c1_c2_pf = oscillator->load_capacitance_pf * 2.0f;
    
    // Calculate startup time (approximate)
    float startup_time_ms = 1000.0f / oscillator->crystal_frequency_mhz;
    
    printf("Pierce Oscillator Design:\n");
    printf("Feedback resistance: %.1f MΩ\n", feedback_r / 1e6f);
    printf("Load capacitors: %.1f pF each\n", c1_c2_pf / 2.0f);
    printf("Estimated startup time: %.1f ms\n", startup_time_ms);
}
```

### **Programmable Oscillators**

#### **Programmable Clock Generators**

```c
// Programmable clock generator configuration
typedef struct {
    float input_frequency_mhz;       // Input reference frequency
    float output_frequency_mhz;      // Desired output frequency
    uint8_t pll_multiplier;         // PLL multiplication factor
    uint8_t pll_divider;            // PLL division factor
    bool fractional_mode;            // Fractional PLL mode
} programmable_clock_t;

void configure_programmable_clock(programmable_clock_t *clock) {
    // Calculate PLL parameters
    if (clock->fractional_mode) {
        // Fractional PLL mode for precise frequency
        configure_fractional_pll(clock->input_frequency_mhz, 
                                clock->output_frequency_mhz);
    } else {
        // Integer PLL mode
        calculate_integer_pll_parameters(clock);
    }
    
    // Configure output dividers
    configure_output_dividers(clock->output_frequency_mhz);
    
    printf("Programmable clock configured:\n");
    printf("Input: %.1f MHz\n", clock->input_frequency_mhz);
    printf("Output: %.1f MHz\n", clock->output_frequency_mhz);
    printf("PLL multiplier: %d\n", clock->pll_multiplier);
    printf("PLL divider: %d\n", clock->pll_divider);
}

void calculate_integer_pll_parameters(programmable_clock_t *clock) {
    // Find integer PLL parameters
    float target_ratio = clock->output_frequency_mhz / clock->input_frequency_mhz;
    
    // Search for suitable multiplier/divider combination
    for (uint8_t mult = 1; mult <= 64; mult++) {
        for (uint8_t div = 1; div <= 64; div++) {
            float ratio = (float)mult / (float)div;
            if (fabs(ratio - target_ratio) < 0.001f) {
                clock->pll_multiplier = mult;
                clock->pll_divider = div;
                return;
            }
        }
    }
    
    printf("Warning: No suitable integer PLL parameters found\n");
}
```

---

## 🌐 **Clock Distribution Network**

### **Clock Tree Architecture**

#### **Clock Tree Design**

```c
// Clock tree structure
typedef struct {
    char* clock_name;                // Clock name
    float frequency_mhz;             // Clock frequency
    uint8_t fanout;                  // Number of destinations
    float max_skew_ps;               // Maximum skew
    bool requires_buffering;          // Buffer requirement
    clock_tree_node_t *children;     // Child nodes
} clock_tree_node_t;

typedef struct {
    uint8_t num_clocks;              // Number of clock domains
    clock_tree_node_t *root_clocks;  // Root clock sources
    float global_skew_ps;            // Global skew budget
} clock_tree_t;

void design_clock_tree(clock_tree_t *tree) {
    // Calculate skew budget distribution
    float skew_per_level_ps = tree->global_skew_ps / 4.0f;  // 4 levels max
    
    // Design each clock domain
    for (uint8_t i = 0; i < tree->num_clocks; i++) {
        design_clock_domain(&tree->root_clocks[i], skew_per_level_ps);
    }
    
    // Verify global skew
    verify_global_skew(tree);
    
    printf("Clock tree designed with %d domains\n", tree->num_clocks);
    printf("Global skew budget: %.1f ps\n", tree->global_skew_ps);
}

void design_clock_domain(clock_tree_node_t *node, float skew_budget_ps) {
    // Calculate local skew budget
    float local_skew_ps = skew_budget_ps / node->fanout;
    
    // Design clock distribution
    if (node->requires_buffering) {
        design_clock_buffers(node, local_skew_ps);
    }
    
    // Design child nodes
    if (node->children != NULL) {
        for (uint8_t i = 0; i < node->fanout; i++) {
            design_clock_domain(&node->children[i], local_skew_ps);
        }
    }
}
```

#### **Clock Buffer Selection**

```c
// Clock buffer specifications
typedef struct {
    float input_frequency_mhz;       // Maximum input frequency
    float output_frequency_mhz;      // Maximum output frequency
    uint8_t fanout;                  // Fanout capability
    float propagation_delay_ps;      // Propagation delay
    float skew_ps;                   // Output skew
    float power_consumption_mw;      // Power consumption
} clock_buffer_specs_t;

bool select_clock_buffer(clock_buffer_specs_t *buffer, 
                        float required_frequency_mhz, 
                        uint8_t required_fanout) {
    // Check frequency capability
    if (buffer->output_frequency_mhz < required_frequency_mhz) {
        return false;  // Frequency too low
    }
    
    // Check fanout capability
    if (buffer->fanout < required_fanout) {
        return false;  // Fanout too low
    }
    
    // Check power consumption
    if (buffer->power_consumption_mw > MAX_POWER_MW) {
        return false;  // Power too high
    }
    
    return true;  // Buffer suitable
}
```

### **Clock Routing Guidelines**

#### **PCB Layout Considerations**

```c
// Clock routing guidelines
typedef struct {
    float trace_width_mm;            // Trace width
    float trace_spacing_mm;          // Trace spacing
    float trace_length_mm;           // Trace length
    bool differential_routing;        // Differential routing
    float impedance_ohms;            // Characteristic impedance
} clock_routing_specs_t;

void apply_clock_routing_guidelines(clock_routing_specs_t *routing) {
    // Route clock signals first
    route_clock_signals_priority();
    
    // Use dedicated clock layers
    use_dedicated_clock_layer();
    
    // Minimize clock skew
    minimize_clock_skew(routing->trace_length_mm);
    
    // Use proper termination
    if (routing->differential_routing) {
        configure_differential_termination(routing->impedance_ohms);
    } else {
        configure_single_ended_termination(routing->impedance_ohms);
    }
    
    // Avoid crossing other signals
    avoid_signal_crossings();
    
    printf("Clock routing guidelines applied:\n");
    printf("Trace width: %.2f mm\n", routing->trace_width_mm);
    printf("Trace spacing: %.2f mm\n", routing->trace_spacing_mm);
    printf("Differential routing: %s\n", routing->differential_routing ? "Yes" : "No");
}
```

---

## ⚙️ **Clock Management Techniques**

### **Dynamic Frequency Scaling**

#### **DVFS Implementation**

```c
// Dynamic Voltage and Frequency Scaling
typedef struct {
    float frequency_mhz;             // Clock frequency
    float voltage_v;                 // Supply voltage
    float power_consumption_mw;      // Power consumption
    uint8_t performance_level;       // Performance level
} dvfs_state_t;

typedef struct {
    uint8_t num_states;             // Number of DVFS states
    dvfs_state_t *states;           // DVFS states
    uint8_t current_state;          // Current state
    bool auto_scaling;               // Auto-scaling enabled
} dvfs_system_t;

void configure_dvfs_system(dvfs_system_t *dvfs) {
    // Initialize DVFS states
    dvfs->states[0] = (dvfs_state_t){48.0f, 1.8f, 50.0f, 0};    // Low power
    dvfs->states[1] = (dvfs_state_t){72.0f, 2.0f, 100.0f, 1};   // Medium power
    dvfs->states[2] = (dvfs_state_t){96.0f, 2.2f, 200.0f, 2};   // High power
    
    // Configure voltage regulators
    configure_voltage_regulators(dvfs);
    
    // Configure clock generators
    configure_clock_generators(dvfs);
    
    // Enable auto-scaling if requested
    if (dvfs->auto_scaling) {
        enable_auto_scaling();
    }
    
    printf("DVFS system configured with %d states\n", dvfs->num_states);
}

void change_dvfs_state(dvfs_system_t *dvfs, uint8_t new_state) {
    if (new_state >= dvfs->num_states) {
        printf("Error: Invalid DVFS state\n");
        return;
    }
    
    // Change voltage first (voltage scaling)
    change_voltage(dvfs->states[new_state].voltage_v);
    
    // Wait for voltage to stabilize
    wait_for_voltage_stable();
    
    // Change frequency (frequency scaling)
    change_frequency(dvfs->states[new_state].frequency_mhz);
    
    // Update current state
    dvfs->current_state = new_state;
    
    printf("DVFS state changed to level %d\n", new_state);
    printf("Frequency: %.1f MHz, Voltage: %.1fV\n", 
           dvfs->states[new_state].frequency_mhz,
           dvfs->states[new_state].voltage_v);
}
```

#### **Power State Management**

```c
// Power state management
typedef enum {
    POWER_STATE_ACTIVE,             // Full power, full performance
    POWER_STATE_IDLE,               // Reduced power, reduced performance
    POWER_STATE_SLEEP,              // Minimal power, minimal performance
    POWER_STATE_DEEP_SLEEP          // Ultra-low power, no performance
} power_state_t;

typedef struct {
    power_state_t current_state;    // Current power state
    float clock_frequency_mhz;      // Current clock frequency
    float voltage_v;                // Current supply voltage
    uint32_t wakeup_time_ms;        // Wakeup time
} power_management_t;

void change_power_state(power_management_t *pm, power_state_t new_state) {
    power_state_t old_state = pm->current_state;
    
    switch (new_state) {
        case POWER_STATE_ACTIVE:
            // Full power mode
            pm->clock_frequency_mhz = 96.0f;
            pm->voltage_v = 2.2f;
            enable_all_clocks();
            break;
            
        case POWER_STATE_IDLE:
            // Idle mode
            pm->clock_frequency_mhz = 48.0f;
            pm->voltage_v = 1.8f;
            disable_unused_clocks();
            break;
            
        case POWER_STATE_SLEEP:
            // Sleep mode
            pm->clock_frequency_mhz = 1.0f;
            pm->voltage_v = 1.2f;
            enable_only_essential_clocks();
            break;
            
        case POWER_STATE_DEEP_SLEEP:
            // Deep sleep mode
            pm->clock_frequency_mhz = 0.032f;  // 32kHz
            pm->voltage_v = 0.9f;
            enable_only_watchdog_clock();
            break;
    }
    
    // Apply changes
    change_voltage(pm->voltage_v);
    change_frequency(pm->clock_frequency_mhz);
    
    pm->current_state = new_state;
    
    printf("Power state changed: %s → %s\n", 
           get_power_state_name(old_state),
           get_power_state_name(new_state));
}
```

---

## 🔄 **Clock Synchronization**

### **Phase-Locked Loops**

#### **PLL Configuration**

```c
// Phase-locked loop configuration
typedef struct {
    float reference_frequency_mhz;   // Reference frequency
    float output_frequency_mhz;      // Output frequency
    uint16_t multiplier;             // PLL multiplier
    uint16_t divider;                // PLL divider
    float bandwidth_khz;             // Loop bandwidth
    float lock_time_ms;              // Lock time
} pll_config_t;

void configure_pll(pll_config_t *pll) {
    // Calculate PLL parameters
    pll->multiplier = (uint16_t)(pll->output_frequency_mhz / pll->reference_frequency_mhz);
    pll->divider = 1;  // Reference divider
    
    // Configure PLL registers
    write_pll_multiplier(pll->multiplier);
    write_pll_divider(pll->divider);
    write_pll_bandwidth(pll->bandwidth_khz);
    
    // Enable PLL
    enable_pll();
    
    // Wait for lock
    wait_for_pll_lock(pll->lock_time_ms);
    
    printf("PLL configured:\n");
    printf("Reference: %.1f MHz\n", pll->reference_frequency_mhz);
    printf("Output: %.1f MHz\n", pll->output_frequency_mhz);
    printf("Multiplier: %d\n", pll->multiplier);
    printf("Lock time: %.1f ms\n", pll->lock_time_ms);
}
```

#### **PLL Lock Detection**

```c
// PLL lock detection
typedef struct {
    bool is_locked;                  // PLL lock status
    uint32_t lock_timeout_ms;        // Lock timeout
    uint32_t unlock_timeout_ms;      // Unlock timeout
    bool auto_restart;               // Auto-restart on unlock
} pll_lock_detection_t;

bool wait_for_pll_lock(pll_lock_detection_t *detection, uint32_t timeout_ms) {
    uint32_t start_time = get_system_time_ms();
    
    while (!detection->is_locked) {
        // Check lock status
        detection->is_locked = read_pll_lock_status();
        
        // Check timeout
        if (get_system_time_ms() - start_time > timeout_ms) {
            printf("Error: PLL lock timeout\n");
            return false;
        }
        
        // Small delay
        delay_ms(1);
    }
    
    printf("PLL locked successfully\n");
    return true;
}
```

### **Clock Recovery**

#### **Data Clock Extraction**

```c
// Clock recovery system
typedef struct {
    float data_rate_mbps;            // Data rate
    float recovered_frequency_mhz;   // Recovered clock frequency
    float jitter_tolerance_ps;       // Jitter tolerance
    bool requires_clock_recovery;    // Clock recovery requirement
} clock_recovery_t;

void configure_clock_recovery(clock_recovery_t *recovery) {
    if (recovery->requires_clock_recovery) {
        // Configure clock recovery circuit
        configure_cdr_circuit(recovery->data_rate_mbps);
        
        // Set jitter tolerance
        set_jitter_tolerance(recovery->jitter_tolerance_ps);
        
        // Enable clock recovery
        enable_clock_recovery();
        
        printf("Clock recovery configured:\n");
        printf("Data rate: %.1f Mbps\n", recovery->data_rate_mbps);
        printf("Jitter tolerance: %.1f ps\n", recovery->jitter_tolerance_ps);
    } else {
        printf("Clock recovery not required\n");
    }
}
```

---

## 🔒 **Clock Integrity**

### **Jitter Analysis**

#### **Jitter Measurement**

```c
// Jitter analysis
typedef struct {
    float period_jitter_ps;          // Period jitter
    float cycle_jitter_ps;           // Cycle-to-cycle jitter
    float long_term_jitter_ps;       // Long-term jitter
    float rms_jitter_ps;             // RMS jitter
    float peak_jitter_ps;            // Peak-to-peak jitter
} jitter_analysis_t;

void analyze_clock_jitter(float *periods_ns, uint32_t num_periods, 
                         jitter_analysis_t *analysis) {
    // Calculate average period
    float avg_period_ns = 0.0f;
    for (uint32_t i = 0; i < num_periods; i++) {
        avg_period_ns += periods_ns[i];
    }
    avg_period_ns /= num_periods;
    
    // Calculate jitter statistics
    float sum_squares = 0.0f;
    float max_deviation = 0.0f;
    float min_deviation = 0.0f;
    
    for (uint32_t i = 0; i < num_periods; i++) {
        float deviation = periods_ns[i] - avg_period_ns;
        sum_squares += deviation * deviation;
        
        if (deviation > max_deviation) max_deviation = deviation;
        if (deviation < min_deviation) min_deviation = deviation;
    }
    
    // Calculate jitter values
    analysis->rms_jitter_ps = sqrt(sum_squares / num_periods) * 1000.0f;
    analysis->peak_jitter_ps = (max_deviation - min_deviation) * 1000.0f;
    analysis->period_jitter_ps = analysis->rms_jitter_ps;
    
    printf("Jitter analysis results:\n");
    printf("RMS jitter: %.1f ps\n", analysis->rms_jitter_ps);
    printf("Peak-to-peak jitter: %.1f ps\n", analysis->peak_jitter_ps);
    printf("Average period: %.1f ns\n", avg_period_ns);
}
```

### **Skew Management**

#### **Clock Skew Control**

```c
// Clock skew management
typedef struct {
    float max_skew_ps;               // Maximum allowed skew
    float measured_skew_ps;          // Measured skew
    bool skew_within_spec;           // Skew within specification
    uint8_t num_skew_adjustments;    // Number of skew adjustments
} skew_management_t;

void manage_clock_skew(skew_management_t *skew) {
    // Measure current skew
    skew->measured_skew_ps = measure_clock_skew();
    
    // Check if skew is within specification
    skew->skew_within_spec = (skew->measured_skew_ps <= skew->max_skew_ps);
    
    if (!skew->skew_within_spec) {
        printf("Warning: Clock skew exceeds specification\n");
        printf("Measured: %.1f ps, Maximum: %.1f ps\n", 
               skew->measured_skew_ps, skew->max_skew_ps);
        
        // Attempt skew adjustment
        if (adjust_clock_skew()) {
            skew->num_skew_adjustments++;
            printf("Skew adjusted successfully\n");
        } else {
            printf("Failed to adjust skew\n");
        }
    } else {
        printf("Clock skew within specification: %.1f ps\n", skew->measured_skew_ps);
    }
}
```

---

## ⚡ **Power Management**

### **Clock Gating**

#### **Clock Gating Implementation**

```c
// Clock gating configuration
typedef struct {
    bool enabled;                    // Clock gating enabled
    uint8_t gating_levels;          // Number of gating levels
    float power_savings_percent;     // Power savings percentage
    uint32_t gating_delay_ns;       // Gating delay
} clock_gating_t;

void configure_clock_gating(clock_gating_t *gating) {
    if (gating->enabled) {
        // Enable clock gating at multiple levels
        for (uint8_t i = 0; i < gating->gating_levels; i++) {
            enable_clock_gating_level(i);
        }
        
        // Configure gating delay
        set_gating_delay(gating->gating_delay_ns);
        
        // Calculate power savings
        gating->power_savings_percent = calculate_power_savings();
        
        printf("Clock gating configured:\n");
        printf("Gating levels: %d\n", gating->gating_levels);
        printf("Power savings: %.1f%%\n", gating->power_savings_percent);
        printf("Gating delay: %d ns\n", gating->gating_delay_ns);
    } else {
        disable_all_clock_gating();
        printf("Clock gating disabled\n");
    }
}

float calculate_power_savings(void) {
    // Estimate power savings based on gated clocks
    uint32_t total_clocks = get_total_clock_count();
    uint32_t gated_clocks = get_gated_clock_count();
    
    float savings_percent = (float)gated_clocks / (float)total_clocks * 100.0f;
    return savings_percent;
}
```

---

## 🔧 **Practical Examples**

### **Example 1: STM32F4 Clock System**

```c
// STM32F4 clock system configuration
typedef struct {
    float hse_frequency_mhz;         // HSE frequency (external crystal)
    float hsi_frequency_mhz;         // HSI frequency (internal oscillator)
    float pll_frequency_mhz;         // PLL output frequency
    float sysclk_frequency_mhz;      // System clock frequency
    float ahb_frequency_mhz;         // AHB bus frequency
    float apb1_frequency_mhz;        // APB1 peripheral frequency
    float apb2_frequency_mhz;        // APB2 peripheral frequency
} stm32f4_clock_config_t;

void configure_stm32f4_clock_system(stm32f4_clock_config_t *config) {
    // Enable HSE (external crystal)
    enable_hse(config->hse_frequency_mhz);
    
    // Configure PLL
    configure_pll_for_sysclk(config->hse_frequency_mhz, config->sysclk_frequency_mhz);
    
    // Configure bus prescalers
    configure_ahb_prescaler(config->sysclk_frequency_mhz, config->ahb_frequency_mhz);
    configure_apb1_prescaler(config->ahb_frequency_mhz, config->apb1_frequency_mhz);
    configure_apb2_prescaler(config->ahb_frequency_mhz, config->apb2_frequency_mhz);
    
    // Switch to PLL as system clock source
    switch_to_pll_system_clock();
    
    printf("STM32F4 clock system configured:\n");
    printf("HSE: %.1f MHz\n", config->hse_frequency_mhz);
    printf("System clock: %.1f MHz\n", config->sysclk_frequency_mhz);
    printf("AHB: %.1f MHz\n", config->ahb_frequency_mhz);
    printf("APB1: %.1f MHz\n", config->apb1_frequency_mhz);
    printf("APB2: %.1f MHz\n", config->apb2_frequency_mhz);
}
```

### **Example 2: Multi-Domain Clock System**

```c
// Multi-domain clock system
typedef struct {
    clock_domain_t cpu_domain;       // CPU clock domain
    clock_domain_t memory_domain;    // Memory clock domain
    clock_domain_t peripheral_domain; // Peripheral clock domain
    clock_domain_t communication_domain; // Communication clock domain
} multi_domain_clock_system_t;

void configure_multi_domain_system(multi_domain_clock_system_t *system) {
    // Configure CPU domain (high performance)
    configure_clock_domain(&system->cpu_domain, 100.0f, 1.0f, 2.0f);
    
    // Configure memory domain (medium performance)
    configure_clock_domain(&system->memory_domain, 50.0f, 1.8f, 1.5f);
    
    // Configure peripheral domain (low performance)
    configure_clock_domain(&system->peripheral_domain, 25.0f, 1.5f, 1.2f);
    
    // Configure communication domain (variable performance)
    configure_clock_domain(&system->communication_domain, 75.0f, 1.8f, 1.8f);
    
    // Configure clock synchronization
    configure_domain_synchronization();
    
    printf("Multi-domain clock system configured:\n");
    printf("CPU: %.1f MHz at %.1fV\n", 
           system->cpu_domain.frequency_mhz, system->cpu_domain.voltage_v);
    printf("Memory: %.1f MHz at %.1fV\n", 
           system->memory_domain.frequency_mhz, system->memory_domain.voltage_v);
    printf("Peripheral: %.1f MHz at %.1fV\n", 
           system->peripheral_domain.frequency_mhz, system->peripheral_domain.voltage_v);
    printf("Communication: %.1f MHz at %.1fV\n", 
           system->communication_domain.frequency_mhz, system->communication_domain.voltage_v);
}
```

---

## 📚 **Additional Resources**

### **Recommended Reading**
- "High-Speed Digital Design" by Howard Johnson
- "Clock Design and Distribution" by various authors
- "PLL Design and Analysis" by various authors

### **Design Tools**
- **Clock Design**: TI Clock Architect, Silicon Labs ClockBuilder
- **Timing Analysis**: Cadence Tempus, Synopsys PrimeTime
- **PCB Design**: Altium Designer, KiCad, Eagle

### **Component Selection**
- **Crystal Oscillators**: Abracon, ECS, IQD
- **Clock Generators**: Silicon Labs, TI, Maxim
- **Clock Buffers**: IDT, ON Semiconductor, Texas Instruments

---

## 🎯 **Key Takeaways**

1. **Clock distribution requires careful planning** to minimize skew and jitter
2. **Multiple clock sources provide flexibility** for different performance requirements
3. **PLLs enable frequency multiplication** and precise clock generation
4. **DVFS enables power optimization** through dynamic frequency and voltage scaling
5. **Clock gating reduces power consumption** by disabling unused clock domains
6. **Proper PCB layout is critical** for maintaining clock signal integrity

---

**Previous Topic**: [Power Supply Design](./Power_Supply_Design.md)  
**Next Topic**: [Thermal Management](./Thermal_Management.md)
