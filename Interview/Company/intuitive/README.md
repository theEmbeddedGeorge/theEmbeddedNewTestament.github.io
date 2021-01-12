## Fan Control App demo 

**Table of Contents:**
- Project Objective
- App Demo Design Analysis
  - Architecture
  - Feature
  - Limitation
  - Improvments
- Usage 
  - Compilation and cleanup
  - Run the demo program
  - Stop the demo program
- Demo Result




## Project Objective
The task is to develop an application to control fan speeds. The application should meet the following
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

## App Demo Design Analysis
### Architecture

### Feature

## Usage
### Compilation and cleanup
***Build:***
```
    make all
```

***Clean up:***
```
    make clean
```

### Run the demo program
***Run fan_control server:***
```
    sudo ./fan_control_server <number-of-module>|<Enter>
```
It takes first argument as the number of module allowed to be connected. If unspecified, 20 will be used as the default value. 

***Run fan_control server:***
```
    sudo ./fan_control_client <module-id>
```
It takes first argument as the module ID. This value will be used by the server to identify the module. Please note that if a module with the same ID already exists, client will not run.

```NOTE: Please use 'sudo' to run the demo program. Otherwise message queues will not be created!```

### Stop the demo program

Use Ctrl-C to generate termination signal to each program.

## Demo Result
```
    sudo ./fan_control_server 10

    sudo ./fan_control_client 0
    sudo ./fan_control_client 1
    sudo ./fan_control_client 2
    sudo ./fan_control_client 3
```

***Fan_control server output:***
```
Max module number to be conncted: 10
Start receiving from message queue!
timer_handler triggered.

No active module. Continue.

timer_handler triggered.

Server: temp val 0.000000 from module 0.
Server: client 0 queue name /fan-control-client-0
Start receiving from message queue!
Server: temp val 93.000000 from module 0.
Server: Received urgent msg from module 0. Adjust speed now!
Start receiving from message queue!
timer_handler triggered.

=======================
Active module number: 1
Max temperature: 93
Temperature by Module: 93
[93 0 0 0 0 0 0 0 0 0 ]
Fan speed duty cycle: 100
=======================

Server: Query all active module for temperature.
Server: temp val 29.000000 from module 0.
Start receiving from message queue!
Server: temp val 0.000000 from module 1.
Server: client 1 queue name /fan-control-client-1
Start receiving from message queue!
Server: temp val 93.000000 from module 1.
Server: Received urgent msg from module 1. Adjust speed now!
Start receiving from message queue!
timer_handler triggered.

=======================
Active module number: 2
Max temperature: 93
Temperature by Module: 93
[29 93 0 0 0 0 0 0 0 0 ]
Fan speed duty cycle: 100
=======================

```

***Fan_control client 0 output:***
```
Module ID: 0
Client 0: Send module ATTACH message.
Client 0 Temp_val: 83
Client 0 Temp_val: 86
Client 0 Temp_val: 77
Client 0 Temp_val: 15
Client 0 Temp_val: 93
Client 0: Module Temperature above threshold! Send urgent message!.
Client 0 Temp_val: 35
Client 0: Receive msg from server. Reply..
Client 0 Temp_val: 86
Client 0 Temp_val: 92
Client 0: Module Temperature above threshold! Send urgent message!.
Client 0 Temp_val: 49
Client 0 Temp_val: 21
Client 0 Temp_val: 62
Client 0: Receive msg from server. Reply..
Client 0 Temp_val: 27
Client 0 Temp_val: 90
Client 0 Temp_val: 59
Client 0 Temp_val: 63
Client 0 Temp_val: 26
Client 0: Receive msg from server. Reply..
```

***Fan_control client 1 output:***
```
Module ID: 1
Client 1: Send module ATTACH message.
Client 1 Temp_val: 83
Client 1 Temp_val: 86
Client 1 Temp_val: 77
Client 1 Temp_val: 15
Client 1 Temp_val: 93
Client 1: Module Temperature above threshold! Send urgent message!.
Client 1 Temp_val: 35
Client 1: Receive msg from server. Reply..
Client 1 Temp_val: 86
Client 1 Temp_val: 92
Client 1: Module Temperature above threshold! Send urgent message!.
Client 1 Temp_val: 49
Client 1 Temp_val: 21
Client 1 Temp_val: 62
Client 1: Receive msg from server. Reply..

```
