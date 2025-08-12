# 🧨 Interrupt Handling in RTOS-Based Systems

> Concepts for low-latency ISRs, nesting, priority mapping, and deferring work to tasks

## 📋 Table of Contents

- Overview
- Interrupt Latency and Determinism
- ISR Design Principles
- Interrupt Priorities and Kernel Interaction
- Deferring Work to Tasks
- Synchronization From ISRs
- Measuring and Tuning Latency
- Best Practices and Pitfalls
- Interview Questions

---

## 🎯 Overview

Interrupts handle urgent, asynchronous events with minimal latency. In RTOS systems, ISRs should be short and primarily signal tasks to perform the bulk of work. Proper priority configuration and correct use of FromISR APIs are essential for determinism.

---

## ⏱️ Interrupt Latency and Determinism

- **Sources of Latency**: Interrupt masking, critical sections, higher-priority ISRs, memory wait states.
- **Determinism Goal**: Bound the worst-case latency; keep critical sections short and minimize nested work.
- **Jitter**: Variability in latency; reduce by avoiding long non-preemptible sections and heavy ISRs.

---

## 🛠️ ISR Design Principles

- Do the minimum necessary in the ISR: capture timestamp/data, clear the source, and signal a task.
- Avoid blocking, dynamic memory, or long loops in ISRs.
- Ensure the ISR is reentrant or protect shared state.
- Use volatile-qualified shared variables and atomics where required.

---

## 🎚️ Interrupt Priorities and Kernel Interaction

- On Cortex-M, lower numeric priority value = higher urgency; only priorities numerically larger (less urgent) than `configMAX_SYSCALL_INTERRUPT_PRIORITY` may call RTOS APIs.
- The kernel may disable interrupts briefly for critical sections; keep those sections small in drivers.
- Nesting: A higher-priority interrupt can preempt a lower one; keep nesting depth small to bound latency.

---

## 📬 Deferring Work to Tasks

Common patterns:
- **Queue to Task**: Send a message from ISR to a worker task.
- **Event Group**: Set bits to notify events.
- **Stream/Message Buffer**: Transfer bytes/messages efficiently to a single receiver.
- **Direct-to-Task Notifications**: Lightweight, fast signaling to a specific task.

Benefits: keeps ISRs short, leverages task priorities, and eases testing.

---

## 🔗 Synchronization From ISRs

- Use the `...FromISR` API variants (e.g., `xQueueSendFromISR`, `xTaskNotifyFromISR`).
- Pass `pxHigherPriorityTaskWoken`/`xHigherPriorityTaskWoken` and request a context switch if needed.
- Never use mutex APIs from ISRs; use semaphores or notifications. Mutexes implement priority inheritance and are task context only.

---

## 🔍 Measuring and Tuning Latency

- **Instrumentation**: Toggle a GPIO at ISR entry/exit and measure with a logic analyzer.
- **Cycle Counters**: Use DWT cycle counter (Cortex-M) for fine-grained timing.
- **Trace Tools**: Kernel trace to observe ISR-to-task wakeups and scheduling.
- **Tuning**: Reduce critical sections, lower tick rate if it causes contention, optimize memory wait states.

---

## ✅ Best Practices and ⚠️ Pitfalls

Best Practices:
- Keep ISRs short; defer to tasks.
- Use the smallest interrupt priority capable of meeting latency needs.
- Validate `configPRIO_BITS`, NVIC priorities, and `configMAX_SYSCALL_INTERRUPT_PRIORITY`.
- Use direct-to-task notifications for fastest signaling.

Pitfalls:
- Calling non-FromISR APIs in interrupt context.
- Misconfigured NVIC priorities leading to hard faults or non-determinism.
- Heavy computation in ISRs causing jitter and missed deadlines.

---

## 🎤 Interview Questions

1. What contributes to interrupt latency, and how do you bound it?
2. Why should ISR work be minimized and deferred to tasks? How do you implement that?
3. Which RTOS APIs are safe to call in an ISR and why?
4. How do you configure NVIC priorities to work correctly with an RTOS?
5. How would you measure ISR latency on a Cortex-M MCU?


