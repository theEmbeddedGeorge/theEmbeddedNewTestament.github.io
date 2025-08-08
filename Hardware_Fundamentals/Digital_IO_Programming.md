# 🔌 Digital I/O Programming

> **Mastering Digital Input/Output Operations for Embedded Systems**  
> Reading switches, driving LEDs, keypad scanning, and digital signal processing

## 📋 Table of Contents

- [🎯 Overview](#-overview)
- [🔌 Basic Digital I/O Operations](#-basic-digital-io-operations)
- [🔘 Switch Reading Techniques](#-switch-reading-techniques)
- [💡 LED Control Patterns](#-led-control-patterns)
- [⌨️ Keypad Scanning](#️-keypad-scanning)
- [🔢 Seven-Segment Display Control](#-seven-segment-display-control)
- [🔄 State Machine Implementation](#-state-machine-implementation)
- [⚡ Performance Optimization](#-performance-optimization)
- [🎯 Common Applications](#-common-applications)
- [⚠️ Common Pitfalls](#️-common-pitfalls)
- [✅ Best Practices](#-best-practices)
- [🎯 Interview Questions](#-interview-questions)
- [📚 Additional Resources](#-additional-resources)

---

## 🎯 Overview

Digital I/O programming is the foundation of embedded system interaction with the physical world. It involves reading digital inputs (switches, sensors) and controlling digital outputs (LEDs, relays, displays).

**Key Concepts:**
- **Input Reading**: Debouncing, edge detection, state machines
- **Output Control**: PWM, patterns, timing control
- **Interface Design**: Keypads, displays, multiplexing
- **Performance**: Optimization, real-time constraints

---

## 🔌 Basic Digital I/O Operations

### **Reading Digital Input**
```c
// Basic digital input reading
uint8_t read_digital_input(GPIO_TypeDef* GPIOx, uint16_t pin) {
    return (GPIOx->IDR >> pin) & 0x01;
}

// Reading multiple inputs at once
uint16_t read_multiple_inputs(GPIO_TypeDef* GPIOx, uint16_t mask) {
    return GPIOx->IDR & mask;
}
```

### **Writing Digital Output**
```c
// Basic digital output writing
void write_digital_output(GPIO_TypeDef* GPIOx, uint16_t pin, uint8_t state) {
    if (state) {
        GPIOx->BSRR = (1U << pin);  // Set bit
    } else {
        GPIOx->BSRR = (1U << (pin + 16));  // Reset bit
    }
}

// Writing multiple outputs at once
void write_multiple_outputs(GPIO_TypeDef* GPIOx, uint16_t mask, uint16_t state) {
    GPIOx->BSRR = (state & mask) | ((~state & mask) << 16);
}
```

### **Toggle Output**
```c
// Toggle digital output
void toggle_output(GPIO_TypeDef* GPIOx, uint16_t pin) {
    GPIOx->ODR ^= (1U << pin);
}

// Toggle multiple outputs
void toggle_multiple_outputs(GPIO_TypeDef* GPIOx, uint16_t mask) {
    GPIOx->ODR ^= mask;
}
```

---

## 🔘 Switch Reading Techniques

### **Simple Switch Reading**
```c
// Basic switch reading (no debouncing)
uint8_t read_switch_simple(GPIO_TypeDef* GPIOx, uint16_t pin) {
    return !read_digital_input(GPIOx, pin);  // Inverted for pull-up
}
```

### **Debounced Switch Reading**
```c
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t pin;
    uint8_t last_state;
    uint8_t current_state;
    uint32_t debounce_time;
    uint32_t last_change_time;
} DebouncedSwitch_t;

void switch_init(DebouncedSwitch_t* sw, GPIO_TypeDef* GPIOx, uint16_t pin, uint32_t debounce_ms) {
    sw->GPIOx = GPIOx;
    sw->pin = pin;
    sw->debounce_time = debounce_ms;
    sw->last_state = 0;
    sw->current_state = 0;
    sw->last_change_time = 0;
    
    // Configure as input with pull-up
    gpio_input_pullup_config(GPIOx, pin);
}

uint8_t read_switch_debounced(DebouncedSwitch_t* sw) {
    uint8_t raw_state = !read_digital_input(sw->GPIOx, sw->pin);
    uint32_t current_time = HAL_GetTick();
    
    if (raw_state != sw->last_state) {
        if (current_time - sw->last_change_time > sw->debounce_time) {
            sw->current_state = raw_state;
            sw->last_state = raw_state;
            sw->last_change_time = current_time;
        }
    }
    
    return sw->current_state;
}
```

### **Edge Detection**
```c
typedef enum {
    EDGE_NONE = 0,
    EDGE_RISING = 1,
    EDGE_FALLING = 2,
    EDGE_BOTH = 3
} EdgeType_t;

typedef struct {
    DebouncedSwitch_t switch_data;
    EdgeType_t edge_type;
    uint8_t last_stable_state;
} EdgeDetector_t;

void edge_detector_init(EdgeDetector_t* detector, GPIO_TypeDef* GPIOx, uint16_t pin, 
                       EdgeType_t edge_type, uint32_t debounce_ms) {
    switch_init(&detector->switch_data, GPIOx, pin, debounce_ms);
    detector->edge_type = edge_type;
    detector->last_stable_state = 0;
}

uint8_t detect_edge(EdgeDetector_t* detector) {
    uint8_t current_state = read_switch_debounced(&detector->switch_data);
    uint8_t edge_detected = 0;
    
    if (current_state != detector->last_stable_state) {
        if (detector->edge_type == EDGE_RISING && current_state == 1) {
            edge_detected = 1;
        } else if (detector->edge_type == EDGE_FALLING && current_state == 0) {
            edge_detected = 1;
        } else if (detector->edge_type == EDGE_BOTH) {
            edge_detected = 1;
        }
        
        detector->last_stable_state = current_state;
    }
    
    return edge_detected;
}
```

---

## 💡 LED Control Patterns

### **Basic LED Control**
```c
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t pin;
    uint8_t state;
} LED_t;

void led_init(LED_t* led, GPIO_TypeDef* GPIOx, uint16_t pin) {
    led->GPIOx = GPIOx;
    led->pin = pin;
    led->state = 0;
    
    gpio_pushpull_output_config(GPIOx, pin);
    write_digital_output(GPIOx, pin, 0);
}

void led_on(LED_t* led) {
    write_digital_output(led->GPIOx, led->pin, 1);
    led->state = 1;
}

void led_off(LED_t* led) {
    write_digital_output(led->GPIOx, led->pin, 0);
    led->state = 0;
}

void led_toggle(LED_t* led) {
    led->state = !led->state;
    write_digital_output(led->GPIOx, led->pin, led->state);
}
```

### **LED Patterns**
```c
typedef struct {
    LED_t* leds;
    uint8_t count;
    uint32_t pattern;
    uint8_t position;
} LEDPattern_t;

void led_pattern_init(LEDPattern_t* pattern, LED_t* leds, uint8_t count) {
    pattern->leds = leds;
    pattern->count = count;
    pattern->pattern = 0;
    pattern->position = 0;
}

void led_pattern_set(LEDPattern_t* pattern, uint32_t new_pattern) {
    pattern->pattern = new_pattern;
    pattern->position = 0;
}

void led_pattern_step(LEDPattern_t* pattern) {
    for (int i = 0; i < pattern->count; i++) {
        uint8_t bit = (pattern->pattern >> ((pattern->position + i) % pattern->count)) & 0x01;
        if (bit) {
            led_on(&pattern->leds[i]);
        } else {
            led_off(&pattern->leds[i]);
        }
    }
    pattern->position = (pattern->position + 1) % pattern->count;
}

// Example patterns
#define PATTERN_KNIGHT_RIDER 0x0F0F0F0F
#define PATTERN_BINARY_COUNT 0x00000000  // Will be incremented
#define PATTERN_ALTERNATING 0x55555555
```

### **LED Blinking with Timer**
```c
typedef struct {
    LED_t led;
    uint32_t blink_period;
    uint32_t last_toggle_time;
    uint8_t blinking;
} BlinkingLED_t;

void blinking_led_init(BlinkingLED_t* bled, GPIO_TypeDef* GPIOx, uint16_t pin, uint32_t period_ms) {
    led_init(&bled->led, GPIOx, pin);
    bled->blink_period = period_ms;
    bled->last_toggle_time = 0;
    bled->blinking = 0;
}

void blinking_led_start(BlinkingLED_t* bled) {
    bled->blinking = 1;
    bled->last_toggle_time = HAL_GetTick();
}

void blinking_led_stop(BlinkingLED_t* bled) {
    bled->blinking = 0;
    led_off(&bled->led);
}

void blinking_led_update(BlinkingLED_t* bled) {
    if (bled->blinking) {
        uint32_t current_time = HAL_GetTick();
        if (current_time - bled->last_toggle_time >= bled->blink_period) {
            led_toggle(&bled->led);
            bled->last_toggle_time = current_time;
        }
    }
}
```

---

## ⌨️ Keypad Scanning

### **Matrix Keypad Structure**
```c
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t* row_pins;
    uint16_t* col_pins;
    uint8_t rows;
    uint8_t cols;
    uint8_t last_key;
    uint8_t key_pressed;
} MatrixKeypad_t;

void keypad_init(MatrixKeypad_t* keypad, GPIO_TypeDef* GPIOx, 
                 uint16_t* row_pins, uint16_t* col_pins, 
                 uint8_t rows, uint8_t cols) {
    keypad->GPIOx = GPIOx;
    keypad->row_pins = row_pins;
    keypad->col_pins = col_pins;
    keypad->rows = rows;
    keypad->cols = cols;
    keypad->last_key = 0xFF;
    keypad->key_pressed = 0;
    
    // Configure row pins as outputs
    for (int i = 0; i < rows; i++) {
        gpio_pushpull_output_config(GPIOx, row_pins[i]);
        write_digital_output(GPIOx, row_pins[i], 1);
    }
    
    // Configure column pins as inputs with pull-up
    for (int i = 0; i < cols; i++) {
        gpio_input_pullup_config(GPIOx, col_pins[i]);
    }
}
```

### **Keypad Scanning Algorithm**
```c
uint8_t keypad_scan(MatrixKeypad_t* keypad) {
    uint8_t key = 0xFF;
    
    // Scan each row
    for (int row = 0; row < keypad->rows; row++) {
        // Set current row low
        write_digital_output(keypad->GPIOx, keypad->row_pins[row], 0);
        
        // Check each column
        for (int col = 0; col < keypad->cols; col++) {
            if (!read_digital_input(keypad->GPIOx, keypad->col_pins[col])) {
                key = row * keypad->cols + col;
                break;
            }
        }
        
        // Set row back to high
        write_digital_output(keypad->GPIOx, keypad->row_pins[row], 1);
        
        if (key != 0xFF) break;
    }
    
    return key;
}

uint8_t keypad_get_key(MatrixKeypad_t* keypad) {
    uint8_t current_key = keypad_scan(keypad);
    uint8_t key = 0xFF;
    
    if (current_key != 0xFF && current_key != keypad->last_key) {
        key = current_key;
        keypad->key_pressed = 1;
    } else if (current_key == 0xFF) {
        keypad->key_pressed = 0;
    }
    
    keypad->last_key = current_key;
    return key;
}
```

### **Keypad with Debouncing**
```c
typedef struct {
    MatrixKeypad_t keypad;
    uint32_t debounce_time;
    uint32_t last_scan_time;
} DebouncedKeypad_t;

void debounced_keypad_init(DebouncedKeypad_t* dkeypad, GPIO_TypeDef* GPIOx,
                          uint16_t* row_pins, uint16_t* col_pins,
                          uint8_t rows, uint8_t cols, uint32_t debounce_ms) {
    keypad_init(&dkeypad->keypad, GPIOx, row_pins, col_pins, rows, cols);
    dkeypad->debounce_time = debounce_ms;
    dkeypad->last_scan_time = 0;
}

uint8_t debounced_keypad_get_key(DebouncedKeypad_t* dkeypad) {
    uint32_t current_time = HAL_GetTick();
    
    if (current_time - dkeypad->last_scan_time >= dkeypad->debounce_time) {
        dkeypad->last_scan_time = current_time;
        return keypad_get_key(&dkeypad->keypad);
    }
    
    return 0xFF;
}
```

---

## 🔢 Seven-Segment Display Control

### **Seven-Segment Display Structure**
```c
// Seven-segment display segment mapping
const uint8_t SEGMENT_PATTERNS[16] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71  // F
};

typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t* segment_pins;
    uint16_t* digit_pins;
    uint8_t segments;
    uint8_t digits;
    uint8_t current_digit;
} SevenSegmentDisplay_t;

void seven_segment_init(SevenSegmentDisplay_t* display, GPIO_TypeDef* GPIOx,
                       uint16_t* segment_pins, uint16_t* digit_pins,
                       uint8_t segments, uint8_t digits) {
    display->GPIOx = GPIOx;
    display->segment_pins = segment_pins;
    display->digit_pins = digit_pins;
    display->segments = segments;
    display->digits = digits;
    display->current_digit = 0;
    
    // Configure segment pins as outputs
    for (int i = 0; i < segments; i++) {
        gpio_pushpull_output_config(GPIOx, segment_pins[i]);
        write_digital_output(GPIOx, segment_pins[i], 0);
    }
    
    // Configure digit pins as outputs
    for (int i = 0; i < digits; i++) {
        gpio_pushpull_output_config(GPIOx, digit_pins[i]);
        write_digital_output(GPIOx, digit_pins[i], 1); // Common cathode
    }
}
```

### **Display Control Functions**
```c
void seven_segment_set_digit(SevenSegmentDisplay_t* display, uint8_t digit, uint8_t value) {
    if (digit < display->digits && value < 16) {
        // Turn off all digits
        for (int i = 0; i < display->digits; i++) {
            write_digital_output(display->GPIOx, display->digit_pins[i], 1);
        }
        
        // Set segments for current digit
        uint8_t pattern = SEGMENT_PATTERNS[value];
        for (int i = 0; i < display->segments; i++) {
            uint8_t segment_state = (pattern >> i) & 0x01;
            write_digital_output(display->GPIOx, display->segment_pins[i], segment_state);
        }
        
        // Turn on current digit
        write_digital_output(display->GPIOx, display->digit_pins[digit], 0);
    }
}

void seven_segment_display_number(SevenSegmentDisplay_t* display, uint16_t number) {
    uint8_t digits[4];
    uint8_t digit_count = 0;
    
    // Extract digits
    if (number == 0) {
        digits[0] = 0;
        digit_count = 1;
    } else {
        while (number > 0 && digit_count < display->digits) {
            digits[digit_count++] = number % 10;
            number /= 10;
        }
    }
    
    // Display digits
    for (int i = 0; i < digit_count; i++) {
        seven_segment_set_digit(display, i, digits[digit_count - 1 - i]);
    }
}

void seven_segment_multiplex_update(SevenSegmentDisplay_t* display, uint8_t* values) {
    // Turn off all digits
    for (int i = 0; i < display->digits; i++) {
        write_digital_output(display->GPIOx, display->digit_pins[i], 1);
    }
    
    // Set segments for current digit
    uint8_t pattern = SEGMENT_PATTERNS[values[display->current_digit]];
    for (int i = 0; i < display->segments; i++) {
        uint8_t segment_state = (pattern >> i) & 0x01;
        write_digital_output(display->GPIOx, display->segment_pins[i], segment_state);
    }
    
    // Turn on current digit
    write_digital_output(display->GPIOx, display->digit_pins[display->current_digit], 0);
    
    // Move to next digit
    display->current_digit = (display->current_digit + 1) % display->digits;
}
```

---

## 🔄 State Machine Implementation

### **Simple State Machine**
```c
typedef enum {
    STATE_IDLE,
    STATE_DEBOUNCE,
    STATE_PRESSED,
    STATE_RELEASE
} ButtonState_t;

typedef struct {
    DebouncedSwitch_t switch_data;
    ButtonState_t state;
    uint32_t state_timer;
    uint8_t button_pressed;
} ButtonStateMachine_t;

void button_state_machine_init(ButtonStateMachine_t* bsm, GPIO_TypeDef* GPIOx, uint16_t pin) {
    switch_init(&bsm->switch_data, GPIOx, pin, 50);
    bsm->state = STATE_IDLE;
    bsm->state_timer = 0;
    bsm->button_pressed = 0;
}

void button_state_machine_update(ButtonStateMachine_t* bsm) {
    uint8_t raw_input = !read_digital_input(bsm->switch_data.GPIOx, bsm->switch_data.pin);
    uint32_t current_time = HAL_GetTick();
    
    switch (bsm->state) {
        case STATE_IDLE:
            if (raw_input) {
                bsm->state = STATE_DEBOUNCE;
                bsm->state_timer = current_time;
            }
            break;
            
        case STATE_DEBOUNCE:
            if (current_time - bsm->state_timer > 50) {
                if (raw_input) {
                    bsm->state = STATE_PRESSED;
                    bsm->button_pressed = 1;
                } else {
                    bsm->state = STATE_IDLE;
                }
            }
            break;
            
        case STATE_PRESSED:
            if (!raw_input) {
                bsm->state = STATE_RELEASE;
                bsm->state_timer = current_time;
            }
            break;
            
        case STATE_RELEASE:
            if (current_time - bsm->state_timer > 50) {
                bsm->state = STATE_IDLE;
                bsm->button_pressed = 0;
            }
            break;
    }
}
```

---

## ⚡ Performance Optimization

### **Bulk Operations**
```c
// Read multiple inputs efficiently
uint16_t read_port(GPIO_TypeDef* GPIOx) {
    return GPIOx->IDR;
}

// Write multiple outputs efficiently
void write_port(GPIO_TypeDef* GPIOx, uint16_t data) {
    GPIOx->ODR = data;
}

// Atomic operations for critical sections
void atomic_write_output(GPIO_TypeDef* GPIOx, uint16_t pin, uint8_t state) {
    __disable_irq();
    if (state) {
        GPIOx->BSRR = (1U << pin);
    } else {
        GPIOx->BSRR = (1U << (pin + 16));
    }
    __enable_irq();
}
```

### **Interrupt-Driven I/O**
```c
// Configure GPIO interrupt
void gpio_interrupt_config(GPIO_TypeDef* GPIOx, uint16_t pin, uint8_t trigger) {
    // Configure as input
    gpio_set_mode(GPIOx, pin, GPIO_MODE_INPUT);
    
    // Configure interrupt trigger
    if (trigger & 0x01) { // Rising edge
        EXTI->RTSR |= (1U << pin);
    }
    if (trigger & 0x02) { // Falling edge
        EXTI->FTSR |= (1U << pin);
    }
    
    // Enable interrupt
    EXTI->IMR |= (1U << pin);
    
    // Configure NVIC
    NVIC_EnableIRQ(EXTI0_IRQn + pin);
}

// GPIO interrupt handler
void EXTI0_IRQHandler(void) {
    if (EXTI->PR & (1U << 0)) {
        EXTI->PR = (1U << 0); // Clear interrupt flag
        
        // Handle button press
        button_pressed_callback();
    }
}
```

---

## 🎯 Common Applications

### **Digital Input Monitoring**
```c
// Monitor multiple digital inputs
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t input_mask;
    uint16_t last_state;
    uint16_t state_changes;
} DigitalInputMonitor_t;

void input_monitor_init(DigitalInputMonitor_t* monitor, GPIO_TypeDef* GPIOx, uint16_t mask) {
    monitor->GPIOx = GPIOx;
    monitor->input_mask = mask;
    monitor->last_state = read_port(GPIOx) & mask;
    monitor->state_changes = 0;
}

uint16_t input_monitor_update(DigitalInputMonitor_t* monitor) {
    uint16_t current_state = read_port(monitor->GPIOx) & monitor->input_mask;
    uint16_t changes = current_state ^ monitor->last_state;
    
    monitor->state_changes |= changes;
    monitor->last_state = current_state;
    
    return changes;
}
```

### **Digital Output Sequencer**
```c
// Sequence digital outputs
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t* output_pins;
    uint8_t pin_count;
    uint32_t* sequence;
    uint8_t sequence_length;
    uint8_t current_step;
    uint32_t step_duration;
    uint32_t last_step_time;
} OutputSequencer_t;

void sequencer_init(OutputSequencer_t* seq, GPIO_TypeDef* GPIOx,
                   uint16_t* pins, uint8_t count,
                   uint32_t* sequence, uint8_t length,
                   uint32_t duration_ms) {
    seq->GPIOx = GPIOx;
    seq->output_pins = pins;
    seq->pin_count = count;
    seq->sequence = sequence;
    seq->sequence_length = length;
    seq->current_step = 0;
    seq->step_duration = duration_ms;
    seq->last_step_time = 0;
    
    // Configure pins as outputs
    for (int i = 0; i < count; i++) {
        gpio_pushpull_output_config(GPIOx, pins[i]);
    }
}

void sequencer_update(OutputSequencer_t* seq) {
    uint32_t current_time = HAL_GetTick();
    
    if (current_time - seq->last_step_time >= seq->step_duration) {
        // Set outputs for current step
        uint32_t step_data = seq->sequence[seq->current_step];
        for (int i = 0; i < seq->pin_count; i++) {
            uint8_t state = (step_data >> i) & 0x01;
            write_digital_output(seq->GPIOx, seq->output_pins[i], state);
        }
        
        // Move to next step
        seq->current_step = (seq->current_step + 1) % seq->sequence_length;
        seq->last_step_time = current_time;
    }
}
```

---

## ⚠️ Common Pitfalls

### **1. Missing Debouncing**
```c
// ❌ Wrong: No debouncing
uint8_t read_button_wrong(GPIO_TypeDef* GPIOx, uint16_t pin) {
    return !read_digital_input(GPIOx, pin);
}

// ✅ Correct: With debouncing
uint8_t read_button_correct(DebouncedSwitch_t* sw) {
    return read_switch_debounced(sw);
}
```

### **2. Race Conditions**
```c
// ❌ Wrong: Race condition possible
void toggle_led_wrong(GPIO_TypeDef* GPIOx, uint16_t pin) {
    uint8_t state = read_digital_input(GPIOx, pin);
    write_digital_output(GPIOx, pin, !state);
}

// ✅ Correct: Atomic operation
void toggle_led_correct(GPIO_TypeDef* GPIOx, uint16_t pin) {
    GPIOx->ODR ^= (1U << pin);
}
```

### **3. Incorrect Pull-up/Pull-down**
```c
// ❌ Wrong: Floating input
void configure_input_wrong(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_set_mode(GPIOx, pin, GPIO_MODE_INPUT);
    // Missing pull-up/pull-down
}

// ✅ Correct: Proper pull-up/pull-down
void configure_input_correct(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_set_mode(GPIOx, pin, GPIO_MODE_INPUT);
    gpio_set_pullup_pulldown(GPIOx, pin, GPIO_PUPD_UP);
}
```

---

## ✅ Best Practices

### **1. Use Constants for Pin Definitions**
```c
// Define pin constants
#define LED_RED_PIN      GPIO_PIN_0
#define LED_GREEN_PIN    GPIO_PIN_1
#define LED_BLUE_PIN     GPIO_PIN_2
#define BUTTON_PIN       GPIO_PIN_3

// Use in code
void gpio_init_example(void) {
    led_init(&red_led, GPIOA, LED_RED_PIN);
    led_init(&green_led, GPIOA, LED_GREEN_PIN);
    led_init(&blue_led, GPIOA, LED_BLUE_PIN);
    switch_init(&button, GPIOA, BUTTON_PIN, 50);
}
```

### **2. Group Related Operations**
```c
// Group related GPIO operations
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t pins[8];
    uint8_t count;
} GPIOGroup_t;

void gpio_group_write(GPIOGroup_t* group, uint8_t pattern) {
    for (int i = 0; i < group->count; i++) {
        uint8_t state = (pattern >> i) & 0x01;
        write_digital_output(group->GPIOx, group->pins[i], state);
    }
}
```

### **3. Use Interrupts for Critical Inputs**
```c
// Use interrupts for critical inputs
void critical_input_init(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_input_pullup_config(GPIOx, pin);
    gpio_interrupt_config(GPIOx, pin, 0x03); // Both edges
}
```

---

## 🎯 Interview Questions

### **Basic Questions**
1. **What is debouncing and why is it important?**
   - Mechanical switches bounce, causing multiple transitions

2. **What is the difference between active-high and active-low signals?**
   - Active-high: 1 = active, 0 = inactive; Active-low: 0 = active, 1 = inactive

3. **How do you read a matrix keypad?**
   - Scan rows sequentially, check columns for pressed keys

### **Advanced Questions**
1. **Design a debouncing circuit for a mechanical switch**
   - Use RC filter, Schmitt trigger, or software debouncing

2. **Implement a state machine for button handling**
   - States: idle, debounce, pressed, release

3. **How do you optimize GPIO performance?**
   - Use bulk operations, interrupts, atomic operations

### **Practical Questions**
1. **Create a LED pattern controller**
   - Use arrays, timers, state machines

2. **Implement a keypad scanner with debouncing**
   - Matrix scanning, debouncing, state tracking

3. **Design a seven-segment display driver**
   - Segment mapping, multiplexing, timing

---

## 📚 Additional Resources

### **Documentation**
- [STM32 GPIO Programming Guide](https://www.st.com/resource/en/user_manual/dm00031936-stm32f0xx-peripherals-stm32f0xx-reference-manual-stmicroelectronics.pdf)
- [ARM Cortex-M GPIO Reference](https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/peripherals/general-purpose-input-output-gpio)

### **Tools**
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) - GPIO configuration
- [GPIO Calculator](https://www.st.com/resource/en/user_manual/dm00104712-stm32cubemx-user-manual-stmicroelectronics.pdf)

### **Related Topics**
- **[GPIO Configuration](./GPIO_Configuration.md)** - GPIO modes, configuration, electrical characteristics
- **[External Interrupts](./External_Interrupts.md)** - Edge/level triggered interrupts, debouncing
- **[Timer/Counter Programming](./Timer_Counter_Programming.md)** - Input capture, output compare, frequency measurement

---

**Next Topic:** [Analog I/O](./Analog_IO.md) → [Pulse Width Modulation](./Pulse_Width_Modulation.md)
