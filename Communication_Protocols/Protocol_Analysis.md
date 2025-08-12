# Protocol Analysis and Debugging

Effective protocol analysis accelerates bring-up, reveals timing bugs, and derisks field failures. This guide covers instrumentation, capture strategy, timing analysis, and a practical checklist for UART, SPI, I2C, CAN, and Ethernet-based protocols.

---

## Instruments and Measurement Fundamentals

### Logic Analyzer Selection and Configuration
- **Digital sampling**: Choose adequate memory depth and sample rate
- **Protocol decoders**: UART/SPI/I2C/CAN with customizable parameters
- **Trigger capabilities**: Edge, pattern, state, and protocol-specific triggers
- **Memory depth calculation**: `Memory_Time = Memory_Depth / Sample_Rate`

#### Sample Rate Requirements
```c
// Calculate minimum sample rate for reliable edge detection
uint32_t calculate_min_sample_rate(uint32_t signal_frequency, uint32_t edge_accuracy_ns) {
    // Nyquist: 2x signal frequency minimum
    uint32_t nyquist_rate = signal_frequency * 2;
    
    // Edge accuracy: higher sample rate = better edge precision
    uint32_t accuracy_rate = 1000000000 / edge_accuracy_ns;
    
    // Use the higher of the two rates, with 10x margin for noisy signals
    uint32_t min_rate = MAX(nyquist_rate, accuracy_rate) * 10;
    
    return min_rate;
}

// Example: 1MHz SPI clock with 10ns edge accuracy
// Min sample rate = MAX(2MHz, 100MHz) * 10 = 1GHz
```

#### Memory Depth Planning
```c
// Calculate required memory depth for capture duration
uint32_t calculate_memory_depth(uint32_t sample_rate, uint32_t capture_time_ms) {
    uint32_t samples_needed = (sample_rate * capture_time_ms) / 1000;
    
    // Add 20% margin for trigger positioning
    uint32_t memory_depth = samples_needed * 1.2;
    
    return memory_depth;
}

// Example: 1GHz sample rate, 10ms capture
// Memory depth = (1GHz * 10ms) / 1000 * 1.2 = 12MSamples
```

### Oscilloscope Measurements and Analysis
- **Analog waveform fidelity**: Rise/fall time, ringing, overshoot, jitter
- **Bandwidth requirements**: 3-5x highest frequency component for accurate measurements
- **Probe considerations**: 10x vs 1x, active vs passive, differential vs single-ended

#### Signal Integrity Measurements
```c
// Calculate signal integrity metrics
typedef struct {
    float rise_time_ns;
    float fall_time_ns;
    float overshoot_percent;
    float undershoot_percent;
    float jitter_rms_ps;
    float jitter_peak_peak_ps;
} signal_metrics_t;

signal_metrics_t analyze_signal_integrity(float *waveform, uint32_t samples, 
                                         float sample_period_ns) {
    signal_metrics_t metrics = {0};
    
    // Find 10% and 90% crossing points for rise/fall time
    float v_10 = 0.1 * V_MAX;
    float v_90 = 0.9 * V_MAX;
    
    uint32_t t10_rise = find_crossing(waveform, samples, v_10, RISING);
    uint32_t t90_rise = find_crossing(waveform, samples, v_90, RISING);
    uint32_t t10_fall = find_crossing(waveform, samples, v_10, FALLING);
    uint32_t t90_fall = find_crossing(waveform, samples, v_90, FALLING);
    
    metrics.rise_time_ns = (t90_rise - t10_rise) * sample_period_ns;
    metrics.fall_time_ns = (t10_fall - t90_fall) * sample_period_ns;
    
    // Calculate overshoot/undershoot
    float v_max = find_peak(waveform, samples, MAX);
    float v_min = find_peak(waveform, samples, MIN);
    
    metrics.overshoot_percent = ((v_max - V_MAX) / V_MAX) * 100.0f;
    metrics.undershoot_percent = ((V_MIN - v_min) / V_MIN) * 100.0f;
    
    // Calculate jitter (simplified)
    metrics.jitter_rms_ps = calculate_jitter_rms(waveform, samples, sample_period_ns);
    metrics.jitter_peak_peak_ps = calculate_jitter_peak_peak(waveform, samples, sample_period_ns);
    
    return metrics;
}
```

### Protocol Analyzers and Specialized Tools
- **CAN/CAN-FD analyzers**: Dedicated decoders with error analysis
- **USB analyzers**: Protocol-level decoding with timing analysis
- **Ethernet taps**: Port mirroring (SPAN) for lossless captures
- **Software capture**: Host-based packet capture with timestamping

---

## Advanced Capture Strategy

