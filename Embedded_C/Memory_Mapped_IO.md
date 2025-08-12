# Memory-Mapped I/O

## 📋 Table of Contents
- [Overview](#-overview)
- [Memory-Mapped I/O Basics](#-memory-mapped-io-basics)
- [Hardware Register Access](#-hardware-register-access)
- [Volatile Keyword Usage](#-volatile-keyword-usage)
- [Register Bit Manipulation](#-register-bit-manipulation)
- [Peripheral Configuration](#-peripheral-configuration)
- [Interrupt Handling](#-interrupt-handling)
- [DMA with Memory-Mapped I/O](#-dma-with-memory-mapped-io)
- [Real-time Considerations](#-real-time-considerations)
- [Common Pitfalls](#-common-pitfalls)
- [Best Practices](#-best-practices)
- [Interview Questions](#-interview-questions)
- [Additional Resources](#-additional-resources)

## 🎯 Overview

### Concept: Typed volatile views over fixed addresses

Treat peripheral registers as `volatile` objects at known addresses with precise widths. Use minimal, typed accessors and avoid accidental non-volatile aliases.

### Why it matters in embedded
- Prevents the compiler from eliding or reordering critical I/O operations.
- Clarifies intent (read-only status vs write-only command registers).
- Eases review and static analysis.

### Minimal example
```c
typedef struct {
  volatile uint32_t CTRL;
  volatile const uint32_t STAT;  // read-only
  volatile uint32_t DATA;
} periph_t;

#define PERIPH ((periph_t*)0x40010000u)

static inline void periph_enable(void) { PERIPH->CTRL |= 1u; }
static inline uint32_t periph_ready(void) { return (PERIPH->STAT & 1u) != 0u; }
```

### Try it
1. Intentionally drop `volatile` and compile with `-O2`; show hoisted loads or removed writes.
2. Add a memory barrier where required by the architecture (e.g., after enabling clocks) and measure behavior.

### Takeaways
- Always access registers through `volatile`-qualified types/pointers.
- Be explicit about read-only/write-only semantics via `const` on `volatile` fields.
- Consider memory barriers for ordering on platforms that require them.

---

## 🧪 Guided Labs
1) Optimization surprise
- Access the same register through `volatile` and non-`volatile` aliases; compare generated code.

2) Ordering
- Insert a write to enable a peripheral followed immediately by a dependent read; add/remove a barrier and observe behavior on a platform that requires ordering.

## ✅ Check Yourself
- What does `volatile` not guarantee (e.g., atomicity)?
- How would you model a write-only register field safely?

## 🔗 Cross-links
- `Embedded_C/Type_Qualifiers.md` for qualifiers
- `Embedded_C/Compiler_Intrinsics.md` for barriers

Memory-mapped I/O allows direct access to hardware registers through memory addresses, enabling efficient communication with peripherals. This technique is fundamental in embedded systems for controlling hardware without dedicated I/O instructions.

## 🔧 Memory-Mapped I/O Basics

### Register Structure Definition
```c
// Memory-mapped I/O register structure
typedef struct {
    volatile uint32_t control;      // Control register
    volatile uint32_t status;       // Status register
    volatile uint32_t data;         // Data register
    volatile uint32_t reserved;     // Reserved for alignment
} __attribute__((aligned(4))) peripheral_registers_t;

// Memory-mapped I/O base addresses
#define PERIPHERAL_BASE_ADDRESS    0x40000000
#define GPIO_BASE_ADDRESS          0x40020000
#define UART_BASE_ADDRESS          0x40021000
#define SPI_BASE_ADDRESS           0x40022000
#define I2C_BASE_ADDRESS           0x40023000

// Peripheral register mapping
peripheral_registers_t* map_peripheral_registers(uint32_t base_address) {
    // Ensure base address is aligned
    if (base_address & 0x3) {
        return NULL;  // Invalid alignment
    }
    
    return (peripheral_registers_t*)base_address;
}

// Example usage
peripheral_registers_t* uart_regs = map_peripheral_registers(UART_BASE_ADDRESS);
if (uart_regs) {
    // Access UART registers directly
    uart_regs->control = 0x01;  // Enable UART
}
```

### Register Access Macros
```c
// Safe register access macros
#define REG_READ(addr)             (*(volatile uint32_t*)(addr))
#define REG_WRITE(addr, value)     (*(volatile uint32_t*)(addr) = (value))
#define REG_SET_BITS(addr, mask)   (*(volatile uint32_t*)(addr) |= (mask))
#define REG_CLEAR_BITS(addr, mask) (*(volatile uint32_t*)(addr) &= ~(mask))

// Bit manipulation macros
#define BIT_SET(reg, bit)          ((reg) |= (1U << (bit)))
#define BIT_CLEAR(reg, bit)        ((reg) &= ~(1U << (bit)))
#define BIT_TOGGLE(reg, bit)       ((reg) ^= (1U << (bit)))
#define BIT_READ(reg, bit)         (((reg) >> (bit)) & 1U)

// Example usage
void configure_gpio_pin(uint32_t gpio_base, uint8_t pin, uint8_t mode) {
    uint32_t* gpio_regs = (uint32_t*)gpio_base;
    
    // Set pin mode
    REG_SET_BITS(gpio_regs[0], (mode << (pin * 2)));
    
    // Enable GPIO clock
    REG_SET_BITS(gpio_regs[1], (1U << pin));
}
```

## 🔧 Hardware Register Access

### GPIO Register Access
```c
// GPIO register structure
typedef struct {
    volatile uint32_t moder;    // Mode register
    volatile uint32_t otyper;   // Output type register
    volatile uint32_t ospeedr;  // Output speed register
    volatile uint32_t pupdr;    // Pull-up/pull-down register
    volatile uint32_t idr;      // Input data register
    volatile uint32_t odr;      // Output data register
    volatile uint32_t bsrr;     // Bit set/reset register
    volatile uint32_t lckr;     // Configuration lock register
    volatile uint32_t afrl;     // Alternate function low register
    volatile uint32_t afrh;     // Alternate function high register
} __attribute__((aligned(4))) gpio_registers_t;

// GPIO configuration functions
void gpio_set_pin_mode(gpio_registers_t* gpio, uint8_t pin, uint8_t mode) {
    uint32_t mask = 3U << (pin * 2);  // 2 bits per pin
    uint32_t value = mode << (pin * 2);
    
    // Clear and set mode bits
    gpio->moder &= ~mask;
    gpio->moder |= value;
}

void gpio_set_pin_output(gpio_registers_t* gpio, uint8_t pin, bool high) {
    if (high) {
        gpio->bsrr = (1U << pin);  // Set bit
    } else {
        gpio->bsrr = (1U << (pin + 16));  // Reset bit
    }
}

bool gpio_read_pin_input(gpio_registers_t* gpio, uint8_t pin) {
    return (gpio->idr & (1U << pin)) != 0;
}
```

### UART Register Access
```c
// UART register structure
typedef struct {
    volatile uint32_t sr;       // Status register
    volatile uint32_t dr;       // Data register
    volatile uint32_t brr;      // Baud rate register
    volatile uint32_t cr1;      // Control register 1
    volatile uint32_t cr2;      // Control register 2
    volatile uint32_t cr3;      // Control register 3
    volatile uint32_t gtpr;     // Guard time and prescaler register
} __attribute__((aligned(4))) uart_registers_t;

// UART configuration
void uart_configure(uart_registers_t* uart, uint32_t baud_rate, uint32_t system_clock) {
    // Calculate baud rate divisor
    uint32_t divisor = system_clock / baud_rate;
    uart->brr = divisor;
    
    // Enable UART
    uart->cr1 = UART_CR1_UE | UART_CR1_TE | UART_CR1_RE;
}

void uart_send_byte(uart_registers_t* uart, uint8_t data) {
    // Wait for transmit data register empty
    while (!(uart->sr & UART_SR_TXE));
    
    // Send data
    uart->dr = data;
}

uint8_t uart_receive_byte(uart_registers_t* uart) {
    // Wait for receive data register not empty
    while (!(uart->sr & UART_SR_RXNE));
    
    // Read data
    return (uint8_t)(uart->dr & 0xFF);
}
```

## ⚡ Volatile Keyword Usage

### Volatile Register Access
```c
// Volatile register access patterns
typedef struct {
    volatile uint32_t control;
    volatile uint32_t status;
    volatile uint32_t data;
} volatile_peripheral_t;

// Correct volatile usage
void safe_register_access(volatile_peripheral_t* peripheral) {
    // Read status register (volatile ensures actual memory read)
    uint32_t status = peripheral->status;
    
    // Check specific bits
    if (status & STATUS_BIT_READY) {
        // Write to data register
        peripheral->data = 0x12345678;
        
        // Set control bit
        peripheral->control |= CONTROL_BIT_ENABLE;
    }
}

// Incorrect non-volatile usage
typedef struct {
    uint32_t control;  // WRONG: Missing volatile
    uint32_t status;   // WRONG: Missing volatile
    uint32_t data;     // WRONG: Missing volatile
} non_volatile_peripheral_t;

void unsafe_register_access(non_volatile_peripheral_t* peripheral) {
    // Compiler may optimize away this read
    uint32_t status = peripheral->status;  // May be cached
    
    // Compiler may optimize away this write
    peripheral->data = 0x12345678;  // May not actually write to hardware
}
```

### Volatile Pointer Usage
```c
// Volatile pointer to non-volatile data
volatile uint32_t* const hardware_register = (volatile uint32_t*)0x40000000;

// Non-volatile pointer to volatile data
uint32_t* volatile status_pointer = (uint32_t*)0x40000004;

// Volatile pointer to volatile data
volatile uint32_t* volatile config_register = (volatile uint32_t*)0x40000008;

// Safe hardware access functions
uint32_t read_hardware_register(volatile uint32_t* reg) {
    return *reg;  // Volatile ensures actual memory read
}

void write_hardware_register(volatile uint32_t* reg, uint32_t value) {
    *reg = value;  // Volatile ensures actual memory write
}

// Example usage
void configure_hardware(void) {
    // Read current status
    uint32_t status = read_hardware_register(hardware_register);
    
    // Modify configuration
    write_hardware_register(config_register, 0x12345678);
}
```

## 🔧 Register Bit Manipulation

### Bit Field Definitions
```c
// Register bit field definitions
#define GPIO_MODER_INPUT    0x00
#define GPIO_MODER_OUTPUT   0x01
#define GPIO_MODER_ALT      0x02
#define GPIO_MODER_ANALOG   0x03

#define GPIO_OTYPER_PUSH_PULL  0x00
#define GPIO_OTYPER_OPEN_DRAIN 0x01

#define GPIO_OSPEEDR_LOW     0x00
#define GPIO_OSPEEDR_MEDIUM  0x01
#define GPIO_OSPEEDR_HIGH    0x02
#define GPIO_OSPEEDR_VERY_HIGH 0x03

// Bit manipulation functions
void gpio_configure_pin(gpio_registers_t* gpio, uint8_t pin, 
                       uint8_t mode, uint8_t output_type, uint8_t speed) {
    uint32_t moder_mask = 3U << (pin * 2);
    uint32_t moder_value = mode << (pin * 2);
    
    uint32_t otyper_mask = 1U << pin;
    uint32_t otyper_value = output_type << pin;
    
    uint32_t ospeedr_mask = 3U << (pin * 2);
    uint32_t ospeedr_value = speed << (pin * 2);
    
    // Configure mode
    gpio->moder &= ~moder_mask;
    gpio->moder |= moder_value;
    
    // Configure output type
    gpio->otyper &= ~otyper_mask;
    gpio->otyper |= otyper_value;
    
    // Configure speed
    gpio->ospeedr &= ~ospeedr_mask;
    gpio->ospeedr |= ospeedr_value;
}

// Atomic bit operations
void gpio_atomic_set_pin(gpio_registers_t* gpio, uint8_t pin) {
    // Use BSRR for atomic set
    gpio->bsrr = (1U << pin);
}

void gpio_atomic_clear_pin(gpio_registers_t* gpio, uint8_t pin) {
    // Use BSRR for atomic clear
    gpio->bsrr = (1U << (pin + 16));
}

void gpio_atomic_toggle_pin(gpio_registers_t* gpio, uint8_t pin) {
    // Read current state and toggle
    uint32_t current_state = gpio->odr;
    if (current_state & (1U << pin)) {
        gpio->bsrr = (1U << (pin + 16));  // Clear
    } else {
        gpio->bsrr = (1U << pin);  // Set
    }
}
```

### Register Read-Modify-Write
```c
// Safe read-modify-write operations
uint32_t register_read_modify_write(volatile uint32_t* reg, 
                                   uint32_t mask, uint32_t value) {
    uint32_t old_value = *reg;
    *reg = (old_value & ~mask) | (value & mask);
    return old_value;
}

// Example: Configure UART control register
void uart_configure_control(uart_registers_t* uart, uint32_t config_bits) {
    // Read current control register
    uint32_t current_cr1 = uart->cr1;
    
    // Clear configuration bits and set new values
    current_cr1 &= ~(UART_CR1_CONFIG_MASK);
    current_cr1 |= (config_bits & UART_CR1_CONFIG_MASK);
    
    // Write back to register
    uart->cr1 = current_cr1;
}

// Atomic register operations
void atomic_register_set_bits(volatile uint32_t* reg, uint32_t bits) {
    *reg |= bits;  // Atomic OR operation
}

void atomic_register_clear_bits(volatile uint32_t* reg, uint32_t bits) {
    *reg &= ~bits;  // Atomic AND operation
}

void atomic_register_toggle_bits(volatile uint32_t* reg, uint32_t bits) {
    *reg ^= bits;  // Atomic XOR operation
}
```

## ⚙️ Peripheral Configuration

### Peripheral Clock Control
```c
// Clock control register structure
typedef struct {
    volatile uint32_t ahb1enr;   // AHB1 peripheral clock enable
    volatile uint32_t ahb2enr;   // AHB2 peripheral clock enable
    volatile uint32_t apb1enr;   // APB1 peripheral clock enable
    volatile uint32_t apb2enr;   // APB2 peripheral clock enable
} __attribute__((aligned(4))) rcc_registers_t;

#define RCC_BASE_ADDRESS 0x40023800

// Clock enable/disable functions
void enable_peripheral_clock(uint32_t peripheral_bit, uint32_t clock_register) {
    volatile uint32_t* rcc = (volatile uint32_t*)RCC_BASE_ADDRESS;
    rcc[clock_register] |= (1U << peripheral_bit);
}

void disable_peripheral_clock(uint32_t peripheral_bit, uint32_t clock_register) {
    volatile uint32_t* rcc = (volatile uint32_t*)RCC_BASE_ADDRESS;
    rcc[clock_register] &= ~(1U << peripheral_bit);
}

// Example: Enable GPIOA clock
void enable_gpio_a_clock(void) {
    enable_peripheral_clock(0, 0);  // GPIOA is bit 0 in AHB1ENR
}
```

### Peripheral Reset Control
```c
// Reset control functions
void reset_peripheral(uint32_t peripheral_bit, uint32_t reset_register) {
    volatile uint32_t* rcc = (volatile uint32_t*)RCC_BASE_ADDRESS;
    
    // Assert reset
    rcc[reset_register] |= (1U << peripheral_bit);
    
    // Wait for reset to take effect
    for (volatile int i = 0; i < 100; i++);
    
    // De-assert reset
    rcc[reset_register] &= ~(1U << peripheral_bit);
    
    // Wait for reset to complete
    for (volatile int i = 0; i < 100; i++);
}

// Example: Reset UART peripheral
void reset_uart_peripheral(void) {
    reset_peripheral(4, 1);  // UART1 is bit 4 in APB1RSTR
}
```

## 🔄 Interrupt Handling

### Interrupt Register Access
```c
// NVIC (Nested Vectored Interrupt Controller) registers
typedef struct {
    volatile uint32_t iser[8];    // Interrupt set-enable registers
    volatile uint32_t icer[8];    // Interrupt clear-enable registers
    volatile uint32_t ispr[8];    // Interrupt set-pending registers
    volatile uint32_t icpr[8];    // Interrupt clear-pending registers
    volatile uint32_t iabr[8];    // Interrupt active bit registers
    volatile uint32_t ipr[60];    // Interrupt priority registers
} __attribute__((aligned(4))) nvic_registers_t;

#define NVIC_BASE_ADDRESS 0xE000E100

// Interrupt control functions
void enable_interrupt(uint8_t irq_number) {
    volatile uint32_t* nvic = (volatile uint32_t*)NVIC_BASE_ADDRESS;
    uint8_t reg_index = irq_number / 32;
    uint8_t bit_position = irq_number % 32;
    
    nvic->iser[reg_index] = (1U << bit_position);
}

void disable_interrupt(uint8_t irq_number) {
    volatile uint32_t* nvic = (volatile uint32_t*)NVIC_BASE_ADDRESS;
    uint8_t reg_index = irq_number / 32;
    uint8_t bit_position = irq_number % 32;
    
    nvic->icer[reg_index] = (1U << bit_position);
}

void set_interrupt_priority(uint8_t irq_number, uint8_t priority) {
    volatile uint32_t* nvic = (volatile uint32_t*)NVIC_BASE_ADDRESS;
    uint8_t reg_index = irq_number / 4;
    uint8_t byte_position = (irq_number % 4) * 8;
    
    uint32_t mask = 0xFF << byte_position;
    uint32_t value = priority << byte_position;
    
    nvic->ipr[reg_index] = (nvic->ipr[reg_index] & ~mask) | value;
}
```

### Peripheral Interrupt Configuration
```c
// UART interrupt configuration
void uart_enable_interrupts(uart_registers_t* uart, uint32_t interrupt_bits) {
    // Enable specific interrupts in UART
    uart->cr1 |= interrupt_bits;
    
    // Enable UART interrupt in NVIC
    enable_interrupt(UART_IRQ_NUMBER);
}

void uart_disable_interrupts(uart_registers_t* uart, uint32_t interrupt_bits) {
    // Disable specific interrupts in UART
    uart->cr1 &= ~interrupt_bits;
    
    // Check if any interrupts are still enabled
    if (!(uart->cr1 & UART_INTERRUPT_MASK)) {
        disable_interrupt(UART_IRQ_NUMBER);
    }
}

// UART interrupt handler
void uart_interrupt_handler(uart_registers_t* uart) {
    uint32_t status = uart->sr;
    
    // Check for receive interrupt
    if (status & UART_SR_RXNE) {
        uint8_t data = (uint8_t)(uart->dr & 0xFF);
        process_received_data(data);
    }
    
    // Check for transmit interrupt
    if (status & UART_SR_TXE) {
        if (has_data_to_transmit()) {
            uart->dr = get_next_byte_to_transmit();
        } else {
            uart->cr1 &= ~UART_CR1_TXEIE;  // Disable TXE interrupt
        }
    }
}
```

## 🔄 DMA with Memory-Mapped I/O

### DMA Peripheral Configuration
```c
// DMA peripheral configuration
typedef struct {
    volatile uint32_t cr;         // Control register
    volatile uint32_t ndtr;       // Number of data register
    volatile uint32_t par;        // Peripheral address register
    volatile uint32_t mar;        // Memory address register
    volatile uint32_t reserved;
    volatile uint32_t fcr;        // FIFO control register
} __attribute__((aligned(4))) dma_stream_registers_t;

void configure_dma_for_peripheral(dma_stream_registers_t* dma_stream,
                                 uint32_t peripheral_address,
                                 uint32_t memory_address,
                                 uint32_t transfer_count) {
    // Disable DMA stream
    dma_stream->cr &= ~DMA_CR_EN;
    
    // Wait for DMA to disable
    while (dma_stream->cr & DMA_CR_EN);
    
    // Configure peripheral address
    dma_stream->par = peripheral_address;
    
    // Configure memory address
    dma_stream->mar = memory_address;
    
    // Configure transfer count
    dma_stream->ndtr = transfer_count;
    
    // Configure control register
    dma_stream->cr = DMA_CR_DIR_PERIPH_TO_MEM |
                     DMA_CR_MINC |
                     DMA_CR_PSIZE_WORD |
                     DMA_CR_MSIZE_WORD |
                     DMA_CR_PL_HIGH;
    
    // Enable DMA stream
    dma_stream->cr |= DMA_CR_EN;
}
```

### DMA with UART Example
```c
// UART DMA configuration
void uart_configure_dma_receive(uart_registers_t* uart, 
                               dma_stream_registers_t* dma_stream,
                               uint8_t* buffer, uint32_t buffer_size) {
    // Configure DMA for UART receive
    configure_dma_for_peripheral(dma_stream,
                                (uint32_t)&uart->dr,  // UART data register
                                (uint32_t)buffer,     // Memory buffer
                                buffer_size);
    
    // Enable UART DMA receive
    uart->cr3 |= UART_CR3_DMAR;
    
    // Enable UART receive
    uart->cr1 |= UART_CR1_RE;
}

void uart_configure_dma_transmit(uart_registers_t* uart,
                                dma_stream_registers_t* dma_stream,
                                const uint8_t* buffer, uint32_t buffer_size) {
    // Configure DMA for UART transmit
    configure_dma_for_peripheral(dma_stream,
                                (uint32_t)&uart->dr,  // UART data register
                                (uint32_t)buffer,     // Memory buffer
                                buffer_size);
    
    // Enable UART DMA transmit
    uart->cr3 |= UART_CR3_DMAT;
    
    // Enable UART transmit
    uart->cr1 |= UART_CR1_TE;
}
```

## ⏱️ Real-time Considerations

### Register Access Timing
```c
// Timing-critical register access
typedef struct {
    volatile uint32_t* register_address;
    uint32_t access_time_ns;
    bool is_timing_critical;
} timing_critical_register_t;

// Optimized register access for real-time systems
uint32_t fast_register_read(volatile uint32_t* reg) {
    // Ensure single instruction access
    return *reg;
}

void fast_register_write(volatile uint32_t* reg, uint32_t value) {
    // Ensure single instruction write
    *reg = value;
}

// Critical timing register access
void critical_timing_register_access(volatile uint32_t* reg, uint32_t value) {
    // Disable interrupts during critical access
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    
    // Perform register access
    *reg = value;
    
    // Restore interrupt state
    if (!primask) {
        __enable_irq();
    }
}
```

### Register Access Latency
```c
// Measure register access latency
typedef struct {
    uint32_t read_latency_ns;
    uint32_t write_latency_ns;
    uint32_t access_count;
} register_latency_monitor_t;

register_latency_monitor_t* create_register_latency_monitor(void) {
    register_latency_monitor_t* monitor = malloc(sizeof(register_latency_monitor_t));
    if (monitor) {
        monitor->read_latency_ns = 0;
        monitor->write_latency_ns = 0;
        monitor->access_count = 0;
    }
    return monitor;
}

uint32_t measure_register_read_latency(volatile uint32_t* reg) {
    uint32_t start_time = get_system_tick_count();
    uint32_t value = *reg;
    uint32_t end_time = get_system_tick_count();
    
    return (end_time - start_time) * SYSTEM_TICK_PERIOD_NS;
}
```

## ⚠️ Common Pitfalls

### 1. Missing Volatile Keyword
```c
// WRONG: Missing volatile keyword
typedef struct {
    uint32_t control;  // Missing volatile
    uint32_t status;   // Missing volatile
} incorrect_peripheral_t;

void incorrect_access(incorrect_peripheral_t* peripheral) {
    // Compiler may optimize away this access
    peripheral->control = 0x01;
    // May not actually write to hardware
}

// CORRECT: Using volatile keyword
typedef struct {
    volatile uint32_t control;  // Correct
    volatile uint32_t status;   // Correct
} correct_peripheral_t;

void correct_access(correct_peripheral_t* peripheral) {
    // Volatile ensures actual hardware access
    peripheral->control = 0x01;
}
```

### 2. Race Conditions
```c
// WRONG: Race condition in register access
void unsafe_register_modification(volatile uint32_t* reg, uint32_t mask, uint32_t value) {
    uint32_t current = *reg;  // Read
    current &= ~mask;         // Modify
    current |= value;
    *reg = current;           // Write
    // Race condition between read and write
}

// CORRECT: Atomic register modification
void safe_register_modification(volatile uint32_t* reg, uint32_t mask, uint32_t value) {
    // Use atomic operations or disable interrupts
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    
    uint32_t current = *reg;
    current = (current & ~mask) | (value & mask);
    *reg = current;
    
    if (!primask) {
        __enable_irq();
    }
}
```

### 3. Incorrect Register Alignment
```c
// WRONG: Unaligned register access
void incorrect_register_access(void) {
    uint8_t* unaligned_ptr = (uint8_t*)0x40000001;  // Unaligned address
    uint32_t* reg = (uint32_t*)unaligned_ptr;        // May cause alignment fault
    *reg = 0x12345678;  // Potential alignment fault
}

// CORRECT: Aligned register access
void correct_register_access(void) {
    uint32_t* reg = (uint32_t*)0x40000000;  // Aligned address
    *reg = 0x12345678;  // Safe access
}
```

## ✅ Best Practices

### 1. Register Access Safety
```c
// Safe register access patterns
typedef struct {
    volatile uint32_t* register_ptr;
    uint32_t register_mask;
    uint32_t register_shift;
    const char* register_name;
} safe_register_access_t;

safe_register_access_t* create_safe_register_access(volatile uint32_t* reg,
                                                   uint32_t mask,
                                                   uint32_t shift,
                                                   const char* name) {
    safe_register_access_t* access = malloc(sizeof(safe_register_access_t));
    if (access) {
        access->register_ptr = reg;
        access->register_mask = mask;
        access->register_shift = shift;
        access->register_name = name;
    }
    return access;
}

uint32_t safe_register_read(safe_register_access_t* access) {
    if (!access || !access->register_ptr) {
        return 0;
    }
    
    uint32_t value = *access->register_ptr;
    return (value >> access->register_shift) & access->register_mask;
}

void safe_register_write(safe_register_access_t* access, uint32_t value) {
    if (!access || !access->register_ptr) {
        return;
    }
    
    uint32_t current = *access->register_ptr;
    uint32_t masked_value = (value & access->register_mask) << access->register_shift;
    
    *access->register_ptr = (current & ~(access->register_mask << access->register_shift)) | masked_value;
}
```

### 2. Register Validation
```c
// Register access validation
bool is_valid_register_address(uint32_t address) {
    // Check if address is in valid peripheral range
    return (address >= PERIPHERAL_BASE_ADDRESS && 
            address < (PERIPHERAL_BASE_ADDRESS + PERIPHERAL_SIZE));
}

bool is_aligned_register_address(uint32_t address, uint32_t alignment) {
    return (address % alignment) == 0;
}

volatile uint32_t* validate_and_map_register(uint32_t address, uint32_t alignment) {
    if (!is_valid_register_address(address)) {
        return NULL;
    }
    
    if (!is_aligned_register_address(address, alignment)) {
        return NULL;
    }
    
    return (volatile uint32_t*)address;
}
```

### 3. Register Access Logging
```c
// Register access logging for debugging
typedef struct {
    uint32_t address;
    uint32_t value;
    bool is_write;
    uint32_t timestamp;
} register_access_log_t;

typedef struct {
    register_access_log_t* log_entries;
    size_t log_size;
    size_t current_index;
    bool logging_enabled;
} register_logger_t;

register_logger_t* create_register_logger(size_t log_size) {
    register_logger_t* logger = malloc(sizeof(register_logger_t));
    if (!logger) return NULL;
    
    logger->log_entries = malloc(log_size * sizeof(register_access_log_t));
    logger->log_size = log_size;
    logger->current_index = 0;
    logger->logging_enabled = true;
    
    return logger;
}

void log_register_access(register_logger_t* logger, uint32_t address, 
                        uint32_t value, bool is_write) {
    if (!logger || !logger->logging_enabled) {
        return;
    }
    
    logger->log_entries[logger->current_index].address = address;
    logger->log_entries[logger->current_index].value = value;
    logger->log_entries[logger->current_index].is_write = is_write;
    logger->log_entries[logger->current_index].timestamp = get_system_tick_count();
    
    logger->current_index = (logger->current_index + 1) % logger->log_size;
}
```

## 🎯 Interview Questions

### Basic Questions
1. **What is memory-mapped I/O and why is it used?**
   - Direct access to hardware registers through memory addresses
   - Used for efficient peripheral control without dedicated I/O instructions

2. **Why is the volatile keyword important in memory-mapped I/O?**
   - Prevents compiler optimization of register accesses
   - Ensures actual memory reads/writes to hardware

3. **What are the key considerations for register access?**
   - Proper alignment
   - Volatile keyword usage
   - Race condition prevention
   - Atomic operations

### Advanced Questions
1. **How would you implement atomic register operations?**
   - Use hardware atomic instructions
   - Disable interrupts during critical sections
   - Use read-modify-write with proper synchronization

2. **What are the challenges of memory-mapped I/O in multi-core systems?**
   - Cache coherency issues
   - Race conditions between cores
   - Memory ordering requirements

3. **How would you optimize register access for real-time systems?**
   - Minimize access latency
   - Use appropriate memory barriers
   - Implement critical section protection

## 📚 Additional Resources

### Standards and Documentation
- **ARM Architecture Reference**: Memory-mapped I/O specifications
- **C Standard**: Volatile keyword behavior
- **Hardware Reference Manuals**: Register specifications

### Related Topics
- **[DMA Buffer Management](./DMA_Buffer_Management.md)** - DMA with memory-mapped I/O
- **[Cache-Aware Programming](./Cache_Aware_Programming.md)** - Cache considerations
- **[Interrupt Handling](./Interrupt_Handling.md)** - Interrupt management
- **[Performance Optimization](./performance_optimization.md)** - Register access optimization

### Tools and Libraries
- **Register access libraries**: Hardware abstraction layers
- **Debugging tools**: Register monitoring and analysis
- **Performance profilers**: Register access timing analysis

---

**Next Topic:** [Shared Memory Programming](./Shared_Memory_Programming.md) → [Real-time Systems](./Real_Time_Systems.md)
