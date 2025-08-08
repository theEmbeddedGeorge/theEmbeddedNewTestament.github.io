# Bit Manipulation for Embedded Systems

> **Essential bit operations and manipulation techniques for embedded C programming**

## 📋 **Table of Contents**
- [Overview](#overview)
- [Bitwise Operators](#bitwise-operators)
- [Bit Fields](#bit-fields)
- [Bit Masks](#bit-masks)
- [Bit Shifting](#bit-shifting)
- [Hardware Register Operations](#hardware-register-operations)
- [Common Bit Tricks](#common-bit-tricks)
- [Performance Considerations](#performance-considerations)
- [Common Pitfalls](#common-pitfalls)
- [Best Practices](#best-practices)
- [Interview Questions](#interview-questions)

---

## 🎯 **Overview**

Bit manipulation is crucial in embedded systems for:
- **Hardware register access** - Setting/clearing individual bits
- **Memory efficiency** - Packing multiple values into single variables
- **Performance optimization** - Fast bit-level operations
- **Protocol implementation** - Parsing binary data formats
- **Flag management** - Efficient boolean state tracking

### **Key Concepts**
- **Bitwise operators** - AND, OR, XOR, NOT, shift operations
- **Bit fields** - Named bit positions in structures
- **Bit masks** - Patterns for setting/clearing specific bits
- **Endianness** - Byte order considerations
- **Bit counting** - Counting set bits efficiently

---

## 🔧 **Bitwise Operators**

### **Basic Operators**

#### **AND (&) Operator**
```c
// Bitwise AND - result bit is 1 only if both operands are 1
uint8_t a = 0b10101010;  // 170
uint8_t b = 0b11001100;  // 204
uint8_t result = a & b;   // 0b10001000 = 136

// Common uses
uint8_t mask_lower_nibble = 0x0F;  // 0b00001111
uint8_t value = 0xAB;              // 0b10101011
uint8_t lower_nibble = value & mask_lower_nibble;  // 0x0B
```

#### **OR (|) Operator**
```c
// Bitwise OR - result bit is 1 if either operand is 1
uint8_t a = 0b10101010;  // 170
uint8_t b = 0b11001100;  // 204
uint8_t result = a | b;   // 0b11101110 = 238

// Common uses
uint8_t set_bit_3 = 0x08;  // 0b00001000
uint8_t value = 0x01;      // 0b00000001
uint8_t result = value | set_bit_3;  // 0b00001001
```

#### **XOR (^) Operator**
```c
// Bitwise XOR - result bit is 1 if operands are different
uint8_t a = 0b10101010;  // 170
uint8_t b = 0b11001100;  // 204
uint8_t result = a ^ b;   // 0b01100110 = 102

// Common uses
uint8_t toggle_bit_2 = 0x04;  // 0b00000100
uint8_t value = 0x01;         // 0b00000001
uint8_t result = value ^ toggle_bit_2;  // 0b00000101
```

#### **NOT (~) Operator**
```c
// Bitwise NOT - inverts all bits
uint8_t a = 0b10101010;  // 170
uint8_t result = ~a;      // 0b01010101 = 85

// Common uses
uint8_t mask = 0x0F;      // 0b00001111
uint8_t inverted_mask = ~mask;  // 0b11110000
```

### **Shift Operators**

#### **Left Shift (<<)**
```c
// Left shift - moves bits left, fills with zeros
uint8_t value = 0b00000001;  // 1
uint8_t result = value << 3;  // 0b00001000 = 8

// Common uses
uint8_t create_mask(uint8_t bit_position) {
    return 1 << bit_position;  // Creates mask for specific bit
}

uint8_t mask_bit_5 = 1 << 5;  // 0b00100000
```

#### **Right Shift (>>)**
```c
// Right shift - moves bits right, fills with zeros (for unsigned)
uint8_t value = 0b10000000;  // 128
uint8_t result = value >> 3;  // 0b00010000 = 16

// Common uses
uint8_t get_bit_value(uint8_t value, uint8_t bit_position) {
    return (value >> bit_position) & 1;  // Extract specific bit
}
```

---

## 🏗️ **Bit Fields**

### **Structure Bit Fields**

#### **Basic Bit Fields**
```c
// Bit field structure
typedef struct {
    uint8_t flag1 : 1;    // 1 bit
    uint8_t flag2 : 1;    // 1 bit
    uint8_t mode  : 2;    // 2 bits (0-3)
    uint8_t count : 4;    // 4 bits (0-15)
} status_flags_t;

// Usage
status_flags_t status = {0};
status.flag1 = 1;     // Set flag1
status.mode = 2;      // Set mode to 2
status.count = 10;    // Set count to 10

// Access
if (status.flag1) {
    // Flag1 is set
}
```

#### **Hardware Register Bit Fields**
```c
// GPIO register bit fields
typedef struct {
    uint32_t pin0  : 1;   // GPIO pin 0
    uint32_t pin1  : 1;   // GPIO pin 1
    uint32_t pin2  : 1;   // GPIO pin 2
    uint32_t pin3  : 1;   // GPIO pin 3
    uint32_t pin4  : 1;   // GPIO pin 4
    uint32_t pin5  : 1;   // GPIO pin 5
    uint32_t pin6  : 1;   // GPIO pin 6
    uint32_t pin7  : 1;   // GPIO pin 7
    uint32_t reserved : 24; // Reserved bits
} gpio_data_t;

// Usage
volatile gpio_data_t* const GPIO_DATA = (gpio_data_t*)0x40020000;

void set_led_on(void) {
    GPIO_DATA->pin13 = 1;  // Set LED pin high
}

void set_led_off(void) {
    GPIO_DATA->pin13 = 0;  // Set LED pin low
}
```

#### **Protocol Bit Fields**
```c
// CAN message ID bit fields
typedef struct {
    uint32_t priority : 3;    // Priority (0-7)
    uint32_t source   : 8;    // Source ID (0-255)
    uint32_t type     : 5;    // Message type (0-31)
    uint32_t reserved : 16;   // Reserved bits
} can_id_t;

// Usage
can_id_t message_id = {0};
message_id.priority = 2;
message_id.source = 0x45;
message_id.type = 0x0A;

uint32_t can_id = *(uint32_t*)&message_id;  // Convert to uint32_t
```

### **Union Bit Fields**

#### **Multiple Views of Same Data**
```c
// Union for different views of same data
typedef union {
    uint32_t as_uint32;
    struct {
        uint32_t low_byte  : 8;
        uint32_t mid_byte  : 8;
        uint32_t high_byte : 8;
        uint32_t top_byte  : 8;
    } as_bytes;
    struct {
        uint32_t data    : 24;
        uint32_t control : 8;
    } as_data_control;
} data_union_t;

// Usage
data_union_t data = {0};
data.as_uint32 = 0x12345678;

uint8_t low = data.as_bytes.low_byte;   // 0x78
uint8_t mid = data.as_bytes.mid_byte;   // 0x56
uint8_t high = data.as_bytes.high_byte; // 0x34
uint8_t top = data.as_bytes.top_byte;   // 0x12
```

---

## 🎭 **Bit Masks**

### **Basic Bit Masks**

#### **Single Bit Masks**
```c
// Define bit masks
#define BIT_0   (1 << 0)   // 0b00000001
#define BIT_1   (1 << 1)   // 0b00000010
#define BIT_2   (1 << 2)   // 0b00000100
#define BIT_3   (1 << 3)   // 0b00001000
#define BIT_4   (1 << 4)   // 0b00010000
#define BIT_5   (1 << 5)   // 0b00100000
#define BIT_6   (1 << 6)   // 0b01000000
#define BIT_7   (1 << 7)   // 0b10000000

// Usage
uint8_t flags = 0;
flags |= BIT_3;        // Set bit 3
flags &= ~BIT_1;       // Clear bit 1
flags ^= BIT_5;        // Toggle bit 5

if (flags & BIT_3) {
    // Bit 3 is set
}
```

#### **Multi-bit Masks**
```c
// Multi-bit masks
#define MASK_LOWER_NIBBLE  0x0F    // 0b00001111
#define MASK_UPPER_NIBBLE  0xF0    // 0b11110000
#define MASK_LOWER_BYTE    0xFF    // 0b11111111
#define MASK_UPPER_BYTE    0xFF00  // 0b1111111100000000

// Usage
uint16_t value = 0xABCD;
uint8_t lower_byte = value & MASK_LOWER_BYTE;      // 0xCD
uint8_t upper_byte = (value >> 8) & MASK_LOWER_BYTE; // 0xAB
```

### **Hardware Register Masks**

#### **GPIO Register Masks**
```c
// GPIO register masks
#define GPIO_PIN_MASK(pin)     (1 << (pin))
#define GPIO_PIN_SET(pin)      (1 << (pin))
#define GPIO_PIN_CLEAR(pin)    (1 << ((pin) + 16))

// GPIO register addresses
volatile uint32_t* const GPIO_DATA = (uint32_t*)0x40020000;
volatile uint32_t* const GPIO_SET = (uint32_t*)0x40020008;
volatile uint32_t* const GPIO_CLEAR = (uint32_t*)0x4002000C;

// GPIO operations
void gpio_set_pin(uint8_t pin) {
    *GPIO_SET = GPIO_PIN_SET(pin);
}

void gpio_clear_pin(uint8_t pin) {
    *GPIO_CLEAR = GPIO_PIN_CLEAR(pin);
}

void gpio_toggle_pin(uint8_t pin) {
    *GPIO_DATA ^= GPIO_PIN_MASK(pin);
}

bool gpio_read_pin(uint8_t pin) {
    return (*GPIO_DATA & GPIO_PIN_MASK(pin)) != 0;
}
```

#### **UART Register Masks**
```c
// UART register masks
#define UART_TX_READY     (1 << 7)   // Transmit ready
#define UART_RX_READY     (1 << 0)   // Receive ready
#define UART_TX_BUSY      (1 << 16)  // Transmit busy
#define UART_RX_OVERRUN   (1 << 3)   // Receive overrun

// UART register addresses
volatile uint32_t* const UART_STATUS = (uint32_t*)0x40021000;
volatile uint32_t* const UART_DATA = (uint32_t*)0x40021004;

// UART operations
bool uart_tx_ready(void) {
    return (*UART_STATUS & UART_TX_READY) != 0;
}

bool uart_rx_ready(void) {
    return (*UART_STATUS & UART_RX_READY) != 0;
}

void uart_send_byte(uint8_t data) {
    while (!uart_tx_ready()) {
        // Wait for TX ready
    }
    *UART_DATA = data;
}

uint8_t uart_receive_byte(void) {
    while (!uart_rx_ready()) {
        // Wait for RX ready
    }
    return (uint8_t)(*UART_DATA & 0xFF);
}
```

---

## 🔄 **Bit Shifting**

### **Arithmetic vs Logical Shifts**

#### **Logical Shift (Unsigned)**
```c
// Logical shift - fills with zeros
uint8_t value = 0b10000000;  // 128
uint8_t left_shift = value << 1;   // 0b00000000 = 0 (overflow)
uint8_t right_shift = value >> 1;  // 0b01000000 = 64

// Common uses
uint8_t multiply_by_2(uint8_t value) {
    return value << 1;  // Multiply by 2
}

uint8_t divide_by_2(uint8_t value) {
    return value >> 1;  // Divide by 2
}

uint8_t multiply_by_power_of_2(uint8_t value, uint8_t power) {
    return value << power;  // Multiply by 2^power
}
```

#### **Arithmetic Shift (Signed)**
```c
// Arithmetic shift - preserves sign bit
int8_t value = -128;  // 0b10000000
int8_t right_shift = value >> 1;  // 0b11000000 = -64

// Be careful with signed shifts
int8_t negative_value = -1;  // 0b11111111
int8_t shifted = negative_value >> 1;  // 0b11111111 = -1 (implementation dependent)
```

### **Bit Manipulation Functions**

#### **Set, Clear, Toggle Bits**
```c
// Set specific bit
uint32_t set_bit(uint32_t value, uint8_t bit_position) {
    return value | (1 << bit_position);
}

// Clear specific bit
uint32_t clear_bit(uint32_t value, uint8_t bit_position) {
    return value & ~(1 << bit_position);
}

// Toggle specific bit
uint32_t toggle_bit(uint32_t value, uint8_t bit_position) {
    return value ^ (1 << bit_position);
}

// Check if bit is set
bool is_bit_set(uint32_t value, uint8_t bit_position) {
    return (value & (1 << bit_position)) != 0;
}

// Usage
uint32_t flags = 0;
flags = set_bit(flags, 3);    // Set bit 3
flags = clear_bit(flags, 1);  // Clear bit 1
flags = toggle_bit(flags, 5); // Toggle bit 5

if (is_bit_set(flags, 3)) {
    // Bit 3 is set
}
```

#### **Extract and Insert Bits**
```c
// Extract bits from position
uint32_t extract_bits(uint32_t value, uint8_t start_bit, uint8_t num_bits) {
    uint32_t mask = (1 << num_bits) - 1;
    return (value >> start_bit) & mask;
}

// Insert bits at position
uint32_t insert_bits(uint32_t value, uint32_t bits, uint8_t start_bit, uint8_t num_bits) {
    uint32_t mask = ((1 << num_bits) - 1) << start_bit;
    return (value & ~mask) | ((bits << start_bit) & mask);
}

// Usage
uint32_t data = 0x12345678;
uint32_t extracted = extract_bits(data, 8, 8);  // Extract byte at position 8
uint32_t modified = insert_bits(data, 0xAA, 8, 8);  // Insert 0xAA at position 8
```

---

## 🔧 **Hardware Register Operations**

### **Register Read-Modify-Write**

#### **Safe Register Operations**
```c
// Safe way to modify specific bits in register
void set_register_bits(volatile uint32_t* reg, uint32_t mask) {
    *reg |= mask;  // Set bits
}

void clear_register_bits(volatile uint32_t* reg, uint32_t mask) {
    *reg &= ~mask;  // Clear bits
}

void toggle_register_bits(volatile uint32_t* reg, uint32_t mask) {
    *reg ^= mask;  // Toggle bits
}

// Usage
volatile uint32_t* const CONTROL_REG = (uint32_t*)0x40020000;
#define ENABLE_BIT    (1 << 0)
#define INTERRUPT_BIT (1 << 1)
#define CLOCK_BIT     (1 << 2)

set_register_bits(CONTROL_REG, ENABLE_BIT | INTERRUPT_BIT);
clear_register_bits(CONTROL_REG, CLOCK_BIT);
```

#### **Atomic Register Operations**
```c
// Atomic operations (disable interrupts during modification)
void atomic_set_bits(volatile uint32_t* reg, uint32_t mask) {
    uint32_t old_interrupt_state = disable_interrupts();
    *reg |= mask;
    restore_interrupts(old_interrupt_state);
}

void atomic_clear_bits(volatile uint32_t* reg, uint32_t mask) {
    uint32_t old_interrupt_state = disable_interrupts();
    *reg &= ~mask;
    restore_interrupts(old_interrupt_state);
}
```

### **Register Field Operations**

#### **Field Read/Write**
```c
// Read field from register
uint32_t read_register_field(volatile uint32_t* reg, uint32_t mask, uint8_t shift) {
    return (*reg & mask) >> shift;
}

// Write field to register
void write_register_field(volatile uint32_t* reg, uint32_t mask, uint8_t shift, uint32_t value) {
    *reg = (*reg & ~mask) | ((value << shift) & mask);
}

// Usage
volatile uint32_t* const CONFIG_REG = (uint32_t*)0x40020004;
#define MODE_MASK     (0x03 << 0)   // 2 bits at position 0
#define DIVIDER_MASK  (0x0F << 8)   // 4 bits at position 8

// Read mode
uint32_t mode = read_register_field(CONFIG_REG, MODE_MASK, 0);

// Write divider
write_register_field(CONFIG_REG, DIVIDER_MASK, 8, 5);
```

---

## 🎯 **Common Bit Tricks**

### **Bit Counting**

#### **Count Set Bits**
```c
// Count number of set bits (population count)
uint8_t count_set_bits(uint32_t value) {
    uint8_t count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

// Faster method using lookup table
static const uint8_t bit_count_table[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    // ... (complete table)
};

uint8_t fast_count_set_bits(uint32_t value) {
    return bit_count_table[value & 0xFF] +
           bit_count_table[(value >> 8) & 0xFF] +
           bit_count_table[(value >> 16) & 0xFF] +
           bit_count_table[(value >> 24) & 0xFF];
}
```

#### **Find First Set Bit**
```c
// Find position of least significant set bit
int8_t find_first_set_bit(uint32_t value) {
    if (value == 0) return -1;
    
    uint8_t position = 0;
    while ((value & 1) == 0) {
        value >>= 1;
        position++;
    }
    return position;
}

// Using built-in function (if available)
int8_t find_first_set_bit_builtin(uint32_t value) {
    if (value == 0) return -1;
    return __builtin_ctz(value);  // GCC built-in
}
```

### **Power of 2 Operations**

#### **Check if Power of 2**
```c
// Check if number is power of 2
bool is_power_of_2(uint32_t value) {
    return value != 0 && (value & (value - 1)) == 0;
}

// Get next power of 2
uint32_t next_power_of_2(uint32_t value) {
    if (value == 0) return 1;
    
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value + 1;
}
```

### **Bit Reversal**

#### **Reverse Bits**
```c
// Reverse bits in byte
uint8_t reverse_bits(uint8_t value) {
    uint8_t result = 0;
    for (uint8_t i = 0; i < 8; i++) {
        result = (result << 1) | (value & 1);
        value >>= 1;
    }
    return result;
}

// Using lookup table for speed
static const uint8_t reverse_table[256] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
    // ... (complete table)
};

uint8_t fast_reverse_bits(uint8_t value) {
    return reverse_table[value];
}
```

---

## ⚡ **Performance Considerations**

### **Optimization Techniques**

#### **Compiler Optimizations**
```c
// Use built-in functions when available
uint8_t count_bits_builtin(uint32_t value) {
    return __builtin_popcount(value);  // GCC built-in
}

uint8_t find_first_bit_builtin(uint32_t value) {
    return __builtin_ctz(value);  // Count trailing zeros
}

uint8_t find_last_bit_builtin(uint32_t value) {
    return 31 - __builtin_clz(value);  // Count leading zeros
}
```

#### **Lookup Tables**
```c
// Pre-computed lookup tables for common operations
static const uint8_t bit_reverse_table[256] = {
    // Pre-computed bit reversal table
};

static const uint8_t bit_count_table[256] = {
    // Pre-computed bit count table
};

// Fast operations using lookup tables
uint8_t fast_bit_reverse(uint8_t value) {
    return bit_reverse_table[value];
}

uint8_t fast_bit_count(uint8_t value) {
    return bit_count_table[value];
}
```

### **Memory Access Patterns**

#### **Efficient Bit Operations**
```c
// Efficient bit operations for large arrays
void set_bits_in_array(uint32_t* array, uint32_t size, uint32_t bit_mask) {
    for (uint32_t i = 0; i < size; i++) {
        array[i] |= bit_mask;
    }
}

void clear_bits_in_array(uint32_t* array, uint32_t size, uint32_t bit_mask) {
    for (uint32_t i = 0; i < size; i++) {
        array[i] &= ~bit_mask;
    }
}
```

---

## ⚠️ **Common Pitfalls**

### **Sign Extension**

#### **Signed vs Unsigned Shifts**
```c
// ❌ Bad: Sign extension with signed types
int8_t negative_value = -1;  // 0b11111111
int8_t shifted = negative_value >> 1;  // Implementation dependent

// ✅ Good: Use unsigned for bit operations
uint8_t unsigned_value = 0xFF;  // 255
uint8_t shifted = unsigned_value >> 1;  // 127 (predictable)
```

### **Overflow Issues**

#### **Shift Overflow**
```c
// ❌ Bad: Shift by more than type size
uint8_t value = 1;
uint8_t result = value << 8;  // Undefined behavior

// ✅ Good: Check shift amount
uint8_t safe_shift(uint8_t value, uint8_t shift_amount) {
    if (shift_amount >= 8) return 0;
    return value << shift_amount;
}
```

### **Endianness Issues**

#### **Byte Order**
```c
// ❌ Bad: Assuming byte order
uint32_t value = 0x12345678;
uint8_t* bytes = (uint8_t*)&value;
uint8_t first_byte = bytes[0];  // Depends on endianness

// ✅ Good: Explicit byte order
uint32_t value = 0x12345678;
uint8_t first_byte = (value >> 24) & 0xFF;  // Always MSB
uint8_t last_byte = value & 0xFF;           // Always LSB
```

---

## ✅ **Best Practices**

### **Clear Bit Operations**

#### **Self-Documenting Code**
```c
// ✅ Good: Clear bit operation names
#define SET_BIT(reg, bit)     ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit)   ((reg) &= ~(1 << (bit)))
#define TOGGLE_BIT(reg, bit)  ((reg) ^= (1 << (bit)))
#define TEST_BIT(reg, bit)    ((reg) & (1 << (bit)))

// Usage
volatile uint32_t* const STATUS_REG = (uint32_t*)0x40020000;
SET_BIT(*STATUS_REG, 3);      // Set bit 3
CLEAR_BIT(*STATUS_REG, 1);    // Clear bit 1
if (TEST_BIT(*STATUS_REG, 5)) {
    // Bit 5 is set
}
```

#### **Hardware Register Access**
```c
// ✅ Good: Structured register access
typedef struct {
    volatile uint32_t DATA;
    volatile uint32_t CONTROL;
    volatile uint32_t STATUS;
} uart_registers_t;

uart_registers_t* const UART0 = (uart_registers_t*)0x40021000;

// Register bit definitions
#define UART_ENABLE     (1 << 0)
#define UART_TX_ENABLE  (1 << 1)
#define UART_RX_ENABLE  (1 << 2)
#define UART_TX_READY   (1 << 7)
#define UART_RX_READY   (1 << 0)

// Safe register operations
void uart_enable(void) {
    UART0->CONTROL |= UART_ENABLE | UART_TX_ENABLE | UART_RX_ENABLE;
}

bool uart_tx_ready(void) {
    return (UART0->STATUS & UART_TX_READY) != 0;
}
```

### **Error Checking**

#### **Boundary Checks**
```c
// ✅ Good: Safe bit operations
uint32_t safe_set_bit(uint32_t value, uint8_t bit_position) {
    if (bit_position >= 32) return value;  // Boundary check
    return value | (1 << bit_position);
}

uint32_t safe_clear_bit(uint32_t value, uint8_t bit_position) {
    if (bit_position >= 32) return value;  // Boundary check
    return value & ~(1 << bit_position);
}

bool safe_test_bit(uint32_t value, uint8_t bit_position) {
    if (bit_position >= 32) return false;  // Boundary check
    return (value & (1 << bit_position)) != 0;
}
```

---

## 🎯 **Interview Questions**

### **Basic Bit Operations**
1. **What is the difference between `&` and `&&`?**
   - `&` is bitwise AND operator
   - `&&` is logical AND operator
   - `&` operates on individual bits
   - `&&` operates on boolean values

2. **How do you set a specific bit?**
   - Use OR operation: `value |= (1 << bit_position)`
   - Example: `flags |= (1 << 3)` sets bit 3

3. **How do you clear a specific bit?**
   - Use AND with NOT: `value &= ~(1 << bit_position)`
   - Example: `flags &= ~(1 << 1)` clears bit 1

### **Bit Manipulation**
4. **How do you check if a number is power of 2?**
   - Check if `(value & (value - 1)) == 0` and `value != 0`
   - Power of 2 has exactly one bit set

5. **How do you count set bits efficiently?**
   - Use lookup table for small values
   - Use built-in functions like `__builtin_popcount()`
   - Use Brian Kernighan's algorithm

6. **How do you find the first set bit?**
   - Use `__builtin_ctz()` for trailing zeros
   - Or use loop: `while ((value & 1) == 0)`

### **Hardware Applications**
7. **Why is `volatile` important for hardware registers?**
   - Prevents compiler from optimizing away reads/writes
   - Ensures memory operations are preserved
   - Required for memory-mapped I/O

8. **How do you safely modify hardware registers?**
   - Use read-modify-write pattern
   - Disable interrupts during critical operations
   - Use atomic operations when possible

9. **What's the difference between logical and arithmetic shift?**
   - Logical shift fills with zeros (unsigned)
   - Arithmetic shift preserves sign bit (signed)
   - Right shift behavior differs for negative numbers

### **Advanced Topics**
10. **How do you reverse bits in a byte?**
    - Use lookup table for speed
    - Or use loop: `result = (result << 1) | (value & 1)`

11. **How do you swap two variables without temporary?**
    - Use XOR: `a ^= b; b ^= a; a ^= b;`
    - Or use addition/subtraction (risky with overflow)

12. **How do you check for endianness?**
    - Use union: `union { uint16_t i; uint8_t c[2]; } u = {1};`
    - Check `u.c[0]` to determine endianness

---

## 📚 **Additional Resources**

- [Bit Twiddling Hacks](https://graphics.stanford.edu/~seander/bithacks.html)
- [Hacker's Delight](https://en.wikipedia.org/wiki/Hacker%27s_Delight)
- [GCC Built-in Functions](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)

---

**Next Topic:** [Structure Alignment](./Structure_Alignment.md) → [Inline Functions and Macros](./Inline_Functions_Macros.md)
