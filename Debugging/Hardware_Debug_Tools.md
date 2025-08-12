# Hardware Debug Tools for Embedded Systems

## Overview
Hardware debug tools are essential for embedded system development, providing visibility into both digital and analog signals, memory states, and program execution. This guide covers the three fundamental hardware debugging approaches: JTAG/SWD debugging, logic analyzer usage, and oscilloscope measurements.

## Table of Contents
1. [JTAG/SWD Debugging](#jtagswd-debugging)
2. [Logic Analyzer Usage](#logic-analyzer-usage)
3. [Oscilloscope Measurements](#oscilloscope-measurements)
4. [Tool Integration and Workflow](#tool-integration-and-workflow)
5. [Common Pitfalls and Solutions](#common-pitfalls-and-solutions)
6. [Best Practices](#best-practices)
7. [Interview Questions](#interview-questions)

---

## JTAG/SWD Debugging

### Core Concepts

#### What is JTAG/SWD?
**JTAG (Joint Test Action Group)** and **SWD (Serial Wire Debug)** are industry-standard interfaces for debugging embedded systems. They provide:

- **Non-intrusive debugging**: Run code while monitoring execution
- **Real-time inspection**: Examine registers, memory, and variables without stopping execution
- **Hardware breakpoints**: Set breakpoints that don't modify code
- **Flash programming**: Download firmware directly to target devices

#### JTAG vs SWD Architecture
```
JTAG Architecture:
Target Device ←→ JTAG Interface ←→ Debug Host
     ↑              ↑                ↑
  TMS/TCK      Level Shifter    GDB/IDE
  TDI/TDO      Buffer/Driver   OpenOCD
   TRST         Power Supply    Debugger

SWD Architecture (ARM-specific):
Target Device ←→ SWD Interface ←→ Debug Host
     ↑              ↑                ↑
   SWDIO         Level Shifter    GDB/IDE
   SWCLK         Buffer/Driver   OpenOCD
   SWO (opt)     Power Supply    Debugger
```

#### Debug Access Port (DAP)
The DAP is the core component that provides debug access:

```c
// DAP register structure (simplified)
typedef struct {
    uint32_t IDCODE;      // Device identification
    uint32_t CTRLSTAT;    // Control and status
    uint32_t RESEND;      // Resend last data
    uint32_t RDBUFF;      // Read buffer
    uint32_t SELECT;      // AP bank select
    uint32_t RDATA;       // Read data
    uint32_t WDATA;       // Write data
} dap_registers_t;

// AP (Access Port) selection
#define AP_SELECT_APBANK_SHIFT    4
#define AP_SELECT_APSEL_SHIFT     0
#define AP_SELECT_APBANK_MASK     0xF0
#define AP_SELECT_APSEL_MASK      0x0F
```

### Implementation and Setup

#### Hardware Configuration
```c
// JTAG pin configuration for STM32F4
typedef struct {
    GPIO_TypeDef *port;
    uint16_t tms_pin;     // Test Mode Select
    uint16_t tck_pin;     // Test Clock
    uint16_t tdi_pin;     // Test Data In
    uint16_t tdo_pin;     // Test Data Out
    uint16_t trst_pin;    // Test Reset (optional)
    uint16_t srst_pin;    // System Reset
} jtag_config_t;

jtag_config_t jtag_config = {
    .port = GPIOA,
    .tms_pin = GPIO_Pin_13,
    .tck_pin = GPIO_Pin_14,
    .tdi_pin = GPIO_Pin_15,
    .tdo_pin = GPIO_Pin_3,
    .trst_pin = GPIO_Pin_4,
    .srst_pin = GPIO_Pin_5
};

void jtag_gpio_init(void) {
    GPIO_InitTypeDef gpio_init;
    
    // Enable GPIO clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    // Configure JTAG pins as alternate function
    gpio_init.GPIO_Pin = jtag_config.tms_pin | jtag_config.tck_pin | 
                         jtag_config.tdi_pin | jtag_config.tdo_pin;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(jtag_config.port, &gpio_init);
    
    // Configure TRST and SRST as output
    gpio_init.GPIO_Pin = jtag_config.trst_pin | jtag_config.srst_pin;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_init.GPIO_OType = GPIO_OType_OD;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(jtag_config.port, &gpio_init);
}
```

#### OpenOCD Configuration
```bash
# STM32F4 Discovery board configuration
# interface/stlink.cfg
source [find interface/stlink.cfg]

# target/stm32f4x.cfg
source [find target/stm32f4x.cfg]

# Custom configuration
adapter_khz 2000
transport select swd

# Reset configuration
reset_config srst_only srst_nogate
adapter_nsrst_delay 100
adapter_nsrst_assert_width 100

# Flash configuration
flash bank stm32f4x 0x08000000 0 0 0 stm32f4x.cfg
```

#### GDB Integration
```bash
# GDB initialization script (.gdbinit)
set target-charset ASCII
set target-async on
set target-wide-charset UTF-32

# Connect to OpenOCD
target remote localhost:3333

# Load symbols
symbol-file firmware.elf

# Set breakpoints
break main
break uart_send_byte

# Configure watchpoints
watch *0x20000000
watch *0x20000004

# Start debugging
continue
```

### Advanced Debugging Techniques

#### Conditional Breakpoints
```c
// Example: Break only when specific condition is met
void process_sensor_data(uint8_t sensor_id, uint16_t value) {
    // Set conditional breakpoint: sensor_id == 5 && value > 1000
    if (sensor_id == 5 && value > 1000) {
        // Debug point: Sensor 5 has high value
        debug_high_value(sensor_id, value);
    }
    
    // Normal processing
    store_sensor_data(sensor_id, value);
}

// GDB conditional breakpoint
// (gdb) break process_sensor_data if sensor_id == 5 && value > 1000
```

#### Memory Watchpoints
```c
// Example: Monitor critical variables
typedef struct {
    uint32_t system_state;
    uint32_t error_count;
    uint32_t watchdog_counter;
} system_status_t;

volatile system_status_t g_system_status;

// Set watchpoints in GDB
// (gdb) watch g_system_status.system_state
// (gdb) watch g_system_status.error_count
// (gdb) rwatch g_system_status.watchdog_counter  // Read watch
// (gdb) awatch g_system_status.error_count       // Access watch
```

#### Real-time Debugging
```c
// Example: Non-intrusive monitoring
typedef struct {
    uint32_t timestamp;
    uint8_t event_type;
    uint16_t event_data;
} debug_event_t;

debug_event_t debug_buffer[100];
volatile uint32_t debug_index = 0;

void debug_log_event(uint8_t event_type, uint16_t event_data) {
    if (debug_index < 100) {
        debug_buffer[debug_index].timestamp = xTaskGetTickCount();
        debug_buffer[debug_index].event_type = event_type;
        debug_buffer[debug_index].event_data = event_data;
        debug_index++;
    }
}

// Monitor in GDB without stopping execution
// (gdb) set pagination off
// (gdb) while 1; print debug_index; print debug_buffer[debug_index-1]; sleep 1; end
```

---

## Logic Analyzer Usage

### Core Concepts

#### What is a Logic Analyzer?
A logic analyzer is a digital signal analyzer that captures and displays multiple digital signals simultaneously. It's essential for:

- **Protocol analysis**: Decode I2C, SPI, UART, CAN, and other digital protocols
- **Timing analysis**: Measure signal timing, setup/hold times, and propagation delays
- **State analysis**: Monitor state machines and sequential logic
- **Troubleshooting**: Identify signal integrity issues and protocol violations

#### Sampling Theory
```
Nyquist Sampling Theorem:
- Minimum sampling rate = 2 × highest frequency component
- Practical sampling rate = 4-10× highest frequency for good resolution

Example: 1 MHz SPI clock
- Minimum: 2 MHz
- Recommended: 8-10 MHz
- Buffer size: 1 second × 10 MHz = 10 million samples
```

#### Trigger Mechanisms
```c
// Example: Complex trigger conditions
typedef enum {
    TRIGGER_SIMPLE,      // Single edge
    TRIGGER_PATTERN,     // Pattern match
    TRIGGER_SEQUENCE,    // Sequence of events
    TRIGGER_DURATION,    // Time-based
    TRIGGER_COUNT        // Event count
} trigger_type_t;

typedef struct {
    trigger_type_t type;
    uint32_t channel_mask;
    uint32_t pattern;
    uint32_t duration;
    uint32_t count;
} trigger_config_t;

// SPI trigger: CS low + clock edge + data pattern
trigger_config_t spi_trigger = {
    .type = TRIGGER_SEQUENCE,
    .channel_mask = 0x07,  // CS, CLK, MOSI
    .pattern = 0x05,        // CS low, CLK high, MOSI high
    .duration = 100,        // 100 ns minimum
    .count = 1
};
```

### Implementation Examples

#### I2C Protocol Analysis
```c
// Example: I2C communication with logic analyzer
void i2c_write_register(uint8_t device_addr, uint8_t reg_addr, uint8_t data) {
    // Logic analyzer will capture:
    // 1. Start condition (SDA falling while SCL high)
    // 2. Device address + write bit (7 bits + 1 bit)
    // 3. Register address (8 bits)
    // 4. Data byte (8 bits)
    // 5. Stop condition (SDA rising while SCL high)
    
    i2c_start();
    
    // Send device address (7 bits) + write bit (0)
    uint8_t addr_byte = (device_addr << 1) | 0x00;
    i2c_send_byte(addr_byte);
    
    // Send register address
    i2c_send_byte(reg_addr);
    
    // Send data
    i2c_send_byte(data);
    
    i2c_stop();
}

// Logic analyzer setup for I2C
void setup_i2c_analysis(void) {
    // Channel 0: SCL (clock)
    // Channel 1: SDA (data)
    // Trigger: SDA falling edge while SCL high (start condition)
    // Sampling rate: 1 MHz (for 100 kHz I2C)
    // Buffer size: 1 second capture
}
```

#### SPI Protocol Analysis
```c
// Example: SPI communication with logic analyzer
void spi_transfer_buffer(uint8_t *tx_data, uint8_t *rx_data, uint16_t length) {
    // Logic analyzer will capture:
    // 1. Chip select activation (CS low)
    // 2. Clock signal (SCK)
    // 3. Data on MOSI (master out)
    // 4. Data on MISO (master in)
    // 5. Chip select deactivation (CS high)
    
    cs_low();  // Activate chip select
    
    for (uint16_t i = 0; i < length; i++) {
        // Send data and receive response
        rx_data[i] = spi_transfer_byte(tx_data[i]);
        
        // Logic analyzer shows:
        // - Clock edges and frequency
        // - Data timing (setup/hold times)
        // - Protocol compliance
    }
    
    cs_high();  // Deactivate chip select
}

// Logic analyzer setup for SPI
void setup_spi_analysis(void) {
    // Channel 0: CS (chip select)
    // Channel 1: SCK (clock)
    // Channel 2: MOSI (master out)
    // Channel 3: MISO (master in)
    // Trigger: CS falling edge
    // Sampling rate: 10 MHz (for 1 MHz SPI)
    // Protocol decoder: SPI
}
```

#### Advanced Triggering
```c
// Example: Complex trigger for CAN bus
typedef struct {
    uint32_t can_id;
    uint8_t data_length;
    uint8_t data[8];
} can_message_t;

void can_send_message(can_message_t *msg) {
    // Logic analyzer trigger: CAN ID = 0x123 + data length = 8
    // This helps capture specific CAN messages for analysis
    
    can_transmit_message(msg);
}

// Logic analyzer setup for CAN
void setup_can_analysis(void) {
    // Channel 0: CAN_H (high)
    // Channel 1: CAN_L (low)
    // Channel 2: CAN_RX (receive)
    // Channel 3: CAN_TX (transmit)
    
    // Trigger: CAN ID = 0x123 AND data length = 8
    // Protocol decoder: CAN 2.0A/B
    // Sampling rate: 20 MHz (for 1 Mbps CAN)
}
```

### Data Analysis and Decoding

#### Protocol Decoding
```python
# Python script for logic analyzer data analysis
import numpy as np
import matplotlib.pyplot as plt

def decode_spi_data(csv_file):
    """Decode SPI data from logic analyzer export"""
    data = np.loadtxt(csv_file, delimiter=',', skiprows=1)
    
    # Extract signals
    time = data[:, 0]
    cs = data[:, 1]
    sck = data[:, 2]
    mosi = data[:, 3]
    miso = data[:, 4]
    
    # Find chip select edges
    cs_edges = np.diff(cs)
    cs_falling = np.where(cs_edges == -1)[0]
    cs_rising = np.where(cs_edges == 1)[0]
    
    # Decode SPI transactions
    transactions = []
    for i, cs_fall in enumerate(cs_falling):
        if i < len(cs_rising):
            cs_rise = cs_rising[i]
            
            # Extract data between CS edges
            transaction_data = {
                'start_time': time[cs_fall],
                'end_time': time[cs_rise],
                'mosi_data': [],
                'miso_data': []
            }
            
            # Decode 8-bit transfers
            for j in range(cs_fall, cs_rise, 8):
                if j + 7 < len(mosi):
                    mosi_byte = decode_byte(mosi[j:j+8], sck[j:j+8])
                    miso_byte = decode_byte(miso[j:j+8], sck[j:j+8])
                    
                    transaction_data['mosi_data'].append(mosi_byte)
                    transaction_data['miso_data'].append(miso_byte)
            
            transactions.append(transaction_data)
    
    return transactions

def decode_byte(data, clock):
    """Decode 8-bit byte from data and clock signals"""
    byte = 0
    for i in range(8):
        if clock[i] == 1:  # Sample on rising edge
            byte = (byte << 1) | data[i]
    return byte
```

---

## Oscilloscope Measurements

### Core Concepts

#### What is an Oscilloscope?
An oscilloscope is an analog signal analyzer that displays voltage over time. It's essential for:

- **Signal integrity analysis**: Measure rise/fall times, overshoot, and noise
- **Timing measurements**: Precise time measurements between events
- **Power analysis**: Measure power supply ripple and transient response
- **Analog signal analysis**: Monitor sensor outputs and analog circuits

#### Measurement Fundamentals
```
Key Measurements:
1. Amplitude: Peak-to-peak, RMS, DC offset
2. Time: Period, frequency, rise/fall time, pulse width
3. Phase: Phase difference between signals
4. Statistics: Mean, min/max, standard deviation

Signal Characteristics:
- Rise time: 10% to 90% of final value
- Fall time: 90% to 10% of initial value
- Overshoot: Signal exceeding final value
- Ringing: Oscillations around final value
```

#### Probe Selection and Calibration
```c
// Example: Probe compensation and calibration
typedef struct {
    float attenuation;      // 1x, 10x, 100x
    float bandwidth;        // MHz
    float input_capacitance; // pF
    float input_resistance;  // MΩ
} probe_specs_t;

probe_specs_t probe_10x = {
    .attenuation = 10.0,
    .bandwidth = 100.0,
    .input_capacitance = 15.0,
    .input_resistance = 10.0
};

// Probe compensation procedure
void compensate_probe(void) {
    // 1. Connect probe to calibration output (1 kHz square wave)
    // 2. Adjust probe compensation capacitor
    // 3. Verify square wave has clean edges (no overshoot/ringing)
    // 4. Check both positive and negative edges
}
```

### Implementation Examples

#### GPIO Signal Analysis
```c
// Example: GPIO timing measurement
void gpio_timing_test(void) {
    // Configure GPIO for timing test
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = GPIO_Pin_0;
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NONE;
    GPIO_Init(GPIOA, &gpio_init);
    
    while (1) {
        // Generate test signal for oscilloscope
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        delay_us(100);  // 100 μs high
        
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        delay_us(100);  // 100 μs low
        
        // Oscilloscope measurements:
        // - Frequency: 5 kHz
        // - Duty cycle: 50%
        // - Rise time: < 10 ns
        // - Fall time: < 10 ns
        // - Overshoot: < 5%
    }
}

// Oscilloscope setup for GPIO analysis
void setup_gpio_analysis(void) {
    // Channel 1: GPIO signal
    // Time base: 50 μs/div
    // Voltage scale: 2 V/div
    // Trigger: Rising edge, level = 1.5 V
    // Coupling: DC
    // Bandwidth: 100 MHz
}
```

#### Power Supply Analysis
```c
// Example: Power supply ripple measurement
void measure_power_supply(void) {
    // Connect oscilloscope to power supply output
    // Measure: VCC, GND, and load current sense
    
    // Oscilloscope measurements:
    // 1. DC voltage level: 3.3 V ±5%
    // 2. AC ripple voltage: < 50 mVpp
    // 3. Ripple frequency: Switching frequency
    // 4. Transient response: Load step response
    // 5. Noise floor: < 10 mVpp
    
    // Trigger setup for ripple analysis
    // - Coupling: AC (remove DC component)
    // - Time base: 1 ms/div
    // - Voltage scale: 20 mV/div
    // - Trigger: Auto or normal
}

// Power supply test with load switching
void power_supply_load_test(void) {
    // Switch load to create transients
    GPIO_SetBits(GPIOB, GPIO_Pin_0);  // Enable load 1
    delay_ms(100);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_1);  // Enable load 2
    delay_ms(100);
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_0); // Disable load 1
    delay_ms(100);
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_1); // Disable load 2
    delay_ms(100);
    
    // Oscilloscope shows:
    // - Voltage droop during load increase
    // - Voltage overshoot during load decrease
    // - Recovery time to stable voltage
    // - Ripple increase under load
}
```

#### Communication Signal Analysis
```c
// Example: UART signal analysis
void uart_signal_test(void) {
    // Configure UART for test
    USART_InitTypeDef uart_init;
    uart_init.USART_BaudRate = 115200;
    uart_init.USART_WordLength = USART_WordLength_8b;
    uart_init.USART_StopBits = USART_StopBits_1;
    uart_init.USART_Parity = USART_Parity_No;
    uart_init.USART_Mode = USART_Mode_Tx;
    uart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &uart_init);
    
    // Send test pattern
    uint8_t test_pattern[] = {0x55, 0xAA, 0x00, 0xFF};
    
    while (1) {
        for (int i = 0; i < 4; i++) {
            USART_SendData(USART1, test_pattern[i]);
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        }
        delay_ms(100);
        
        // Oscilloscope measurements:
        // - Baud rate: 115200 bps
        // - Bit time: 8.68 μs
        // - Start bit: 1 bit time low
        // - Data bits: 8 bits
        // - Stop bit: 1 bit time high
        // - Signal levels: 0V (low), 3.3V (high)
    }
}

// Oscilloscope setup for UART analysis
void setup_uart_analysis(void) {
    // Channel 1: UART TX signal
    // Time base: 2 μs/div
    // Voltage scale: 1 V/div
    // Trigger: Falling edge (start bit), level = 1.5 V
    // Coupling: DC
    // Cursor measurements: Bit timing, signal levels
}
```

---

## Tool Integration and Workflow

### Integrated Debugging Environment
```c
// Example: Combined debugging approach
typedef struct {
    jtag_debugger_t jtag;
    logic_analyzer_t logic;
    oscilloscope_t scope;
    debug_host_t host;
} integrated_debug_system_t;

void setup_integrated_debugging(void) {
    // 1. Initialize JTAG/SWD connection
    jtag_init(&debug_system.jtag);
    jtag_connect(&debug_system.jtag);
    
    // 2. Configure logic analyzer
    logic_init(&debug_system.logic);
    logic_set_trigger(&debug_system.logic, SPI_TRIGGER);
    
    // 3. Configure oscilloscope
    scope_init(&debug_system.scope);
    scope_set_trigger(&debug_system.scope, GPIO_TRIGGER);
    
    // 4. Synchronize tools
    sync_debug_tools(&debug_system);
}

// Synchronized debugging workflow
void synchronized_debug_session(void) {
    // 1. Set breakpoint in code
    jtag_set_breakpoint(&debug_system.jtag, main);
    
    // 2. Start logic analyzer capture
    logic_start_capture(&debug_system.logic);
    
    // 3. Start oscilloscope acquisition
    scope_start_acquisition(&debug_system.scope);
    
    // 4. Run code until breakpoint
    jtag_continue(&debug_system.jtag);
    
    // 5. Analyze all captured data
    analyze_debug_data(&debug_system);
}
```

### Automated Testing and Analysis
```python
# Python script for automated debug analysis
import subprocess
import time
import json

class AutomatedDebugger:
    def __init__(self):
        self.openocd_process = None
        self.logic_analyzer = None
        self.oscilloscope = None
    
    def start_debug_session(self, firmware_path):
        """Start integrated debugging session"""
        # Start OpenOCD
        self.openocd_process = subprocess.Popen([
            'openocd', '-f', 'interface/stlink.cfg', 
            '-f', 'target/stm32f4x.cfg'
        ])
        time.sleep(2)
        
        # Start logic analyzer
        self.logic_analyzer.start_capture()
        
        # Start oscilloscope
        self.oscilloscope.start_acquisition()
        
        # Load firmware
        self.load_firmware(firmware_path)
    
    def run_test_scenario(self, test_name):
        """Run specific test scenario"""
        # Set breakpoints
        self.set_breakpoint('test_' + test_name)
        
        # Run until breakpoint
        self.continue_execution()
        
        # Capture data from all tools
        logic_data = self.logic_analyzer.get_capture()
        scope_data = self.oscilloscope.get_acquisition()
        
        # Analyze results
        results = self.analyze_test_results(test_name, logic_data, scope_data)
        
        return results
    
    def analyze_test_results(self, test_name, logic_data, scope_data):
        """Analyze test results from all tools"""
        results = {
            'test_name': test_name,
            'timestamp': time.time(),
            'logic_analysis': self.analyze_logic_data(logic_data),
            'scope_analysis': self.analyze_scope_data(scope_data),
            'pass_fail': self.evaluate_test_criteria(results)
        }
        
        return results
```

---

## Common Pitfalls and Solutions

### JTAG/SWD Issues
```c
// Common JTAG problems and solutions
typedef enum {
    JTAG_ERROR_CONNECTION,     // Connection failed
    JTAG_ERROR_TARGET,         // Target not responding
    JTAG_ERROR_POWER,          // Power supply issues
    JTAG_ERROR_CLOCK,          // Clock configuration
    JTAG_ERROR_RESET           // Reset configuration
} jtag_error_t;

// Connection troubleshooting
void troubleshoot_jtag_connection(void) {
    // 1. Check physical connections
    if (!check_jtag_pins()) {
        printf("ERROR: JTAG pins not connected\n");
        return;
    }
    
    // 2. Verify power supply
    if (!check_target_power()) {
        printf("ERROR: Target not powered\n");
        return;
    }
    
    // 3. Check clock configuration
    if (!check_target_clock()) {
        printf("ERROR: Target clock not running\n");
        return;
    }
    
    // 4. Verify reset configuration
    if (!check_reset_config()) {
        printf("ERROR: Reset configuration incorrect\n");
        return;
    }
}

// Power supply verification
bool check_target_power(void) {
    // Measure VCC, VDD, and VSS
    float vcc = measure_voltage(VCC_PIN);
    float vdd = measure_voltage(VDD_PIN);
    float vss = measure_voltage(VSS_PIN);
    
    // Check voltage levels
    if (vcc < 2.7 || vcc > 3.6) {
        printf("ERROR: VCC out of range: %.2fV\n", vcc);
        return false;
    }
    
    if (vdd < 1.7 || vdd > 2.1) {
        printf("ERROR: VDD out of range: %.2fV\n", vdd);
        return false;
    }
    
    if (vss != 0.0) {
        printf("ERROR: VSS not 0V: %.2fV\n", vss);
        return false;
    }
    
    return true;
}
```

### Logic Analyzer Issues
```c
// Common logic analyzer problems
typedef enum {
    LOGIC_ERROR_TRIGGER,       // Trigger not working
    LOGIC_ERROR_SAMPLING,      // Sampling rate issues
    LOGIC_ERROR_BUFFER,        # Buffer overflow
    LOGIC_ERROR_DECODING,      # Protocol decode errors
    LOGIC_ERROR_TIMING         # Timing measurement errors
} logic_error_t;

// Trigger troubleshooting
void troubleshoot_logic_trigger(void) {
    // 1. Check signal levels
    if (!check_signal_levels()) {
        printf("ERROR: Signal levels incorrect\n");
        return;
    }
    
    // 2. Verify trigger configuration
    if (!check_trigger_config()) {
        printf("ERROR: Trigger configuration incorrect\n");
        return;
    }
    
    // 3. Check sampling rate
    if (!check_sampling_rate()) {
        printf("ERROR: Sampling rate too low\n");
        return;
    }
}

// Signal level verification
bool check_signal_levels(void) {
    // Measure high and low levels
    float vih = measure_voltage(LOGIC_HIGH);
    float vil = measure_voltage(LOGIC_LOW);
    
    // Check against logic analyzer thresholds
    if (vih < LOGIC_ANALYZER_VIH) {
        printf("ERROR: High level too low: %.2fV\n", vih);
        return false;
    }
    
    if (vil > LOGIC_ANALYZER_VIL) {
        printf("ERROR: Low level too high: %.2fV\n", vil);
        return false;
    }
    
    return true;
}
```

### Oscilloscope Issues
```c
// Common oscilloscope problems
typedef enum {
    SCOPE_ERROR_TRIGGER,       // Trigger not working
    SCOPE_ERROR_MEASUREMENT,   # Measurement errors
    SCOPE_ERROR_PROBE,         # Probe issues
    SCOPE_ERROR_BANDWIDTH,     # Bandwidth limitations
    SCOPE_ERROR_NOISE          # Excessive noise
} scope_error_t;

// Probe troubleshooting
void troubleshoot_scope_probe(void) {
    // 1. Check probe compensation
    if (!check_probe_compensation()) {
        printf("ERROR: Probe not compensated\n");
        return;
    }
    
    // 2. Verify probe selection
    if (!check_probe_selection()) {
        printf("ERROR: Wrong probe selected\n");
        return;
    }
    
    // 3. Check ground connection
    if (!check_probe_ground()) {
        printf("ERROR: Probe ground not connected\n");
        return;
    }
}

// Probe compensation check
bool check_probe_compensation(void) {
    // Generate calibration signal
    generate_calibration_signal();
    
    // Measure signal characteristics
    float rise_time = measure_rise_time();
    float overshoot = measure_overshoot();
    float ringing = measure_ringing();
    
    // Check compensation criteria
    if (rise_time > MAX_RISE_TIME) {
        printf("ERROR: Rise time too slow: %.2fns\n", rise_time);
        return false;
    }
    
    if (overshoot > MAX_OVERSHOOT) {
        printf("ERROR: Overshoot too high: %.1f%%\n", overshoot);
        return false;
    }
    
    if (ringing > MAX_RINGING) {
        printf("ERROR: Ringing too high: %.1f%%\n", ringing);
        return false;
    }
    
    return true;
}
```

---

## Best Practices

### Tool Selection and Setup
1. **Choose appropriate tools** for your debugging needs
   - JTAG/SWD: Code debugging and memory inspection
   - Logic analyzer: Digital protocol analysis
   - Oscilloscope: Analog signal and timing analysis

2. **Proper tool configuration** ensures accurate measurements
   - Set correct voltage levels and thresholds
   - Configure appropriate sampling rates
   - Use proper trigger conditions

3. **Tool synchronization** provides correlated data
   - Use common time base when possible
   - Correlate events across different tools
   - Maintain consistent measurement conditions

### Measurement Accuracy
1. **Minimize probe loading** effects
   - Use appropriate probe impedance
   - Keep probe leads short
   - Avoid ground loops

2. **Proper grounding** techniques
   - Use single ground point
   - Minimize ground lead length
   - Avoid ground loops

3. **Signal integrity** considerations
   - Use appropriate bandwidth
   - Minimize noise pickup
   - Proper shielding when needed

### Documentation and Analysis
1. **Record all measurements** for future reference
   - Document tool settings
   - Save captured data
   - Note environmental conditions

2. **Systematic analysis** approach
   - Start with simple measurements
   - Build complexity gradually
   - Correlate multiple measurements

3. **Regular calibration** and maintenance
   - Calibrate tools regularly
   - Check probe compensation
   - Verify measurement accuracy

---

## Interview Questions

### Basic Level
1. **What is the difference between JTAG and SWD?**
   - JTAG: Full-featured debug interface with multiple signals
   - SWD: Simplified 2-wire interface (SWDIO, SWCLK)
   - SWD is ARM-specific, JTAG is industry standard

2. **How do you set up a logic analyzer for SPI analysis?**
   - Connect CS, SCK, MOSI, MISO signals
   - Set appropriate sampling rate (4-10× clock frequency)
   - Configure trigger on CS falling edge
   - Use protocol decoder for automatic analysis

3. **What measurements can you make with an oscilloscope?**
   - Voltage levels (peak, RMS, average)
   - Time measurements (period, frequency, rise/fall time)
   - Phase relationships between signals
   - Power supply characteristics

### Intermediate Level
1. **How do you troubleshoot a JTAG connection that won't establish?**
   - Check physical connections and pin assignments
   - Verify target power supply and clock
   - Check reset configuration
   - Verify debug interface configuration

2. **What is the Nyquist sampling theorem and why is it important?**
   - Minimum sampling rate = 2 × highest frequency
   - Prevents aliasing and ensures accurate reconstruction
   - Practical sampling rate = 4-10× for good resolution

3. **How do you measure setup and hold times with a logic analyzer?**
   - Trigger on clock edge
   - Measure data signal timing relative to clock
   - Use cursor measurements for precise timing
   - Verify against datasheet specifications

### Advanced Level
1. **How do you implement real-time debugging without stopping execution?**
   - Use hardware breakpoints and watchpoints
   - Implement debug event logging
   - Use trace capabilities
   - Monitor critical variables in real-time

2. **What are the trade-offs between different logic analyzer buffer sizes?**
   - Larger buffer: Longer capture time, higher cost
   - Smaller buffer: Faster processing, lower cost
   - Balance based on protocol requirements and analysis needs

3. **How do you correlate measurements between different debug tools?**
   - Use common time base and triggers
   - Implement timestamp correlation
   - Use external synchronization signals
   - Post-process data for alignment

### Practical Scenarios
1. **Debug a UART communication issue using available tools**
   - Use oscilloscope to verify signal levels and timing
   - Use logic analyzer to decode protocol and identify errors
   - Use JTAG to inspect UART registers and variables
   - Correlate findings across all tools

2. **Optimize SPI communication timing for maximum speed**
   - Measure current timing with oscilloscope
   - Identify bottlenecks and optimize accordingly
   - Verify signal integrity at higher speeds
   - Test reliability under various conditions

3. **Troubleshoot a power supply stability issue**
   - Use oscilloscope to measure ripple and transients
   - Use logic analyzer to correlate with system activity
   - Identify load patterns causing instability
   - Implement and verify solutions
