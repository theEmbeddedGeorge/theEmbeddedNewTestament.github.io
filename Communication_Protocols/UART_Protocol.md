# 🔌 UART Protocol

> **Mastering Universal Asynchronous Receiver/Transmitter for Embedded Systems**  
> Baud rate, data bits, parity, stop bits, flow control, and practical applications

## 📋 Table of Contents

- [🎯 Overview](#-overview)
- [🔧 UART Fundamentals](#-uart-fundamentals)
- [⚙️ UART Configuration](#️-uart-configuration)
- [📊 Data Frame Structure](#-data-frame-structure)
- [🔍 Error Detection and Handling](#-error-detection-and-handling)
- [⚡ Flow Control](#-flow-control)
- [🎯 Common Applications](#-common-applications)
- [⚠️ Common Pitfalls](#️-common-pitfalls)
- [✅ Best Practices](#-best-practices)
- [🎯 Interview Questions](#-interview-questions)
- [📚 Additional Resources](#-additional-resources)

---

## 🎯 Overview

UART (Universal Asynchronous Receiver/Transmitter) is a widely used serial communication protocol for embedded systems. It provides simple, reliable, and cost-effective communication between devices without requiring a shared clock signal.

**Key Concepts:**
- **Asynchronous Communication**: No shared clock signal required
- **Baud Rate**: Data transmission speed in bits per second
- **Data Frame**: Start bit, data bits, parity bit, stop bits
- **Error Detection**: Parity checking, frame errors, overrun detection

---

## 🔧 UART Fundamentals

### **UART Parameters**
```c
// UART configuration structure
typedef struct {
    uint32_t baud_rate;      // Baud rate in bits per second
    uint32_t data_bits;      // Number of data bits (7, 8, 9)
    uint32_t stop_bits;      // Number of stop bits (1, 2)
    uint32_t parity;         // Parity type (NONE, EVEN, ODD)
    uint32_t flow_control;   // Flow control (NONE, RTS_CTS, RTS_CTS_DTR_DSR)
    uint32_t mode;           // Mode (RX_ONLY, TX_ONLY, TX_RX)
} UART_Config_t;

// Common baud rates
#define UART_BAUD_9600    9600
#define UART_BAUD_19200   19200
#define UART_BAUD_38400   38400
#define UART_BAUD_57600   57600
#define UART_BAUD_115200  115200
#define UART_BAUD_230400  230400
#define UART_BAUD_460800  460800
#define UART_BAUD_921600  921600
```

### **UART Frame Structure**
```c
// UART frame structure
typedef struct {
    uint8_t start_bit;      // Always 0
    uint8_t data_bits[8];   // Data bits (LSB first)
    uint8_t parity_bit;     // Parity bit (optional)
    uint8_t stop_bits[2];   // Stop bits (1 or 2)
} UART_Frame_t;

// Calculate frame time
uint32_t uart_frame_time_calculate(uint32_t baud_rate, uint8_t data_bits, 
                                   uint8_t stop_bits, uint8_t parity_enabled) {
    uint8_t total_bits = 1 + data_bits + stop_bits; // Start bit + data + stop
    if (parity_enabled) total_bits++;
    
    return (total_bits * 1000000) / baud_rate; // Time in microseconds
}
```

---

## ⚙️ UART Configuration

### **Basic UART Configuration**
```c
// Configure UART for basic communication
void uart_basic_config(UART_HandleTypeDef* huart, UART_Config_t* config) {
    // Configure UART parameters
    huart->Instance = USART1;
    huart->Init.BaudRate = config->baud_rate;
    huart->Init.WordLength = config->data_bits == 9 ? UART_WORDLENGTH_9B : UART_WORDLENGTH_8B;
    huart->Init.StopBits = config->stop_bits == 2 ? UART_STOPBITS_2 : UART_STOPBITS_1;
    huart->Init.Parity = config->parity;
    huart->Init.Mode = config->mode;
    huart->Init.HwFlowCtl = config->flow_control;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    
    HAL_UART_Init(huart);
}

// Initialize UART with default settings
void uart_init_default(UART_HandleTypeDef* huart) {
    UART_Config_t config = {
        .baud_rate = UART_BAUD_115200,
        .data_bits = 8,
        .stop_bits = 1,
        .parity = UART_PARITY_NONE,
        .flow_control = UART_HWCONTROL_NONE,
        .mode = UART_MODE_TX_RX
    };
    
    uart_basic_config(huart, &config);
}
```

### **UART with DMA Configuration**
```c
// Configure UART with DMA for efficient data transfer
void uart_dma_config(UART_HandleTypeDef* huart, DMA_HandleTypeDef* hdma_tx, 
                     DMA_HandleTypeDef* hdma_rx) {
    // Configure TX DMA
    hdma_tx->Instance = DMA1_Stream7;
    hdma_tx->Init.Request = DMA_REQUEST_USART1_TX;
    hdma_tx->Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tx->Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tx->Init.MemInc = DMA_MINC_ENABLE;
    hdma_tx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_tx->Init.Mode = DMA_NORMAL;
    hdma_tx->Init.Priority = DMA_PRIORITY_LOW;
    hdma_tx->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    
    HAL_DMA_Init(hdma_tx);
    __HAL_LINKDMA(huart, hdmatx, *hdma_tx);
    
    // Configure RX DMA
    hdma_rx->Instance = DMA1_Stream2;
    hdma_rx->Init.Request = DMA_REQUEST_USART1_RX;
    hdma_rx->Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_rx->Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_rx->Init.MemInc = DMA_MINC_ENABLE;
    hdma_rx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_rx->Init.Mode = DMA_CIRCULAR;
    hdma_rx->Init.Priority = DMA_PRIORITY_HIGH;
    hdma_rx->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    
    HAL_DMA_Init(hdma_rx);
    __HAL_LINKDMA(huart, hdmarx, *hdma_rx);
}
```

### **UART with Interrupts**
```c
// Configure UART with interrupts
void uart_interrupt_config(UART_HandleTypeDef* huart) {
    // Enable UART interrupts
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);  // Receive not empty
    __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);   // Transmit empty
    __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);   // Error interrupt
    
    // Configure NVIC
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

// UART interrupt handler
void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

// UART receive complete callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART1) {
        // Handle received data
        uart_rx_complete_callback();
    }
}
```

---

## 📊 Data Frame Structure

### **Frame Generation**
```c
// Generate UART frame
void uart_frame_generate(UART_Frame_t* frame, uint8_t data, uint8_t parity_enabled) {
    frame->start_bit = 0;
    
    // Set data bits (LSB first)
    for (int i = 0; i < 8; i++) {
        frame->data_bits[i] = (data >> i) & 0x01;
    }
    
    // Calculate parity bit
    if (parity_enabled) {
        uint8_t ones_count = 0;
        for (int i = 0; i < 8; i++) {
            if (frame->data_bits[i]) ones_count++;
        }
        frame->parity_bit = (ones_count % 2) ? 1 : 0; // Even parity
    } else {
        frame->parity_bit = 0;
    }
    
    // Set stop bits
    frame->stop_bits[0] = 1;
    frame->stop_bits[1] = 1;
}

// Parse UART frame
uint8_t uart_frame_parse(UART_Frame_t* frame, uint8_t parity_enabled) {
    uint8_t data = 0;
    
    // Extract data bits (LSB first)
    for (int i = 0; i < 8; i++) {
        if (frame->data_bits[i]) {
            data |= (1 << i);
        }
    }
    
    // Check parity
    if (parity_enabled) {
        uint8_t ones_count = 0;
        for (int i = 0; i < 8; i++) {
            if (frame->data_bits[i]) ones_count++;
        }
        uint8_t expected_parity = (ones_count % 2) ? 1 : 0;
        
        if (frame->parity_bit != expected_parity) {
            return 0xFF; // Parity error
        }
    }
    
    return data;
}
```

### **Baud Rate Calculation**
```c
// Calculate baud rate register value
uint32_t uart_baud_rate_calculate(uint32_t clock_freq, uint32_t target_baud_rate) {
    // For oversampling by 16
    uint32_t mantissa = clock_freq / (16 * target_baud_rate);
    uint32_t fraction = ((clock_freq % (16 * target_baud_rate)) * 16) / (16 * target_baud_rate);
    
    return (mantissa << 4) | (fraction & 0x0F);
}

// Validate baud rate
uint8_t uart_baud_rate_validate(uint32_t clock_freq, uint32_t baud_rate) {
    uint32_t actual_baud_rate = clock_freq / (16 * uart_baud_rate_calculate(clock_freq, baud_rate));
    uint32_t error = abs((int32_t)(actual_baud_rate - baud_rate));
    float error_percent = (float)error * 100.0f / baud_rate;
    
    return (error_percent < 2.0f) ? 1 : 0; // Accept if error < 2%
}
```

---

## 🔍 Error Detection and Handling

### **Error Types**
```c
// UART error types
typedef enum {
    UART_ERROR_NONE = 0x00,
    UART_ERROR_PARITY = 0x01,
    UART_ERROR_FRAMING = 0x02,
    UART_ERROR_OVERRUN = 0x04,
    UART_ERROR_NOISE = 0x08
} UART_Error_t;

// Error handling structure
typedef struct {
    UART_HandleTypeDef* huart;
    UART_Error_t last_error;
    uint32_t error_count;
    uint32_t total_errors;
} UART_Error_Handler_t;

void uart_error_handler_init(UART_Error_Handler_t* error_handler, UART_HandleTypeDef* huart) {
    error_handler->huart = huart;
    error_handler->last_error = UART_ERROR_NONE;
    error_handler->error_count = 0;
    error_handler->total_errors = 0;
}
```

### **Error Detection**
```c
// Check for UART errors
UART_Error_t uart_check_errors(UART_HandleTypeDef* huart) {
    UART_Error_t errors = UART_ERROR_NONE;
    
    // Check parity error
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE)) {
        errors |= UART_ERROR_PARITY;
        __HAL_UART_CLEAR_PEFLAG(huart);
    }
    
    // Check framing error
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE)) {
        errors |= UART_ERROR_FRAMING;
        __HAL_UART_CLEAR_FEFLAG(huart);
    }
    
    // Check overrun error
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) {
        errors |= UART_ERROR_OVERRUN;
        __HAL_UART_CLEAR_OREFLAG(huart);
    }
    
    // Check noise error
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE)) {
        errors |= UART_ERROR_NOISE;
        __HAL_UART_CLEAR_NEFLAG(huart);
    }
    
    return errors;
}

// Handle UART errors
void uart_handle_errors(UART_Error_Handler_t* error_handler) {
    UART_Error_t errors = uart_check_errors(error_handler->huart);
    
    if (errors != UART_ERROR_NONE) {
        error_handler->last_error = errors;
        error_handler->error_count++;
        error_handler->total_errors++;
        
        // Log error
        uart_log_error(error_handler, errors);
    }
}
```

---

## ⚡ Flow Control

### **Hardware Flow Control**
```c
// Hardware flow control configuration
typedef struct {
    GPIO_TypeDef* rts_port;
    uint16_t rts_pin;
    GPIO_TypeDef* cts_port;
    uint16_t cts_pin;
    uint8_t flow_control_enabled;
} UART_Flow_Control_t;

void uart_flow_control_init(UART_Flow_Control_t* flow_ctrl, GPIO_TypeDef* rts_port, 
                           uint16_t rts_pin, GPIO_TypeDef* cts_port, uint16_t cts_pin) {
    flow_ctrl->rts_port = rts_port;
    flow_ctrl->rts_pin = rts_pin;
    flow_ctrl->cts_port = cts_port;
    flow_ctrl->cts_pin = cts_pin;
    flow_ctrl->flow_control_enabled = 1;
    
    // Configure RTS as output
    gpio_pushpull_output_config(rts_port, rts_pin);
    gpio_write_output(rts_port, rts_pin, 1); // RTS high (ready to receive)
    
    // Configure CTS as input with pull-up
    gpio_input_pullup_config(cts_port, cts_pin);
}

// Check if ready to transmit
uint8_t uart_ready_to_transmit(UART_Flow_Control_t* flow_ctrl) {
    if (!flow_ctrl->flow_control_enabled) return 1;
    
    // Check CTS (Clear To Send)
    return gpio_read_input(flow_ctrl->cts_port, flow_ctrl->cts_pin);
}

// Set RTS (Request To Send)
void uart_set_rts(UART_Flow_Control_t* flow_ctrl, uint8_t state) {
    if (flow_ctrl->flow_control_enabled) {
        gpio_write_output(flow_ctrl->rts_port, flow_ctrl->rts_pin, state);
    }
}
```

### **Software Flow Control**
```c
// Software flow control (XON/XOFF)
typedef struct {
    uint8_t xon_char;
    uint8_t xoff_char;
    uint8_t flow_control_enabled;
    uint8_t xoff_sent;
    uint8_t xon_sent;
} UART_Software_Flow_Control_t;

void uart_software_flow_control_init(UART_Software_Flow_Control_t* flow_ctrl) {
    flow_ctrl->xon_char = 0x11;  // DC1 (XON)
    flow_ctrl->xoff_char = 0x13; // DC3 (XOFF)
    flow_ctrl->flow_control_enabled = 1;
    flow_ctrl->xoff_sent = 0;
    flow_ctrl->xon_sent = 0;
}

// Send XOFF
void uart_send_xoff(UART_HandleTypeDef* huart, UART_Software_Flow_Control_t* flow_ctrl) {
    if (flow_ctrl->flow_control_enabled && !flow_ctrl->xoff_sent) {
        HAL_UART_Transmit(huart, &flow_ctrl->xoff_char, 1, 1000);
        flow_ctrl->xoff_sent = 1;
        flow_ctrl->xon_sent = 0;
    }
}

// Send XON
void uart_send_xon(UART_HandleTypeDef* huart, UART_Software_Flow_Control_t* flow_ctrl) {
    if (flow_ctrl->flow_control_enabled && !flow_ctrl->xon_sent) {
        HAL_UART_Transmit(huart, &flow_ctrl->xon_char, 1, 1000);
        flow_ctrl->xon_sent = 1;
        flow_ctrl->xoff_sent = 0;
    }
}
```

---

## 🎯 Common Applications

### **Debug Console**
```c
// UART debug console
typedef struct {
    UART_HandleTypeDef* huart;
    uint8_t buffer[256];
    uint16_t buffer_index;
    uint8_t echo_enabled;
} UART_Console_t;

void uart_console_init(UART_Console_t* console, UART_HandleTypeDef* huart) {
    console->huart = huart;
    console->buffer_index = 0;
    console->echo_enabled = 1;
    
    // Send welcome message
    const char* welcome_msg = "UART Console Ready\r\n";
    HAL_UART_Transmit(huart, (uint8_t*)welcome_msg, strlen(welcome_msg), 1000);
}

void uart_console_process(UART_Console_t* console) {
    uint8_t received_char;
    
    if (HAL_UART_Receive(console->huart, &received_char, 1, 10) == HAL_OK) {
        // Echo character if enabled
        if (console->echo_enabled) {
            HAL_UART_Transmit(console->huart, &received_char, 1, 100);
        }
        
        // Handle special characters
        if (received_char == '\r' || received_char == '\n') {
            if (console->buffer_index > 0) {
                console->buffer[console->buffer_index] = '\0';
                uart_console_execute_command(console, (char*)console->buffer);
                console->buffer_index = 0;
            }
        } else if (received_char == '\b' || received_char == 127) {
            // Backspace
            if (console->buffer_index > 0) {
                console->buffer_index--;
                const char* backspace = "\b \b";
                HAL_UART_Transmit(console->huart, (uint8_t*)backspace, 3, 100);
            }
        } else if (console->buffer_index < sizeof(console->buffer) - 1) {
            console->buffer[console->buffer_index++] = received_char;
        }
    }
}
```

### **Data Logger**
```c
// UART data logger
typedef struct {
    UART_HandleTypeDef* huart;
    uint8_t* data_buffer;
    uint32_t buffer_size;
    uint32_t write_index;
    uint32_t read_index;
    uint8_t buffer_full;
} UART_Data_Logger_t;

void uart_data_logger_init(UART_Data_Logger_t* logger, UART_HandleTypeDef* huart,
                          uint8_t* buffer, uint32_t size) {
    logger->huart = huart;
    logger->data_buffer = buffer;
    logger->buffer_size = size;
    logger->write_index = 0;
    logger->read_index = 0;
    logger->buffer_full = 0;
}

void uart_data_logger_write(UART_Data_Logger_t* logger, uint8_t* data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        logger->data_buffer[logger->write_index] = data[i];
        logger->write_index = (logger->write_index + 1) % logger->buffer_size;
        
        if (logger->write_index == logger->read_index) {
            logger->buffer_full = 1;
            logger->read_index = (logger->read_index + 1) % logger->buffer_size;
        }
    }
}

void uart_data_logger_flush(UART_Data_Logger_t* logger) {
    while (logger->read_index != logger->write_index) {
        HAL_UART_Transmit(logger->huart, &logger->data_buffer[logger->read_index], 1, 100);
        logger->read_index = (logger->read_index + 1) % logger->buffer_size;
    }
    logger->buffer_full = 0;
}
```

### **Protocol Implementation**
```c
// Simple protocol implementation
typedef struct {
    UART_HandleTypeDef* huart;
    uint8_t start_byte;
    uint8_t end_byte;
    uint8_t escape_byte;
} UART_Protocol_t;

void uart_protocol_init(UART_Protocol_t* protocol, UART_HandleTypeDef* huart) {
    protocol->huart = huart;
    protocol->start_byte = 0xAA;
    protocol->end_byte = 0x55;
    protocol->escape_byte = 0x7D;
}

// Send protocol packet
void uart_protocol_send_packet(UART_Protocol_t* protocol, uint8_t* data, uint32_t length) {
    // Send start byte
    HAL_UART_Transmit(protocol->huart, &protocol->start_byte, 1, 100);
    
    // Send data with escaping
    for (uint32_t i = 0; i < length; i++) {
        if (data[i] == protocol->start_byte || data[i] == protocol->end_byte || 
            data[i] == protocol->escape_byte) {
            HAL_UART_Transmit(protocol->huart, &protocol->escape_byte, 1, 100);
            uint8_t escaped_byte = data[i] ^ 0x20;
            HAL_UART_Transmit(protocol->huart, &escaped_byte, 1, 100);
        } else {
            HAL_UART_Transmit(protocol->huart, &data[i], 1, 100);
        }
    }
    
    // Send end byte
    HAL_UART_Transmit(protocol->huart, &protocol->end_byte, 1, 100);
}
```

---

## ⚠️ Common Pitfalls

### **1. Baud Rate Mismatch**
```c
// ❌ Wrong: Mismatched baud rates
void uart_baud_rate_wrong(UART_HandleTypeDef* huart1, UART_HandleTypeDef* huart2) {
    huart1->Init.BaudRate = 115200;
    huart2->Init.BaudRate = 9600; // Mismatch!
}

// ✅ Correct: Matching baud rates
void uart_baud_rate_correct(UART_HandleTypeDef* huart1, UART_HandleTypeDef* huart2) {
    huart1->Init.BaudRate = 115200;
    huart2->Init.BaudRate = 115200; // Match!
}
```

### **2. Missing Error Handling**
```c
// ❌ Wrong: No error handling
void uart_receive_wrong(UART_HandleTypeDef* huart, uint8_t* data) {
    HAL_UART_Receive(huart, data, 1, 1000);
    // No error checking
}

// ✅ Correct: With error handling
HAL_StatusTypeDef uart_receive_correct(UART_HandleTypeDef* huart, uint8_t* data) {
    HAL_StatusTypeDef status = HAL_UART_Receive(huart, data, 1, 1000);
    if (status != HAL_OK) {
        uart_handle_errors(huart);
    }
    return status;
}
```

### **3. Buffer Overflow**
```c
// ❌ Wrong: No buffer overflow protection
void uart_receive_wrong(uint8_t* buffer, uint32_t size) {
    uint32_t index = 0;
    while (1) {
        HAL_UART_Receive(huart, &buffer[index], 1, 1000);
        index++; // No bounds checking
    }
}

// ✅ Correct: With buffer overflow protection
void uart_receive_correct(uint8_t* buffer, uint32_t size) {
    uint32_t index = 0;
    while (index < size) {
        if (HAL_UART_Receive(huart, &buffer[index], 1, 1000) == HAL_OK) {
            index++;
        }
    }
}
```

---

## ✅ Best Practices

### **1. Use Appropriate Baud Rates**
```c
// Use appropriate baud rates for different applications
void configure_uart_baud_rate(UART_Config_t* config, uint8_t application_type) {
    switch (application_type) {
        case UART_DEBUG_CONSOLE:
            config->baud_rate = UART_BAUD_115200; // Fast for debugging
            break;
        case UART_SENSOR_COMMUNICATION:
            config->baud_rate = UART_BAUD_9600; // Slow for sensors
            break;
        case UART_HIGH_SPEED_DATA:
            config->baud_rate = UART_BAUD_921600; // Very fast for data transfer
            break;
    }
}
```

### **2. Implement Proper Error Handling**
```c
// Always implement proper error handling
void uart_robust_communication(UART_HandleTypeDef* huart) {
    UART_Error_Handler_t error_handler;
    uart_error_handler_init(&error_handler, huart);
    
    // Check for errors before communication
    UART_Error_t errors = uart_check_errors(huart);
    if (errors != UART_ERROR_NONE) {
        uart_handle_errors(&error_handler);
    }
}
```

### **3. Use Flow Control for High-Speed Communication**
```c
// Use flow control for reliable high-speed communication
void uart_high_speed_communication(UART_HandleTypeDef* huart) {
    UART_Flow_Control_t flow_ctrl;
    uart_flow_control_init(&flow_ctrl, GPIOA, GPIO_PIN_12, GPIOA, GPIO_PIN_11);
    
    // Check if ready to transmit
    if (uart_ready_to_transmit(&flow_ctrl)) {
        // Send data
        HAL_UART_Transmit(huart, data, length, 1000);
    }
}
```

---

## 🎯 Interview Questions

### **Basic Questions**
1. **What is UART and how does it work?**
   - Universal Asynchronous Receiver/Transmitter, serial communication without clock

2. **What is baud rate and why is it important?**
   - Bits per second, determines communication speed

3. **What are the components of a UART frame?**
   - Start bit, data bits, parity bit, stop bits

### **Advanced Questions**
1. **How do you handle UART errors?**
   - Parity, framing, overrun, noise error detection and handling

2. **What is flow control and when do you use it?**
   - Hardware (RTS/CTS) or software (XON/XOFF) flow control

3. **How do you implement a reliable UART protocol?**
   - Error detection, flow control, packet framing

### **Practical Questions**
1. **Design a UART-based debug console**
   - Command parsing, echo, error handling

2. **Implement UART communication with error recovery**
   - Error detection, retransmission, flow control

3. **Create a UART data logger**
   - Buffer management, data formatting, error handling

---

## 📚 Additional Resources

### **Documentation**
- [STM32 UART Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [ARM Cortex-M UART Programming](https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/peripherals/uart)

### **Tools**
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) - UART configuration
- [UART Calculator](https://www.st.com/resource/en/user_manual/dm00104712-stm32cubemx-user-manual-stmicroelectronics.pdf)

### **Related Topics**
- **[SPI Protocol](./SPI_Protocol.md)** - Clock polarity, phase, data order, chip select management
- **[I2C Protocol](./I2C_Protocol.md)** - Addressing, clock stretching, multi-master arbitration
- **[RS232/RS422/RS485](./RS232_RS422_RS485.md)** - Electrical standards, multi-drop communication

---

**Next Topic:** [SPI Protocol](./SPI_Protocol.md) → [I2C Protocol](./I2C_Protocol.md)
