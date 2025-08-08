# C Language Fundamentals for Embedded Systems

> **Essential C programming concepts for embedded software development**

## 📋 **Table of Contents**
- [Overview](#overview)
- [Variables and Data Types](#variables-and-data-types)
- [Functions](#functions)
- [Control Structures](#control-structures)
- [Memory Management](#memory-management)
- [Pointers](#pointers)
- [Arrays and Strings](#arrays-and-strings)
- [Structures and Unions](#structures-and-unions)
- [Preprocessor Directives](#preprocessor-directives)
- [Common Pitfalls](#common-pitfalls)
- [Best Practices](#best-practices)
- [Interview Questions](#interview-questions)

---

## 🎯 **Overview**

C is the primary programming language for embedded systems due to its:
- **Direct hardware access** - Ability to manipulate memory addresses and registers
- **Efficiency** - Minimal runtime overhead and predictable performance
- **Portability** - Works across different microcontrollers and architectures
- **Mature ecosystem** - Extensive toolchains, libraries, and community support

### **Key Characteristics for Embedded Development**
- **Static typing** - Compile-time type checking
- **Manual memory management** - Direct control over memory allocation
- **Low-level access** - Pointer arithmetic and bit manipulation
- **Procedural programming** - Function-based code organization

---

## 🔢 **Variables and Data Types**

### **Basic Data Types**

#### **Integer Types**
```c
// Signed integers
int8_t   small_int;    // 8-bit signed (-128 to 127)
int16_t  medium_int;   // 16-bit signed (-32768 to 32767)
int32_t  large_int;    // 32-bit signed (-2^31 to 2^31-1)
int64_t  huge_int;     // 64-bit signed

// Unsigned integers
uint8_t  small_uint;   // 8-bit unsigned (0 to 255)
uint16_t medium_uint;  // 16-bit unsigned (0 to 65535)
uint32_t large_uint;   // 32-bit unsigned (0 to 2^32-1)
uint64_t huge_uint;    // 64-bit unsigned

// Traditional C types (avoid in embedded)
int      platform_dependent;  // Size varies by platform
long     also_variable;       // Size varies by platform
```

#### **Floating Point Types**
```c
float    single_precision;    // 32-bit IEEE 754
double   double_precision;    // 64-bit IEEE 754 (avoid in embedded)
```

#### **Character Types**
```c
char     character;           // Usually 8-bit
uint8_t  byte_data;          // Explicit 8-bit unsigned
```

### **Variable Declaration and Initialization**

#### **Best Practices**
```c
// ✅ Good: Explicit initialization
uint32_t counter = 0;
uint8_t status = 0xFF;
float temperature = 25.5f;

// ❌ Bad: Uninitialized variables
uint32_t counter;  // Contains garbage data
```

#### **Constants**
```c
// Compile-time constants
#define MAX_BUFFER_SIZE 1024
#define PI 3.14159f

// Runtime constants
const uint32_t TIMEOUT_MS = 5000;
const float VOLTAGE_REFERENCE = 3.3f;

// Enum constants
typedef enum {
    LED_OFF = 0,
    LED_ON = 1,
    LED_BLINK = 2
} led_state_t;
```

### **Storage Classes**

#### **Static Variables**
```c
void function_with_static(void) {
    static uint32_t call_count = 0;  // Retains value between calls
    call_count++;
    // Variable persists throughout program execution
}
```

#### **Volatile Variables**
```c
// Hardware register access
volatile uint32_t* const GPIO_DATA = (uint32_t*)0x40020000;
volatile uint8_t interrupt_flag = 0;

// Interrupt service routine
void ISR_Timer(void) {
    interrupt_flag = 1;  // Modified by interrupt
}
```

---

## 🔧 **Functions**

### **Function Declaration and Definition**

#### **Basic Function Structure**
```c
// Function declaration (prototype)
uint32_t calculate_checksum(const uint8_t* data, uint16_t length);

// Function definition
uint32_t calculate_checksum(const uint8_t* data, uint16_t length) {
    uint32_t checksum = 0;
    
    for (uint16_t i = 0; i < length; i++) {
        checksum += data[i];
    }
    
    return checksum;
}
```

#### **Function Parameters**
```c
// Pass by value (default)
void increment_counter(uint32_t counter) {
    counter++;  // Local copy, original unchanged
}

// Pass by pointer (modify original)
void increment_counter_ptr(uint32_t* counter) {
    (*counter)++;  // Modifies original variable
}

// Pass by const pointer (read-only)
void print_data(const uint8_t* data, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
}
```

### **Return Values**

#### **Multiple Return Values**
```c
// Using structures for multiple returns
typedef struct {
    uint32_t result;
    uint8_t status;
} operation_result_t;

operation_result_t divide_numbers(uint32_t dividend, uint32_t divisor) {
    operation_result_t result;
    
    if (divisor == 0) {
        result.status = 1;  // Error
        result.result = 0;
    } else {
        result.status = 0;  // Success
        result.result = dividend / divisor;
    }
    
    return result;
}
```

#### **Error Handling**
```c
// Return error codes
typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_PARAM = -1,
    ERROR_TIMEOUT = -2,
    ERROR_HARDWARE = -3
} error_code_t;

error_code_t initialize_sensor(uint8_t sensor_id) {
    if (sensor_id > MAX_SENSORS) {
        return ERROR_INVALID_PARAM;
    }
    
    // Hardware initialization code...
    
    return SUCCESS;
}
```

---

## 🎛️ **Control Structures**

### **Conditional Statements**

#### **If-Else Statements**
```c
// Basic if-else
if (temperature > 50.0f) {
    turn_on_cooling_fan();
} else if (temperature < 10.0f) {
    turn_on_heater();
} else {
    turn_off_climate_control();
}

// Nested conditions
if (system_status == SYSTEM_READY) {
    if (battery_level > 20) {
        start_operation();
    } else {
        enter_low_power_mode();
    }
} else {
    perform_system_check();
}
```

#### **Switch Statements**
```c
// Efficient for multiple conditions
switch (command) {
    case CMD_START:
        start_system();
        break;
        
    case CMD_STOP:
        stop_system();
        break;
        
    case CMD_RESET:
        reset_system();
        break;
        
    default:
        handle_unknown_command();
        break;
}
```

### **Loops**

#### **For Loops**
```c
// Standard for loop
for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = 0;  // Clear buffer
}

// Infinite loop (common in embedded)
for (;;) {
    process_data();
    delay_ms(100);
}

// Loop with multiple variables
for (uint16_t i = 0, j = BUFFER_SIZE - 1; i < j; i++, j--) {
    // Swap elements
    uint8_t temp = buffer[i];
    buffer[i] = buffer[j];
    buffer[j] = temp;
}
```

#### **While Loops**
```c
// Condition-checked loop
uint16_t timeout_counter = 0;
while (!data_ready() && timeout_counter < MAX_TIMEOUT) {
    delay_ms(1);
    timeout_counter++;
}

// Infinite loop with break
while (1) {
    if (emergency_stop_pressed()) {
        break;
    }
    process_operation();
}
```

#### **Do-While Loops**
```c
// Execute at least once
do {
    read_sensor_data();
    process_reading();
} while (sensor_active());
```

---

## 💾 **Memory Management**

### **Stack vs Heap**

#### **Stack Memory**
```c
void function_with_stack_vars(void) {
    uint8_t local_var = 0;           // Stack allocation
    uint32_t array[100];             // Stack array
    struct sensor_data data;         // Stack structure
    
    // Variables automatically deallocated when function returns
}
```

#### **Heap Memory (Avoid in Embedded)**
```c
// ❌ Avoid dynamic allocation in embedded systems
void bad_function(void) {
    uint8_t* dynamic_array = malloc(1000);  // Heap allocation
    // ... use array ...
    free(dynamic_array);  // Manual deallocation required
}
```

### **Memory Allocation Strategies**

#### **Static Allocation**
```c
// Global variables
static uint8_t global_buffer[1024];
static uint32_t system_counter = 0;

// Static local variables
void function_with_static(void) {
    static uint8_t persistent_data[256];  // Retains data between calls
    // ... use persistent_data ...
}
```

#### **Memory Pools**
```c
// Pre-allocated memory pool
#define POOL_SIZE 10
#define BLOCK_SIZE 64

static uint8_t memory_pool[POOL_SIZE][BLOCK_SIZE];
static uint8_t pool_usage[POOL_SIZE] = {0};

uint8_t* allocate_from_pool(void) {
    for (uint8_t i = 0; i < POOL_SIZE; i++) {
        if (pool_usage[i] == 0) {
            pool_usage[i] = 1;
            return memory_pool[i];
        }
    }
    return NULL;  // Pool full
}
```

---

## 🎯 **Pointers**

### **Basic Pointer Operations**

#### **Pointer Declaration and Assignment**
```c
// Pointer declarations
uint8_t* data_ptr;           // Pointer to uint8_t
uint32_t* register_ptr;      // Pointer to uint32_t
const uint8_t* read_only_ptr; // Pointer to const data

// Assignment
uint8_t data = 0x42;
data_ptr = &data;  // Address of data

// Dereferencing
uint8_t value = *data_ptr;  // Get value at address
*data_ptr = 0x55;           // Set value at address
```

#### **Pointer Arithmetic**
```c
// Array traversal
uint8_t buffer[100];
uint8_t* ptr = buffer;

for (uint16_t i = 0; i < 100; i++) {
    *ptr = i;  // Set value
    ptr++;     // Move to next element
}

// Pointer arithmetic
uint32_t* word_ptr = (uint32_t*)buffer;
word_ptr += 5;  // Move 5 words (20 bytes)
```

### **Function Pointers**

#### **Callback Functions**
```c
// Function pointer type
typedef void (*callback_t)(uint8_t data);

// Function that takes callback
void process_data(uint8_t* data, uint16_t length, callback_t callback) {
    for (uint16_t i = 0; i < length; i++) {
        callback(data[i]);
    }
}

// Callback implementation
void print_data(uint8_t data) {
    printf("Data: %02X\n", data);
}

// Usage
process_data(buffer, 10, print_data);
```

---

## 📊 **Arrays and Strings**

### **Array Operations**

#### **Array Declaration and Access**
```c
// Array declarations
uint8_t sensor_data[10];
uint32_t timestamps[100];
float temperature_readings[50];

// Array initialization
uint8_t lookup_table[4] = {0x00, 0x01, 0x02, 0x03};
uint16_t coefficients[] = {1, 2, 4, 8, 16};  // Size inferred

// Array access
sensor_data[0] = 0x42;
uint8_t value = sensor_data[5];
```

#### **Multi-dimensional Arrays**
```c
// 2D array
uint8_t matrix[3][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};

// Access element
uint8_t element = matrix[1][2];  // Value: 7
```

### **String Handling**

#### **String Operations**
```c
// String declaration
char message[] = "Hello, World!";
char buffer[50];

// String length
uint16_t length = 0;
while (message[length] != '\0') {
    length++;
}

// String copy (simple implementation)
void string_copy(char* dest, const char* src) {
    uint16_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}
```

---

## 🏗️ **Structures and Unions**

### **Structures**

#### **Basic Structure**
```c
// Structure definition
typedef struct {
    uint8_t id;
    uint32_t timestamp;
    float value;
    uint8_t status;
} sensor_reading_t;

// Structure usage
sensor_reading_t reading;
reading.id = 1;
reading.timestamp = get_current_time();
reading.value = 25.5f;
reading.status = 0;
```

#### **Nested Structures**
```c
// Nested structure
typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} time_t;

typedef struct {
    uint8_t day;
    uint8_t month;
    uint16_t year;
} date_t;

typedef struct {
    date_t date;
    time_t time;
} datetime_t;
```

### **Unions**

#### **Memory-efficient Data Storage**
```c
// Union for different data types
typedef union {
    uint32_t as_uint32;
    uint8_t as_bytes[4];
    float as_float;
} data_union_t;

// Usage
data_union_t data;
data.as_float = 3.14159f;
uint8_t byte0 = data.as_bytes[0];  // Access as bytes
```

---

## 🔧 **Preprocessor Directives**

### **Common Directives**

#### **Include and Define**
```c
// Include files
#include <stdint.h>
#include <stdbool.h>
#include "hardware_config.h"

// Macro definitions
#define MAX_BUFFER_SIZE 1024
#define LED_PIN 13
#define TIMEOUT_MS 5000

// Conditional compilation
#ifdef DEBUG_MODE
    #define DEBUG_PRINT(x) printf(x)
#else
    #define DEBUG_PRINT(x)
#endif
```

#### **Function-like Macros**
```c
// Simple macros
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Multi-line macros
#define INIT_SENSOR(id) do { \
    sensor_config_t config = {0}; \
    config.id = id; \
    config.enabled = true; \
    configure_sensor(&config); \
} while(0)
```

---

## ⚠️ **Common Pitfalls**

### **Memory Issues**
```c
// ❌ Buffer overflow
uint8_t buffer[10];
for (uint16_t i = 0; i < 20; i++) {  // Writing beyond buffer
    buffer[i] = 0;
}

// ✅ Bounds checking
uint8_t buffer[10];
for (uint16_t i = 0; i < sizeof(buffer); i++) {
    buffer[i] = 0;
}
```

### **Type Issues**
```c
// ❌ Implicit type conversion
uint8_t small_var = 300;  // Overflow: 300 > 255

// ✅ Explicit casting
uint8_t small_var = (uint8_t)300;  // Explicit overflow
```

### **Pointer Issues**
```c
// ❌ Dangling pointer
uint8_t* ptr;
{
    uint8_t local_var = 42;
    ptr = &local_var;  // Pointer to local variable
}  // local_var goes out of scope
// ptr now points to invalid memory

// ✅ Valid pointer usage
static uint8_t global_var = 42;
uint8_t* ptr = &global_var;  // Pointer to global variable
```

---

## ✅ **Best Practices**

### **Code Organization**
```c
// ✅ Clear function names
uint32_t calculate_checksum(const uint8_t* data, uint16_t length);
void initialize_hardware(void);
bool is_system_ready(void);

// ✅ Consistent naming conventions
uint8_t sensor_id;      // snake_case for variables
uint32_t MAX_RETRIES;   // UPPER_CASE for constants
typedef struct {
    uint8_t status;
    uint32_t timestamp;
} system_state_t;       // _t suffix for types
```

### **Error Handling**
```c
// ✅ Consistent error handling
error_code_t process_data(const uint8_t* data, uint16_t length) {
    if (data == NULL) {
        return ERROR_INVALID_PARAM;
    }
    
    if (length == 0) {
        return ERROR_INVALID_PARAM;
    }
    
    // Process data...
    return SUCCESS;
}
```

### **Documentation**
```c
/**
 * @brief Calculates CRC32 checksum for data buffer
 * @param data Pointer to data buffer
 * @param length Length of data buffer in bytes
 * @return 32-bit CRC checksum
 * @note Uses polynomial 0x04C11DB7
 */
uint32_t calculate_crc32(const uint8_t* data, uint16_t length);
```

---

## 🎯 **Interview Questions**

### **Basic Concepts**
1. **What is the difference between `int` and `uint8_t`?**
   - `int` is platform-dependent (usually 16 or 32 bits)
   - `uint8_t` is exactly 8 bits, portable across platforms

2. **Explain the `volatile` keyword.**
   - Tells compiler that variable can change unexpectedly (e.g., by hardware)
   - Prevents compiler optimizations that might skip reads/writes

3. **What is the difference between `#define` and `const`?**
   - `#define` is preprocessor directive (text substitution)
   - `const` is compile-time constant with type checking

### **Memory and Pointers**
4. **What is a null pointer?**
   - Pointer that doesn't point to any valid memory location
   - Usually represented as `NULL` or `0`

5. **Explain pointer arithmetic.**
   - Adding/subtracting integers to pointers
   - Moves pointer by `sizeof(type)` * integer value

6. **What is the difference between stack and heap?**
   - Stack: Automatic allocation/deallocation, limited size
   - Heap: Manual allocation/deallocation, larger size

### **Advanced Topics**
7. **What is a function pointer?**
   - Pointer that points to a function
   - Used for callbacks and dynamic function calls

8. **Explain the `static` keyword.**
   - For variables: retains value between function calls
   - For functions: limits scope to current file

9. **What is the difference between `struct` and `union`?**
   - `struct`: All members stored separately (larger memory)
   - `union`: Members share same memory location (smaller memory)

---

## 📚 **Additional Resources**

- [C Programming Language (K&R)](https://en.wikipedia.org/wiki/The_C_Programming_Language)
- [Embedded C Coding Standard](https://barrgroup.com/embedded-systems/books/embedded-c-coding-standard)
- [C99 Standard](https://web.archive.org/web/20181230041359if_/http://www.open-std.org/jtc1/sc22/wg14/www/abq/c17_updated_proposed_fdis.pdf)

---

**Next Topic:** [Memory Management](./Memory_Management.md) → [Pointers and Memory Addresses](./Pointers_Memory_Addresses.md)
