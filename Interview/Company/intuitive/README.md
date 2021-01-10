## Fan Control
#### Usage
```
make
./stack
```

## Task 
Your task is to develop an application to control fan speeds. The application should meet the following
requirements:

    • The temperature of each subsystem is provided as a 32-bit floating point number in °C via IPC.

    • The number of subsystems and the number of fans present should both be configurable at startup, but you may assume that each of these numbers has an upper bound. You may assume that the number of each is constant after startup.

    • The speed of each fan is set by writing a 32-bit unsigned integer to a specific hardware register that is different for each fan. This integer is in PWM counts and is proportional to fan duty cycle.

    • The PWM counts corresponding to 100% duty cycle may be different for different fans. You may assume that 0 PWM counts always represents 0% duty cycle

The fan control algorithm should behave as follows:

    1. The most recent temperature measurements from each subsystem should be collected, and the fan duty cycle should be computed from the maximum of the most recent temperatures of all subsystems.

    2. All fans should be set to the same duty cycle.

    3. If the temperature is 25° C or below, the fans should run at 20% duty cycle.

    4. If the temperature is 75° C or above, the fans should run at 100% duty cycle.

    5. If the maximum measured subsystem temperature is in between 25° C and 75° C, the fans should run at a
    duty cycle linearly interpolated between 20% and 100% duty cycle.

The submission should include:

    1. a small demo program to communicate subsystem temperatures and write fan duty
    cycle in PWM counts. 

    2. Minimalist interfaces for reading temperature measurements over IPC, configuring the application, and writing to hardware registers should be mocked out as you see fit. 

For your test program, you
may make up the number of fans, the number of subsystems, and the max PWM counts of each fan as you
please.

## Analysis
### Architecture

### Data Structure and function prototype

***FAN***
```C
/* 
 * Fan structure to control fan speed. Components:
 * 
 * 1. Current fan speed set last time. 
 * 2. Read_speed callback function to read current fan speed.
 * 3. Set_speed callback function to set fan speed.
 * 4. Module ID to indicate which module it belongs to.
 * 
 * set_speed callback function takes two arguments:
 * 1. PWM_counts.  0 for 0 duty cycle, 100 for 100% duty cycle.
 * 2. Register address to map for control speed.
 * 
 * read_speed callback function takes two arguments:
 * 1. value.  speed value read back from fan.
 * 2. Register address to map for read speed.
 * 
 * Speed callback function return status code: 0 for success, 
 * failed otherwise.
 * 
 * Use callback function so that
 * fan from different vendors can have their own set_speed callback
 * that converts PWM_counts to corresponding duty cycle.
 */
typedef struct {
    uint8_t module_id;
    uint32_t current_spd;
    int (*read_speed)(uint32_t* value, uint32_t REG_addr);
    int (*set_speed)(uint32_t PWM_counts, uint32_t REG_addr);
} Fan, *pFan;
```

***Message convey between sub module and fan control***
```C
double temperature;
```

***Fan control module***
```C
/*
 * Log message function to log events calssified by different levels.
 * Can directly print or send to log module. 
 */
void log_msg(char* message, int level) {
    if (level <= DEBUG_LEVEL)
        printf("%s\n");
}



```
