# 🧵 Task Creation and Management (FreeRTOS)

> Conceptual guide to task lifecycle, priorities, stack sizing, and context switching

## 📋 Table of Contents

- **Overview**
- **Task Lifecycle**
- **Priorities and Scheduling**
- **Stack Sizing and Monitoring**
- **Delays, Timeouts, and Periodicity**
- **Task Communication (Overview)**
- **Context Switching (Concepts)**
- **Common Pitfalls**
- **Best Practices**
- **Interview Questions**

---

## 🎯 Overview

Tasks are concurrent execution units scheduled by the RTOS. Proper design requires understanding lifecycle transitions, priority assignment, stack sizing, and timing behavior.

---

## 🔄 Task Lifecycle

States: NEW → READY ↔ RUNNING ↔ BLOCKED ↔ SUSPENDED → DELETED

- **NEW**: Created but not yet scheduled (before first `vTaskStartScheduler`).
- **READY**: Eligible to run, waiting CPU time based on priority.
- **RUNNING**: Actively executing on the CPU.
- **BLOCKED**: Waiting for time, queue/semaphore/event group, or I/O.
- **SUSPENDED**: Temporarily disabled; requires `vTaskResume`.
- **DELETED**: Task resources freed; TCB/stack returned to heap (or static memory if used).

Design aim: Spend most time BLOCKED to allow other tasks to run and save power.

---

## 🧭 Priorities and Scheduling

- **Priority Levels**: Integer levels; higher = more urgent.
- **Preemption**: A higher-priority READY task immediately preempts a lower one.
- **Time Slicing**: Among equal-priority READY tasks, the kernel may round-robin (configurable).
- **Starvation Risk**: High-priority tasks that never block can starve others.

Priority assignment heuristic:
- Interrupt-like tasks (hard deadlines): highest priorities, short CPU bursts, frequent blocking.
- Control/feedback loops: medium-high priorities, periodic execution.
- UI/housekeeping: lower priorities, tolerant to jitter.

---

## 📦 Stack Sizing and Monitoring

- **Size Drivers**: Call depth, local variables, library calls (e.g., printf), ISR nesting.
- **Measure**: Enable high-water mark and check with `uxTaskGetStackHighWaterMark`.
- **Overflow Protection**: Enable `configCHECK_FOR_STACK_OVERFLOW`.
- **Static vs Dynamic**: Consider `xTaskCreateStatic` for fixed allocations; avoids heap fragmentation.

Rule of thumb: start conservatively, measure, then trim. Avoid `printf` in tasks or use tiny printf.

---

## ⏱️ Delays, Timeouts, and Periodicity

- **Cooperative Delays**: `vTaskDelay` yields the CPU for N ticks.
- **Absolute Periodicity**: `vTaskDelayUntil` maintains fixed-rate execution despite jitter.
- **Timeouts**: All blocking calls accept a timeout for deterministic unblocking.

Example pattern:
```c
TickType_t lastWake = xTaskGetTickCount();
const TickType_t period = pdMS_TO_TICKS(10);
for (;;) {
  run_control_loop();
  vTaskDelayUntil(&lastWake, period); // consistent 10ms loop
}
```

---

## 🗣️ Task Communication (Overview)

- **Queues**: Copy messages between tasks/ISRs; bounded buffers with timeouts.
- **Semaphores**: Binary (events) and counting (resource units).
- **Mutexes**: Mutual exclusion with priority inheritance; avoid from ISRs.
- **Event Groups**: Bitwise events; efficient for multi-condition waits.
- **Stream/Message Buffers**: Byte/message streams with zero-copy to a single receiver.

Guideline: Prefer queues/event groups for ISR→task signaling; mutexes for shared resources in tasks only.

---

## 🔁 Context Switching (Concepts)

- Triggered by: tick ISR, yield, or an ISR that unblocks a higher-priority task.
- Saves current task context (registers, PSR, return address) and restores next task.
- Cost depends on MCU architecture and FPU usage; minimize unnecessary switches by batching work and using event-driven designs.

---

## ⚠️ Common Pitfalls

- Unbounded blocking without timeouts → deadlocks.
- Priority inversion → use mutexes with inheritance or redesign.
- Oversized stacks or too many tasks → memory pressure.
- Doing heavy work in ISRs → jitter and missed deadlines.
- Tick rate too high → power/CPU overhead; too low → coarse timing.

---

## ✅ Best Practices

- Keep tasks short and event-driven; block often.
- Use `vTaskDelayUntil` for periodic work; avoid busy-waiting.
- Measure stack usage; enable overflow checks.
- Use static allocation where possible on small MCUs.
- Centralize priority scheme; document rationale.

---

## 🎤 Interview Questions

1. How do you choose task priorities to meet deadlines and avoid starvation?
2. Explain `vTaskDelay` vs `vTaskDelayUntil` and when to use each.
3. How do you size a task stack and verify it in production?
4. When would you use a queue vs an event group vs a semaphore?
5. What are the trade-offs of tick rates and tickless idle?

---

Next: Add synchronization details in a separate module (Queues, Semaphores, Mutexes, Event Groups).