### Trigger Configuration for Complex Scenarios
```c
// Multi-condition trigger setup
typedef struct {
    uint8_t trigger_type;      // EDGE, PATTERN, STATE, PROTOCOL
    uint8_t trigger_source;    // Channel number
    uint8_t trigger_condition; // RISING, FALLING, HIGH, LOW
    uint32_t trigger_value;    // Pattern or threshold value
    uint32_t pre_trigger;      // Pre-trigger samples
    uint32_t post_trigger;     // Post-trigger samples
} trigger_config_t;

// Configure complex trigger for UART frame error
err_t configure_uart_error_trigger(trigger_config_t *config) {
    config->trigger_type = TRIGGER_PROTOCOL;
    config->trigger_source = UART_RX_CHANNEL;
    config->trigger_condition = UART_FRAME_ERROR;
    config->pre_trigger = 1000;   // 1ms pre-trigger
    config->post_trigger = 5000;  // 5ms post-trigger
    
    return configure_logic_analyzer_trigger(config);
}

// Pattern trigger for specific byte sequences
err_t configure_pattern_trigger(uint8_t *pattern, uint8_t pattern_len) {
    trigger_config_t config = {0};
    config.trigger_type = TRIGGER_PATTERN;
    config.trigger_source = DATA_CHANNEL;
    config.trigger_value = pattern_to_uint32(pattern, pattern_len);
    
    return configure_logic_analyzer_trigger(&config);
}
```

### Correlated Multi-Instrument Capture
```c
// Synchronize multiple instruments for comprehensive analysis
typedef struct {
    uint32_t timestamp_ns;
    uint8_t instrument_id;
    uint8_t event_type;
    uint32_t event_data;
} correlated_event_t;

// Event correlation buffer
#define MAX_CORRELATED_EVENTS 1000
static correlated_event_t event_buffer[MAX_CORRELATED_EVENTS];
static uint32_t event_count = 0;

// Add event from any instrument
void add_correlated_event(uint8_t instrument_id, uint8_t event_type, uint32_t event_data) {
    if (event_count < MAX_CORRELATED_EVENTS) {
        event_buffer[event_count].timestamp_ns = get_high_resolution_time();
        event_buffer[event_count].instrument_id = instrument_id;
        event_buffer[event_count].event_type = event_type;
        event_buffer[event_count].event_data = event_data;
        event_count++;
    }
}

// Analyze timing relationships between events
void analyze_event_timing(void) {
    for (uint32_t i = 1; i < event_count; i++) {
        uint32_t delta_ns = event_buffer[i].timestamp_ns - event_buffer[i-1].timestamp_ns;
        
        // Check for timing violations
        if (delta_ns > MAX_ALLOWED_DELTA_NS) {
            printf("Timing violation: %lu ns between events %lu and %lu\n", 
                   delta_ns, i-1, i);
        }
    }
}
```

---

## UART Protocol Analysis

### Advanced UART Timing Analysis
```c
// UART timing budget analysis
typedef struct {
    uint32_t baud_rate;
    uint32_t bit_time_ns;
    uint32_t inter_byte_time_ns;
    uint32_t isr_latency_ns;
    uint32_t buffer_processing_time_ns;
    uint32_t margin_ns;
} uart_timing_budget_t;

uart_timing_budget_t calculate_uart_timing(uint32_t baud_rate, uint8_t data_bits, 
                                          uint8_t stop_bits, uint8_t parity) {
    uart_timing_budget_t budget = {0};
    
    budget.baud_rate = baud_rate;
    budget.bit_time_ns = 1000000000 / baud_rate;
    
    // Calculate frame time (start + data + parity + stop)
    uint8_t frame_bits = 1 + data_bits + (parity ? 1 : 0) + stop_bits;
    uint32_t frame_time_ns = frame_bits * budget.bit_time_ns;
    
    // Inter-byte time includes frame time plus any idle time
    budget.inter_byte_time_ns = frame_time_ns;
    
    // Calculate required ISR latency
    budget.isr_latency_ns = budget.bit_time_ns / 2;  // Must sample in middle of bit
    
    // Buffer processing time (copy, parse, queue)
    budget.buffer_processing_time_ns = 1000;  // 1µs typical
    
    // Required margin
    budget.margin_ns = budget.inter_byte_time_ns - budget.isr_latency_ns - 
                      budget.buffer_processing_time_ns;
    
    return budget;
}

// Example: 115200 baud, 8N1
// Bit time = 8.68 µs
// Frame time = 10 bits × 8.68 µs = 86.8 µs
// ISR latency must be < 4.34 µs (half bit time)
// Buffer processing: 1 µs
// Margin: 86.8 - 4.34 - 1 = 81.46 µs
```

