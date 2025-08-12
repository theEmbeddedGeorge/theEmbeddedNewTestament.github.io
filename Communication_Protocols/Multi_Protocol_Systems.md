# Multi-Protocol Systems

Embedded products often bridge, translate, or coordinate multiple protocols (e.g., UART↔CAN, CAN↔Ethernet, I2C↔SPI). This document provides patterns to design reliable, maintainable multi-protocol systems with predictable behavior under load.

---

## Design Goals
- Lossless or controlled-loss routing under bursty traffic
- Bounded latency with prioritization
- Backpressure and flow control across protocol boundaries
- Extensibility for new endpoints and message formats

---

## Reference Architecture

- Hardware interfaces: UART, SPI, I2C, CAN/CAN-FD, Ethernet/Wi‑Fi, USB
- Software layers:
  - Drivers (ISR/DMA)
  - I/O queues (RX/TX ring buffers)
  - Protocol adapters (frame parse/compose)
  - Router (message classification and forwarding)
  - Application services (control, management, persistence)
- RTOS building blocks: tasks per interface, central router task, event groups, message queues, memory pools

```text
ISR/DMA → RX Queue → Adapter → Router → Adapter → TX Queue → Driver/DMA → Wire
```

---

## Message Model

- Canonical message:
  - Header: source, destination, protocol, priority, length, timestamp
  - Payload: opaque bytes for adapter
- Routing table:
  - Maps (source, type) → output(s) with optional transforms
- Priority classes:
  - Critical, High, Normal, Low mapped to scheduler priorities and queue depths

---

## Flow Control and Backpressure

- UART: software/hardware flow control (XON/XOFF, RTS/CTS)
- CAN: natural arbitration; still implement queue depth limits and drop policy
- Ethernet/TCP: TCP flow control assists, but preserve internal backpressure to prevent memory exhaustion
- Strategies:
  - Watermarks and hysteresis on queues
  - Headroom reservations for critical traffic
  - Leaky-bucket or token-bucket rate limiting between domains

---

## Scheduling and Latency

- Assign priorities: ISR top, then router, then interface tasks by criticality
- Use DMA for bulk transfers to free CPU cycles
- Avoid lengthy work in ISR; push to tasks via queues
- Pre-allocate buffers (no heap on hot path)

---

## Error Handling

- Per-interface error counters and status snapshots
- End-to-end retries with capped backoff and jitter
- Drop policies: tail drop for low priority, or RED-like probabilistic drop
- Persistent fault isolation: disable flapping outputs temporarily

---

## Example Router Loop (pseudo C)

```c
for (;;) {
  message_t *msg = queue_receive(router_q, ROUTER_WAIT_MS);
  if (!msg) continue;

  route_t *r = route_lookup(msg);
  if (!r) { stats.unknown++; buffer_free(msg); continue; }

  for (int i = 0; i < r->num_outputs; i++) {
    adapter_t *ad = r->outputs[i];
    if (!adapter_try_send(ad, msg)) {
      // Apply backpressure policy
      if (msg->priority <= PRIORITY_NORMAL) { stats.drop++; }
      else { adapter_blocking_send(ad, msg, TIMEOUT_MS); }
    }
  }

  buffer_free(msg);
}
```

---

## Bridging Examples

### UART ↔ CAN
- Packetize UART stream into frames with length + CRC
- Map UART message types to CAN IDs; enforce CAN payload ≤ 8/64 bytes (Classic/FD)
- Apply rate limiting from UART→CAN to respect bus utilization targets

### CAN ↔ Ethernet (UDP)
- UDP multicast for broadcast-like semantics; include CAN-ID, timestamp, and flags in UDP payload
- Ensure loss tolerance or implement app-level ACKs if needed
- Consider DSCP marking for priority traffic

---

## Time Synchronization

- For cross-domain correlation: PTP over Ethernet; propagate time to MCU domain
- For CAN-only networks: periodic time-sync frames or gateway time stamping
- Store timestamps in canonical message headers

---

## Security Considerations

- Authenticate management/control messages crossing domains
- Validate lengths and IDs; drop malformed or unauthorized frames
- Rate-limit untrusted inputs; partition critical vs non-critical traffic

---

## Test Strategy

- Soak tests with mixed traffic profiles and bursts
- Fault injection: drop, duplicate, reorder frames; bit errors
- Latency/jitter measurements per path; 99th/99.9th percentile targets
- Capacity tests: fill queues to watermarks; verify drop policy and recovery

---

## Deployment Checklist

- Routing table versioned and remotely updatable
- Queue depths sized from measured traffic distributions
- Backpressure verified end-to-end
- Logs include per-path counters and timestamps
- Watchdog coverage for router and interface tasks


