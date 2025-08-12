# 🧠 FreeRTOS Basics

> Understanding the core ideas of an RTOS and how FreeRTOS applies them in embedded systems

## 📋 Table of Contents

- **Overview**
- **What is an RTOS?**
- **Why FreeRTOS?**
- **Core Concepts**
- **FreeRTOS Architecture at a Glance**
- **Timing and the Kernel Tick**
- **Task Model and States**
- **Memory and Footprint**
- **ISRs vs Tasks**
- **Best Practices and Pitfalls**
- **Quick Example**
- **Interview Questions**

---

## 🎯 Overview

Real-Time Operating Systems (RTOS) enable deterministic, predictable execution under constraints like deadlines and bounded latency. FreeRTOS is a lightweight, portable RTOS widely used on MCUs for task scheduling, synchronization, and timing services.

---

## 🤔 What is an RTOS?

- **Determinism**: Bounded response time to events.
- **Preemption**: Higher-priority tasks can preempt lower-priority tasks.
- **Scheduling**: Selects the next task to run based on priority and state.
- **Time Services**: Delays, timeouts, periodic work.
- **Synchronization**: Safe coordination between concurrent tasks and ISRs.

RTOS ≠ “fast”; it is “predictable.” Deterministic latency is the goal.

---

## 💡 Why FreeRTOS?

- **Portable**: Supports many MCU families and toolchains.
- **Small Footprint**: Fits into tens of KB of Flash/RAM.
- **Familiar APIs**: Simple C interface for tasks, queues, semaphores, timers.
- **Configurable**: Tune features via `FreeRTOSConfig.h`.
- **Ecosystem**: Rich examples, community, and commercial support (FreeRTOS+).

---

## 🧠 Core Concepts

- **Task**: Independent execution context with its own stack and priority.
- **Scheduler**: Preemptive priority-based (optional time-slicing among equal priorities).
- **Tick**: Periodic interrupt that advances time and drives delays/timeouts.
- **Ready List**: Per-priority queues of tasks ready to run.
- **Blocked State**: Task waits on time, queue, semaphore, event group, etc.
- **Critical Sections**: Temporarily suspend preemption for short, atomic work.
- **Synchronization Primitives**: Queues, semaphores (binary/counting), mutexes (with priority inheritance), event groups, stream/message buffers.
- **Software Timers**: Deferred work executed in a timer service task.

---

## 🏗️ FreeRTOS Architecture at a Glance

```
Application Tasks  ─┐  ┌─> Queues/Semaphores/Mutexes/EventGroups
                     │  │
Software Timers   ───┼──┤  (Kernel objects in RAM)
                     │  │
Idle Task         ───┘  └─> Scheduler (priority-based, preemptive)
                             ↑
                             │
                         SysTick/Timer ISR (kernel tick)
```

- Each task has a TCB (Task Control Block) + stack.
- The scheduler selects the highest-priority READY task.
- The tick interrupt updates delays, unblocks expired tasks, and may trigger a context switch.

---

## ⏱️ Timing and the Kernel Tick

- **Tick Rate (`configTICK_RATE_HZ`)**: Commonly 100–1000 Hz.
- **Delays**: `vTaskDelay`, `vTaskDelayUntil` use ticks; accuracy is ±1 tick.
- **Tickless Idle**: Suppress periodic ticks while idle to save power; requires a wake-up source.
- **Timeouts**: All blocking APIs accept timeouts (in ticks) for deterministic waits.

Design tip: Choose a tick rate that balances timing resolution vs. CPU overhead and power.

---

## 🔁 Task Model and States

States: READY ↔ RUNNING ↔ BLOCKED ↔ SUSPENDED

- **READY**: Eligible to run; waiting for CPU based on priority.
- **RUNNING**: Currently executing on the CPU.
- **BLOCKED**: Waiting on time or a kernel object (queue/semaphore/event group).
- **SUSPENDED**: Not scheduled until explicitly resumed.

Priorities are integer levels; higher value = higher priority (port-dependent). Starvation is possible if a very high-priority task never blocks.

---

## 🧮 Memory and Footprint

- **Per-task Stack**: Size depends on worst-case call depth and ISR usage via `portSAVE_CONTEXT`/`portRESTORE_CONTEXT`.
- **Kernel Heap**: Selected by `heap_1`..`heap_5`:
  - `heap_1`: Static bump allocator (no free). Smallest, fastest.
  - `heap_2`: Free with coalescing. Simple fragmentation handling.
  - `heap_3`: Wraps `malloc/free`. Depends on C library.
  - `heap_4`: Best-fit with coalescing. Good fragmentation characteristics.
  - `heap_5`: Multiple non-contiguous regions. Flexible for constrained MCUs.
- **Idle Hook / Malloc Failed Hook / Stack Overflow Hook**: Enable for diagnostics.

Choose stack sizes by measurement (watermarking) and enable stack overflow checking.

---

## ⚡ ISRs vs Tasks

- ISRs should be minimal: capture data/state and defer processing to tasks (via queues/stream buffers/event groups).
- Use the “FromISR” API variants inside interrupts.
- Ensure correct interrupt priority configuration (only permissive priorities may call FreeRTOS APIs; depends on port and `configMAX_SYSCALL_INTERRUPT_PRIORITY`).

---

## ✅ Best Practices and ⚠️ Pitfalls

Best Practices:
- Keep high-priority tasks short and frequently blocking.
- Assign priorities by criticality and latency, not by feature importance.
- Use bounded queues and timeouts; avoid infinite blocking.
- Instrument: enable run-time stats and stack high-water marks.
- Prefer event-driven designs over tight polling.

Pitfalls:
- Priority inversion without mutexes that implement priority inheritance.
- Oversized tick rate increasing power and CPU overhead.
- Excessive heap fragmentation (use `heap_4`/`heap_5` or static allocations).
- Doing heavy work in ISRs or high-priority tasks leading to starvation.

---

## 🧪 Quick Example (conceptual)

```c
// Two cooperative tasks using delays; conceptual snippet only
void vBlinkTask(void* arg) {
  for (;;) {
    led_toggle();
    vTaskDelay(pdMS_TO_TICKS(500)); // periodic blink
  }
}

void vButtonTask(void* arg) {
  for (;;) {
    if (button_pressed()) {
      xQueueSend(xEvents, &(int){1}, 0);
    }
    vTaskDelay(pdMS_TO_TICKS(10)); // debounce period
  }
}
```

Focus: periodic work with `vTaskDelay`, event passing via a queue, and short duty cycles so higher-priority tasks can preempt.

---

## 🎤 Interview Questions

1. What makes an RTOS “real-time” and how is that different from “fast”?
2. Explain the role of the kernel tick and how tickless idle works.
3. Describe task states and how tasks transition between them.
4. How would you size task stacks and select a heap scheme on a constrained MCU?
5. Why should ISR work be minimized and deferred? How is that implemented in FreeRTOS?

---

Next: See `Task_Creation_Management.md` for deeper task lifecycle, priorities, and stack management.