### UART Error Detection and Analysis
```c
// UART error statistics and analysis
typedef struct {
    uint32_t frame_errors;
    uint32_t parity_errors;
    uint32_t overrun_errors;
    uint32_t noise_errors;
    uint32_t total_errors;
    uint32_t total_frames;
    float error_rate;
} uart_error_stats_t;

// Analyze UART errors from logic analyzer capture
uart_error_stats_t analyze_uart_errors(uint8_t *capture_data, uint32_t capture_length) {
    uart_error_stats_t stats = {0};
    
    for (uint32_t i = 0; i < capture_length - 10; i++) {
        // Look for UART frame patterns
        if (is_uart_start_bit(capture_data, i)) {
            stats.total_frames++;
            
            // Check for frame errors
            if (has_frame_error(capture_data, i)) {
                stats.frame_errors++;
            }
            
            // Check for parity errors
            if (has_parity_error(capture_data, i)) {
                stats.parity_errors++;
            }
            
            // Check for overrun
            if (has_overrun_error(capture_data, i)) {
                stats.overrun_errors++;
            }
        }
    }
    
    stats.total_errors = stats.frame_errors + stats.parity_errors + 
                        stats.overrun_errors + stats.noise_errors;
    
    if (stats.total_frames > 0) {
        stats.error_rate = (float)stats.total_errors / stats.total_frames * 100.0f;
    }
    
    return stats;
}

// UART signal quality analysis
typedef struct {
    float signal_to_noise_ratio_db;
    float eye_diagram_width_percent;
    float jitter_rms_ps;
    float voltage_levels_valid;
} uart_signal_quality_t;

uart_signal_quality_t analyze_uart_signal_quality(float *analog_waveform, 
                                                 uint32_t samples, 
                                                 float sample_period_ns) {
    uart_signal_quality_t quality = {0};
    
    // Calculate SNR from high and low levels
    float v_high = calculate_high_level(waveform, samples);
    float v_low = calculate_low_level(waveform, samples);
    float noise_rms = calculate_noise_rms(waveform, samples);
    
    float signal_amplitude = v_high - v_low;
    quality.signal_to_noise_ratio_db = 20 * log10(signal_amplitude / (2 * noise_rms));
    
    // Calculate eye diagram width
    quality.eye_diagram_width_percent = calculate_eye_width(waveform, samples, sample_period_ns);
    
    // Calculate jitter
    quality.jitter_rms_ps = calculate_jitter_rms(waveform, samples, sample_period_ns);
    
    // Validate voltage levels
    quality.voltage_levels_valid = (v_high > V_IH_MIN && v_low < V_IL_MAX) ? 1.0f : 0.0f;
    
    return quality;
}
```

---

## SPI Protocol Analysis

### SPI Timing Analysis and Validation
```c
// SPI timing parameters and validation
typedef struct {
    uint32_t clock_frequency_hz;
    uint32_t clock_period_ns;
    uint32_t setup_time_ns;
    uint32_t hold_time_ns;
    uint32_t clock_to_output_ns;
    uint32_t chip_select_delay_ns;
    uint8_t clock_polarity;    // CPOL: 0 or 1
    uint8_t clock_phase;       // CPHA: 0 or 1
} spi_timing_params_t;

// Validate SPI timing against device specifications
err_t validate_spi_timing(spi_timing_params_t *measured, spi_timing_params_t *required) {
    err_t result = ERR_OK;
    
    // Check setup time
    if (measured->setup_time_ns < required->setup_time_ns) {
        printf("Setup time violation: %lu ns < %lu ns required\n", 
               measured->setup_time_ns, required->setup_time_ns);
        result = ERR_TIMEOUT;
    }
    
    // Check hold time
    if (measured->hold_time_ns < required->hold_time_ns) {
        printf("Hold time violation: %lu ns < %lu ns required\n", 
               measured->hold_time_ns, required->hold_time_ns);
        result = ERR_TIMEOUT;
    }
    
    // Check clock frequency
    if (measured->clock_frequency_hz > required->clock_frequency_hz) {
        printf("Clock frequency violation: %lu Hz > %lu Hz max\n", 
               measured->clock_frequency_hz, required->clock_frequency_hz);
        result = ERR_TIMEOUT;
    }
    
    return result;
}

// Calculate SPI timing from oscilloscope measurements
spi_timing_params_t calculate_spi_timing(float *sclk_waveform, float *mosi_waveform,
                                        float *miso_waveform, float *cs_waveform,
                                        uint32_t samples, float sample_period_ns) {
    spi_timing_params_t timing = {0};
    
    // Find clock edges
    uint32_t *clock_edges = find_clock_edges(sclk_waveform, samples);
    uint32_t edge_count = count_clock_edges(clock_edges);
    
    // Calculate clock frequency
    if (edge_count >= 2) {
        uint32_t period_samples = clock_edges[1] - clock_edges[0];
        timing.clock_period_ns = period_samples * sample_period_ns;
        timing.clock_frequency_hz = 1000000000 / timing.clock_period_ns;
    }
    
    // Calculate setup time (data stable before clock edge)
    timing.setup_time_ns = calculate_setup_time(mosi_waveform, sclk_waveform, 
                                               samples, sample_period_ns);
    
    // Calculate hold time (data stable after clock edge)
    timing.hold_time_ns = calculate_hold_time(mosi_waveform, sclk_waveform, 
                                             samples, sample_period_ns);
    
    // Calculate clock-to-output delay
    timing.clock_to_output_ns = calculate_clock_to_output(miso_waveform, sclk_waveform,
                                                         samples, sample_period_ns);
    
    // Calculate chip select delay
    timing.chip_select_delay_ns = calculate_cs_delay(cs_waveform, sclk_waveform,
                                                    samples, sample_period_ns);
    
    return timing;
}
```

