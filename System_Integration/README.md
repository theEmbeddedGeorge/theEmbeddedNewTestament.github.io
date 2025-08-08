# System Integration Guide

## Overview
System integration is the process of combining hardware and software components into a complete, functional embedded system. This guide covers essential system integration topics including bootloaders, firmware updates, development tools, and integration techniques.

## Table of Contents
1. [Bootloader Development](#bootloader-development)
2. [Firmware Update Mechanisms](#firmware-update-mechanisms)
3. [Watchdog Timers and System Recovery](#watchdog-timers-and-system-recovery)
4. [Error Handling and Logging](#error-handling-and-logging)
5. [Cross-compilation Setup](#cross-compilation-setup)
6. [Build Systems](#build-systems)
7. [Version Control for Embedded Projects](#version-control-for-embedded-projects)

---

## Bootloader Development

### What is a Bootloader?
A bootloader is a small program that runs before the main application and is responsible for:
- Hardware initialization
- Loading the main application
- Providing update mechanisms
- System recovery

### Basic Bootloader Structure

#### Bootloader Architecture
```c
// Example: Basic bootloader structure
typedef struct {
    uint32_t magic_number;
    uint32_t version;
    uint32_t application_address;
    uint32_t application_size;
    uint32_t checksum;
} bootloader_header_t;

// Bootloader main function
int bootloader_main(void) {
    // 1. Initialize hardware
    hardware_init();
    
    // 2. Check for update request
    if (check_update_request()) {
        perform_firmware_update();
    }
    
    // 3. Validate application
    if (validate_application()) {
        // 4. Jump to application
        jump_to_application();
    } else {
        // 5. Enter recovery mode
        enter_recovery_mode();
    }
    
    return 0;
}
```

#### Hardware Initialization
```c
// Hardware initialization sequence
void hardware_init(void) {
    // 1. Configure clock system
    configure_system_clock();
    
    // 2. Initialize memory
    initialize_memory();
    
    // 3. Configure GPIO
    configure_gpio();
    
    // 4. Initialize communication interfaces
    initialize_uart();
    initialize_spi();
    
    // 5. Initialize watchdog
    initialize_watchdog();
    
    // 6. Configure interrupt vectors
    configure_interrupts();
}

// System clock configuration
void configure_system_clock(void) {
    // Configure PLL for desired frequency
    PLL->CR = PLL_CR_PLLON | PLL_CR_PLLSRC_HSE;
    
    // Wait for PLL to lock
    while (!(PLL->CR & PLL_CR_PLLRDY));
    
    // Switch to PLL as system clock
    RCC->CFGR = RCC_CFGR_SW_PLL;
    
    // Wait for switch
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
```

### Application Validation

#### Checksum Verification
```c
// Calculate CRC32 checksum
uint32_t calculate_crc32(const uint8_t *data, uint32_t length) {
    uint32_t crc = 0xFFFFFFFF;
    
    for (uint32_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return ~crc;
}

// Validate application
int validate_application(void) {
    bootloader_header_t *header = (bootloader_header_t*)APPLICATION_ADDRESS;
    
    // Check magic number
    if (header->magic_number != BOOTLOADER_MAGIC) {
        return -1;
    }
    
    // Check version
    if (header->version < MINIMUM_VERSION) {
        return -1;
    }
    
    // Calculate checksum
    uint32_t calculated_checksum = calculate_crc32(
        (uint8_t*)APPLICATION_ADDRESS + sizeof(bootloader_header_t),
        header->application_size
    );
    
    // Verify checksum
    if (calculated_checksum != header->checksum) {
        return -1;
    }
    
    return 0;
}
```

#### Application Jump
```c
// Jump to application
void jump_to_application(void) {
    // Disable interrupts
    __disable_irq();
    
    // Reset all peripherals
    reset_peripherals();
    
    // Set vector table to application
    SCB->VTOR = APPLICATION_ADDRESS;
    
    // Set stack pointer
    uint32_t *app_stack = (uint32_t*)APPLICATION_ADDRESS;
    __set_MSP(*app_stack);
    
    // Jump to application
    uint32_t *app_reset = (uint32_t*)(APPLICATION_ADDRESS + 4);
    ((void (*)(void))*app_reset)();
}
```

---

## Firmware Update Mechanisms

### Over-the-Air (OTA) Updates

#### OTA Update Protocol
```c
// OTA update structure
typedef struct {
    uint32_t update_id;
    uint32_t firmware_size;
    uint32_t firmware_version;
    uint32_t checksum;
    uint8_t update_data[];
} ota_update_t;

// OTA update process
int perform_ota_update(void) {
    ota_update_t *update = (ota_update_t*)UPDATE_BUFFER_ADDRESS;
    
    // 1. Receive update data
    if (receive_update_data(update) != 0) {
        return -1;
    }
    
    // 2. Validate update
    if (validate_update(update) != 0) {
        return -1;
    }
    
    // 3. Backup current firmware
    if (backup_current_firmware() != 0) {
        return -1;
    }
    
    // 4. Write new firmware
    if (write_new_firmware(update) != 0) {
        restore_firmware_backup();
        return -1;
    }
    
    // 5. Verify new firmware
    if (verify_new_firmware(update) != 0) {
        restore_firmware_backup();
        return -1;
    }
    
    // 6. Update bootloader
    update_bootloader_info(update);
    
    return 0;
}
```

#### Update Validation
```c
// Validate update data
int validate_update(ota_update_t *update) {
    // Check update ID
    if (update->update_id == 0) {
        return -1;
    }
    
    // Check firmware size
    if (update->firmware_size > MAX_FIRMWARE_SIZE) {
        return -1;
    }
    
    // Check firmware version
    if (update->firmware_version <= get_current_version()) {
        return -1;
    }
    
    // Verify checksum
    uint32_t calculated_checksum = calculate_crc32(
        update->update_data,
        update->firmware_size
    );
    
    if (calculated_checksum != update->checksum) {
        return -1;
    }
    
    return 0;
}
```

### Firmware Storage Management

#### Dual Bank Firmware Storage
```c
// Dual bank firmware storage
typedef struct {
    uint32_t bank_a_address;
    uint32_t bank_b_address;
    uint32_t active_bank;
    uint32_t bank_size;
} dual_bank_storage_t;

// Initialize dual bank storage
void init_dual_bank_storage(dual_bank_storage_t *storage) {
    storage->bank_a_address = FIRMWARE_BANK_A_ADDRESS;
    storage->bank_b_address = FIRMWARE_BANK_B_ADDRESS;
    storage->bank_size = FIRMWARE_BANK_SIZE;
    storage->active_bank = get_active_bank();
}

// Switch firmware bank
int switch_firmware_bank(dual_bank_storage_t *storage) {
    uint32_t new_bank = (storage->active_bank == storage->bank_a_address) ?
                       storage->bank_b_address : storage->bank_a_address;
    
    // Validate new bank
    if (validate_firmware_bank(new_bank) != 0) {
        return -1;
    }
    
    // Update bootloader configuration
    update_bootloader_config(new_bank);
    
    // Set active bank
    storage->active_bank = new_bank;
    
    return 0;
}
```

---

## Watchdog Timers and System Recovery

### Watchdog Timer Implementation

#### Hardware Watchdog
```c
// Watchdog timer configuration
typedef struct {
    uint32_t timeout_ms;
    uint32_t window_ms;
    uint32_t prescaler;
} watchdog_config_t;

// Initialize watchdog
int init_watchdog(watchdog_config_t *config) {
    // Enable watchdog clock
    RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;
    
    // Configure watchdog
    WWDG->CR = WWDG_CR_WDGA | WWDG_CR_T;
    WWDG->CFR = WWDG_CFR_WDGA | WWDG_CFR_W | config->prescaler;
    
    // Enable watchdog
    WWDG->CR |= WWDG_CR_WDGA;
    
    return 0;
}

// Feed watchdog
void feed_watchdog(void) {
    WWDG->CR = (WWDG->CR & WWDG_CR_WDGA) | WWDG_CR_T;
}

// Watchdog task
void watchdog_task(void) {
    static uint32_t last_feed = 0;
    uint32_t current_time = get_system_time();
    
    // Feed watchdog every 100ms
    if (current_time - last_feed >= 100) {
        feed_watchdog();
        last_feed = current_time;
    }
}
```

#### Software Watchdog
```c
// Software watchdog implementation
typedef struct {
    uint32_t timeout_ms;
    uint32_t last_kick;
    uint32_t task_id;
    void (*recovery_handler)(void);
} software_watchdog_t;

// Initialize software watchdog
int init_software_watchdog(software_watchdog_t *wd, uint32_t timeout_ms, void (*recovery_handler)(void)) {
    wd->timeout_ms = timeout_ms;
    wd->last_kick = get_system_time();
    wd->recovery_handler = recovery_handler;
    
    return 0;
}

// Kick software watchdog
void kick_software_watchdog(software_watchdog_t *wd) {
    wd->last_kick = get_system_time();
}

// Check software watchdog
void check_software_watchdog(software_watchdog_t *wd) {
    uint32_t current_time = get_system_time();
    
    if (current_time - wd->last_kick > wd->timeout_ms) {
        // Watchdog timeout - call recovery handler
        if (wd->recovery_handler) {
            wd->recovery_handler();
        }
    }
}
```

### System Recovery Mechanisms

#### Recovery Mode
```c
// Recovery mode implementation
typedef enum {
    RECOVERY_MODE_NONE,
    RECOVERY_MODE_SAFE,
    RECOVERY_MODE_FACTORY_RESET,
    RECOVERY_MODE_FIRMWARE_UPDATE
} recovery_mode_t;

// Enter recovery mode
void enter_recovery_mode(recovery_mode_t mode) {
    // Save recovery mode
    save_recovery_mode(mode);
    
    // Disable interrupts
    __disable_irq();
    
    // Reset system
    NVIC_SystemReset();
}

// Recovery mode handler
void recovery_mode_handler(void) {
    recovery_mode_t mode = get_recovery_mode();
    
    switch (mode) {
        case RECOVERY_MODE_SAFE:
            // Load safe firmware
            load_safe_firmware();
            break;
            
        case RECOVERY_MODE_FACTORY_RESET:
            // Reset to factory settings
            factory_reset();
            break;
            
        case RECOVERY_MODE_FIRMWARE_UPDATE:
            // Enter firmware update mode
            enter_firmware_update_mode();
            break;
            
        default:
            // Default recovery - try to load last known good firmware
            load_last_known_good_firmware();
            break;
    }
}
```

---

## Error Handling and Logging

### Error Handling Framework

#### Error Codes and Messages
```c
// Error code definitions
typedef enum {
    ERROR_NONE = 0,
    ERROR_INVALID_PARAMETER = -1,
    ERROR_MEMORY_ALLOCATION = -2,
    ERROR_TIMEOUT = -3,
    ERROR_COMMUNICATION = -4,
    ERROR_HARDWARE_FAULT = -5,
    ERROR_FIRMWARE_CORRUPTION = -6
} error_code_t;

// Error information structure
typedef struct {
    error_code_t code;
    uint32_t line;
    const char *file;
    const char *function;
    uint32_t timestamp;
} error_info_t;

// Error handling function
int handle_error(error_code_t code, const char *file, int line, const char *function) {
    error_info_t error = {
        .code = code,
        .line = line,
        .file = file,
        .function = function,
        .timestamp = get_system_time()
    };
    
    // Log error
    log_error(&error);
    
    // Handle error based on severity
    switch (code) {
        case ERROR_HARDWARE_FAULT:
        case ERROR_FIRMWARE_CORRUPTION:
            // Critical error - enter recovery mode
            enter_recovery_mode(RECOVERY_MODE_SAFE);
            break;
            
        case ERROR_MEMORY_ALLOCATION:
        case ERROR_TIMEOUT:
            // Non-critical error - retry or continue
            break;
            
        default:
            // Log and continue
            break;
    }
    
    return code;
}

// Error handling macro
#define HANDLE_ERROR(code) handle_error(code, __FILE__, __LINE__, __FUNCTION__)
```

### Logging System

#### Logging Framework
```c
// Log levels
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRITICAL
} log_level_t;

// Log entry structure
typedef struct {
    log_level_t level;
    uint32_t timestamp;
    uint32_t task_id;
    char message[256];
} log_entry_t;

// Logging function
void log_message(log_level_t level, const char *format, ...) {
    log_entry_t entry;
    va_list args;
    
    entry.level = level;
    entry.timestamp = get_system_time();
    entry.task_id = get_current_task_id();
    
    va_start(args, format);
    vsnprintf(entry.message, sizeof(entry.message), format, args);
    va_end(args);
    
    // Write to log buffer
    write_log_entry(&entry);
    
    // Output to console if enabled
    if (level >= get_log_level()) {
        printf("[%u] %s: %s\n", entry.timestamp, get_log_level_string(level), entry.message);
    }
}

// Log macros
#define LOG_DEBUG(format, ...) log_message(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) log_message(LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) log_message(LOG_LEVEL_WARNING, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) log_message(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)
#define LOG_CRITICAL(format, ...) log_message(LOG_LEVEL_CRITICAL, format, ##__VA_ARGS__)
```

---

## Cross-compilation Setup

### Toolchain Configuration

#### ARM Toolchain Setup
```bash
# Install ARM toolchain
sudo apt-get install gcc-arm-none-eabi binutils-arm-none-eabi

# Set up environment variables
export ARM_TOOLCHAIN_PATH=/usr/bin
export PATH=$PATH:$ARM_TOOLCHAIN_PATH

# Verify installation
arm-none-eabi-gcc --version
```

#### Cross-compilation Makefile
```makefile
# Cross-compilation Makefile
CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

# Compiler flags
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
         -O2 -g -Wall -Wextra -std=c99 \
         -fno-stack-protector -fomit-frame-pointer \
         -ffunction-sections -fdata-sections

# Linker flags
LDFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
          -T$(LINKER_SCRIPT) -Wl,--gc-sections \
          -Wl,--print-memory-usage

# Source files
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

# Target
TARGET = firmware

# Default target
all: $(TARGET).elf $(TARGET).bin $(TARGET).hex

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

# Generate binary file
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Generate hex file
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Clean
clean:
	rm -f *.o *.elf *.bin *.hex

.PHONY: all clean
```

### Build Environment

#### CMake Configuration
```cmake
# CMakeLists.txt for cross-compilation
cmake_minimum_required(VERSION 3.10)

# Set cross-compilation
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Toolchain path
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

# Compiler flags
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_RELEASE "-O2")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_SOURCE_DIR}/linker.ld -Wl,--gc-sections")

# Project
project(embedded_firmware C ASM)

# Source files
file(GLOB_RECURSE SOURCES "src/*.c" "src/*.s")

# Create executable
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Custom target for binary
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE>
)
```

---

## Build Systems

### Make-based Build System

#### Advanced Makefile
```makefile
# Advanced Makefile with multiple targets
# Configuration
BOARD ?= stm32f4
DEBUG ?= 1
OPTIMIZATION ?= 2

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Include directories
INCLUDES = -I$(INC_DIR) -I$(SRC_DIR)

# Compiler flags
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
         -O$(OPTIMIZATION) -g$(if $(DEBUG),, -s) \
         -Wall -Wextra -std=c99 \
         -fno-stack-protector -fomit-frame-pointer \
         -ffunction-sections -fdata-sections \
         $(INCLUDES)

# Linker flags
LDFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
          -T$(LINKER_SCRIPT) -Wl,--gc-sections \
          -Wl,--print-memory-usage

# Targets
.PHONY: all clean flash debug release

all: $(BIN_DIR)/firmware.elf

debug: CFLAGS += -DDEBUG
debug: all

release: DEBUG = 0
release: OPTIMIZATION = 3
release: all

# Compile objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(BIN_DIR)/firmware.elf: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

# Flash target
flash: $(BIN_DIR)/firmware.elf
	$(FLASH_TOOL) -c $(FLASH_CONFIG) -d $(DEVICE) -f $<

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
```

### CMake-based Build System

#### Modern CMake Configuration
```cmake
# Modern CMake configuration
cmake_minimum_required(VERSION 3.15)

# Project configuration
project(EmbeddedFirmware VERSION 1.0.0 LANGUAGES C ASM)

# Options
option(BUILD_TESTS "Build tests" OFF)
option(ENABLE_DEBUG "Enable debug build" ON)
option(ENABLE_OPTIMIZATION "Enable optimization" ON)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fno-stack-protector")

if(ENABLE_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -DDEBUG")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -s")
endif()

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_SOURCE_DIR}/linker.ld -Wl,--gc-sections")

# Find source files
file(GLOB_RECURSE SOURCES "src/*.c" "src/*.s")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Add compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    BOARD_${BOARD}
    $<$<BOOL:${ENABLE_DEBUG}>:DEBUG>
)

# Custom target for binary
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE>
    COMMENT "Generating binary and size report"
)
```

---

## Version Control for Embedded Projects

### Git Configuration

#### .gitignore for Embedded Projects
```gitignore
# Build artifacts
build/
bin/
*.o
*.elf
*.bin
*.hex
*.map
*.lst

# IDE files
.vscode/
.idea/
*.swp
*.swo

# Dependencies
vendor/
third_party/

# Documentation
docs/_build/

# Logs
*.log
logs/

# Temporary files
*.tmp
*.bak
*~

# OS files
.DS_Store
Thumbs.db
```

#### Git Hooks
```bash
#!/bin/bash
# pre-commit hook for embedded projects

# Check for trailing whitespace
if git diff --cached --check; then
    echo "Error: Trailing whitespace found"
    exit 1
fi

# Check for debug code
if git diff --cached | grep -i "debug\|printf\|fprintf"; then
    echo "Warning: Debug code found in commit"
    read -p "Continue anyway? (y/N) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Run static analysis
if command -v cppcheck >/dev/null 2>&1; then
    echo "Running static analysis..."
    cppcheck --error-exitcode=1 --enable=all src/
fi
```

### Version Management

#### Semantic Versioning
```c
// Version information
typedef struct {
    uint8_t major;
    uint8_t minor;
    uint16_t patch;
    uint32_t build;
} version_info_t;

// Version string
#define VERSION_STRING "1.2.3"

// Version information
const version_info_t firmware_version = {
    .major = 1,
    .minor = 2,
    .patch = 3,
    .build = BUILD_NUMBER
};

// Get version string
const char* get_version_string(void) {
    static char version_str[32];
    snprintf(version_str, sizeof(version_str), "%d.%d.%d.%d",
             firmware_version.major,
             firmware_version.minor,
             firmware_version.patch,
             firmware_version.build);
    return version_str;
}
```

#### Version Compatibility
```c
// Version compatibility check
int check_version_compatibility(version_info_t *required_version) {
    // Check major version
    if (firmware_version.major != required_version->major) {
        return -1;  // Incompatible major version
    }
    
    // Check minor version
    if (firmware_version.minor < required_version->minor) {
        return -1;  // Required minor version not met
    }
    
    // Check patch version
    if (firmware_version.minor == required_version->minor &&
        firmware_version.patch < required_version->patch) {
        return -1;  // Required patch version not met
    }
    
    return 0;  // Compatible
}
```

---

## System Integration Best Practices

### Integration Checklist
- [ ] Verify hardware compatibility
- [ ] Test bootloader functionality
- [ ] Validate firmware update process
- [ ] Implement error handling and recovery
- [ ] Set up logging and monitoring
- [ ] Configure build system
- [ ] Establish version control workflow
- [ ] Document integration procedures

### Common Integration Issues
1. **Memory layout conflicts** - ensure proper memory mapping
2. **Interrupt vector conflicts** - coordinate interrupt usage
3. **Resource conflicts** - manage shared resources
4. **Timing issues** - verify real-time constraints
5. **Communication problems** - test all interfaces

### Integration Testing
```c
// Integration test framework
typedef struct {
    const char *test_name;
    int (*test_function)(void);
    int expected_result;
} integration_test_t;

// Run integration tests
int run_integration_tests(void) {
    integration_test_t tests[] = {
        {"Hardware Initialization", test_hardware_init, 0},
        {"Bootloader Functionality", test_bootloader, 0},
        {"Firmware Update", test_firmware_update, 0},
        {"Error Handling", test_error_handling, 0},
        {"Recovery Mode", test_recovery_mode, 0}
    };
    
    int passed = 0;
    int total = sizeof(tests) / sizeof(tests[0]);
    
    for (int i = 0; i < total; i++) {
        printf("Running test: %s\n", tests[i].test_name);
        
        int result = tests[i].test_function();
        if (result == tests[i].expected_result) {
            printf("PASS: %s\n", tests[i].test_name);
            passed++;
        } else {
            printf("FAIL: %s (expected %d, got %d)\n", 
                   tests[i].test_name, tests[i].expected_result, result);
        }
    }
    
    printf("Integration tests: %d/%d passed\n", passed, total);
    return (passed == total) ? 0 : -1;
}
```

---

## Resources

### Tools and Software
- [ARM GCC Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain)
- [OpenOCD](http://openocd.org/) - Open-source debugger
- [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
- [J-Link Software](https://www.segger.com/downloads/jlink/)

### Documentation
- [ARM Cortex-M Programming](https://developer.arm.com/documentation/den0013/d)
- [STM32 Reference Manuals](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html)
- [Embedded Systems Design](https://www.embedded.com/)

### Books and References
- "Making Embedded Systems" by Elecia White
- "Embedded Systems Design" by Arnold Berger
- "The Art of Programming Embedded Systems" by Jack Ganssle
