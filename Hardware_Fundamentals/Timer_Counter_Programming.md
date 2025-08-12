# ⏱️ Timer/Counter Programming

> **Mastering Timer and Counter Operations for Embedded Systems**  
> Input capture, output compare, frequency measurement, and timing applications

## 📋 Table of Contents

- [🎯 Overview](#-overview)
- [🔧 Timer Fundamentals](#-timer-fundamentals)
- [⚙️ Timer Configuration](#️-timer-configuration)
- [📊 Input Capture](#-input-capture)
- [🎛️ Output Compare](#️-output-compare)
- [🔄 Frequency Measurement](#-frequency-measurement)
- [⚡ PWM Generation](#-pwm-generation)
- [🎯 Common Applications](#-common-applications)
- [⚠️ Common Pitfalls](#️-common-pitfalls)
- [✅ Best Practices](#-best-practices)
- [🎯 Interview Questions](#-interview-questions)
- [📚 Additional Resources](#-additional-resources)

---

## 🎯 Overview

### Concept: Timers are schedulers and timestampers

Use timers to measure, schedule, and generate pulses with bounded jitter. Configure prescalers and auto‑reload to match your required resolution and range.

### Why it matters in embedded
- Wrong prescaler causes drift or overflow.
- Input capture/compare enables precise measurements and pulse generation.
- Deterministic timing underpins PWM, communication timeouts, and RTOS ticks.

### Minimal example
```c
// Configure a periodic interrupt at 1 kHz
void tim_init_1khz(void){ /* enable clock, set PSC/ARR, enable IRQ */ }
void TIMx_IRQHandler(void){ /* clear flag; do bounded work; defer heavy work */ }
```

### Try it
1. Measure timer ISR jitter with a logic analyzer; vary IRQ priority.
2. Use input capture to time an external signal; compare to scope measurement.

### Takeaways
- Choose PSC/ARR from required frequency and clock.
- Keep ISRs short; use compare events for precise edges instead of software loops.
- Document time bases centrally to avoid drift errors.

---

## 🧪 Guided Labs
1) Timer as scheduler
- Configure a timer to trigger tasks at different intervals; measure timing accuracy.

2) ISR jitter measurement
- Use a timer ISR to toggle a pin; measure edge-to-edge timing with an oscilloscope.

## ✅ Check Yourself
- How do you calculate the maximum timer frequency for your MCU?
- When should you use hardware vs software timing?

## 🔗 Cross-links
- `Hardware_Fundamentals/Interrupts_Exceptions.md` for ISR handling
- `Embedded_C/Type_Qualifiers.md` for volatile access

Timers and counters are essential peripherals in embedded systems for precise timing, frequency measurement, PWM generation, and event counting. Understanding timer programming is crucial for real-time applications.

**Key Concepts:**
- **Timer Modes**: Up-counting, down-counting, center-aligned
- **Input Capture**: Edge detection, frequency measurement, pulse width measurement
- **Output Compare**: PWM generation, timing control, waveform generation
- **Interrupts**: Timer interrupts, DMA integration

---

## 🔧 Timer Fundamentals

### **Timer Parameters**
```c
// Timer configuration structure
typedef struct {
    uint32_t prescaler;      // Timer prescaler value
    uint32_t period;         // Timer period (ARR value)
    uint32_t clock_freq;     // Timer clock frequency
    uint32_t mode;           // Timer mode (UP, DOWN, CENTER)
    uint32_t auto_reload;    // Auto-reload enable/disable
} Timer_Config_t;

// Timer modes
#define TIMER_MODE_UP      0x00
#define TIMER_MODE_DOWN    0x01
#define TIMER_MODE_CENTER  0x02

// Calculate timer frequency
uint32_t timer_frequency_calculate(uint32_t clock_freq, uint32_t prescaler, uint32_t period) {
    return clock_freq / ((prescaler + 1) * (period + 1));
}

// Calculate timer period for target frequency
uint32_t timer_period_calculate(uint32_t clock_freq, uint32_t prescaler, uint32_t target_freq) {
    return (clock_freq / ((prescaler + 1) * target_freq)) - 1;
}
```

### **Timer Channels**
```c
// Timer channel configuration
typedef struct {
    uint32_t channel;        // Timer channel (1-4)
    uint32_t mode;           // Channel mode (INPUT, OUTPUT, PWM)
    uint32_t polarity;       // Input/output polarity
    uint32_t filter;         // Input filter
} Timer_Channel_Config_t;

// Channel modes
#define TIMER_CHANNEL_INPUT   0x00
#define TIMER_CHANNEL_OUTPUT  0x01
#define TIMER_CHANNEL_PWM     0x02
```

---

## ⚙️ Timer Configuration

### **Basic Timer Configuration**
```c
// Configure timer for basic operation
void timer_basic_config(TIM_HandleTypeDef* htim, Timer_Config_t* config) {
    // Configure timer base
    htim->Instance = TIM2;
    htim->Init.Prescaler = config->prescaler;
    htim->Init.CounterMode = config->mode;
    htim->Init.Period = config->period;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = config->auto_reload ? TIM_AUTORELOAD_PRELOAD_ENABLE : TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    HAL_TIM_Base_Init(htim);
}

// Initialize timer with default settings
void timer_init_default(TIM_HandleTypeDef* htim, uint32_t frequency) {
    Timer_Config_t config = {
        .prescaler = 0,
        .period = 999,
        .clock_freq = SystemCoreClock,
        .mode = TIMER_MODE_UP,
        .auto_reload = 1
    };
    
    // Calculate period for target frequency
    config.period = timer_period_calculate(config.clock_freq, config.prescaler, frequency);
    
    timer_basic_config(htim, &config);
}
```

### **Timer with Interrupts**
```c
// Configure timer with interrupts
void timer_interrupt_config(TIM_HandleTypeDef* htim) {
    // Enable timer update interrupt
    __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
    
    // Configure NVIC
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

// Timer interrupt handler
void TIM2_IRQHandler(void) {
    if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET) {
        if (__HAL_TIM_GET_IT_SOURCE(&htim2, TIM_IT_UPDATE) != RESET) {
            __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
            
            // Handle timer update
            timer_update_callback();
        }
    }
}
```

### **Timer with DMA**
```c
// Configure timer with DMA
void timer_dma_config(TIM_HandleTypeDef* htim, DMA_HandleTypeDef* hdma) {
    // Configure DMA for timer
    hdma->Instance = DMA1_Stream6;
    hdma->Init.Request = DMA_REQUEST_TIM2_UP;
    hdma->Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma->Init.PeriphInc = DMA_PINC_DISABLE;
    hdma->Init.MemInc = DMA_MINC_ENABLE;
    hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma->Init.Mode = DMA_NORMAL;
    hdma->Init.Priority = DMA_PRIORITY_HIGH;
    hdma->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    
    HAL_DMA_Init(hdma);
    __HAL_LINKDMA(htim, hdma[TIM_DMA_ID_UPDATE], *hdma);
}
```

---

## 📊 Input Capture

### **Input Capture Configuration**
```c
// Input capture configuration structure
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint32_t polarity;
    uint32_t filter;
    uint32_t prescaler;
} Input_Capture_Config_t;

void input_capture_config(Input_Capture_Config_t* ic_config) {
    TIM_IC_InitTypeDef sConfigIC = {0};
    
    // Configure input capture channel
    sConfigIC.ICPolarity = ic_config->polarity;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = ic_config->prescaler;
    sConfigIC.ICFilter = ic_config->filter;
    
    HAL_TIM_IC_ConfigChannel(ic_config->htim, &sConfigIC, ic_config->channel);
    
    // Start input capture
    HAL_TIM_IC_Start_IT(ic_config->htim, ic_config->channel);
}

// Input capture callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM2) {
        // Handle input capture
        uint32_t capture_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        input_capture_callback(capture_value);
    }
}
```

### **Frequency Measurement**
```c
// Frequency measurement structure
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint32_t last_capture;
    uint32_t frequency;
    uint32_t capture_count;
} Frequency_Measurement_t;

void frequency_measurement_init(Frequency_Measurement_t* freq_meas, TIM_HandleTypeDef* htim, uint32_t channel) {
    freq_meas->htim = htim;
    freq_meas->channel = channel;
    freq_meas->last_capture = 0;
    freq_meas->frequency = 0;
    freq_meas->capture_count = 0;
    
    // Configure input capture
    Input_Capture_Config_t ic_config = {
        .htim = htim,
        .channel = channel,
        .polarity = TIM_ICPOLARITY_RISING,
        .filter = TIM_ICFILTER_FDIV1,
        .prescaler = TIM_ICPSC_DIV1
    };
    
    input_capture_config(&ic_config);
}

uint32_t frequency_measurement_calculate(Frequency_Measurement_t* freq_meas, uint32_t capture_value) {
    uint32_t period = 0;
    
    if (freq_meas->last_capture != 0) {
        if (capture_value > freq_meas->last_capture) {
            period = capture_value - freq_meas->last_capture;
        } else {
            // Timer overflow occurred
            period = (freq_meas->htim->Init.Period - freq_meas->last_capture) + capture_value;
        }
        
        // Calculate frequency
        uint32_t timer_freq = SystemCoreClock / (freq_meas->htim->Init.Prescaler + 1);
        freq_meas->frequency = timer_freq / period;
    }
    
    freq_meas->last_capture = capture_value;
    freq_meas->capture_count++;
    
    return freq_meas->frequency;
}
```

### **Pulse Width Measurement**
```c
// Pulse width measurement structure
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint32_t rising_edge;
    uint32_t falling_edge;
    uint32_t pulse_width;
    uint8_t edge_state;
} Pulse_Width_Measurement_t;

void pulse_width_measurement_init(Pulse_Width_Measurement_t* pwm_meas, TIM_HandleTypeDef* htim, uint32_t channel) {
    pwm_meas->htim = htim;
    pwm_meas->channel = channel;
    pwm_meas->rising_edge = 0;
    pwm_meas->falling_edge = 0;
    pwm_meas->pulse_width = 0;
    pwm_meas->edge_state = 0;
}

uint32_t pulse_width_measurement_update(Pulse_Width_Measurement_t* pwm_meas, uint32_t capture_value) {
    if (pwm_meas->edge_state == 0) {
        // Rising edge detected
        pwm_meas->rising_edge = capture_value;
        pwm_meas->edge_state = 1;
        
        // Configure for falling edge
        TIM_IC_InitTypeDef sConfigIC = {0};
        sConfigIC.ICPolarity = TIM_ICPOLARITY_FALLING;
        sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
        sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
        sConfigIC.ICFilter = TIM_ICFILTER_FDIV1;
        HAL_TIM_IC_ConfigChannel(pwm_meas->htim, &sConfigIC, pwm_meas->channel);
    } else {
        // Falling edge detected
        pwm_meas->falling_edge = capture_value;
        pwm_meas->edge_state = 0;
        
        // Calculate pulse width
        if (pwm_meas->falling_edge > pwm_meas->rising_edge) {
            pwm_meas->pulse_width = pwm_meas->falling_edge - pwm_meas->rising_edge;
        } else {
            // Timer overflow occurred
            pwm_meas->pulse_width = (pwm_meas->htim->Init.Period - pwm_meas->rising_edge) + pwm_meas->falling_edge;
        }
        
        // Configure for rising edge
        TIM_IC_InitTypeDef sConfigIC = {0};
        sConfigIC.ICPolarity = TIM_ICPOLARITY_RISING;
        sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
        sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
        sConfigIC.ICFilter = TIM_ICFILTER_FDIV1;
        HAL_TIM_IC_ConfigChannel(pwm_meas->htim, &sConfigIC, pwm_meas->channel);
    }
    
    return pwm_meas->pulse_width;
}
```

---

## 🎛️ Output Compare

### **Output Compare Configuration**
```c
// Output compare configuration structure
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint32_t compare_value;
    uint32_t mode;
    uint32_t polarity;
} Output_Compare_Config_t;

void output_compare_config(Output_Compare_Config_t* oc_config) {
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    // Configure output compare channel
    sConfigOC.OCMode = oc_config->mode;
    sConfigOC.Pulse = oc_config->compare_value;
    sConfigOC.OCPolarity = oc_config->polarity;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    HAL_TIM_OC_ConfigChannel(oc_config->htim, &sConfigOC, oc_config->channel);
    
    // Start output compare
    HAL_TIM_OC_Start(oc_config->htim, oc_config->channel);
}

// Set output compare value
void output_compare_set_value(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t compare_value) {
    __HAL_TIM_SET_COMPARE(htim, channel, compare_value);
}
```

### **PWM Generation**
```c
// PWM generation structure
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint32_t frequency;
    uint32_t duty_cycle;
    uint32_t period;
} PWM_Generation_t;

void pwm_generation_init(PWM_Generation_t* pwm_gen, TIM_HandleTypeDef* htim, uint32_t channel, 
                        uint32_t frequency, uint32_t duty_cycle) {
    pwm_gen->htim = htim;
    pwm_gen->channel = channel;
    pwm_gen->frequency = frequency;
    pwm_gen->duty_cycle = duty_cycle;
    
    // Calculate period
    uint32_t timer_freq = SystemCoreClock / (htim->Init.Prescaler + 1);
    pwm_gen->period = timer_freq / frequency;
    
    // Configure timer
    htim->Init.Period = pwm_gen->period - 1;
    HAL_TIM_Base_Init(htim);
    
    // Configure PWM channel
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = (pwm_gen->period * duty_cycle) / 100;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, channel);
    HAL_TIM_PWM_Start(htim, channel);
}

// Set PWM duty cycle
void pwm_generation_set_duty_cycle(PWM_Generation_t* pwm_gen, uint32_t duty_cycle) {
    pwm_gen->duty_cycle = duty_cycle;
    uint32_t compare_value = (pwm_gen->period * duty_cycle) / 100;
    __HAL_TIM_SET_COMPARE(pwm_gen->htim, pwm_gen->channel, compare_value);
}
```

---

## 🔄 Frequency Measurement

### **High-Frequency Measurement**
```c
// High-frequency measurement using timer
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t gate_time;
    uint32_t pulse_count;
    uint32_t frequency;
} High_Frequency_Measurement_t;

void high_frequency_measurement_init(High_Frequency_Measurement_t* hf_meas, TIM_HandleTypeDef* htim, uint32_t gate_time_ms) {
    hf_meas->htim = htim;
    hf_meas->gate_time = gate_time_ms;
    hf_meas->pulse_count = 0;
    hf_meas->frequency = 0;
    
    // Configure timer for external clock mode
    htim->Init.Prescaler = 0;
    htim->Init.Period = 0xFFFF;
    HAL_TIM_Base_Init(htim);
    
    // Configure external clock source
    __HAL_TIM_SET_CLOCKSOURCE(htim, TIM_CLOCKSOURCE_TI1);
    __HAL_TIM_SET_CLOCKSOURCE2(htim, TIM_CLOCKSOURCE2_TI1);
}

uint32_t high_frequency_measurement_read(High_Frequency_Measurement_t* hf_meas) {
    uint32_t start_count = __HAL_TIM_GET_COUNTER(hf_meas->htim);
    
    // Wait for gate time
    HAL_Delay(hf_meas->gate_time);
    
    uint32_t end_count = __HAL_TIM_GET_COUNTER(hf_meas->htim);
    uint32_t pulse_count = end_count - start_count;
    
    // Calculate frequency
    hf_meas->frequency = (pulse_count * 1000) / hf_meas->gate_time;
    
    return hf_meas->frequency;
}
```

### **Low-Frequency Measurement**
```c
// Low-frequency measurement using input capture
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint32_t measurement_time;
    uint32_t pulse_count;
    uint32_t frequency;
} Low_Frequency_Measurement_t;

void low_frequency_measurement_init(Low_Frequency_Measurement_t* lf_meas, TIM_HandleTypeDef* htim, 
                                   uint32_t channel, uint32_t measurement_time_ms) {
    lf_meas->htim = htim;
    lf_meas->channel = channel;
    lf_meas->measurement_time = measurement_time_ms;
    lf_meas->pulse_count = 0;
    lf_meas->frequency = 0;
    
    // Configure input capture
    Input_Capture_Config_t ic_config = {
        .htim = htim,
        .channel = channel,
        .polarity = TIM_ICPOLARITY_RISING,
        .filter = TIM_ICFILTER_FDIV1,
        .prescaler = TIM_ICPSC_DIV1
    };
    
    input_capture_config(&ic_config);
}

uint32_t low_frequency_measurement_read(Low_Frequency_Measurement_t* lf_meas) {
    lf_meas->pulse_count = 0;
    
    // Start measurement
    uint32_t start_time = HAL_GetTick();
    
    while ((HAL_GetTick() - start_time) < lf_meas->measurement_time) {
        // Count pulses in input capture callback
        // This would be implemented in the interrupt handler
    }
    
    // Calculate frequency
    lf_meas->frequency = (lf_meas->pulse_count * 1000) / lf_meas->measurement_time;
    
    return lf_meas->frequency;
}
```

---

## ⚡ PWM Generation

### **Variable Frequency PWM**
```c
// Variable frequency PWM generation
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint32_t min_frequency;
    uint32_t max_frequency;
    uint32_t current_frequency;
    uint32_t duty_cycle;
} Variable_Frequency_PWM_t;

void variable_frequency_pwm_init(Variable_Frequency_PWM_t* vf_pwm, TIM_HandleTypeDef* htim, 
                                uint32_t channel, uint32_t min_freq, uint32_t max_freq) {
    vf_pwm->htim = htim;
    vf_pwm->channel = channel;
    vf_pwm->min_frequency = min_freq;
    vf_pwm->max_frequency = max_freq;
    vf_pwm->current_frequency = min_freq;
    vf_pwm->duty_cycle = 50;
    
    // Configure initial PWM
    PWM_Generation_t pwm_gen;
    pwm_generation_init(&pwm_gen, htim, channel, min_freq, 50);
}

void variable_frequency_pwm_set_frequency(Variable_Frequency_PWM_t* vf_pwm, uint32_t frequency) {
    if (frequency < vf_pwm->min_frequency) {
        frequency = vf_pwm->min_frequency;
    } else if (frequency > vf_pwm->max_frequency) {
        frequency = vf_pwm->max_frequency;
    }
    
    vf_pwm->current_frequency = frequency;
    
    // Recalculate timer period
    uint32_t timer_freq = SystemCoreClock / (vf_pwm->htim->Init.Prescaler + 1);
    uint32_t period = timer_freq / frequency;
    
    // Update timer period
    __HAL_TIM_SET_AUTORELOAD(vf_pwm->htim, period - 1);
    
    // Update compare value for duty cycle
    uint32_t compare_value = (period * vf_pwm->duty_cycle) / 100;
    __HAL_TIM_SET_COMPARE(vf_pwm->htim, vf_pwm->channel, compare_value);
}
```

### **Multi-Channel PWM**
```c
// Multi-channel PWM generation
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channels[4];
    uint8_t channel_count;
    uint32_t frequency;
    uint32_t duty_cycles[4];
} Multi_Channel_PWM_t;

void multi_channel_pwm_init(Multi_Channel_PWM_t* mc_pwm, TIM_HandleTypeDef* htim, 
                           uint32_t* channels, uint8_t count, uint32_t frequency) {
    mc_pwm->htim = htim;
    mc_pwm->channel_count = count;
    mc_pwm->frequency = frequency;
    
    for (int i = 0; i < count; i++) {
        mc_pwm->channels[i] = channels[i];
        mc_pwm->duty_cycles[i] = 0;
    }
    
    // Configure timer
    uint32_t timer_freq = SystemCoreClock / (htim->Init.Prescaler + 1);
    uint32_t period = timer_freq / frequency;
    
    htim->Init.Period = period - 1;
    HAL_TIM_Base_Init(htim);
    
    // Configure each channel
    for (int i = 0; i < count; i++) {
        TIM_OC_InitTypeDef sConfigOC = {0};
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        
        HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, channels[i]);
        HAL_TIM_PWM_Start(htim, channels[i]);
    }
}

void multi_channel_pwm_set_duty_cycle(Multi_Channel_PWM_t* mc_pwm, uint8_t channel_index, uint32_t duty_cycle) {
    if (channel_index < mc_pwm->channel_count) {
        mc_pwm->duty_cycles[channel_index] = duty_cycle;
        
        uint32_t period = mc_pwm->htim->Init.Period + 1;
        uint32_t compare_value = (period * duty_cycle) / 100;
        
        __HAL_TIM_SET_COMPARE(mc_pwm->htim, mc_pwm->channels[channel_index], compare_value);
    }
}
```

---

## 🎯 Common Applications

### **Precise Timing**
```c
// Precise timing using timer
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t start_time;
    uint32_t elapsed_time;
} Precise_Timer_t;

void precise_timer_init(Precise_Timer_t* ptimer, TIM_HandleTypeDef* htim) {
    ptimer->htim = htim;
    ptimer->start_time = 0;
    ptimer->elapsed_time = 0;
    
    // Configure timer for microsecond resolution
    htim->Init.Prescaler = (SystemCoreClock / 1000000) - 1; // 1MHz timer
    htim->Init.Period = 0xFFFF;
    HAL_TIM_Base_Init(htim);
    HAL_TIM_Base_Start(htim);
}

void precise_timer_start(Precise_Timer_t* ptimer) {
    ptimer->start_time = __HAL_TIM_GET_COUNTER(ptimer->htim);
}

uint32_t precise_timer_elapsed(Precise_Timer_t* ptimer) {
    uint32_t current_time = __HAL_TIM_GET_COUNTER(ptimer->htim);
    
    if (current_time >= ptimer->start_time) {
        ptimer->elapsed_time = current_time - ptimer->start_time;
    } else {
        // Timer overflow occurred
        ptimer->elapsed_time = (0xFFFF - ptimer->start_time) + current_time;
    }
    
    return ptimer->elapsed_time;
}
```

### **Event Counting**
```c
// Event counting using timer
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t event_count;
    uint32_t last_count;
} Event_Counter_t;

void event_counter_init(Event_Counter_t* ecounter, TIM_HandleTypeDef* htim) {
    ecounter->htim = htim;
    ecounter->event_count = 0;
    ecounter->last_count = 0;
    
    // Configure timer for external clock mode
    htim->Init.Prescaler = 0;
    htim->Init.Period = 0xFFFF;
    HAL_TIM_Base_Init(htim);
    
    // Configure external clock source
    __HAL_TIM_SET_CLOCKSOURCE(htim, TIM_CLOCKSOURCE_TI1);
    HAL_TIM_Base_Start(htim);
}

uint32_t event_counter_read(Event_Counter_t* ecounter) {
    uint32_t current_count = __HAL_TIM_GET_COUNTER(ecounter->htim);
    
    if (current_count >= ecounter->last_count) {
        ecounter->event_count += current_count - ecounter->last_count;
    } else {
        // Timer overflow occurred
        ecounter->event_count += (0xFFFF - ecounter->last_count) + current_count;
    }
    
    ecounter->last_count = current_count;
    return ecounter->event_count;
}
```

---

## ⚠️ Common Pitfalls

### **1. Timer Overflow**
```c
// ❌ Wrong: No overflow handling
uint32_t timer_elapsed_wrong(TIM_HandleTypeDef* htim, uint32_t start_time) {
    uint32_t current_time = __HAL_TIM_GET_COUNTER(htim);
    return current_time - start_time; // Wrong if overflow occurred
}

// ✅ Correct: With overflow handling
uint32_t timer_elapsed_correct(TIM_HandleTypeDef* htim, uint32_t start_time) {
    uint32_t current_time = __HAL_TIM_GET_COUNTER(htim);
    
    if (current_time >= start_time) {
        return current_time - start_time;
    } else {
        // Timer overflow occurred
        return (htim->Init.Period - start_time) + current_time;
    }
}
```

### **2. Incorrect Frequency Calculation**
```c
// ❌ Wrong: Incorrect frequency calculation
uint32_t frequency_calculate_wrong(uint32_t period) {
    return 1000000 / period; // Wrong if timer prescaler is not 1MHz
}

// ✅ Correct: Proper frequency calculation
uint32_t frequency_calculate_correct(TIM_HandleTypeDef* htim, uint32_t period) {
    uint32_t timer_freq = SystemCoreClock / (htim->Init.Prescaler + 1);
    return timer_freq / period;
}
```

### **3. Missing Interrupt Configuration**
```c
// ❌ Wrong: No interrupt configuration
void timer_config_wrong(TIM_HandleTypeDef* htim) {
    HAL_TIM_Base_Init(htim);
    HAL_TIM_Base_Start(htim);
    // Missing interrupt configuration
}

// ✅ Correct: With interrupt configuration
void timer_config_correct(TIM_HandleTypeDef* htim) {
    HAL_TIM_Base_Init(htim);
    
    // Enable timer interrupt
    __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    
    HAL_TIM_Base_Start(htim);
}
```

---

## ✅ Best Practices

### **1. Use Appropriate Timer Resolution**
```c
// Use appropriate timer resolution for different applications
void configure_timer_resolution(TIM_HandleTypeDef* htim, uint8_t application_type) {
    switch (application_type) {
        case TIMER_HIGH_PRECISION:
            htim->Init.Prescaler = (SystemCoreClock / 1000000) - 1; // 1μs resolution
            break;
        case TIMER_MEDIUM_PRECISION:
            htim->Init.Prescaler = (SystemCoreClock / 1000) - 1; // 1ms resolution
            break;
        case TIMER_LOW_PRECISION:
            htim->Init.Prescaler = (SystemCoreClock / 100) - 1; // 10ms resolution
            break;
    }
}
```

### **2. Implement Proper Error Handling**
```c
// Always implement proper error handling for timer operations
HAL_StatusTypeDef timer_robust_operation(TIM_HandleTypeDef* htim) {
    HAL_StatusTypeDef status = HAL_TIM_Base_Init(htim);
    
    if (status != HAL_OK) {
        // Handle initialization error
        return status;
    }
    
    status = HAL_TIM_Base_Start(htim);
    if (status != HAL_OK) {
        // Handle start error
        return status;
    }
    
    return HAL_OK;
}
```

### **3. Use DMA for High-Speed Applications**
```c
// Use DMA for high-speed timer applications
void timer_dma_high_speed(TIM_HandleTypeDef* htim, DMA_HandleTypeDef* hdma, uint32_t* buffer, uint32_t size) {
    // Configure DMA for timer
    timer_dma_config(htim, hdma);
    
    // Start timer with DMA
    HAL_TIM_Base_Start_DMA(htim, TIM_DMA_UPDATE, (uint32_t*)buffer, size);
}
```

---

## 🎯 Interview Questions

### **Basic Questions**
1. **What is the difference between timer and counter?**
   - Timer: Counts internal clock, Counter: Counts external events

2. **What is input capture and how does it work?**
   - Captures timer value on external event, used for frequency/pulse width measurement

3. **What is output compare and its applications?**
   - Compares timer value with preset value, used for PWM, timing control

### **Advanced Questions**
1. **How do you measure frequency using a timer?**
   - Input capture method, gate time method, or frequency counter

2. **What is timer overflow and how do you handle it?**
   - Timer reaches maximum value and wraps around, handle with conditional logic

3. **How do you generate PWM with variable frequency?**
   - Dynamically adjust timer period and compare values

### **Practical Questions**
1. **Design a frequency counter using timer**
   - Input capture configuration, overflow handling, frequency calculation

2. **Implement PWM generation with duty cycle control**
   - Timer configuration, output compare, duty cycle calculation

3. **Create a precise timing system**
   - Timer configuration, overflow handling, microsecond resolution

---

## 📚 Additional Resources

### **Documentation**
- [STM32 Timer Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [ARM Cortex-M Timer Programming](https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/peripherals/general-purpose-timers)

### **Tools**
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) - Timer configuration
- [Timer Calculator](https://www.st.com/resource/en/user_manual/dm00104712-stm32cubemx-user-manual-stmicroelectronics.pdf)

### **Related Topics**
- **[GPIO Configuration](./GPIO_Configuration.md)** - GPIO modes, configuration, electrical characteristics
- **[Pulse Width Modulation](./Pulse_Width_Modulation.md)** - PWM generation, frequency control, duty cycle
- **[External Interrupts](./External_Interrupts.md)** - Edge/level triggered interrupts, debouncing

---

**Next Topic:** [External Interrupts](./External_Interrupts.md) → [Watchdog Timers](./Watchdog_Timers.md)