### SPI Protocol Decoding and Analysis
```c
// SPI frame decoder
typedef struct {
    uint8_t *data;
    uint32_t data_length;
    uint8_t chip_select;
    uint32_t timestamp_ns;
    uint8_t frame_type;  // READ, WRITE, READ_WRITE
    uint8_t address;
    uint16_t payload_length;
} spi_frame_t;

// Decode SPI frames from logic analyzer capture
spi_frame_t* decode_spi_frames(uint8_t *capture_data, uint32_t capture_length,
                               spi_timing_params_t *timing, uint32_t *frame_count) {
    // Allocate frame buffer
    spi_frame_t *frames = malloc(MAX_SPI_FRAMES * sizeof(spi_frame_t));
    *frame_count = 0;
    
    uint32_t bit_index = 0;
    uint32_t frame_start = 0;
    
    for (uint32_t i = 0; i < capture_length && *frame_count < MAX_SPI_FRAMES; i++) {
        // Detect chip select assertion
        if (is_chip_select_asserted(capture_data, i)) {
            frame_start = i;
            frames[*frame_count].timestamp_ns = i * timing->clock_period_ns;
            frames[*frame_count].chip_select = get_chip_select_number(capture_data, i);
        }
        
        // Detect chip select deassertion
        if (is_chip_select_deasserted(capture_data, i) && frame_start > 0) {
            // Frame complete, decode it
            uint32_t frame_length = i - frame_start;
            frames[*frame_count].data_length = frame_length / 8;  // 8 bits per byte
            
            // Allocate data buffer
            frames[*frame_count].data = malloc(frames[*frame_count].data_length);
            
            // Decode data bits
            decode_spi_data_bits(capture_data, frame_start, frame_length, 
                                frames[*frame_count].data, timing);
            
            // Determine frame type and address
            analyze_spi_frame_content(&frames[*frame_count]);
            
            (*frame_count)++;
            frame_start = 0;
        }
    }
    
    return frames;
}

// Analyze SPI frame content for common protocols
void analyze_spi_frame_content(spi_frame_t *frame) {
    if (frame->data_length >= 1) {
        // Common SPI command formats
        uint8_t command = frame->data[0];
        
        if (command & 0x80) {
            // Read command
            frame->frame_type = SPI_FRAME_READ;
            frame->address = command & 0x7F;
        } else {
            // Write command
            frame->frame_type = SPI_FRAME_WRITE;
            frame->address = command & 0x7F;
        }
        
        frame->payload_length = frame->data_length - 1;
    }
}
```

---

## I2C Protocol Analysis

### I2C Timing and Signal Analysis
```c
// I2C timing parameters
typedef struct {
    uint32_t clock_frequency_hz;
    uint32_t clock_period_ns;
    uint32_t setup_time_ns;
    uint32_t hold_time_ns;
    uint32_t data_setup_time_ns;
    uint32_t data_hold_time_ns;
    uint32_t clock_low_time_ns;
    uint32_t clock_high_time_ns;
    uint32_t start_hold_time_ns;
    uint32_t stop_setup_time_ns;
} i2c_timing_params_t;

// I2C signal quality analysis
typedef struct {
    float scl_rise_time_ns;
    float scl_fall_time_ns;
    float sda_rise_time_ns;
    float sda_fall_time_ns;
    float pull_up_resistance_ohms;
    float bus_capacitance_pf;
    float noise_margin_mv;
} i2c_signal_quality_t;

// Calculate I2C bus parameters from measurements
i2c_signal_quality_t analyze_i2c_signal_quality(float *scl_waveform, float *sda_waveform,
                                                uint32_t samples, float sample_period_ns) {
    i2c_signal_quality_t quality = {0};
    
    // Calculate rise and fall times
    quality.scl_rise_time_ns = calculate_rise_time(scl_waveform, samples, sample_period_ns);
    quality.scl_fall_time_ns = calculate_fall_time(scl_waveform, samples, sample_period_ns);
    quality.sda_rise_time_ns = calculate_rise_time(sda_waveform, samples, sample_period_ns);
    quality.sda_fall_time_ns = calculate_fall_time(sda_waveform, samples, sample_period_ns);
    
    // Calculate pull-up resistance from rise time
    // τ = RC, where τ is rise time, R is pull-up resistance, C is bus capacitance
    float avg_rise_time_ns = (quality.scl_rise_time_ns + quality.sda_rise_time_ns) / 2.0f;
    quality.bus_capacitance_pf = estimate_bus_capacitance();  // From PCB design
    quality.pull_up_resistance_ohms = (avg_rise_time_ns * 1e-9) / 
                                     (quality.bus_capacitance_pf * 1e-12);
    
    // Calculate noise margin
    float v_ih_min = 0.7 * V_DD;  // Input high minimum
    float v_il_max = 0.3 * V_DD;  // Input low maximum
    float v_oh_min = 0.9 * V_DD;  // Output high minimum
    float v_ol_max = 0.1 * V_DD;  // Output low maximum
    
    quality.noise_margin_mv = MIN(v_oh_min - v_ih_min, v_il_max - v_ol_max) * 1000.0f;
    
    return quality;
}
```

