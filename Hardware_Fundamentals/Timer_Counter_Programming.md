> ## 🚀 Practice & deep-dive on EmbeddedInterviewLab
>
> Get these hardware concepts as ranked interview questions with model answers, plus interactive deep-dive guides.
>
> 👉 **[Browse peripheral & hardware questions →](https://embeddedinterviewlab.com/questions/domain/peripherals?utm_source=github&utm_medium=referral&utm_campaign=kb_cta&utm_content=hardware_fundamentals)** &nbsp;·&nbsp; **[Read the in-depth guide →](https://embeddedinterviewlab.com/topics/timers-pwm?utm_source=github&utm_medium=referral&utm_campaign=kb_topic&utm_content=hardware_fundamentals)**

---

# ⏱️ Timer/Counter Programming

> **Mastering Timer and Counter Operations for Embedded Systems**  
> Input capture, output compare, frequency measurement, and timing applications

---

## 📋 **Table of Contents**

- [Overview](#overview)
- [Quick Reference: Key Facts](#quick-reference-key-facts)
- [Visual Understanding](#visual-understanding)
- [Conceptual Foundation](#conceptual-foundation)
- [Core Concepts](#core-concepts)
- [Practical Considerations](#practical-considerations)
- [Additional Resources](#additional-resources)

---

## 🎯 **Overview**

Timers and counters are essential peripherals in embedded systems for precise timing, frequency measurement, PWM generation, and event counting. Understanding timer programming is crucial for real-time applications.

---

## 🚀 **Quick Reference: Key Facts**

- **Timer Modes**: Up-counting, down-counting, center-aligned
- **Input Capture**: Edge detection, frequency measurement, pulse width measurement
- **Output Compare**: PWM generation, timing control, waveform generation
- **Prescaler**: Divides clock frequency to achieve desired timing resolution
- **Auto-reload**: Automatically reloads counter value for continuous operation
- **Interrupts**: Timer interrupts for precise timing events
- **DMA Integration**: High-speed data transfer without CPU intervention

### **Interviewer intent (what they’re probing)**
- Can you compute timer frequency/period from clock and prescaler?
- Do you understand capture vs compare use cases?
- Can you reason about jitter, ISR cost, and DMA trade‑offs?

---

## 🔍 **Visual Understanding**

### **Timer Block Diagram**
```
Clock Source → Prescaler → Counter → Compare/Output → Interrupt/DMA
     ↓            ↓         ↓           ↓              ↓
System Clock  Divide by   Count Up/   Generate      Trigger
(84 MHz)      PSC+1      Down/      PWM/Events    CPU Events
              Center
```

### **Timer Modes Visualization**
```
Up-Counting:    0 → 1 → 2 → ... → ARR → 0 → 1 → ...
Down-Counting:  ARR → ARR-1 → ... → 1 → 0 → ARR → ...
Center-Aligned: 0 → 1 → ... → ARR → ARR-1 → ... → 1 → 0 → ...
```

### **Input Capture vs Output Compare**
```
Input Capture:  External Event → Capture Timer Value → Calculate Timing
                     ↓                ↓                ↓
                GPIO Edge         Store Count      Frequency/Pulse
                                 in Register      Width

Output Compare: Timer Count → Compare with CCR → Generate Output Event
                     ↓            ↓              ↓
                Increment      Match Value    PWM/Interrupt
                Counter       (CCR Register)  Generation
```

---

## 🧠 **Conceptual Foundation**

### **The Timer as a Time Foundation**
Timers serve as the fundamental building blocks for all time-related operations in embedded systems. They provide:
- **Precise Timing**: Hardware-based timing with minimal jitter
- **Event Scheduling**: Predictable execution of periodic tasks
- **Measurement Capability**: Accurate frequency and time interval measurement
- **Waveform Generation**: Creation of precise timing patterns and PWM signals

### **Why Timer Programming Matters**
Timer programming is critical because:
- **Real-Time Requirements**: Many embedded applications require precise timing
- **System Synchronization**: Coordinating multiple system events and peripherals
- **Power Efficiency**: Timers enable sleep modes and wake-up timing
- **Performance Optimization**: Hardware timers offload timing tasks from the CPU

### **The Timer Design Challenge**
Designing timer systems involves balancing several competing concerns:
- **Resolution vs. Range**: Higher resolution (smaller prescaler) reduces maximum period
- **Accuracy vs. Complexity**: More precise timing requires careful configuration
- **Hardware vs. Software**: Hardware timers vs. software timing loops
- **Interrupt Frequency**: Balancing timing precision with system overhead

---

## 🎯 **Core Concepts**

### **Concept: Timer Configuration and Frequency Calculation**

**Why it matters**: Proper timer configuration ensures accurate timing and prevents overflow errors. Understanding the relationship between clock frequency, prescaler, and period is essential for achieving desired timing resolution and range.

**Minimal example**
```c
// Basic timer configuration structure
typedef struct {
    uint32_t prescaler;      // Timer prescaler value
    uint32_t period;         // Timer period (ARR value)
    uint32_t clock_freq;     // Timer clock frequency
    uint32_t mode;           // Timer mode (UP, DOWN, CENTER)
} timer_config_t;

// Calculate timer frequency
uint32_t calculate_timer_frequency(uint32_t clock_freq, uint32_t prescaler, uint32_t period) {
    return clock_freq / ((prescaler + 1) * (period + 1));
}

// Calculate timer period for target frequency
uint32_t calculate_timer_period(uint32_t clock_freq, uint32_t prescaler, uint32_t target_freq) {
    return (clock_freq / ((prescaler + 1) * target_freq)) - 1;
}
```

**Try it**: Calculate the prescaler and period values needed for a 1kHz timer using an 84MHz clock.

**Takeaways**
- Prescaler divides the input clock frequency
- Period determines the timer overflow frequency
- Higher prescaler values provide longer timing periods
- Always verify calculations to prevent overflow

### **Concept: Input Capture for Precise Timing Measurement**

**Why it matters**: Input capture enables precise measurement of external events, making it essential for frequency measurement, pulse width analysis, and event timing in embedded systems.

**Minimal example**
```c
// Input capture configuration
typedef struct {
    uint32_t channel;        // Timer channel (1-4)
    uint32_t edge;           // Rising/Falling edge
    uint32_t filter;         // Input filter value
    bool interrupt_enable;   // Enable capture interrupt
} input_capture_config_t;

// Configure input capture
void configure_input_capture(TIM_HandleTypeDef* htim, input_capture_config_t* config) {
    // Configure channel as input
    TIM_IC_InitTypeDef sConfigIC = {0};
    sConfigIC.ICPolarity = config->edge;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = config->filter;
    
    HAL_TIM_IC_ConfigChannel(htim, &sConfigIC, config->channel);
    
    // Enable interrupt if requested
    if (config->interrupt_enable) {
        __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1 << (config->channel - 1));
    }
}
```

**Try it**: Implement input capture to measure the frequency of an external signal.

**Takeaways**
- Input capture stores timer value when external event occurs
- Edge selection affects measurement accuracy
- Input filtering reduces noise sensitivity
- Interrupts enable real-time event processing

### **Concept: Output Compare for Precise Event Generation**

**Why it matters**: Output compare enables precise generation of timing events, PWM signals, and periodic outputs. It's fundamental for motor control, audio generation, and timing synchronization.

**Minimal example**
```c
// Output compare configuration
typedef struct {
    uint32_t channel;        // Timer channel (1-4)
    uint32_t compare_value;  // Compare value (CCR)
    uint32_t mode;           // Output mode (Toggle, PWM, Force)
    bool interrupt_enable;   // Enable compare interrupt
} output_compare_config_t;

// Configure output compare
void configure_output_compare(TIM_HandleTypeDef* htim, output_compare_config_t* config) {
    // Configure channel as output
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = config->mode;
    sConfigOC.Pulse = config->compare_value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    HAL_TIM_OC_ConfigChannel(htim, &sConfigOC, config->channel);
    
    // Enable interrupt if requested
    if (config->interrupt_enable) {
        __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1 << (config->channel - 1));
    }
}
```

**Try it**: Generate a 1kHz square wave with 50% duty cycle using output compare.

**Takeaways**
- Output compare generates events when timer matches compare value
- Compare value determines timing of output events
- Multiple channels enable complex timing patterns
- Interrupts provide precise event notification

### **Concept: Timer Interrupts and DMA Integration**

**Why it matters**: Timer interrupts and DMA integration enable efficient handling of timing events and high-speed data transfer, reducing CPU overhead and improving system performance.

**Minimal example**
```c
// Timer interrupt configuration
void configure_timer_interrupt(TIM_HandleTypeDef* htim, uint32_t priority) {
    // Enable timer update interrupt
    __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
    
    // Configure NVIC priority
    HAL_NVIC_SetPriority(TIM2_IRQn, priority, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

// Timer interrupt handler
void TIM2_IRQHandler(void) {
    if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET) {
        if (__HAL_TIM_GET_IT_SOURCE(&htim2, TIM_IT_UPDATE) != RESET) {
            __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
            
            // Handle timer event
            handle_timer_event();
        }
    }
}
```

**Try it**: Implement a timer interrupt that toggles an LED every 500ms.

**Takeaways**
- Timer interrupts provide precise timing for event handling
- Keep interrupt handlers short and efficient
- Use DMA for high-speed data transfer
- Proper priority configuration prevents timing conflicts

---

## 🧪 **Guided Labs**

### **Lab 1: Basic Timer Configuration and Interrupts**
**Objective**: Configure a timer to generate periodic interrupts and measure timing accuracy.

**Steps**:
1. Configure a timer for 1kHz operation
2. Enable timer interrupts
3. Toggle GPIO pin in interrupt handler
4. Measure timing accuracy with oscilloscope

**Expected Outcome**: Understanding of timer configuration and interrupt timing.

### **Lab 2: Input Capture for Frequency Measurement**
**Objective**: Use input capture to measure the frequency of an external signal.

**Steps**:
1. Configure timer channel for input capture
2. Generate test signal with function generator
3. Implement frequency calculation algorithm
4. Compare measured vs. actual frequency

**Expected Outcome**: Practical experience with input capture and frequency measurement.

### **Lab 3: Output Compare for PWM Generation**
**Objective**: Generate PWM signals with variable duty cycle using output compare.

**Steps**:
1. Configure timer for PWM mode
2. Implement duty cycle control
3. Generate different PWM frequencies
4. Measure PWM characteristics with oscilloscope

**Expected Outcome**: Understanding of PWM generation and output compare operation.

---

## ✅ **Check Yourself**

### **Basic Understanding**
- What is the difference between timer and counter modes?
- How do you calculate timer frequency from clock, prescaler, and period?
- What are the main applications of input capture and output compare?

### **Practical Application**
- How would you configure a timer for 100Hz operation with 1ms resolution?
- What considerations are important when choosing timer prescaler values?
- How do you implement precise timing measurement using timers?

### **Advanced Concepts**
- How do you handle timer overflow in long-duration measurements?
- What are the trade-offs between hardware and software timing?
- How do you synchronize multiple timers for complex applications?

---

## 🔗 **Cross-links**

- **[GPIO Configuration](./GPIO_Configuration.md)** - GPIO modes, configuration, electrical characteristics
- **[Pulse Width Modulation](./Pulse_Width_Modulation.md)** - PWM generation, frequency control, duty cycle
- **[External Interrupts](./External_Interrupts.md)** - Edge/level triggered interrupts, debouncing
- **[Interrupts and Exceptions](./Interrupts_Exceptions.md)** - Interrupt handling and ISR design
- **[Hardware Abstraction Layer](./Hardware_Abstraction_Layer.md)** - Timer abstraction and portability

---

## 🎯 **Practical Considerations**

### **System-Level Design Decisions**
- **Timer Selection**: Choose appropriate timer based on resolution and range requirements
- **Interrupt Frequency**: Balance timing precision with system overhead
- **Resource Allocation**: Consider timer sharing between multiple applications

### **Performance and Optimization**
- **Prescaler Selection**: Optimize for desired timing resolution and range
- **Interrupt Efficiency**: Minimize ISR execution time
- **DMA Usage**: Use DMA for high-speed timer applications

### **Debugging and Testing**
- **Timing Verification**: Use oscilloscope or logic analyzer to verify timing
- **Interrupt Debugging**: Monitor interrupt timing and frequency
- **Performance Profiling**: Measure timer accuracy and jitter

---

## 📚 **Additional Resources**

### **Documentation**
- [STM32 Timer Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [ARM Cortex-M Timer Programming](https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/peripherals/general-purpose-timers)

### **Tools**
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) - Timer configuration
- [Timer Calculator](https://www.st.com/resource/en/user_manual/dm00104712-stm32cubemx-user-manual-stmicroelectronics.pdf)

### **Books**
- "Embedded Systems: Introduction to ARM Cortex-M Microcontrollers" by Jonathan Valvano
- "Making Embedded Systems" by Elecia White

---

**Next Topic:** [Watchdog Timers](./Watchdog_Timers.md) → [Interrupts and Exceptions](./Interrupts_Exceptions.md)
