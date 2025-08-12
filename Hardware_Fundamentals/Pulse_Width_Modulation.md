# ⏱️ Pulse Width Modulation

> **Mastering PWM for Embedded Systems**  
> PWM generation, frequency control, duty cycle, and practical applications

## 📋 Table of Contents

- [🎯 Overview](#-overview)
- [🔧 PWM Fundamentals](#-pwm-fundamentals)
- [⚙️ PWM Configuration](#️-pwm-configuration)
- [🎛️ Duty Cycle Control](#️-duty-cycle-control)
- [🔄 Frequency Control](#-frequency-control)
- [📊 PWM Applications](#-pwm-applications)
- [⚡ Advanced PWM Techniques](#-advanced-pwm-techniques)
- [🎯 Common Applications](#-common-applications)
- [⚠️ Common Pitfalls](#️-common-pitfalls)
- [✅ Best Practices](#-best-practices)
- [🎯 Interview Questions](#-interview-questions)
- [📚 Additional Resources](#-additional-resources)

---

## 🎯 Overview

### Concept: Duty, frequency, and resolution are coupled

PWM is timer compare hardware. Your choice of period (ARR) and clock prescaler sets frequency and duty resolution. Match these to actuator needs and EMC constraints.

### Minimal example
```c
// Set PWM to 20 kHz with 10-bit resolution if clock allows
void pwm_init(void){ /* set PSC/ARR; configure CCx mode; enable output */ }
void pwm_set_duty(uint16_t duty){ /* write CCRx, clamp to ARR */ }
```

### Try it
1. Sweep frequency to move switching noise out of sensitive bands.
2. Evaluate linearity vs dead-time/driver delays on motors/LEDs.

### Takeaways
- Frequency vs resolution tradeoff is dictated by timer clock.
- Use complementary outputs and dead-time for half-bridges.
- Filtered PWM (RC) behaves like a DAC; design the filter.

---

## 🧪 Guided Labs
1) Resolution vs frequency trade-off
- Generate PWM at different frequencies; measure actual resolution with an oscilloscope.

2) Filtered PWM as DAC
- Apply RC filter to PWM output; measure ripple and settling time.

## ✅ Check Yourself
- How does PWM frequency affect motor efficiency and audible noise?
- When should you use center-aligned vs edge-aligned PWM?

## 🔗 Cross-links
- `Hardware_Fundamentals/Timer_Counter_Programming.md` for timer configuration
- `Hardware_Fundamentals/Analog_IO.md` for DAC alternatives

Pulse Width Modulation (PWM) is a technique used to control power delivery to electrical devices by rapidly switching between on and off states. It's widely used in motor control, LED dimming, power supplies, and audio applications.

**Key Concepts:**
- **Duty Cycle**: Percentage of time signal is high
- **Frequency**: Rate of PWM switching
- **Resolution**: Number of discrete duty cycle levels
- **Applications**: Motor control, LED dimming, power conversion

---

## 🔧 PWM Fundamentals

### **PWM Parameters**
```c
// PWM parameters structure
typedef struct {
    uint32_t frequency;      // PWM frequency in Hz
    uint32_t period;         // Timer period value
    uint32_t prescaler;      // Timer prescaler value
    uint16_t duty_cycle;     // Duty cycle (0-100)
    uint16_t resolution;     // PWM resolution in bits
} PWM_Config_t;

// Calculate PWM parameters
void pwm_calculate_parameters(PWM_Config_t* config, uint32_t clock_freq, uint32_t target_freq) {
    // Calculate prescaler and period for target frequency
    uint32_t psc = 1;
    uint32_t arr = clock_freq / target_freq;
    
    // Adjust prescaler if period is too large
    while (arr > 65535 && psc < 65535) {
        psc++;
        arr = (clock_freq / psc) / target_freq;
    }
    
    config->frequency = target_freq;
    config->period = arr - 1;
    config->prescaler = psc - 1;
    config->resolution = 16; // Assuming 16-bit timer
}
```

### **Duty Cycle Calculations**
```c
// Convert duty cycle percentage to timer compare value
uint16_t duty_cycle_to_compare(uint16_t duty_cycle, uint16_t period) {
    return (uint16_t)((duty_cycle * period) / 100);
}

// Convert timer compare value to duty cycle percentage
uint16_t compare_to_duty_cycle(uint16_t compare_value, uint16_t period) {
    return (uint16_t)((compare_value * 100) / period);
}

// Set PWM duty cycle
void pwm_set_duty_cycle(TIM_HandleTypeDef* htim, uint32_t channel, uint16_t duty_cycle) {
    uint16_t compare_value = duty_cycle_to_compare(duty_cycle, htim->Init.Period);
    __HAL_TIM_SET_COMPARE(htim, channel, compare_value);
}
```

---

## ⚙️ PWM Configuration

### **Basic PWM Configuration**
```c
// Configure timer for PWM output
void pwm_timer_config(TIM_HandleTypeDef* htim, PWM_Config_t* config) {
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    // Configure timer base
    htim->Instance = TIM2;
    htim->Init.Prescaler = config->prescaler;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = config->period;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    HAL_TIM_Base_Init(htim);
    
    // Configure PWM channel
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_1);
    
    // Start PWM
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
}
```

### **Multi-Channel PWM Configuration**
```c
// Configure multiple PWM channels
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channels[4];
    uint8_t channel_count;
    PWM_Config_t config;
} MultiChannelPWM_t;

void multi_channel_pwm_init(MultiChannelPWM_t* mpwm, TIM_HandleTypeDef* htim,
                           uint32_t* channels, uint8_t count, uint32_t frequency) {
    mpwm->htim = htim;
    mpwm->channel_count = count;
    
    for (int i = 0; i < count; i++) {
        mpwm->channels[i] = channels[i];
    }
    
    // Calculate PWM parameters
    pwm_calculate_parameters(&mpwm->config, SystemCoreClock, frequency);
    
    // Configure timer base
    htim->Instance = TIM2;
    htim->Init.Prescaler = mpwm->config.prescaler;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = mpwm->config.period;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    HAL_TIM_Base_Init(htim);
    
    // Configure each channel
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    for (int i = 0; i < count; i++) {
        HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, channels[i]);
        HAL_TIM_PWM_Start(htim, channels[i]);
    }
}
```

### **PWM with Interrupts**
```c
// Configure PWM with update interrupt
void pwm_with_interrupt_config(TIM_HandleTypeDef* htim, PWM_Config_t* config) {
    // Configure timer base
    htim->Instance = TIM2;
    htim->Init.Prescaler = config->prescaler;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = config->period;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    HAL_TIM_Base_Init(htim);
    
    // Configure PWM channel
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_1);
    
    // Enable update interrupt
    __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    
    // Start PWM
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
}

// PWM interrupt handler
void TIM2_IRQHandler(void) {
    if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET) {
        if (__HAL_TIM_GET_IT_SOURCE(&htim2, TIM_IT_UPDATE) != RESET) {
            __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
            
            // Handle PWM update
            pwm_update_callback();
        }
    }
}
```

---

## 🎛️ Duty Cycle Control

### **Smooth Duty Cycle Transitions**
```c
// Smooth duty cycle transition
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint16_t current_duty;
    uint16_t target_duty;
    uint16_t step_size;
    uint32_t transition_time;
    uint32_t last_update_time;
} PWM_Transition_t;

void pwm_transition_init(PWM_Transition_t* transition, TIM_HandleTypeDef* htim,
                        uint32_t channel, uint16_t step_size, uint32_t transition_time_ms) {
    transition->htim = htim;
    transition->channel = channel;
    transition->current_duty = 0;
    transition->target_duty = 0;
    transition->step_size = step_size;
    transition->transition_time = transition_time_ms;
    transition->last_update_time = 0;
}

void pwm_transition_set_target(PWM_Transition_t* transition, uint16_t target_duty) {
    transition->target_duty = target_duty;
}

void pwm_transition_update(PWM_Transition_t* transition) {
    uint32_t current_time = HAL_GetTick();
    
    if (current_time - transition->last_update_time >= transition->transition_time) {
        if (transition->current_duty < transition->target_duty) {
            transition->current_duty += transition->step_size;
            if (transition->current_duty > transition->target_duty) {
                transition->current_duty = transition->target_duty;
            }
        } else if (transition->current_duty > transition->target_duty) {
            transition->current_duty -= transition->step_size;
            if (transition->current_duty < transition->target_duty) {
                transition->current_duty = transition->target_duty;
            }
        }
        
        pwm_set_duty_cycle(transition->htim, transition->channel, transition->current_duty);
        transition->last_update_time = current_time;
    }
}
```

### **Duty Cycle Ramping**
```c
// Duty cycle ramping
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint16_t start_duty;
    uint16_t end_duty;
    uint16_t current_duty;
    uint16_t step_size;
    uint32_t ramp_time;
    uint32_t step_time;
    uint32_t last_step_time;
    uint8_t ramping;
} PWM_Ramp_t;

void pwm_ramp_init(PWM_Ramp_t* ramp, TIM_HandleTypeDef* htim, uint32_t channel,
                   uint16_t step_size, uint32_t ramp_time_ms) {
    ramp->htim = htim;
    ramp->channel = channel;
    ramp->step_size = step_size;
    ramp->ramp_time = ramp_time_ms;
    ramp->ramping = 0;
    ramp->current_duty = 0;
}

void pwm_ramp_start(PWM_Ramp_t* ramp, uint16_t start_duty, uint16_t end_duty) {
    ramp->start_duty = start_duty;
    ramp->end_duty = end_duty;
    ramp->current_duty = start_duty;
    ramp->ramping = 1;
    ramp->last_step_time = HAL_GetTick();
    
    // Calculate step time
    uint16_t total_steps = abs(end_duty - start_duty) / ramp->step_size;
    ramp->step_time = ramp->ramp_time / total_steps;
    
    // Set initial duty cycle
    pwm_set_duty_cycle(ramp->htim, ramp->channel, ramp->current_duty);
}

void pwm_ramp_update(PWM_Ramp_t* ramp) {
    if (!ramp->ramping) return;
    
    uint32_t current_time = HAL_GetTick();
    
    if (current_time - ramp->last_step_time >= ramp->step_time) {
        if (ramp->current_duty < ramp->end_duty) {
            ramp->current_duty += ramp->step_size;
            if (ramp->current_duty > ramp->end_duty) {
                ramp->current_duty = ramp->end_duty;
                ramp->ramping = 0;
            }
        } else if (ramp->current_duty > ramp->end_duty) {
            ramp->current_duty -= ramp->step_size;
            if (ramp->current_duty < ramp->end_duty) {
                ramp->current_duty = ramp->end_duty;
                ramp->ramping = 0;
            }
        }
        
        pwm_set_duty_cycle(ramp->htim, ramp->channel, ramp->current_duty);
        ramp->last_step_time = current_time;
    }
}
```

---

## 🔄 Frequency Control

### **Dynamic Frequency Control**
```c
// Dynamic frequency control
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t current_frequency;
    uint32_t target_frequency;
    uint32_t min_frequency;
    uint32_t max_frequency;
} PWM_Frequency_Control_t;

void pwm_frequency_control_init(PWM_Frequency_Control_t* freq_ctrl, TIM_HandleTypeDef* htim,
                               uint32_t min_freq, uint32_t max_freq) {
    freq_ctrl->htim = htim;
    freq_ctrl->min_frequency = min_freq;
    freq_ctrl->max_frequency = max_freq;
    freq_ctrl->current_frequency = min_freq;
    freq_ctrl->target_frequency = min_freq;
}

void pwm_set_frequency(PWM_Frequency_Control_t* freq_ctrl, uint32_t frequency) {
    if (frequency < freq_ctrl->min_frequency) {
        frequency = freq_ctrl->min_frequency;
    } else if (frequency > freq_ctrl->max_frequency) {
        frequency = freq_ctrl->max_frequency;
    }
    
    freq_ctrl->target_frequency = frequency;
    
    // Calculate new timer parameters
    PWM_Config_t config;
    pwm_calculate_parameters(&config, SystemCoreClock, frequency);
    
    // Update timer configuration
    freq_ctrl->htim->Init.Prescaler = config.prescaler;
    freq_ctrl->htim->Init.Period = config.period;
    
    HAL_TIM_Base_Init(freq_ctrl->htim);
    freq_ctrl->current_frequency = frequency;
}
```

### **Frequency Sweeping**
```c
// Frequency sweeping
typedef struct {
    PWM_Frequency_Control_t* freq_ctrl;
    uint32_t start_frequency;
    uint32_t end_frequency;
    uint32_t sweep_time;
    uint32_t current_time;
    uint8_t sweeping;
} PWM_Frequency_Sweep_t;

void pwm_frequency_sweep_init(PWM_Frequency_Sweep_t* sweep, PWM_Frequency_Control_t* freq_ctrl,
                             uint32_t sweep_time_ms) {
    sweep->freq_ctrl = freq_ctrl;
    sweep->sweep_time = sweep_time_ms;
    sweep->sweeping = 0;
}

void pwm_frequency_sweep_start(PWM_Frequency_Sweep_t* sweep, uint32_t start_freq, uint32_t end_freq) {
    sweep->start_frequency = start_freq;
    sweep->end_frequency = end_freq;
    sweep->current_time = 0;
    sweep->sweeping = 1;
    
    // Set initial frequency
    pwm_set_frequency(sweep->freq_ctrl, start_freq);
}

void pwm_frequency_sweep_update(PWM_Frequency_Sweep_t* sweep) {
    if (!sweep->sweeping) return;
    
    sweep->current_time += 10; // Update every 10ms
    
    // Calculate current frequency
    float progress = (float)sweep->current_time / sweep->sweep_time;
    if (progress > 1.0f) progress = 1.0f;
    
    uint32_t current_freq = sweep->start_frequency + 
                           (uint32_t)(progress * (sweep->end_frequency - sweep->start_frequency));
    
    pwm_set_frequency(sweep->freq_ctrl, current_freq);
    
    if (progress >= 1.0f) {
        sweep->sweeping = 0;
    }
}
```

---

## 📊 PWM Applications

### **LED Dimming**
```c
// LED dimming control
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint16_t brightness;
    uint8_t fade_direction;
    uint16_t fade_speed;
} LED_Dimmer_t;

void led_dimmer_init(LED_Dimmer_t* dimmer, TIM_HandleTypeDef* htim, uint32_t channel) {
    dimmer->htim = htim;
    dimmer->channel = channel;
    dimmer->brightness = 0;
    dimmer->fade_direction = 0; // 0 = fade in, 1 = fade out
    dimmer->fade_speed = 1;
}

void led_dimmer_set_brightness(LED_Dimmer_t* dimmer, uint16_t brightness) {
    dimmer->brightness = brightness;
    pwm_set_duty_cycle(dimmer->htim, dimmer->channel, brightness);
}

void led_dimmer_fade(LED_Dimmer_t* dimmer) {
    if (dimmer->fade_direction == 0) {
        // Fade in
        dimmer->brightness += dimmer->fade_speed;
        if (dimmer->brightness >= 100) {
            dimmer->brightness = 100;
            dimmer->fade_direction = 1;
        }
    } else {
        // Fade out
        dimmer->brightness -= dimmer->fade_speed;
        if (dimmer->brightness <= 0) {
            dimmer->brightness = 0;
            dimmer->fade_direction = 0;
        }
    }
    
    pwm_set_duty_cycle(dimmer->htim, dimmer->channel, dimmer->brightness);
}
```

### **Motor Speed Control**
```c
// Motor speed control
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint16_t speed;
    uint16_t max_speed;
    uint16_t min_speed;
    uint8_t direction;
} Motor_Controller_t;

void motor_controller_init(Motor_Controller_t* motor, TIM_HandleTypeDef* htim, uint32_t channel,
                          uint16_t min_speed, uint16_t max_speed) {
    motor->htim = htim;
    motor->channel = channel;
    motor->min_speed = min_speed;
    motor->max_speed = max_speed;
    motor->speed = 0;
    motor->direction = 0;
}

void motor_set_speed(Motor_Controller_t* motor, uint16_t speed) {
    if (speed > motor->max_speed) {
        speed = motor->max_speed;
    } else if (speed < motor->min_speed) {
        speed = motor->min_speed;
    }
    
    motor->speed = speed;
    pwm_set_duty_cycle(motor->htim, motor->channel, speed);
}

void motor_set_direction(Motor_Controller_t* motor, uint8_t direction) {
    motor->direction = direction;
    // Additional GPIO control for direction if needed
}
```

### **Audio Generation**
```c
// Audio tone generation
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint16_t frequency;
    uint16_t volume;
    uint8_t playing;
} Audio_Generator_t;

void audio_generator_init(Audio_Generator_t* audio, TIM_HandleTypeDef* htim, uint32_t channel) {
    audio->htim = htim;
    audio->channel = channel;
    audio->frequency = 440; // A4 note
    audio->volume = 50;
    audio->playing = 0;
}

void audio_generator_play_tone(Audio_Generator_t* audio, uint16_t frequency, uint16_t volume) {
    audio->frequency = frequency;
    audio->volume = volume;
    audio->playing = 1;
    
    // Set frequency
    PWM_Config_t config;
    pwm_calculate_parameters(&config, SystemCoreClock, frequency);
    
    audio->htim->Init.Prescaler = config.prescaler;
    audio->htim->Init.Period = config.period;
    HAL_TIM_Base_Init(audio->htim);
    
    // Set volume (duty cycle)
    pwm_set_duty_cycle(audio->htim, audio->channel, volume);
}

void audio_generator_stop(Audio_Generator_t* audio) {
    audio->playing = 0;
    pwm_set_duty_cycle(audio->htim, audio->channel, 0);
}
```

---

## ⚡ Advanced PWM Techniques

### **Phase-Shifted PWM**
```c
// Phase-shifted PWM for multi-phase systems
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channels[3];
    uint16_t duty_cycle;
    uint16_t phase_shift;
} Phase_Shifted_PWM_t;

void phase_shifted_pwm_init(Phase_Shifted_PWM_t* pspwm, TIM_HandleTypeDef* htim,
                           uint32_t* channels, uint16_t phase_shift) {
    pspwm->htim = htim;
    pspwm->phase_shift = phase_shift;
    
    for (int i = 0; i < 3; i++) {
        pspwm->channels[i] = channels[i];
    }
}

void phase_shifted_pwm_set_duty(Phase_Shifted_PWM_t* pspwm, uint16_t duty_cycle) {
    pspwm->duty_cycle = duty_cycle;
    
    // Set duty cycle for each channel with phase shift
    for (int i = 0; i < 3; i++) {
        uint16_t phase_adjusted_duty = (duty_cycle + (i * pspwm->phase_shift)) % 100;
        pwm_set_duty_cycle(pspwm->htim, pspwm->channels[i], phase_adjusted_duty);
    }
}
```

### **Dead Time Insertion**
```c
// Dead time insertion for H-bridge control
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel1;
    uint32_t channel2;
    uint16_t dead_time;
} Dead_Time_PWM_t;

void dead_time_pwm_init(Dead_Time_PWM_t* dtpwm, TIM_HandleTypeDef* htim,
                       uint32_t channel1, uint32_t channel2, uint16_t dead_time) {
    dtpwm->htim = htim;
    dtpwm->channel1 = channel1;
    dtpwm->channel2 = channel2;
    dtpwm->dead_time = dead_time;
    
    // Configure dead time
    __HAL_TIM_SET_AUTORELOAD(htim, htim->Init.Period);
    __HAL_TIM_SET_COMPARE(htim, channel1, 0);
    __HAL_TIM_SET_COMPARE(htim, channel2, dead_time);
}
```

---

## 🎯 Common Applications

### **Power Supply Control**
```c
// Buck converter control
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint16_t duty_cycle;
    float output_voltage;
    float target_voltage;
    float input_voltage;
} Buck_Converter_t;

void buck_converter_init(Buck_Converter_t* buck, TIM_HandleTypeDef* htim, uint32_t channel,
                        float input_voltage) {
    buck->htim = htim;
    buck->channel = channel;
    buck->input_voltage = input_voltage;
    buck->output_voltage = 0;
    buck->target_voltage = 0;
    buck->duty_cycle = 0;
}

void buck_converter_set_voltage(Buck_Converter_t* buck, float target_voltage) {
    buck->target_voltage = target_voltage;
    
    // Calculate duty cycle: Vout = Vin * Duty_Cycle
    float duty_cycle = (target_voltage / buck->input_voltage) * 100.0f;
    
    if (duty_cycle > 100.0f) duty_cycle = 100.0f;
    if (duty_cycle < 0.0f) duty_cycle = 0.0f;
    
    buck->duty_cycle = (uint16_t)duty_cycle;
    pwm_set_duty_cycle(buck->htim, buck->channel, buck->duty_cycle);
}
```

### **Servo Control**
```c
// Servo motor control
typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint16_t position;
    uint16_t min_position;
    uint16_t max_position;
} Servo_Controller_t;

void servo_controller_init(Servo_Controller_t* servo, TIM_HandleTypeDef* htim, uint32_t channel) {
    servo->htim = htim;
    servo->channel = channel;
    servo->min_position = 5;   // 0.5ms pulse
    servo->max_position = 25;  // 2.5ms pulse
    servo->position = 15;      // Center position
}

void servo_set_position(Servo_Controller_t* servo, uint16_t position) {
    if (position < servo->min_position) {
        position = servo->min_position;
    } else if (position > servo->max_position) {
        position = servo->max_position;
    }
    
    servo->position = position;
    pwm_set_duty_cycle(servo->htim, servo->channel, position);
}
```

---

## ⚠️ Common Pitfalls

### **1. Incorrect Frequency Calculation**
```c
// ❌ Wrong: Incorrect frequency calculation
void pwm_frequency_wrong(TIM_HandleTypeDef* htim, uint32_t frequency) {
    htim->Init.Period = 1000 / frequency; // Wrong calculation
}

// ✅ Correct: Proper frequency calculation
void pwm_frequency_correct(TIM_HandleTypeDef* htim, uint32_t frequency) {
    uint32_t clock_freq = SystemCoreClock;
    uint32_t period = (clock_freq / frequency) - 1;
    htim->Init.Period = period;
}
```

### **2. Missing Dead Time**
```c
// ❌ Wrong: No dead time for H-bridge
void h_bridge_control_wrong(TIM_HandleTypeDef* htim) {
    // Direct PWM control without dead time
}

// ✅ Correct: With dead time
void h_bridge_control_correct(TIM_HandleTypeDef* htim) {
    // Configure dead time for safe switching
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, DEAD_TIME_VALUE);
}
```

### **3. Insufficient Resolution**
```c
// ❌ Wrong: Low resolution PWM
void pwm_low_resolution_wrong(TIM_HandleTypeDef* htim) {
    htim->Init.Period = 100; // Only 100 steps
}

// ✅ Correct: High resolution PWM
void pwm_high_resolution_correct(TIM_HandleTypeDef* htim) {
    htim->Init.Period = 4095; // 4096 steps (12-bit)
}
```

---

## ✅ Best Practices

### **1. Use Appropriate Frequencies**
```c
// Use appropriate frequencies for different applications
void configure_pwm_frequency(PWM_Config_t* config, uint8_t application_type) {
    switch (application_type) {
        case PWM_LED_DIMMING:
            config->frequency = 1000; // 1kHz for LED dimming
            break;
        case PWM_MOTOR_CONTROL:
            config->frequency = 20000; // 20kHz for motor control
            break;
        case PWM_AUDIO:
            config->frequency = 44100; // 44.1kHz for audio
            break;
        case PWM_POWER_SUPPLY:
            config->frequency = 100000; // 100kHz for power supply
            break;
    }
}
```

### **2. Implement Smooth Transitions**
```c
// Always use smooth transitions for better performance
void smooth_pwm_transition(TIM_HandleTypeDef* htim, uint32_t channel,
                          uint16_t start_duty, uint16_t end_duty, uint16_t steps) {
    uint16_t step_size = (end_duty - start_duty) / steps;
    
    for (int i = 0; i < steps; i++) {
        uint16_t current_duty = start_duty + (i * step_size);
        pwm_set_duty_cycle(htim, channel, current_duty);
        HAL_Delay(10); // Small delay for smooth transition
    }
    
    pwm_set_duty_cycle(htim, channel, end_duty);
}
```

### **3. Monitor PWM Performance**
```c
// Monitor PWM performance and efficiency
typedef struct {
    uint32_t frequency;
    uint16_t duty_cycle;
    float efficiency;
    uint32_t switching_losses;
} PWM_Performance_t;

void pwm_performance_monitor(PWM_Performance_t* perf, TIM_HandleTypeDef* htim) {
    // Calculate switching losses
    perf->switching_losses = perf->frequency * perf->duty_cycle / 100;
    
    // Calculate efficiency (simplified)
    perf->efficiency = 100.0f - (perf->switching_losses / 1000.0f);
}
```

---

## 🎯 Interview Questions

### **Basic Questions**
1. **What is PWM and how does it work?**
   - Pulse Width Modulation, rapid switching between on/off states

2. **What is duty cycle and how is it calculated?**
   - Percentage of time signal is high, (on_time / total_time) * 100

3. **What is the relationship between frequency and period?**
   - Frequency = 1 / Period

### **Advanced Questions**
1. **How do you implement phase-shifted PWM?**
   - Use multiple channels with different phase delays

2. **What is dead time and why is it important?**
   - Delay between switching to prevent shoot-through in H-bridges

3. **How do you optimize PWM for different applications?**
   - Choose appropriate frequency, resolution, and switching strategy

### **Practical Questions**
1. **Design a motor speed controller using PWM**
   - Timer configuration, duty cycle control, feedback loop

2. **Implement LED dimming with smooth transitions**
   - PWM generation, duty cycle ramping, timing control

3. **Create an audio tone generator**
   - Frequency control, volume control, waveform generation

---

## 📚 Additional Resources

### **Documentation**
- [STM32 Timer Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [ARM Cortex-M Timer Programming](https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/peripherals/general-purpose-timers)

### **Tools**
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) - PWM configuration
- [PWM Calculator](https://www.st.com/resource/en/user_manual/dm00104712-stm32cubemx-user-manual-stmicroelectronics.pdf)

### **Related Topics**
- **[GPIO Configuration](./GPIO_Configuration.md)** - GPIO modes, configuration, electrical characteristics
- **[Timer/Counter Programming](./Timer_Counter_Programming.md)** - Input capture, output compare, frequency measurement
- **[Analog I/O](./Analog_IO.md)** - ADC sampling techniques, DAC output generation

---

**Next Topic:** [Timer/Counter Programming](./Timer_Counter_Programming.md) → [External Interrupts](./External_Interrupts.md)