### I2C Protocol Decoding and Error Analysis
```c
// I2C frame structure
typedef struct {
    uint8_t start_condition;
    uint8_t address;
    uint8_t read_write;
    uint8_t address_ack;
    uint8_t *data;
    uint32_t data_length;
    uint8_t *ack_bits;
    uint8_t stop_condition;
    uint32_t timestamp_ns;
} i2c_frame_t;

// I2C error detection
typedef struct {
    uint32_t bus_errors;
    uint32_t arbitration_lost;
    uint32_t nack_errors;
    uint32_t clock_stretching_timeouts;
    uint32_t bus_busy_errors;
    uint32_t total_transactions;
} i2c_error_stats_t;

// Decode I2C frames and detect errors
i2c_error_stats_t decode_i2c_protocol(uint8_t *capture_data, uint32_t capture_length,
                                      i2c_timing_params_t *timing) {
    i2c_error_stats_t stats = {0};
    
    uint32_t i = 0;
    while (i < capture_length) {
        // Look for start condition (SDA falling while SCL high)
        if (is_i2c_start_condition(capture_data, i)) {
            stats.total_transactions++;
            
            // Decode address
            uint8_t address = decode_i2c_address(capture_data, i + 1, timing);
            uint8_t read_write = address & 0x01;
            address >>= 1;
            
            // Check for ACK/NACK
            uint8_t ack = capture_data[i + 9];  // 9th bit after start
            if (!ack) {
                stats.nack_errors++;
            }
            
            // Decode data if present
            if (read_write == I2C_READ) {
                // Master reading from slave
                uint32_t data_start = i + 10;
                uint32_t data_length = decode_i2c_data_length(capture_data, data_start, timing);
                
                // Check ACK bits for each byte
                for (uint32_t j = 0; j < data_length; j++) {
                    uint8_t data_ack = capture_data[data_start + j * 9 + 8];
                    if (!data_ack) {
                        stats.nack_errors++;
                    }
                }
            } else {
                // Master writing to slave
                uint32_t data_start = i + 10;
                uint32_t data_length = decode_i2c_data_length(capture_data, data_start, timing);
                
                // Check ACK bits for each byte
                for (uint32_t j = 0; j < data_length; j++) {
                    uint8_t data_ack = capture_data[data_start + j * 9 + 8];
                    if (!data_ack) {
                        stats.nack_errors++;
                    }
                }
            }
            
            // Look for stop condition
            uint32_t stop_pos = find_i2c_stop_condition(capture_data, i, capture_length);
            if (stop_pos == 0) {
                stats.bus_errors++;  // Missing stop condition
            }
            
            i = stop_pos + 1;
        } else {
            i++;
        }
    }
    
    return stats;
}
```

---

## CAN Protocol Analysis

