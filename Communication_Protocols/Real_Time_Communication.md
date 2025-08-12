# Real-Time Communication

Real-time communication requires bounded latency and controlled jitter from sensor/actuator to controller and across the network stack. This guide focuses on techniques for determinism on microcontrollers and embedded Linux.

---

## Core Concepts

- Latency: time from enqueue (producer) to dequeue (consumer)
- Jitter: variability of latency; track 99th/99.9th percentiles
- Determinism: predictability under worst-case load, not just average
- Budgeting: allocate time slices per stage; include margins

---

## End-to-End Budget Example

Let total cycle time be 5 ms; target worst-case latency ≤ 2 ms from sensor IRQ to actuator command.

- ISR (sensor) + DMA completion: ≤ 100 µs
- Copy/parse to message: ≤ 200 µs
- Queue to RT task: ≤ 100 µs
- Network stack (UDP/TCP/CAN) enqueue: ≤ 300 µs
- Wire + peer processing: ≤ 1.0 ms
- Actuator command enqueue: ≤ 300 µs
- Margin: ≥ 0.0–200 µs

Track each stage via GPIO toggles and timestamps to verify.

---

## MCU Techniques

- Use DMA for RX/TX; keep ISRs minimal
- Pre-allocate buffers; avoid heap in critical path
- Prioritize tasks: ISR > network I/O > control loop > logging
- Use lock-free queues or bounded RTOS queues with watermarks
- Disable Nagle (`TCP_NODELAY`) when using small request/response patterns
- Pin critical tasks to highest available priority; avoid priority inversion

---

## Embedded Linux Techniques

- Use PREEMPT_RT or low-latency kernel
- Pin IRQs and threads to isolated CPUs (CPU shielding)
- Use SCHED_FIFO/SCHED_RR with bounded critical sections
- Leverage `SO_PRIORITY`, `IP_TOS`/DSCP for QoS marking
- Use `mqprio`/`tc` for Ethernet TX queue prioritization; enable hardware queues if NIC supports
- Prefer UDP for lowest latency; consider TCP only if required by protocol semantics

---

## Network Transport Choices

- CAN/CAN-FD: natural prioritization via IDs; size payloads conservatively; target bus utilization < 70%
- UDP: minimal overhead; app-level retransmits if necessary
- TCP: reliable but may introduce head-of-line blocking; disable Nagle when latency-sensitive
- TSN/AVB (Ethernet): time-aware shaping and scheduled traffic for microsecond-level determinism

---

## Queueing and Backpressure

- Size queues from measured traffic; use high/low watermarks
- Reserve headroom for critical traffic
- Apply token-bucket rate limiting on non-critical flows
- Drop or degrade non-critical traffic when under pressure

---

## Timestamping and Synchronization

- Use hardware timestamps where available (MAC/PHY)
- PTP for sub-millisecond synchronization across devices
- Propagate timebase to MCU subsystems via shared clocks or periodic sync messages
- Include timestamps in protocol payloads for diagnostics and control

---

## Measurement and Validation

- GPIO bracketing at key stages; capture on logic analyzer
- Cycle counters (DWT) for code sections
- Packet captures with precise timestamping
- Plot latency histograms; compare to budgets

---

## Failure Modes and Mitigations

- Buffer exhaustion → add backpressure, reduce burst size, increase pools
- Priority inversion → use priority inheritance mutexes or redesign locking
- Cache effects on DMA → ensure cache maintenance or map DMA buffers non-cacheable
- PMTU/MSS mismatch → set MSS to avoid fragmentation; validate path MTU
- Congestion → rate-limit, DSCP QoS, or move to TSN

---

## Minimal UDP Low-Latency Path (pseudo C)

```c
// Setup socket with DSCP for priority and small buffers tuned for latency
int s = socket(AF_INET, SOCK_DGRAM, 0);
int tos = 0x2e; // Expedited Forwarding DSCP 46 (example)
setsockopt(s, IPPROTO_IP, IP_TOS, &tos, sizeof(tos));
int rxbuf = 8 * 1024, txbuf = 8 * 1024; // small, avoid buffering delays
setsockopt(s, SOL_SOCKET, SO_RCVBUF, &rxbuf, sizeof(rxbuf));
setsockopt(s, SOL_SOCKET, SO_SNDBUF, &txbuf, sizeof(txbuf));

// Critical loop does minimal work; offload heavy processing to another thread
for (;;) {
  int n = recv(s, buf, sizeof(buf), 0);
  process_minimal(buf, n);
  sendto(s, reply, reply_len, 0, (struct sockaddr*)&peer, sizeof(peer));
}
```

---

## Checklist

- Deterministic memory allocation and buffer sizing
- Priorities mapped to real-time importance
- Latency budget per stage with measurement
- Synchronization strategy documented (PTP or equivalent)
- Backpressure and drop policy defined
- Watchdog and recovery for comms stalls


