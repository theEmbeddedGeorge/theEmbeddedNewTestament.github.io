# Embedded Debugging Guide

## Overview
Debugging embedded systems requires a unique set of tools and techniques compared to desktop applications. This guide covers the essential debugging methods, tools, and strategies used by embedded software engineers.

## Table of Contents
1. [JTAG Debugging](#jtag-debugging)
2. [Logic Analyzer Usage](#logic-analyzer-usage)
3. [Oscilloscope Measurements](#oscilloscope-measurements)
4. [Code Coverage and Static Analysis](#code-coverage-and-static-analysis)
5. [Unit Testing for Embedded Systems](#unit-testing-for-embedded-systems)
6. [Hardware-in-the-Loop Testing](#hardware-in-the-loop-testing)
7. [Performance Profiling](#performance-profiling)

---

## JTAG Debugging

### What is JTAG?
JTAG (Joint Test Action Group) is a standard interface for debugging embedded systems. It allows you to:
- Set breakpoints in code
- Inspect and modify registers and memory
- Step through code execution
- Download firmware to target devices

### JTAG Debugging Setup

#### Hardware Requirements
- JTAG debugger (e.g., J-Link, ST-Link, OpenOCD)
- Target board with JTAG interface
- Debug cables and connectors

#### Software Setup
```bash
# Install OpenOCD (open-source JTAG debugger)
sudo apt-get install openocd

# Install GDB (GNU debugger)
sudo apt-get install gdb-multiarch
```

#### Basic JTAG Commands
```bash
# Start OpenOCD server
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg

# Connect GDB to OpenOCD
gdb-multiarch
(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) load firmware.elf
(gdb) continue
```

### Common JTAG Debugging Scenarios

#### 1. Setting Breakpoints
```c
// Example: Debugging a UART communication issue
void uart_send_byte(uint8_t data) {
    // Set breakpoint here to inspect data
    while (!(UART->SR & UART_SR_TXE));  // Wait for TX empty
    UART->DR = data;  // Send data
}
```

#### 2. Inspecting Registers
```bash
(gdb) info registers
(gdb) print/x $r0
(gdb) print/x $pc
(gdb) print/x $sp
```

#### 3. Memory Inspection
```bash
(gdb) x/16x 0x20000000  # Examine 16 bytes at address
(gdb) print *((int*)0x20000000)  # Print as integer
```

### JTAG Debugging Best Practices
1. **Use hardware breakpoints sparingly** - they're limited resources
2. **Set breakpoints at function entry/exit** for better control
3. **Use conditional breakpoints** to avoid stopping on every iteration
4. **Monitor stack usage** to prevent stack overflow
5. **Use watchpoints** for monitoring variable changes

---

## Logic Analyzer Usage

### What is a Logic Analyzer?
A logic analyzer is a tool for capturing and analyzing digital signals. It's essential for debugging:
- Communication protocols (I2C, SPI, UART)
- Timing issues
- Signal integrity problems
- Protocol violations

### Logic Analyzer Setup

#### Hardware Setup
1. Connect logic analyzer to target signals
2. Set proper voltage levels and thresholds
3. Configure trigger conditions
4. Set sampling rate and buffer size

#### Software Configuration
```python
# Example: Using Saleae Logic Analyzer with Python
import saleae

# Connect to logic analyzer
analyzer = saleae.Saleae()

# Configure channels
analyzer.set_active_channels([0, 1, 2, 3])

# Set trigger
analyzer.set_trigger_one_channel(0, saleae.Trigger.RISING_EDGE)

# Start capture
analyzer.capture_start()
analyzer.capture_wait_until_finished()

# Export data
analyzer.export_data2('capture.csv')
```

### Common Logic Analyzer Scenarios

#### 1. I2C Protocol Analysis
```c
// Example: Debugging I2C communication
void i2c_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data) {
    // Logic analyzer will show:
    // 1. Start condition
    // 2. Device address + write bit
    // 3. Register address
    // 4. Data byte
    // 5. Stop condition
    
    i2c_start();
    i2c_write_byte(device_addr << 1);
    i2c_write_byte(reg_addr);
    i2c_write_byte(data);
    i2c_stop();
}
```

#### 2. SPI Protocol Analysis
```c
// Example: Debugging SPI communication
void spi_transfer(uint8_t *data, uint16_t length) {
    // Logic analyzer will show:
    // 1. Chip select activation
    // 2. Clock signal
    // 3. Data on MOSI/MISO
    // 4. Chip select deactivation
    
    cs_low();
    for (int i = 0; i < length; i++) {
        data[i] = spi_transfer_byte(data[i]);
    }
    cs_high();
}
```

### Logic Analyzer Best Practices
1. **Set appropriate sampling rate** - at least 4x the signal frequency
2. **Use proper trigger conditions** to capture relevant events
3. **Monitor multiple signals simultaneously** for protocol analysis
4. **Save captures** for later analysis and documentation
5. **Use protocol decoders** for automatic analysis

---

## Oscilloscope Measurements

### What is an Oscilloscope?
An oscilloscope is a tool for measuring and analyzing analog and digital signals. It's essential for:
- Signal integrity analysis
- Timing measurements
- Power supply analysis
- Noise and interference detection

### Oscilloscope Setup

#### Basic Measurements
```c
// Example: Measuring GPIO signal timing
void gpio_toggle_test(void) {
    // Oscilloscope will show:
    // 1. Signal rise time
    // 2. Signal fall time
    // 3. Frequency
    // 4. Duty cycle
    
    while (1) {
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        delay_ms(100);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        delay_ms(100);
    }
}
```

#### Power Supply Analysis
```c
// Example: Measuring power supply ripple
void measure_power_supply(void) {
    // Oscilloscope measurements:
    // 1. DC voltage level
    // 2. AC ripple voltage
    // 3. Ripple frequency
    // 4. Transient response
    
    // Connect oscilloscope to power supply output
    // Set AC coupling for ripple measurement
    // Use appropriate voltage and time scales
}
```

### Common Oscilloscope Measurements

#### 1. Signal Timing
- **Rise time**: Time for signal to go from 10% to 90% of final value
- **Fall time**: Time for signal to go from 90% to 10% of initial value
- **Pulse width**: Duration of pulse at 50% amplitude
- **Frequency**: Number of cycles per second

#### 2. Signal Integrity
- **Overshoot**: Signal exceeding final value
- **Undershoot**: Signal going below final value
- **Ringback**: Oscillations around final value
- **Noise**: Random variations in signal

#### 3. Power Analysis
- **DC voltage**: Average voltage level
- **AC ripple**: Variations around DC level
- **Transient response**: Response to load changes
- **Efficiency**: Power output vs. power input

### Oscilloscope Best Practices
1. **Use appropriate probe** for signal type and frequency
2. **Set proper trigger conditions** for stable display
3. **Use appropriate time and voltage scales**
4. **Measure multiple parameters** for comprehensive analysis
5. **Document measurements** for future reference

---

## Code Coverage and Static Analysis

### Code Coverage

#### What is Code Coverage?
Code coverage measures how much of your code is executed during testing. It helps identify:
- Untested code paths
- Dead code
- Missing test cases
- Code quality issues

#### Code Coverage Tools
```bash
# Using gcov for code coverage
gcc -fprofile-arcs -ftest-coverage -o program program.c
./program
gcov program.c

# Using lcov for HTML reports
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_report
```

#### Example: Code Coverage Analysis
```c
// Example: Testing with code coverage
int calculate_average(int *array, int size) {
    if (array == NULL || size <= 0) {
        return -1;  // Error condition
    }
    
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    
    return sum / size;
}

// Test cases for 100% coverage
void test_calculate_average(void) {
    int array1[] = {1, 2, 3, 4, 5};
    int array2[] = {0};
    int *array3 = NULL;
    
    assert(calculate_average(array1, 5) == 3);
    assert(calculate_average(array2, 1) == 0);
    assert(calculate_average(array3, 5) == -1);
    assert(calculate_average(array1, 0) == -1);
}
```

### Static Analysis

#### What is Static Analysis?
Static analysis examines code without executing it to find:
- Potential bugs
- Code quality issues
- Security vulnerabilities
- Performance problems

#### Static Analysis Tools
```bash
# Using cppcheck for static analysis
cppcheck --enable=all --xml --xml-version=2 . 2> report.xml

# Using clang-tidy for additional checks
clang-tidy --checks=* source_file.c

# Using splint for additional checks
splint source_file.c
```

#### Example: Static Analysis Results
```c
// Example: Code with potential issues
void process_data(int *data, int size) {
    int sum = 0;
    
    // Potential issue: No bounds checking
    for (int i = 0; i < size; i++) {
        sum += data[i];  // Could cause buffer overflow
    }
    
    // Potential issue: Unused variable
    int unused_var = 42;
    
    return sum;
}

// Improved version
void process_data(int *data, int size) {
    if (data == NULL || size <= 0) {
        return -1;
    }
    
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    
    return sum;
}
```

---

## Unit Testing for Embedded Systems

### Unit Testing Framework

#### Setting Up Unit Testing
```c
// Example: Simple unit testing framework
#include <assert.h>
#include <stdio.h>

#define TEST_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s:%d - %s\n", __FILE__, __LINE__, #condition); \
            return -1; \
        } \
    } while(0)

#define TEST_RUN(test_func) \
    do { \
        printf("Running %s...\n", #test_func); \
        if (test_func() == 0) { \
            printf("PASS: %s\n", #test_func); \
        } else { \
            printf("FAIL: %s\n", #test_func); \
        } \
    } while(0)
```

#### Example: Unit Tests
```c
// Example: Testing ring buffer implementation
int test_ring_buffer_empty(void) {
    ring_buffer_t buffer;
    ring_buffer_init(&buffer);
    
    TEST_ASSERT(ring_buffer_is_empty(&buffer) == true);
    TEST_ASSERT(ring_buffer_is_full(&buffer) == false);
    
    return 0;
}

int test_ring_buffer_full(void) {
    ring_buffer_t buffer;
    ring_buffer_init(&buffer);
    
    // Fill buffer
    for (int i = 0; i < RING_BUFFER_SIZE; i++) {
        TEST_ASSERT(ring_buffer_push(&buffer, i) == 0);
    }
    
    TEST_ASSERT(ring_buffer_is_full(&buffer) == true);
    TEST_ASSERT(ring_buffer_push(&buffer, 100) == -1);  // Should fail
    
    return 0;
}

int test_ring_buffer_wrap_around(void) {
    ring_buffer_t buffer;
    ring_buffer_init(&buffer);
    
    // Fill and empty buffer to test wrap-around
    for (int i = 0; i < RING_BUFFER_SIZE; i++) {
        ring_buffer_push(&buffer, i);
    }
    
    for (int i = 0; i < RING_BUFFER_SIZE; i++) {
        int value;
        TEST_ASSERT(ring_buffer_pop(&buffer, &value) == 0);
        TEST_ASSERT(value == i);
    }
    
    return 0;
}

// Run all tests
int main(void) {
    TEST_RUN(test_ring_buffer_empty);
    TEST_RUN(test_ring_buffer_full);
    TEST_RUN(test_ring_buffer_wrap_around);
    
    printf("All tests completed!\n");
    return 0;
}
```

### Mock Objects for Hardware

#### Example: Mock UART
```c
// Mock UART for testing
typedef struct {
    uint8_t tx_buffer[256];
    uint8_t rx_buffer[256];
    int tx_index;
    int rx_index;
    int tx_count;
    int rx_count;
} mock_uart_t;

static mock_uart_t mock_uart;

void mock_uart_init(void) {
    memset(&mock_uart, 0, sizeof(mock_uart_t));
}

int mock_uart_send(uint8_t data) {
    if (mock_uart.tx_count < 256) {
        mock_uart.tx_buffer[mock_uart.tx_count++] = data;
        return 0;
    }
    return -1;
}

int mock_uart_receive(uint8_t *data) {
    if (mock_uart.rx_index < mock_uart.rx_count) {
        *data = mock_uart.rx_buffer[mock_uart.rx_index++];
        return 0;
    }
    return -1;
}

// Test UART communication
int test_uart_communication(void) {
    mock_uart_init();
    
    // Test sending data
    TEST_ASSERT(mock_uart_send(0x55) == 0);
    TEST_ASSERT(mock_uart.tx_buffer[0] == 0x55);
    TEST_ASSERT(mock_uart.tx_count == 1);
    
    // Test receiving data
    mock_uart.rx_buffer[0] = 0xAA;
    mock_uart.rx_count = 1;
    
    uint8_t received_data;
    TEST_ASSERT(mock_uart_receive(&received_data) == 0);
    TEST_ASSERT(received_data == 0xAA);
    
    return 0;
}
```

---

## Hardware-in-the-Loop Testing

### HIL Testing Setup

#### What is HIL Testing?
Hardware-in-the-Loop (HIL) testing involves testing embedded software with simulated hardware. It allows:
- Testing without physical hardware
- Automated testing
- Repeatable test scenarios
- Cost-effective testing

#### HIL Testing Framework
```c
// Example: HIL testing framework
typedef struct {
    uint32_t gpio_state;
    uint32_t timer_value;
    uint8_t uart_rx_data;
    uint8_t uart_tx_data;
    uint32_t adc_value;
} hil_environment_t;

static hil_environment_t hil_env;

void hil_init(void) {
    memset(&hil_env, 0, sizeof(hil_environment_t));
}

// Mock hardware functions
uint32_t hil_gpio_read(uint32_t pin) {
    return (hil_env.gpio_state >> pin) & 0x01;
}

void hil_gpio_write(uint32_t pin, uint32_t value) {
    if (value) {
        hil_env.gpio_state |= (1 << pin);
    } else {
        hil_env.gpio_state &= ~(1 << pin);
    }
}

uint32_t hil_timer_get_value(void) {
    return hil_env.timer_value;
}

void hil_timer_set_value(uint32_t value) {
    hil_env.timer_value = value;
}
```

#### Example: HIL Test
```c
// Example: Testing GPIO functionality
int test_gpio_functionality(void) {
    hil_init();
    
    // Test GPIO write
    hil_gpio_write(5, 1);
    TEST_ASSERT(hil_gpio_read(5) == 1);
    
    // Test GPIO read
    hil_gpio_write(3, 1);
    TEST_ASSERT(hil_gpio_read(3) == 1);
    
    // Test GPIO clear
    hil_gpio_write(5, 0);
    TEST_ASSERT(hil_gpio_read(5) == 0);
    
    return 0;
}

// Example: Testing timer functionality
int test_timer_functionality(void) {
    hil_init();
    
    // Test timer value
    hil_timer_set_value(1000);
    TEST_ASSERT(hil_timer_get_value() == 1000);
    
    // Test timer increment
    hil_timer_set_value(hil_timer_get_value() + 100);
    TEST_ASSERT(hil_timer_get_value() == 1100);
    
    return 0;
}
```

---

## Performance Profiling

### Performance Profiling Tools

#### Using GProf
```bash
# Compile with profiling enabled
gcc -pg -o program program.c

# Run program
./program

# Generate profiling report
gprof program gmon.out > profile.txt
```

#### Using Perf
```bash
# Profile CPU usage
perf record ./program
perf report

# Profile specific events
perf record -e cache-misses ./program
perf report
```

#### Example: Performance Analysis
```c
// Example: Performance-critical function
void performance_critical_function(void) {
    // Profile this function for optimization
    for (int i = 0; i < 1000000; i++) {
        // Expensive operation
        complex_calculation(i);
    }
}

// Optimized version
void optimized_function(void) {
    // Use lookup table or algorithm optimization
    for (int i = 0; i < 1000000; i++) {
        // Optimized operation
        optimized_calculation(i);
    }
}
```

### Memory Profiling

#### Using Valgrind
```bash
# Memory leak detection
valgrind --leak-check=full ./program

# Memory usage profiling
valgrind --tool=massif ./program
ms_print massif.out.* > memory_profile.txt
```

#### Example: Memory Analysis
```c
// Example: Memory leak detection
void memory_leak_example(void) {
    // This will cause a memory leak
    int *data = malloc(1000 * sizeof(int));
    // Missing free(data);
}

// Corrected version
void correct_memory_usage(void) {
    int *data = malloc(1000 * sizeof(int));
    if (data != NULL) {
        // Use data
        process_data(data, 1000);
        free(data);  // Proper cleanup
    }
}
```

---

## Debugging Best Practices

### General Debugging Tips
1. **Start with the simplest explanation** - often the issue is basic
2. **Use systematic approach** - eliminate possibilities one by one
3. **Document everything** - keep notes of what you've tried
4. **Use version control** - revert to known good state if needed
5. **Test incrementally** - add features one at a time

### Debugging Checklist
- [ ] Check power supply and voltage levels
- [ ] Verify clock configuration and timing
- [ ] Confirm communication protocol settings
- [ ] Validate memory allocation and usage
- [ ] Check interrupt configuration and priorities
- [ ] Verify peripheral initialization
- [ ] Test with known good reference design

### Common Debugging Mistakes
1. **Assuming hardware is working** - always verify hardware first
2. **Not checking datasheets** - specifications are crucial
3. **Ignoring timing issues** - embedded systems are timing-sensitive
4. **Overlooking memory constraints** - embedded systems have limited resources
5. **Not considering real-time constraints** - timing is critical

---

## Resources

### Tools and Software
- [OpenOCD](http://openocd.org/) - Open-source JTAG debugger
- [GDB](https://www.gnu.org/software/gdb/) - GNU debugger
- [Saleae Logic](https://www.saleae.com/) - Logic analyzer software
- [Sigrok](https://sigrok.org/) - Open-source signal analysis
- [Valgrind](http://valgrind.org/) - Memory profiling tool

### Books and References
- "Debugging Embedded Microprocessor Systems" by Stuart Ball
- "The Art of Debugging" by Norman Matloff
- "Embedded Systems Design" by Arnold Berger

### Online Resources
- [Embedded.com Debugging Section](https://www.embedded.com/category/debugging/)
- [Stack Overflow Embedded Tag](https://stackoverflow.com/questions/tagged/embedded)
- [EEVblog](https://www.eevblog.com/) - Electronics engineering videos