### CAN Bit Timing and Signal Analysis
```c
// CAN bit timing parameters
typedef struct {
    uint32_t nominal_bit_rate;
    uint32_t data_bit_rate;  // For CAN-FD
    uint32_t prescaler;
    uint32_t time_quanta;
    uint32_t sync_seg;
    uint32_t tseg1;
    uint32_t tseg2;
    uint32_t sjw;  // Synchronization jump width
    float sample_point_percent;
} can_bit_timing_t;

// Calculate CAN bit timing from oscilloscope measurements
can_bit_timing_t calculate_can_bit_timing(float *can_h_waveform, float *can_l_waveform,
                                         uint32_t samples, float sample_period_ns) {
    can_bit_timing_t timing = {0};
    
    // Find bit boundaries
    uint32_t *bit_boundaries = find_can_bit_boundaries(can_h_waveform, can_l_waveform, 
                                                      samples);
    uint32_t bit_count = count_can_bits(bit_boundaries);
    
    if (bit_count >= 2) {
        // Calculate nominal bit rate
        uint32_t bit_period_samples = bit_boundaries[1] - bit_boundaries[0];
        uint32_t bit_period_ns = bit_period_samples * sample_period_ns;
        timing.nominal_bit_rate = 1000000000 / bit_period_ns;
        
        // Calculate time quanta (typically 1/16 of bit time)
        timing.time_quanta = bit_period_ns / 16;
        
        // Calculate sample point (typically 87.5% of bit time)
        timing.sample_point_percent = 87.5f;
        
        // Calculate time segments
        timing.sync_seg = 1;  // Always 1 time quantum
        timing.tseg1 = 13;    // 13 time quanta (typical)
        timing.tseg2 = 2;     // 2 time quanta (typical)
        timing.sjw = 1;       // 1 time quantum (typical)
    }
    
    return timing;
}

// Validate CAN bit timing against specifications
err_t validate_can_bit_timing(can_bit_timing_t *measured, can_bit_timing_t *required) {
    err_t result = ERR_OK;
    
    // Check bit rate
    if (measured->nominal_bit_rate > required->nominal_bit_rate) {
        printf("Bit rate violation: %lu bps > %lu bps max\n", 
               measured->nominal_bit_rate, required->nominal_bit_rate);
        result = ERR_TIMEOUT;
    }
    
    // Check sample point
    if (measured->sample_point_percent < 75.0f || 
        measured->sample_point_percent > 90.0f) {
        printf("Sample point violation: %.1f%% not in 75-90%% range\n", 
               measured->sample_point_percent);
        result = ERR_TIMEOUT;
    }
    
    // Check time segments
    if (measured->tseg1 < 3 || measured->tseg1 > 16) {
        printf("TSEG1 violation: %lu not in 3-16 range\n", measured->tseg1);
        result = ERR_TIMEOUT;
    }
    
    if (measured->tseg2 < 2 || measured->tseg2 > 8) {
        printf("TSEG2 violation: %lu not in 2-8 range\n", measured->tseg2);
        result = ERR_TIMEOUT;
    }
    
    return result;
}
```

### CAN Protocol Decoding and Error Analysis
```c
// CAN frame structure
typedef struct {
    uint32_t can_id;
    uint8_t dlc;           // Data length code
    uint8_t rtr;           // Remote transmission request
    uint8_t ide;           // Extended identifier
    uint8_t data[8];       // Data payload
    uint32_t timestamp_ns;
    uint8_t error_flags;
} can_frame_t;

// CAN error analysis
typedef struct {
    uint32_t bit_errors;
    uint32_t stuff_errors;
    uint32_t form_errors;
    uint32_t ack_errors;
    uint32_t crc_errors;
    uint32_t arbitration_lost;
    uint32_t total_frames;
    float error_rate;
} can_error_stats_t;

// Decode CAN frames and detect errors
can_error_stats_t decode_can_protocol(uint8_t *capture_data, uint32_t capture_length,
                                      can_bit_timing_t *timing) {
    can_error_stats_t stats = {0};
    
    uint32_t i = 0;
    while (i < capture_length) {
        // Look for start of frame (dominant bit)
        if (is_can_start_of_frame(capture_data, i)) {
            stats.total_frames++;
            
            // Decode identifier
            uint32_t can_id = decode_can_identifier(capture_data, i + 1, timing);
            
            // Check for extended identifier
            uint8_t ide = (can_id & 0x80000000) ? 1 : 0;
            
            // Decode control field
            uint8_t dlc = decode_can_dlc(capture_data, i + (ide ? 33 : 12), timing);
            uint8_t rtr = decode_can_rtr(capture_data, i + (ide ? 33 : 12), timing);
            
            // Decode data if present
            if (!rtr && dlc > 0) {
                uint32_t data_start = i + (ide ? 33 : 12) + 6;  // Control field is 6 bits
                decode_can_data(capture_data, data_start, dlc, timing);
            }
            
            // Check CRC
            uint32_t crc_start = i + (ide ? 33 : 12) + 6 + (dlc * 8);
            uint16_t calculated_crc = calculate_can_crc(capture_data, i, crc_start - i);
            uint16_t received_crc = decode_can_crc(capture_data, crc_start, timing);
            
            if (calculated_crc != received_crc) {
                stats.crc_errors++;
            }
            
            // Check ACK slot
            uint32_t ack_pos = crc_start + 15;  // CRC is 15 bits
            uint8_t ack = capture_data[ack_pos];
            if (!ack) {
                stats.ack_errors++;
            }
            
            // Look for end of frame
            uint32_t eof_pos = find_can_end_of_frame(capture_data, ack_pos + 1, capture_length);
            if (eof_pos == 0) {
                stats.form_errors++;
            }
            
            i = eof_pos + 7;  // EOF is 7 recessive bits
        } else {
            i++;
        }
    }
    
    // Calculate error rate
    if (stats.total_frames > 0) {
        stats.error_rate = (float)(stats.bit_errors + stats.stuff_errors + 
                                  stats.form_errors + stats.ack_errors + 
                                  stats.crc_errors) / stats.total_frames * 100.0f;
    }
    
    return stats;
}
```

