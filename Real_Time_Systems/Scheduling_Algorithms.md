# 🗓️ Scheduling Algorithms for Real-Time Systems

> Conceptual guide to fixed-priority (Rate Monotonic), Deadline Monotonic, and dynamic-priority (EDF) scheduling

## 📋 Table of Contents

- Overview
- Real-Time Task Model
- Fixed-Priority Scheduling
- Dynamic-Priority Scheduling
- Schedulability Analysis
- Jitter, Latency, and Overheads
- Choosing a Scheduling Strategy
- Best Practices and Pitfalls
- Interview Questions

---

## 🎯 Overview

Real-time scheduling ensures tasks meet timing constraints (deadlines, periods, jitter bounds). On MCUs with an RTOS like FreeRTOS, the kernel typically implements preemptive fixed-priority scheduling with optional time-slicing for equal priorities.

---

## 🧩 Real-Time Task Model

- **Periodic Tasks**: Execute at a fixed period \(T\); each instance has a relative deadline \(D\) and worst-case execution time \(C\).
- **Sporadic Tasks**: Minimum inter-arrival time; deadlines per arrival.
- **Aperiodic Tasks**: No strict period; often served by background servers.
- **Preemption**: A higher-priority READY task preempts a lower-priority one.

Key parameters per task i: \(C_i\) (WCET), \(T_i\) (period), \(D_i\) (deadline), priority \(P_i\).

---

## 📌 Fixed-Priority Scheduling

### Rate Monotonic (RM)
- Priority is inversely proportional to period: shorter period ⇒ higher priority.
- Optimal among fixed-priority policies when \(D_i = T_i\) and tasks are independent.

### Deadline Monotonic (DM)
- Priority is inversely proportional to relative deadline: shorter deadline ⇒ higher priority.
- Generalizes RM when \(D_i \le T_i\).

### Time Slicing (Round-Robin among equals)
- Among equal-priority READY tasks, the kernel may time-slice by tick; use sparingly to avoid jitter.

Pros (fixed priority): simple, predictable, low overhead. Cons: priority assignment is static; can cause priority inversion.

---

## 🔄 Dynamic-Priority Scheduling

### Earliest Deadline First (EDF)
- Always run the READY task with the earliest absolute deadline.
- Optimal uniprocessor scheduler for meeting deadlines when total utilization \(U \le 1\) with negligible overhead.
- Less common on small MCUs due to implementation complexity and overhead; some kernels provide EDF variants.

---

## 📐 Schedulability Analysis

### Utilization-Based (Liu & Layland)
- For RM with \(n\) independent periodic tasks and \(D_i = T_i\):
  - Sufficient bound: \(U = \sum C_i/T_i \le n(2^{1/n} - 1)\). For large \(n\), bound → ~0.693.
  - If above bound, the set may still be schedulable; use response-time analysis.

### Response-Time Analysis (RTA)
- Compute worst-case response time \(R_i\) iteratively:
  - \(R_i^{(k+1)} = C_i + \sum_{j\in hp(i)} \left\lceil \frac{R_i^{(k)}}{T_j} \right\rceil C_j\)
  - Schedulable if \(R_i \le D_i\).

### Jitter and Blocking
- **Jitter**: Variation in start or completion times; affected by tick granularity, time-slicing, ISRs.
- **Blocking**: Priority inversion due to shared resources; bounded by using mutexes with priority inheritance or lock-free designs.

---

## ⚙️ Jitter, Latency, and Overheads

- **Kernel Tick**: Coarse timing; consider `vTaskDelayUntil` for periodic work.
- **Context Switch Cost**: Depends on MCU and FPU use; minimize unnecessary switches.
- **ISR Latency**: Keep ISRs short; defer to tasks via queues/event groups.
- **Cache/Flash Waits**: On high-end MCUs, memory latency can add jitter; prefer ITCM/DTCM where available.

---

## 🧭 Choosing a Scheduling Strategy

- **RM/DM**: Default choice on small MCUs; simple and predictable.
- **EDF**: Consider when utilization is high and deadlines vary; weigh overhead vs. gains.
- **Hybrid**: Fixed-priority kernel with application-level EDF within a server task.

Guidelines:
- Assign higher priorities to tasks with tighter periods/deadlines and stricter latency.
- Avoid equal priorities unless round-robin behavior is intentional.
- Use analytical checks (RTA) for critical deployments; validate with measurement.

---

## ✅ Best Practices and ⚠️ Pitfalls

Best Practices:
- Use `vTaskDelayUntil` for periodic tasks to minimize drift.
- Minimize shared resource contention; prefer message passing.
- Enable run-time stats for CPU utilization; verify assumptions in hardware.

Pitfalls:
- Ignoring ISR execution time in schedulability analysis.
- Excessive time-slicing causing jitter.
- Not bounding priority inversion; missing deadlines due to blocking.

---

## 🎤 Interview Questions

1. Explain RM vs DM vs EDF and when you would choose each.
2. Derive or explain the Liu & Layland utilization bound and its limitations.
3. How do you perform response-time analysis for a task set?
4. What causes jitter in an RTOS system and how can you mitigate it?
5. How do you handle priority inversion in fixed-priority systems?


