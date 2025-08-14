# Power Supply Design

> **Power Supply Architecture for Embedded Systems**  
> From linear regulators to switching topologies - comprehensive power supply design principles

---

## 📋 **Table of Contents**

- [Power Supply Fundamentals](#power-supply-fundamentals)
- [Power Supply Topologies](#power-supply-topologies)
- [Linear Regulators](#linear-regulators)
- [Switching Regulators](#switching-regulators)
- [Power Supply Design Considerations](#power-supply-design-considerations)
- [Protection and Safety](#protection-and-safety)
- [Power Integrity](#power-integrity)
- [Practical Examples](#practical-examples)

---

## ⚡ **Power Supply Fundamentals**

### **What is a Power Supply?**

A power supply converts electrical energy from one form to another, providing stable and regulated power to electronic circuits.

#### **Power Supply Functions**

- **Voltage conversion**: Step-up, step-down, or isolation
- **Regulation**: Maintain stable output under varying conditions
- **Filtering**: Remove noise and ripple
- **Protection**: Safeguard against faults and overloads

#### **Power Supply Requirements**

```c
// Power supply specification structure
typedef struct {
    float input_voltage_min_v;      // Minimum input voltage
    float input_voltage_max_v;      // Maximum input voltage
    float output_voltage_v;         // Required output voltage
    float output_current_ma;        // Required output current
    float output_ripple_mv;         // Maximum output ripple
    float efficiency_percent;       // Minimum efficiency
    float temperature_max_c;        // Maximum operating temperature
} power_supply_specs_t;

void analyze_power_requirements(power_supply_specs_t *specs) {
    // Calculate power requirements
    float power_w = (specs->output_voltage_v * specs->output_current_ma) / 1000.0f;
    
    // Check input voltage range
    float input_range_v = specs->input_voltage_max_v - specs->input_voltage_min_v;
    
    // Determine topology based on requirements
    if (input_range_v > specs->output_voltage_v * 2) {
        // Wide input range - consider switching regulator
        recommend_switching_regulator();
    } else {
        // Narrow input range - linear regulator may be suitable
        recommend_linear_regulator();
    }
}
```

---

## 🔄 **Power Supply Topologies**

### **Linear vs. Switching**

#### **Linear Regulators**

```c
// Linear regulator characteristics
typedef struct {
    float dropout_voltage_v;        // Minimum input-output difference
    float efficiency_percent;       // Typical efficiency
    float output_noise_uv;          // Output noise level
    float response_time_us;         // Load transient response
    bool requires_heat_sink;        // Heat sink requirement
} linear_regulator_specs_t;

float calculate_linear_efficiency(float v_in, float v_out) {
    // Linear regulator efficiency = V_out / V_in
    return (v_out / v_in) * 100.0f;
}

// Example: 5V input, 3.3V output
// Efficiency = (3.3V / 5V) × 100% = 66%
```

#### **Switching Regulators**

```c
// Switching regulator characteristics
typedef struct {
    float efficiency_percent;       // Typical efficiency (85-95%)
    float switching_frequency_khz;  // Switching frequency
    float output_ripple_mv;         // Output ripple voltage
    float response_time_us;         // Load transient response
    bool requires_inductor;         // External inductor required
} switching_regulator_specs_t;

float calculate_switching_efficiency(float v_in, float v_out, float i_out, 
                                   float switching_losses_mw) {
    float p_out = v_out * i_out / 1000.0f;  // Output power in watts
    float p_in = p_out + (switching_losses_mw / 1000.0f);  // Input power
    return (p_out / p_in) * 100.0f;
}
```

### **Topology Selection Guide**

#### **Selection Criteria**

```c
// Topology selection algorithm
typedef enum {
    TOPOLOGY_LINEAR,        // Linear regulator
    TOPOLOGY_BUCK,          // Buck (step-down) converter
    TOPOLOGY_BOOST,         // Boost (step-up) converter
    TOPOLOGY_BUCK_BOOST,    // Buck-boost converter
    TOPOLOGY_FLYBACK,       // Flyback converter
    TOPOLOGY_FORWARD        // Forward converter
} power_topology_t;

power_topology_t select_power_topology(float v_in, float v_out, 
                                      float i_out, float efficiency_target) {
    if (v_in > v_out && efficiency_target < 70.0f) {
        return TOPOLOGY_LINEAR;  // Simple step-down, low efficiency OK
    } else if (v_in > v_out && efficiency_target >= 70.0f) {
        return TOPOLOGY_BUCK;    // Efficient step-down
    } else if (v_in < v_out) {
        return TOPOLOGY_BOOST;   // Step-up conversion
    } else if (v_in > v_out * 1.5f || v_in < v_out * 0.5f) {
        return TOPOLOGY_BUCK_BOOST;  // Wide input range
    } else {
        return TOPOLOGY_LINEAR;  // Default to linear for small differences
    }
}
```

---

## 📉 **Linear Regulators**

### **Linear Regulator Types**

#### **Fixed Output Regulators**

```c
// Fixed output linear regulator example (LM7805)
typedef struct {
    float output_voltage_v;         // Fixed output voltage
    float dropout_voltage_v;        // Dropout voltage
    float max_current_ma;           // Maximum output current
    float line_regulation_percent;  // Line regulation
    float load_regulation_percent;  // Load regulation
} fixed_regulator_specs_t;

void configure_fixed_regulator(fixed_regulator_specs_t *regulator) {
    // Calculate minimum input voltage
    float v_in_min = regulator->output_voltage_v + regulator->dropout_voltage_v;
    
    // Check input voltage requirement
    if (SYSTEM_VOLTAGE < v_in_min) {
        printf("Error: Input voltage too low for regulator\n");
        return;
    }
    
    // Calculate power dissipation
    float power_dissipation_w = (SYSTEM_VOLTAGE - regulator->output_voltage_v) * 
                                (regulator->max_current_ma / 1000.0f);
    
    // Check if heat sink is required
    if (power_dissipation_w > 1.0f) {
        printf("Warning: Heat sink required for %0.2fW dissipation\n", 
               power_dissipation_w);
    }
}
```

#### **Adjustable Output Regulators**

```c
// Adjustable output linear regulator example (LM317)
typedef struct {
    float reference_voltage_v;      // Internal reference voltage
    float output_voltage_min_v;     // Minimum output voltage
    float output_voltage_max_v;     // Maximum output voltage
    float dropout_voltage_v;        // Dropout voltage
    float max_current_ma;           // Maximum output current
} adjustable_regulator_specs_t;

float calculate_output_voltage(adjustable_regulator_specs_t *regulator, 
                              float r1_ohms, float r2_ohms) {
    // V_out = V_ref × (1 + R2/R1)
    return regulator->reference_voltage_v * (1.0f + (r2_ohms / r1_ohms));
}

float calculate_resistor_values(adjustable_regulator_specs_t *regulator, 
                               float desired_voltage_v) {
    // R2/R1 = (V_out/V_ref) - 1
    float ratio = (desired_voltage_v / regulator->reference_voltage_v) - 1.0f;
    
    // Choose R1 = 240Ω (common value for LM317)
    float r1 = 240.0f;
    float r2 = r1 * ratio;
    
    return r2;
}

// Example: 3.3V output with LM317 (V_ref = 1.25V)
// R2/R1 = (3.3V/1.25V) - 1 = 1.64
// R1 = 240Ω, R2 = 240Ω × 1.64 = 394Ω
```

### **Linear Regulator Design**

#### **Input and Output Capacitors**

```c
// Capacitor selection for linear regulators
typedef struct {
    float input_cap_uf;             // Input capacitor value
    float output_cap_uf;            // Output capacitor value
    float esr_max_ohms;             // Maximum ESR
    float voltage_rating_v;         // Voltage rating
} regulator_capacitors_t;

regulator_capacitors_t select_regulator_capacitors(float output_current_ma, 
                                                  float output_voltage_v) {
    regulator_capacitors_t caps;
    
    // Input capacitor: 0.1μF ceramic + 10μF bulk
    caps.input_cap_uf = 10.1f;
    
    // Output capacitor: 0.1μF ceramic + 1μF bulk
    caps.output_cap_uf = 1.1f;
    
    // ESR requirements
    caps.esr_max_ohms = 0.1f;  // Low ESR for stability
    
    // Voltage rating: 2x output voltage minimum
    caps.voltage_rating_v = output_voltage_v * 2.0f;
    
    return caps;
}
```

---

## 🔄 **Switching Regulators**

### **Buck Converter Design**

#### **Buck Converter Basics**

```c
// Buck converter design parameters
typedef struct {
    float input_voltage_v;          // Input voltage
    float output_voltage_v;         // Output voltage
    float output_current_ma;        // Output current
    float switching_frequency_khz;  // Switching frequency
    float ripple_current_percent;   // Inductor ripple current (%)
    float output_ripple_mv;         // Output ripple voltage
} buck_converter_specs_t;

void design_buck_converter(buck_converter_specs_t *specs) {
    // Calculate duty cycle
    float duty_cycle = specs->output_voltage_v / specs->input_voltage_v;
    
    // Calculate inductor value
    float inductor_uh = calculate_inductor_value(specs, duty_cycle);
    
    // Calculate output capacitor
    float capacitor_uf = calculate_output_capacitor(specs);
    
    // Calculate input capacitor
    float input_cap_uf = calculate_input_capacitor(specs);
    
    printf("Buck Converter Design:\n");
    printf("Duty Cycle: %.2f%%\n", duty_cycle * 100.0f);
    printf("Inductor: %.1f μH\n", inductor_uh);
    printf("Output Capacitor: %.1f μF\n", capacitor_uf);
    printf("Input Capacitor: %.1f μF\n", input_cap_uf);
}

float calculate_inductor_value(buck_converter_specs_t *specs, float duty_cycle) {
    // L = (V_in - V_out) × D / (f_sw × ΔI)
    float delta_i = specs->output_current_ma * specs->ripple_current_percent / 100.0f;
    float inductor_uh = ((specs->input_voltage_v - specs->output_voltage_v) * duty_cycle) /
                       (specs->switching_frequency_khz * delta_i / 1000.0f);
    
    return inductor_uh;
}
```

#### **Boost Converter Design**

```c
// Boost converter design parameters
typedef struct {
    float input_voltage_v;          // Input voltage
    float output_voltage_v;         // Output voltage
    float output_current_ma;        // Output current
    float switching_frequency_khz;  // Switching frequency
    float ripple_current_percent;   // Inductor ripple current (%)
} boost_converter_specs_t;

void design_boost_converter(boost_converter_specs_t *specs) {
    // Calculate duty cycle
    float duty_cycle = 1.0f - (specs->input_voltage_v / specs->output_voltage_v);
    
    // Calculate inductor value
    float inductor_uh = calculate_boost_inductor(specs, duty_cycle);
    
    // Calculate output capacitor
    float capacitor_uf = calculate_boost_capacitor(specs);
    
    printf("Boost Converter Design:\n");
    printf("Duty Cycle: %.2f%%\n", duty_cycle * 100.0f);
    printf("Inductor: %.1f μH\n", inductor_uh);
    printf("Output Capacitor: %.1f μF\n", capacitor_uf);
}

float calculate_boost_inductor(boost_converter_specs_t *specs, float duty_cycle) {
    // L = (V_in × D) / (f_sw × ΔI)
    float delta_i = specs->output_current_ma * specs->ripple_current_percent / 100.0f;
    float inductor_uh = (specs->input_voltage_v * duty_cycle) /
                       (specs->switching_frequency_khz * delta_i / 1000.0f);
    
    return inductor_uh;
}
```

### **Switching Regulator Components**

#### **Inductor Selection**

```c
// Inductor selection criteria
typedef struct {
    float inductance_uh;            // Inductance value
    float current_rating_ma;        // Current rating
    float dc_resistance_ohms;       // DC resistance
    float saturation_current_ma;    // Saturation current
    char* core_material;            // Core material
} inductor_specs_t;

bool select_inductor(inductor_specs_t *inductor, float required_inductance_uh, 
                     float peak_current_ma) {
    // Check inductance tolerance (±10% typical)
    float min_inductance = required_inductance_uh * 0.9f;
    float max_inductance = required_inductance_uh * 1.1f;
    
    if (inductor->inductance_uh < min_inductance || 
        inductor->inductance_uh > max_inductance) {
        return false;  // Inductance out of range
    }
    
    // Check current rating
    if (inductor->current_rating_ma < peak_current_ma) {
        return false;  // Current rating too low
    }
    
    // Check saturation current
    if (inductor->saturation_current_ma < peak_current_ma * 1.2f) {
        return false;  // Saturation current too low
    }
    
    return true;  // Inductor suitable
}
```

#### **Capacitor Selection**

```c
// Capacitor selection for switching regulators
typedef struct {
    float capacitance_uf;           // Capacitance value
    float voltage_rating_v;         // Voltage rating
    float esr_max_ohms;            // Maximum ESR
    char* dielectric_type;          // Dielectric type
    float ripple_current_ma;        // Ripple current rating
} switching_capacitor_specs_t;

bool select_switching_capacitor(switching_capacitor_specs_t *capacitor, 
                               float required_capacitance_uf, 
                               float max_voltage_v, 
                               float max_esr_ohms) {
    // Check capacitance (allow ±20% tolerance)
    float min_capacitance = required_capacitance_uf * 0.8f;
    float max_capacitance = required_capacitance_uf * 1.2f;
    
    if (capacitor->capacitance_uf < min_capacitance || 
        capacitor->capacitance_uf > max_capacitance) {
        return false;  // Capacitance out of range
    }
    
    // Check voltage rating (2x safety margin)
    if (capacitor->voltage_rating_v < max_voltage_v * 2.0f) {
        return false;  // Voltage rating too low
    }
    
    // Check ESR
    if (capacitor->esr_max_ohms > max_esr_ohms) {
        return false;  // ESR too high
    }
    
    return true;  // Capacitor suitable
}
```

---

## 🎯 **Power Supply Design Considerations**

### **Input Requirements**

#### **Input Voltage Range**

```c
// Input voltage analysis
typedef struct {
    float voltage_nominal_v;        // Nominal input voltage
    float voltage_tolerance_percent; // Input voltage tolerance
    float voltage_min_v;            // Minimum input voltage
    float voltage_max_v;            // Maximum input voltage
    bool is_ac_input;               // AC or DC input
    float frequency_hz;             // AC frequency (if applicable)
} input_voltage_specs_t;

void analyze_input_requirements(input_voltage_specs_t *input) {
    // Calculate actual voltage range
    input->voltage_min_v = input->voltage_nominal_v * 
                           (1.0f - input->voltage_tolerance_percent / 100.0f);
    input->voltage_max_v = input->voltage_nominal_v * 
                           (1.0f + input->voltage_tolerance_percent / 100.0f);
    
    if (input->is_ac_input) {
        // AC input requires rectification and filtering
        printf("AC input detected - rectifier and filter required\n");
        printf("Peak voltage: %.1fV\n", input->voltage_max_v * 1.414f);
    } else {
        // DC input - direct connection possible
        printf("DC input - direct connection\n");
    }
}
```

#### **Input Current Requirements**

```c
// Input current analysis
typedef struct {
    float output_power_w;           // Output power
    float efficiency_percent;       // Power supply efficiency
    float power_factor;             // Power factor (AC only)
    float input_current_ma;         // Calculated input current
} input_current_specs_t;

float calculate_input_current(input_current_specs_t *specs, 
                             float input_voltage_v, bool is_ac) {
    // Calculate input power
    float input_power_w = specs->output_power_w / (specs->efficiency_percent / 100.0f);
    
    if (is_ac) {
        // AC input current
        specs->input_current_ma = (input_power_w / (input_voltage_v * specs->power_factor)) * 1000.0f;
    } else {
        // DC input current
        specs->input_current_ma = (input_power_w / input_voltage_v) * 1000.0f;
    }
    
    return specs->input_current_ma;
}
```

### **Output Specifications**

#### **Output Voltage Regulation**

```c
// Output voltage regulation analysis
typedef struct {
    float voltage_nominal_v;        // Nominal output voltage
    float voltage_tolerance_percent; // Output voltage tolerance
    float load_regulation_percent;  // Load regulation
    float line_regulation_percent;  // Line regulation
    float temperature_coeff_ppm_c;  // Temperature coefficient
} output_voltage_specs_t;

float calculate_voltage_variation(output_voltage_specs_t *specs, 
                                 float load_variation_percent, 
                                 float line_variation_percent, 
                                 float temperature_change_c) {
    // Total variation = load + line + temperature
    float total_variation_percent = specs->load_regulation_percent + 
                                   specs->line_regulation_percent + 
                                   (specs->temperature_coeff_ppm_c * temperature_change_c / 10000.0f);
    
    float voltage_variation_v = specs->voltage_nominal_v * total_variation_percent / 100.0f;
    
    return voltage_variation_v;
}
```

#### **Output Current Capacity**

```c
// Output current analysis
typedef struct {
    float current_continuous_ma;    // Continuous output current
    float current_peak_ma;          // Peak output current
    float current_startup_ma;       // Startup current requirement
    float current_derating_factor;  // Temperature derating factor
} output_current_specs_t;

float calculate_derated_current(output_current_specs_t *specs, 
                               float ambient_temp_c, float max_temp_c) {
    if (ambient_temp_c <= 25.0f) {
        return specs->current_continuous_ma;  // No derating at room temperature
    }
    
    // Linear derating above 25°C
    float derating_factor = (max_temp_c - ambient_temp_c) / (max_temp_c - 25.0f);
    return specs->current_continuous_ma * derating_factor;
}
```

---

## 🛡️ **Protection and Safety**

### **Overcurrent Protection**

#### **Current Limiting**

```c
// Overcurrent protection configuration
typedef struct {
    float current_limit_ma;         // Current limit threshold
    float response_time_ms;         // Response time
    bool auto_recovery;             // Auto-recovery capability
    bool latched_shutdown;          // Latched shutdown mode
} overcurrent_protection_t;

void configure_overcurrent_protection(overcurrent_protection_t *ocp) {
    // Set current limit threshold
    set_current_limit(ocp->current_limit_ma);
    
    // Configure response time
    set_ocp_response_time(ocp->response_time_ms);
    
    // Configure recovery behavior
    if (ocp->auto_recovery) {
        enable_auto_recovery();
    } else if (ocp->latched_shutdown) {
        enable_latched_shutdown();
    }
    
    printf("Overcurrent protection configured:\n");
    printf("Current limit: %.1f mA\n", ocp->current_limit_ma);
    printf("Response time: %.1f ms\n", ocp->response_time_ms);
}
```

#### **Short Circuit Protection**

```c
// Short circuit protection
typedef struct {
    bool enabled;                   // Protection enabled
    float detection_time_ms;        // Detection time
    float recovery_time_ms;         // Recovery time
    uint8_t max_retry_attempts;     // Maximum retry attempts
} short_circuit_protection_t;

void configure_short_circuit_protection(short_circuit_protection_t *scp) {
    if (scp->enabled) {
        // Enable short circuit detection
        enable_scp_detection();
        
        // Set detection parameters
        set_scp_detection_time(scp->detection_time_ms);
        set_scp_recovery_time(scp->recovery_time_ms);
        set_scp_max_retries(scp->max_retry_attempts);
        
        printf("Short circuit protection enabled\n");
    } else {
        disable_scp_detection();
        printf("Short circuit protection disabled\n");
    }
}
```

### **Overvoltage Protection**

#### **Voltage Monitoring**

```c
// Overvoltage protection
typedef struct {
    float voltage_threshold_v;      // Overvoltage threshold
    float response_time_ms;         // Response time
    bool crowbar_protection;        // Crowbar protection
    bool shutdown_protection;       // Shutdown protection
} overvoltage_protection_t;

void configure_overvoltage_protection(overvoltage_protection_t *ovp) {
    // Set overvoltage threshold
    set_ovp_threshold(ovp->voltage_threshold_v);
    
    // Configure response time
    set_ovp_response_time(ovp->response_time_ms);
    
    // Configure protection method
    if (ovp->crowbar_protection) {
        enable_crowbar_protection();
    } else if (ovp->shutdown_protection) {
        enable_shutdown_protection();
    }
    
    printf("Overvoltage protection configured:\n");
    printf("Threshold: %.2fV\n", ovp->voltage_threshold_v);
    printf("Response time: %.1f ms\n", ovp->response_time_ms);
}
```

---

## 🔌 **Power Integrity**

### **Power Distribution**

#### **Power Plane Design**

```c
// Power plane design considerations
typedef struct {
    float current_density_ma_mm2;   // Current density limit
    float voltage_drop_mv;          // Maximum voltage drop
    float trace_width_mm;           // Trace width
    float trace_length_mm;          // Trace length
    float copper_thickness_oz;      // Copper thickness
} power_plane_specs_t;

float calculate_voltage_drop(power_plane_specs_t *specs, float current_ma) {
    // Calculate resistance: R = ρ × L / (W × T)
    float resistivity_ohm_mm = 0.0172f;  // Copper resistivity
    float thickness_mm = specs->copper_thickness_oz * 0.035f;  // Convert oz to mm
    
    float resistance_ohms = resistivity_ohm_mm * specs->trace_length_mm / 
                           (specs->trace_width_mm * thickness_mm);
    
    // Calculate voltage drop: V = I × R
    float voltage_drop_v = (current_ma / 1000.0f) * resistance_ohms;
    
    return voltage_drop_v * 1000.0f;  // Convert to mV
}

bool check_power_plane_design(power_plane_specs_t *specs, float current_ma) {
    // Check current density
    float current_density = current_ma / (specs->trace_width_mm * specs->copper_thickness_oz * 0.035f);
    if (current_density > specs->current_density_ma_mm2) {
        printf("Warning: Current density too high\n");
        return false;
    }
    
    // Check voltage drop
    float voltage_drop = calculate_voltage_drop(specs, current_ma);
    if (voltage_drop > specs->voltage_drop_mv) {
        printf("Warning: Voltage drop too high: %.1f mV\n", voltage_drop);
        return false;
    }
    
    return true;
}
```

### **Decoupling Capacitors**

#### **Decoupling Strategy**

```c
// Decoupling capacitor configuration
typedef struct {
    float bulk_cap_uf;              // Bulk capacitor value
    float ceramic_cap_nf;           // Ceramic capacitor value
    uint8_t num_ceramic_caps;       // Number of ceramic capacitors
    float esr_max_ohms;             // Maximum ESR
    float placement_distance_mm;     // Maximum placement distance
} decoupling_config_t;

void configure_decoupling(decoupling_config_t *config) {
    // Place bulk capacitor near power input
    place_bulk_capacitor(config->bulk_cap_uf, 0);  // Distance = 0mm
    
    // Place ceramic capacitors near high-frequency components
    for (uint8_t i = 0; i < config->num_ceramic_caps; i++) {
        float distance = config->placement_distance_mm * (i + 1) / config->num_ceramic_caps;
        place_ceramic_capacitor(config->ceramic_cap_nf, distance);
    }
    
    printf("Decoupling configured:\n");
    printf("Bulk capacitor: %.1f μF\n", config->bulk_cap_uf);
    printf("Ceramic capacitors: %d × %.1f nF\n", 
           config->num_ceramic_caps, config->ceramic_cap_nf);
}
```

---

## 🔧 **Practical Examples**

### **Example 1: 3.3V Power Supply for Microcontroller**

```c
// 3.3V power supply design for STM32F4
typedef struct {
    float input_voltage_v;          // 5V USB input
    float output_voltage_v;         // 3.3V output
    float output_current_ma;        // 500mA output
    float efficiency_target;        // 80% efficiency target
} mcu_power_supply_t;

void design_mcu_power_supply(mcu_power_supply_t *specs) {
    // Select topology (5V to 3.3V step-down)
    power_topology_t topology = select_power_topology(specs->input_voltage_v, 
                                                    specs->output_voltage_v, 
                                                    specs->output_current_ma, 
                                                    specs->efficiency_target);
    
    if (topology == TOPOLOGY_BUCK) {
        // Design buck converter
        buck_converter_specs_t buck_specs = {
            .input_voltage_v = specs->input_voltage_v,
            .output_voltage_v = specs->output_voltage_v,
            .output_current_ma = specs->output_current_ma,
            .switching_frequency_khz = 500.0f,  // 500kHz switching
            .ripple_current_percent = 30.0f,    // 30% ripple current
            .output_ripple_mv = 50.0f           // 50mV output ripple
        };
        
        design_buck_converter(&buck_specs);
        
        // Configure protection
        overcurrent_protection_t ocp = {
            .current_limit_ma = specs->output_current_ma * 1.2f,  // 20% margin
            .response_time_ms = 1.0f,
            .auto_recovery = true,
            .latched_shutdown = false
        };
        configure_overcurrent_protection(&ocp);
        
    } else {
        printf("Error: Unexpected topology selected\n");
    }
}
```

### **Example 2: Multi-Rail Power Supply**

```c
// Multi-rail power supply design
typedef struct {
    power_rail_t vdd_3v3;          // 3.3V main supply
    power_rail_t vdd_1v8;          // 1.8V core supply
    power_rail_t vdd_1v2;          // 1.2V analog supply
    power_rail_t vdd_5v;           // 5V supply
} multi_rail_power_supply_t;

void design_multi_rail_supply(multi_rail_power_supply_t *supply) {
    // Design 5V input to 3.3V buck converter
    design_buck_converter_for_rail(&supply->vdd_3v3, 5.0f, 3.3f, 1000);
    
    // Design 3.3V to 1.8V LDO
    design_linear_regulator_for_rail(&supply->vdd_1v8, 3.3f, 1.8f, 500);
    
    // Design 3.3V to 1.2V LDO
    design_linear_regulator_for_rail(&supply->vdd_1v2, 3.3f, 1.2f, 200);
    
    // Design 5V boost converter
    design_boost_converter_for_rail(&supply->vdd_5v, 3.3f, 5.0f, 500);
    
    // Configure power sequencing
    configure_power_sequencing();
    
    printf("Multi-rail power supply designed:\n");
    printf("3.3V: Buck converter from 5V\n");
    printf("1.8V: LDO from 3.3V\n");
    printf("1.2V: LDO from 3.3V\n");
    printf("5V: Boost converter from 3.3V\n");
}
```

---

## 📚 **Additional Resources**

### **Recommended Reading**
- "Power Supply Cookbook" by Marty Brown
- "Switching Power Supply Design" by Abraham Pressman
- "Practical Design of Power Supplies" by Ron Lenk

### **Design Tools**
- **SPICE Simulation**: LTspice, PSpice, TINA-TI
- **Power Supply Design**: TI Webench, Linear Tech LTSpice
- **PCB Design**: Altium Designer, KiCad, Eagle

### **Component Selection**
- **Linear Regulators**: LM317, LM7805, LM1117
- **Switching Regulators**: LM2596, LM2576, LM2678
- **Power Management**: TPS series, LTC series

---

## 🎯 **Key Takeaways**

1. **Power supply selection depends on input/output requirements** and efficiency targets
2. **Linear regulators are simple but inefficient** for large voltage differences
3. **Switching regulators provide high efficiency** but require careful component selection
4. **Protection features are essential** for reliable operation
5. **Power integrity considerations** affect system performance
6. **Multi-rail supplies require careful sequencing** and regulation

---

**Previous Topic**: [Component Selection](./Component_Selection.md)  
**Next Topic**: [Clock Distribution](./Clock_Distribution.md)