---

## Advanced Timing and Jitter Analysis

### High-Resolution Timing Measurements
```c
// High-resolution timer for embedded systems
typedef struct {
    uint32_t timer_frequency_hz;
    uint32_t timer_resolution_ns;
    uint32_t overflow_count;
    uint32_t last_timestamp;
} high_res_timer_t;

// Initialize high-resolution timer
err_t init_high_res_timer(high_res_timer_t *timer) {
    // Configure DWT cycle counter (ARM Cortex-M)
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    
    timer->timer_frequency_hz = SystemCoreClock;
    timer->timer_resolution_ns = 1000000000 / timer->timer_frequency_hz;
    timer->overflow_count = 0;
    timer->last_timestamp = DWT->CYCCNT;
    
    return ERR_OK;
}

// Get high-resolution timestamp
uint64_t get_high_res_timestamp(high_res_timer_t *timer) {
    uint32_t current = DWT->CYCCNT;
    
    // Check for overflow
    if (current < timer->last_timestamp) {
        timer->overflow_count++;
    }
    timer->last_timestamp = current;
    
    // Combine overflow count and current value
    uint64_t timestamp = ((uint64_t)timer->overflow_count << 32) | current;
    
    return timestamp;
}

// Calculate time difference in nanoseconds
uint32_t calculate_time_difference_ns(uint64_t start, uint64_t end, 
                                     high_res_timer_t *timer) {
    uint64_t diff = end - start;
    return (uint32_t)(diff * timer->timer_resolution_ns);
}
```

### Jitter Analysis and Statistics
```c
// Jitter analysis structure
typedef struct {
    uint32_t min_latency_ns;
    uint32_t max_latency_ns;
    uint32_t avg_latency_ns;
    uint32_t jitter_rms_ns;
    uint32_t jitter_peak_peak_ns;
    uint32_t samples_50th_percentile_ns;
    uint32_t samples_95th_percentile_ns;
    uint32_t samples_99th_percentile_ns;
    uint32_t samples_99_9th_percentile_ns;
} jitter_analysis_t;

// Analyze jitter from timing measurements
jitter_analysis_t analyze_jitter(uint32_t *latency_samples, uint32_t sample_count) {
    jitter_analysis_t analysis = {0};
    
    if (sample_count == 0) return analysis;
    
    // Calculate basic statistics
    analysis.min_latency_ns = latency_samples[0];
    analysis.max_latency_ns = latency_samples[0];
    uint64_t sum = 0;
    
    for (uint32_t i = 0; i < sample_count; i++) {
        if (latency_samples[i] < analysis.min_latency_ns) {
            analysis.min_latency_ns = latency_samples[i];
        }
        if (latency_samples[i] > analysis.max_latency_ns) {
            analysis.max_latency_ns = latency_samples[i];
        }
        sum += latency_samples[i];
    }
    
    analysis.avg_latency_ns = (uint32_t)(sum / sample_count);
    analysis.jitter_peak_peak_ns = analysis.max_latency_ns - analysis.min_latency_ns;
    
    // Calculate RMS jitter
    uint64_t variance_sum = 0;
    for (uint32_t i = 0; i < sample_count; i++) {
        int32_t diff = (int32_t)latency_samples[i] - (int32_t)analysis.avg_latency_ns;
        variance_sum += (uint64_t)(diff * diff);
    }
    float variance = (float)variance_sum / sample_count;
    analysis.jitter_rms_ns = (uint32_t)sqrtf(variance);
    
    // Calculate percentiles
    uint32_t *sorted_samples = malloc(sample_count * sizeof(uint32_t));
    memcpy(sorted_samples, latency_samples, sample_count * sizeof(uint32_t));
    qsort(sorted_samples, sample_count, sizeof(uint32_t), compare_uint32);
    
    analysis.samples_50th_percentile_ns = sorted_samples[sample_count / 2];
    analysis.samples_95th_percentile_ns = sorted_samples[(sample_count * 95) / 100];
    analysis.samples_99th_percentile_ns = sorted_samples[(sample_count * 99) / 100];
    analysis.samples_99_9th_percentile_ns = sorted_samples[(sample_count * 999) / 1000];
    
    free(sorted_samples);
    return analysis;
}
```

---

## Comprehensive Debugging Methodology

