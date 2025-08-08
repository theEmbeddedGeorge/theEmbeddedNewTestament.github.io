# 🔌 GPIO Configuration

> **Mastering General Purpose Input/Output for Embedded Systems**  
> Understanding GPIO modes, configuration, and practical applications

## 📋 Table of Contents

- [🎯 Overview](#-overview)
- [🔧 GPIO Modes](#-gpio-modes)
- [⚙️ Configuration Registers](#️-configuration-registers)
- [🔌 Input Configuration](#-input-configuration)
- [💡 Output Configuration](#-output-configuration)
- [🔄 Alternate Function Configuration](#-alternate-function-configuration)
- [⚡ Drive Strength and Slew Rate](#-drive-strength-and-slew-rate)
- [🔒 Pull-up/Pull-down Resistors](#-pull-uppull-down-resistors)
- [🎯 Common Applications](#-common-applications)
- [⚠️ Common Pitfalls](#️-common-pitfalls)
- [✅ Best Practices](#-best-practices)
- [🎯 Interview Questions](#-interview-questions)
- [📚 Additional Resources](#-additional-resources)

---

## 🎯 Overview

GPIO (General Purpose Input/Output) is the foundation of embedded system I/O. Understanding GPIO configuration is essential for interfacing with external devices, sensors, and actuators.

**Key Concepts:**
- **Input/Output Modes**: Digital input, digital output, analog input, alternate function
- **Configuration Registers**: Mode, type, speed, pull-up/pull-down
- **Electrical Characteristics**: Drive strength, slew rate, voltage levels
- **Interrupt Capability**: Edge/level triggered interrupts

---

## 🔧 GPIO Modes

### **Digital Input Mode**
```c
// Configure GPIO as digital input
void gpio_input_config(GPIO_TypeDef* GPIOx, uint16_t pin) {
    // Clear mode bits (00 = Input mode)
    GPIOx->MODER &= ~(3U << (pin * 2));
    
    // Configure as no pull-up/pull-down
    GPIOx->PUPDR &= ~(3U << (pin * 2));
}

// Read digital input
uint8_t gpio_read_input(GPIO_TypeDef* GPIOx, uint16_t pin) {
    return (GPIOx->IDR >> pin) & 0x01;
}
```

### **Digital Output Mode**
```c
// Configure GPIO as digital output
void gpio_output_config(GPIO_TypeDef* GPIOx, uint16_t pin) {
    // Set mode bits (01 = Output mode)
    GPIOx->MODER &= ~(3U << (pin * 2));
    GPIOx->MODER |= (1U << (pin * 2));
    
    // Configure as push-pull
    GPIOx->OTYPER &= ~(1U << pin);
    
    // Configure speed (11 = Very high speed)
    GPIOx->OSPEEDR &= ~(3U << (pin * 2));
    GPIOx->OSPEEDR |= (3U << (pin * 2));
}

// Write digital output
void gpio_write_output(GPIO_TypeDef* GPIOx, uint16_t pin, uint8_t state) {
    if (state) {
        GPIOx->BSRR = (1U << pin);  // Set bit
    } else {
        GPIOx->BSRR = (1U << (pin + 16));  // Reset bit
    }
}
```

### **Alternate Function Mode**
```c
// Configure GPIO for alternate function
void gpio_alternate_config(GPIO_TypeDef* GPIOx, uint16_t pin, uint8_t alternate) {
    // Set mode bits (10 = Alternate function mode)
    GPIOx->MODER &= ~(3U << (pin * 2));
    GPIOx->MODER |= (2U << (pin * 2));
    
    // Configure alternate function
    if (pin < 8) {
        GPIOx->AFR[0] &= ~(0xFU << (pin * 4));
        GPIOx->AFR[0] |= (alternate << (pin * 4));
    } else {
        GPIOx->AFR[1] &= ~(0xFU << ((pin - 8) * 4));
        GPIOx->AFR[1] |= (alternate << ((pin - 8) * 4));
    }
}
```

---

## ⚙️ Configuration Registers

### **Mode Register (MODER)**
```c
typedef enum {
    GPIO_MODE_INPUT     = 0x00,  // Input mode
    GPIO_MODE_OUTPUT    = 0x01,  // General purpose output mode
    GPIO_MODE_ALTERNATE = 0x02,  // Alternate function mode
    GPIO_MODE_ANALOG    = 0x03   // Analog mode
} GPIO_Mode_t;

// Configure GPIO mode
void gpio_set_mode(GPIO_TypeDef* GPIOx, uint16_t pin, GPIO_Mode_t mode) {
    GPIOx->MODER &= ~(3U << (pin * 2));
    GPIOx->MODER |= (mode << (pin * 2));
}
```

### **Output Type Register (OTYPER)**
```c
typedef enum {
    GPIO_OTYPE_PUSHPULL  = 0x00,  // Push-pull output
    GPIO_OTYPE_OPENDRAIN = 0x01   // Open-drain output
} GPIO_OType_t;

// Configure output type
void gpio_set_output_type(GPIO_TypeDef* GPIOx, uint16_t pin, GPIO_OType_t type) {
    if (type == GPIO_OTYPE_OPENDRAIN) {
        GPIOx->OTYPER |= (1U << pin);
    } else {
        GPIOx->OTYPER &= ~(1U << pin);
    }
}
```

### **Output Speed Register (OSPEEDR)**
```c
typedef enum {
    GPIO_SPEED_LOW      = 0x00,  // Low speed
    GPIO_SPEED_MEDIUM   = 0x01,  // Medium speed
    GPIO_SPEED_HIGH     = 0x02,  // High speed
    GPIO_SPEED_VERYHIGH = 0x03   // Very high speed
} GPIO_Speed_t;

// Configure output speed
void gpio_set_speed(GPIO_TypeDef* GPIOx, uint16_t pin, GPIO_Speed_t speed) {
    GPIOx->OSPEEDR &= ~(3U << (pin * 2));
    GPIOx->OSPEEDR |= (speed << (pin * 2));
}
```

---

## 🔌 Input Configuration

### **Pull-up/Pull-down Configuration**
```c
typedef enum {
    GPIO_PUPD_NONE = 0x00,  // No pull-up/pull-down
    GPIO_PUPD_UP   = 0x01,  // Pull-up
    GPIO_PUPD_DOWN = 0x02   // Pull-down
} GPIO_PuPd_t;

// Configure pull-up/pull-down
void gpio_set_pullup_pulldown(GPIO_TypeDef* GPIOx, uint16_t pin, GPIO_PuPd_t pupd) {
    GPIOx->PUPDR &= ~(3U << (pin * 2));
    GPIOx->PUPDR |= (pupd << (pin * 2));
}
```

### **Input with Internal Pull-up**
```c
// Configure input with internal pull-up
void gpio_input_pullup_config(GPIO_TypeDef* GPIOx, uint16_t pin) {
    // Set as input
    gpio_set_mode(GPIOx, pin, GPIO_MODE_INPUT);
    
    // Enable pull-up
    gpio_set_pullup_pulldown(GPIOx, pin, GPIO_PUPD_UP);
}
```

### **Input with External Pull-down**
```c
// Configure input with external pull-down (for buttons)
void gpio_button_config(GPIO_TypeDef* GPIOx, uint16_t pin) {
    // Set as input
    gpio_set_mode(GPIOx, pin, GPIO_MODE_INPUT);
    
    // No internal pull-up/pull-down (external resistor used)
    gpio_set_pullup_pulldown(GPIOx, pin, GPIO_PUPD_NONE);
}
```

---

## 💡 Output Configuration

### **Push-Pull Output**
```c
// Configure push-pull output
void gpio_pushpull_output_config(GPIO_TypeDef* GPIOx, uint16_t pin) {
    // Set as output
    gpio_set_mode(GPIOx, pin, GPIO_MODE_OUTPUT);
    
    // Configure as push-pull
    gpio_set_output_type(GPIOx, pin, GPIO_OTYPE_PUSHPULL);
    
    // Set speed
    gpio_set_speed(GPIOx, pin, GPIO_SPEED_HIGH);
}
```

### **Open-Drain Output**
```c
// Configure open-drain output (for I2C, etc.)
void gpio_opendrain_output_config(GPIO_TypeDef* GPIOx, uint16_t pin) {
    // Set as output
    gpio_set_mode(GPIOx, pin, GPIO_MODE_OUTPUT);
    
    // Configure as open-drain
    gpio_set_output_type(GPIOx, pin, GPIO_OTYPE_OPENDRAIN);
    
    // Set speed
    gpio_set_speed(GPIOx, pin, GPIO_SPEED_HIGH);
}
```

### **LED Control Example**
```c
// LED control functions
void led_init(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_pushpull_output_config(GPIOx, pin);
    gpio_write_output(GPIOx, pin, 0);  // Turn off initially
}

void led_on(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_write_output(GPIOx, pin, 1);
}

void led_off(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_write_output(GPIOx, pin, 0);
}

void led_toggle(GPIO_TypeDef* GPIOx, uint16_t pin) {
    uint8_t current_state = gpio_read_input(GPIOx, pin);
    gpio_write_output(GPIOx, pin, !current_state);
}
```

---

## 🔄 Alternate Function Configuration

### **UART Configuration**
```c
// Configure GPIO for UART
void gpio_uart_config(GPIO_TypeDef* GPIOx, uint16_t tx_pin, uint16_t rx_pin) {
    // Configure TX pin as alternate function
    gpio_alternate_config(GPIOx, tx_pin, GPIO_AF7_USART1);
    
    // Configure RX pin as alternate function
    gpio_alternate_config(GPIOx, rx_pin, GPIO_AF7_USART1);
    
    // Configure pull-up for RX
    gpio_set_pullup_pulldown(GPIOx, rx_pin, GPIO_PUPD_UP);
}
```

### **SPI Configuration**
```c
// Configure GPIO for SPI
void gpio_spi_config(GPIO_TypeDef* GPIOx, uint16_t sck_pin, uint16_t miso_pin, 
                     uint16_t mosi_pin, uint16_t nss_pin) {
    // Configure SCK as alternate function
    gpio_alternate_config(GPIOx, sck_pin, GPIO_AF5_SPI1);
    
    // Configure MISO as alternate function
    gpio_alternate_config(GPIOx, miso_pin, GPIO_AF5_SPI1);
    
    // Configure MOSI as alternate function
    gpio_alternate_config(GPIOx, mosi_pin, GPIO_AF5_SPI1);
    
    // Configure NSS as alternate function
    gpio_alternate_config(GPIOx, nss_pin, GPIO_AF5_SPI1);
}
```

---

## ⚡ Drive Strength and Slew Rate

### **Drive Strength Configuration**
```c
// Configure drive strength for different loads
void gpio_configure_drive_strength(GPIO_TypeDef* GPIOx, uint16_t pin, uint8_t load_type) {
    switch (load_type) {
        case LOAD_LIGHT:  // LED, small load
            gpio_set_speed(GPIOx, pin, GPIO_SPEED_LOW);
            break;
        case LOAD_MEDIUM: // Relay, medium load
            gpio_set_speed(GPIOx, pin, GPIO_SPEED_MEDIUM);
            break;
        case LOAD_HEAVY:  // Motor, heavy load
            gpio_set_speed(GPIOx, pin, GPIO_SPEED_HIGH);
            break;
        case LOAD_HIGH_FREQ: // High frequency signal
            gpio_set_speed(GPIOx, pin, GPIO_SPEED_VERYHIGH);
            break;
    }
}
```

### **Slew Rate Control**
```c
// Control slew rate for signal integrity
void gpio_configure_slew_rate(GPIO_TypeDef* GPIOx, uint16_t pin, uint8_t slew_rate) {
    // Lower speed = slower slew rate = less EMI
    // Higher speed = faster slew rate = better signal integrity
    
    if (slew_rate == SLEW_SLOW) {
        gpio_set_speed(GPIOx, pin, GPIO_SPEED_LOW);
    } else if (slew_rate == SLEW_FAST) {
        gpio_set_speed(GPIOx, pin, GPIO_SPEED_HIGH);
    }
}
```

---

## 🔒 Pull-up/Pull-down Resistors

### **Internal vs External Pull-ups**
```c
// Internal pull-up (good for most applications)
void gpio_internal_pullup_config(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_set_pullup_pulldown(GPIOx, pin, GPIO_PUPD_UP);
}

// External pull-up (for high current or special requirements)
void gpio_external_pullup_config(GPIO_TypeDef* GPIOx, uint16_t pin) {
    // No internal pull-up/pull-down
    gpio_set_pullup_pulldown(GPIOx, pin, GPIO_PUPD_NONE);
    // External resistor must be connected
}
```

### **Pull-down for Active-High Signals**
```c
// Configure pull-down for active-high signals
void gpio_pulldown_config(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_set_pullup_pulldown(GPIOx, pin, GPIO_PUPD_DOWN);
}
```

---

## 🎯 Common Applications

### **Button Interface**
```c
// Button with debouncing
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t pin;
    uint8_t last_state;
    uint32_t debounce_time;
    uint32_t last_press_time;
} Button_t;

void button_init(Button_t* button, GPIO_TypeDef* GPIOx, uint16_t pin) {
    button->GPIOx = GPIOx;
    button->pin = pin;
    button->last_state = 0;
    button->debounce_time = 50; // 50ms debounce
    button->last_press_time = 0;
    
    // Configure as input with pull-up
    gpio_input_pullup_config(GPIOx, pin);
}

uint8_t button_is_pressed(Button_t* button) {
    uint8_t current_state = !gpio_read_input(button->GPIOx, button->pin); // Inverted for pull-up
    uint32_t current_time = HAL_GetTick();
    
    if (current_state != button->last_state) {
        if (current_time - button->last_press_time > button->debounce_time) {
            button->last_state = current_state;
            button->last_press_time = current_time;
            return current_state;
        }
    }
    
    return 0;
}
```

### **LED Matrix Control**
```c
// LED matrix control
void led_matrix_init(GPIO_TypeDef* GPIOx, uint16_t* row_pins, uint16_t* col_pins, 
                     uint8_t rows, uint8_t cols) {
    // Configure row pins as outputs
    for (int i = 0; i < rows; i++) {
        gpio_pushpull_output_config(GPIOx, row_pins[i]);
        gpio_write_output(GPIOx, row_pins[i], 0);
    }
    
    // Configure column pins as outputs
    for (int i = 0; i < cols; i++) {
        gpio_pushpull_output_config(GPIOx, col_pins[i]);
        gpio_write_output(GPIOx, col_pins[i], 1);
    }
}

void led_matrix_set_pixel(GPIO_TypeDef* GPIOx, uint16_t* row_pins, uint16_t* col_pins,
                         uint8_t row, uint8_t col, uint8_t state) {
    if (state) {
        gpio_write_output(GPIOx, row_pins[row], 1);
        gpio_write_output(GPIOx, col_pins[col], 0);
    } else {
        gpio_write_output(GPIOx, row_pins[row], 0);
        gpio_write_output(GPIOx, col_pins[col], 1);
    }
}
```

---

## ⚠️ Common Pitfalls

### **1. Floating Inputs**
```c
// ❌ Wrong: Floating input
void gpio_input_config_wrong(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_set_mode(GPIOx, pin, GPIO_MODE_INPUT);
    // Missing pull-up/pull-down configuration
}

// ✅ Correct: Input with pull-up/pull-down
void gpio_input_config_correct(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_set_mode(GPIOx, pin, GPIO_MODE_INPUT);
    gpio_set_pullup_pulldown(GPIOx, pin, GPIO_PUPD_UP);
}
```

### **2. Incorrect Alternate Function**
```c
// ❌ Wrong: Wrong alternate function
void gpio_uart_config_wrong(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_alternate_config(GPIOx, pin, 0x01); // Wrong AF number
}

// ✅ Correct: Correct alternate function
void gpio_uart_config_correct(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_alternate_config(GPIOx, pin, GPIO_AF7_USART1); // Correct AF number
}
```

### **3. Missing Speed Configuration**
```c
// ❌ Wrong: Missing speed configuration
void gpio_output_config_wrong(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_set_mode(GPIOx, pin, GPIO_MODE_OUTPUT);
    // Missing speed configuration
}

// ✅ Correct: Complete output configuration
void gpio_output_config_correct(GPIO_TypeDef* GPIOx, uint16_t pin) {
    gpio_set_mode(GPIOx, pin, GPIO_MODE_OUTPUT);
    gpio_set_speed(GPIOx, pin, GPIO_SPEED_HIGH);
}
```

---

## ✅ Best Practices

### **1. Always Configure Pull-up/Pull-down**
```c
// Always configure pull-up/pull-down for inputs
void gpio_safe_input_config(GPIO_TypeDef* GPIOx, uint16_t pin, GPIO_PuPd_t pupd) {
    gpio_set_mode(GPIOx, pin, GPIO_MODE_INPUT);
    gpio_set_pullup_pulldown(GPIOx, pin, pupd);
}
```

### **2. Use Constants for Pin Numbers**
```c
// Define pin constants
#define LED_PIN         GPIO_PIN_5
#define BUTTON_PIN      GPIO_PIN_0
#define UART_TX_PIN     GPIO_PIN_9
#define UART_RX_PIN     GPIO_PIN_10

// Use constants in code
void gpio_init_example(void) {
    gpio_pushpull_output_config(GPIOA, LED_PIN);
    gpio_input_pullup_config(GPIOA, BUTTON_PIN);
}
```

### **3. Group Related GPIO Operations**
```c
// Group related GPIO operations
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t pins[8];
    uint8_t count;
} GPIO_Group_t;

void gpio_group_init(GPIO_Group_t* group, GPIO_TypeDef* GPIOx, uint16_t* pins, uint8_t count) {
    group->GPIOx = GPIOx;
    group->count = count;
    
    for (int i = 0; i < count; i++) {
        group->pins[i] = pins[i];
        gpio_pushpull_output_config(GPIOx, pins[i]);
    }
}
```

---

## 🎯 Interview Questions

### **Basic Questions**
1. **What are the different GPIO modes?**
   - Digital input, digital output, analog input, alternate function

2. **What is the difference between push-pull and open-drain outputs?**
   - Push-pull: Can drive high and low, open-drain: Can only drive low

3. **When would you use internal vs external pull-up resistors?**
   - Internal: Most applications, external: High current or special requirements

### **Advanced Questions**
1. **How do you configure GPIO for UART communication?**
   - Set mode to alternate function, configure correct AF number, set pull-up for RX

2. **What is slew rate and why is it important?**
   - Rate of voltage change, affects signal integrity and EMI

3. **How do you handle floating inputs?**
   - Always configure pull-up/pull-down resistors

### **Practical Questions**
1. **Design a button interface with debouncing**
   - Use timer or software debouncing, check state changes

2. **Implement LED control with PWM**
   - Configure GPIO for alternate function, use timer PWM

3. **Create a keypad scanning routine**
   - Configure rows as outputs, columns as inputs, scan sequentially

---

## 📚 Additional Resources

### **Documentation**
- [STM32 GPIO Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [ARM Cortex-M GPIO Programming](https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/peripherals/general-purpose-input-output-gpio)

### **Tools**
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) - GPIO configuration tool
- [GPIO Calculator](https://www.st.com/resource/en/user_manual/dm00104712-stm32cubemx-user-manual-stmicroelectronics.pdf)

### **Related Topics**
- **[Digital I/O Programming](./Digital_IO_Programming.md)** - Reading switches, driving LEDs, keypad scanning
- **[External Interrupts](./External_Interrupts.md)** - Edge/level triggered interrupts, debouncing
- **[Timer/Counter Programming](./Timer_Counter_Programming.md)** - Input capture, output compare, frequency measurement

---

**Next Topic:** [Digital I/O Programming](./Digital_IO_Programming.md) → [Analog I/O](./Analog_IO.md)