### Structured Debug Checklist Implementation
```c
// Debug session management
typedef struct {
    char description[256];
    uint32_t start_timestamp;
    uint32_t end_timestamp;
    uint8_t severity;  // 1=Low, 2=Medium, 3=High, 4=Critical
    uint8_t status;    // 0=Open, 1=Investigating, 2=Resolved, 3=Closed
    char root_cause[512];
    char solution[512];
    char notes[1024];
} debug_session_t;

// Debug checklist implementation
typedef struct {
    uint8_t step_completed;
    char step_description[256];
    uint8_t result;  // 0=Pass, 1=Pass with issues, 2=Fail
    char findings[512];
    char next_actions[512];
} debug_checklist_step_t;

#define DEBUG_STEPS_COUNT 7
static debug_checklist_step_t debug_checklist[DEBUG_STEPS_COUNT] = {
    {0, "Reproduce and bound the problem", 0, "", ""},
    {0, "Validate physical layer", 0, "", ""},
    {0, "Verify timing", 0, "", ""},
    {0, "Confirm configuration", 0, "", ""},
    {0, "Inspect protocol semantics", 0, "", ""},
    {0, "Introduce instrumentation", 0, "", ""},
    {0, "Mitigate, then fix", 0, "", ""}
};

// Execute debug checklist
void execute_debug_checklist(debug_session_t *session) {
    printf("Starting debug session: %s\n", session->description);
    
    for (int i = 0; i < DEBUG_STEPS_COUNT; i++) {
        printf("\nStep %d: %s\n", i + 1, debug_checklist[i].step_description);
        
        // Execute step
        debug_checklist[i].result = execute_debug_step(i);
        debug_checklist[i].step_completed = 1;
        
        // Record findings
        printf("Result: %s\n", debug_checklist[i].result == 0 ? "PASS" : 
                              debug_checklist[i].result == 1 ? "PASS with issues" : "FAIL");
        printf("Findings: %s\n", debug_checklist[i].findings);
        printf("Next actions: %s\n", debug_checklist[i].next_actions);
        
        // Check if we can proceed
        if (debug_checklist[i].result == 2) {
            printf("Critical failure at step %d. Stopping debug session.\n", i + 1);
            break;
        }
    }
    
    // Generate debug report
    generate_debug_report(session);
}

// Execute individual debug step
uint8_t execute_debug_step(uint8_t step_index) {
    switch (step_index) {
        case 0: // Reproduce and bound the problem
            return reproduce_problem_step();
        case 1: // Validate physical layer
            return validate_physical_layer_step();
        case 2: // Verify timing
            return verify_timing_step();
        case 3: // Confirm configuration
            return confirm_configuration_step();
        case 4: // Inspect protocol semantics
            return inspect_protocol_semantics_step();
        case 5: // Introduce instrumentation
            return introduce_instrumentation_step();
        case 6: // Mitigate, then fix
            return mitigate_and_fix_step();
        default:
            return 2; // Fail
    }
}
```

### Automated Problem Detection
```c
// Automated problem detection system
typedef struct {
    uint32_t check_interval_ms;
    uint32_t last_check_time;
    uint8_t enabled;
    uint32_t problem_count;
    char last_problem[256];
} problem_detector_t;

// Problem detection rules
typedef struct {
    char rule_name[64];
    uint8_t (*check_function)(void);
    uint8_t severity;
    uint32_t threshold;
    uint32_t current_count;
} detection_rule_t;

// Example detection rules
static detection_rule_t detection_rules[] = {
    {"UART_Frame_Errors", check_uart_frame_errors, 2, 5, 0},
    {"SPI_Timing_Violations", check_spi_timing_violations, 3, 3, 0},
    {"I2C_Bus_Errors", check_i2c_bus_errors, 2, 10, 0},
    {"CAN_CRC_Errors", check_can_crc_errors, 3, 2, 0},
    {"Network_Timeout", check_network_timeout, 4, 1, 0}
};

// Run automated problem detection
void run_problem_detection(void) {
    uint32_t current_time = sys_now();
    
    for (int i = 0; i < sizeof(detection_rules) / sizeof(detection_rules[0]); i++) {
        if (detection_rules[i].check_function()) {
            detection_rules[i].current_count++;
            
            if (detection_rules[i].current_count >= detection_rules[i].threshold) {
                // Problem detected
                printf("PROBLEM DETECTED: %s (Severity: %d)\n", 
                       detection_rules[i].rule_name, detection_rules[i].severity);
                
                // Take automatic action based on severity
                take_automatic_action(detection_rules[i].severity);
                
                // Reset counter
                detection_rules[i].current_count = 0;
            }
        } else {
            // Reset counter if no problem
            detection_rules[i].current_count = 0;
        }
    }
}

// Example check function
uint8_t check_uart_frame_errors(void) {
    // Check UART error counters
    if (uart_error_stats.frame_errors > 0) {
        return 1; // Problem detected
    }
    return 0; // No problem
}
```

This enhanced Protocol Analysis document now provides comprehensive technical depth with practical implementation examples, detailed timing calculations, and sophisticated debugging methodologies that embedded engineers can directly apply in their projects.


