# Protocol Analysis and Debugging

Effective protocol analysis accelerates bring-up, reveals timing bugs, and derisks field failures. This guide covers instrumentation, capture strategy, timing analysis, and a practical checklist for UART, SPI, I2C, CAN, and Ethernet-based protocols.

---

## Instruments

- Logic analyzer: digital sampling, protocol decoders (UART/SPI/I2C/CAN). Choose adequate memory depth and sample rate.
- Oscilloscope: analog waveform fidelity, rise/fall time, ringing, overshoot, jitter.
- Protocol analyzers: CAN/CAN-FD, LIN, USB, Ethernet taps; dedicated decoders and timing.
- Network tools: port mirroring (SPAN), capture on host/NIC, software packet capture, and timestamping.

Rule of thumb for digital sampling: sample at least 10× the highest signal frequency of interest to capture edges reliably; increase for noisy lines.

---

## Capture Strategy

- Define the question first: throughput? latency? error conditions? state machine bug?
- Trigger precisely: start-of-frame, specific byte sequence, error flag, GPIO marker from firmware.
- Segment captures: short, focused windows around events; maintain a naming convention and notes.
- Correlate layers: align logic analyzer events with firmware logs and network packet timestamps.

---

## UART

- Common issues: baud mismatch, wrong parity/stop bits, inverted polarity, framing and overrun errors.
- Tests:
  - Send PRBS or known pattern to verify bit timing and jitter.
  - Verify ISR latency does not exceed inter-byte time at configured baud.
  - Validate RX ring buffer size and overflow counters.

### Example Timing Budget

- Inter-byte time at 115200, 8N1: 10 bits/byte → ~86.8 µs/byte. Ensure worst-case ISR latency < half inter-byte time if using interrupt-driven RX without DMA.

---

## SPI

- Parameters: mode (CPOL/CPHA), clock rate, chip-select timing, bit order.
- Checks:
  - Verify setup/hold relative to sampling edge via oscilloscope.
  - Confirm CS deassertion between frames when required by slave.
  - Use DMA for long transfers; ensure cache coherence on cached MCUs.
  - Ensure maximum clock respects slave device spec incl. PCB signal integrity.

---

## I2C

- Failure patterns: clock stretching not honored, NACK handling, bus contention, pull-up sizing.
- Checklist:
  - Confirm pull-ups produce compliant rise time for chosen speed (Std/Fast/Fast+). Adjust resistors or reduce bus capacitance.
  - Validate repeated START sequences and addressing (7/10-bit).
  - Bus recovery: generate up to 9 SCL pulses if SDA held low by a slave.

---

## CAN and CAN-FD

- Bit timing: prescaler, time segments, synchronization jump width, and sample point.
- Typical sample point targets: ~87.5% for many speeds; verify against transceiver and topology.
- Error analysis: error counters, arbitration loss, stuff errors, CRC errors.
- Bus utilization: aim < 70% for headroom in real-time systems.

### Example Bit Timing Calculation (conceptual)

- Nominal bit rate = f_clk / (Prescaler × TimeQuantaPerBit)
- Sample point = (Sync_Seg + TSEG1) / (Sync_Seg + TSEG1 + TSEG2)

---

## Ethernet/TCP/IP

- Use SPAN or an in-line tap for lossless captures on full-duplex links.
- Useful filters:
  - `arp`, `icmp`, `tcp.port == 1883`, `udp.port == 5683`, `ip.addr == 198.51.100.10`
- Metrics to extract:
  - RTT (SYN/SYN-ACK), TCP retransmissions, out-of-order, MSS/MTU, DNS latency.
- Timestamps:
  - Hardware timestamping improves accuracy; software timestamps are adequate for many MCU use cases.

---

## Timing and Jitter

- Definitions:
  - Latency: time from event A to B.
  - Jitter: variability of latency; focus on 99th/99.9th percentiles.
- Techniques:
  - GPIO toggles in ISR/task boundaries to bracket stages; capture with LA/scope.
  - Cycle counters (DWT on ARM) for intra-CPU timing.
  - PTP for network time sync when sub-millisecond correlation is required.

---

## Structured Debug Checklist

1) Reproduce and bound the problem
- Define exact stimulus and expected behavior
- Capture logs (firmware + analyzer) with timestamps

2) Validate physical layer
- Signal integrity, voltage levels, terminations, pull-ups, EMC noise

3) Verify timing
- Baud/bit rates, setup/hold, interrupt/DMA service time, buffering

4) Confirm configuration
- Mode, addressing, endianness, CRC/checksum, sequence numbers

5) Inspect protocol semantics
- State machines, retransmission/backoff, flow control, error handling

6) Introduce instrumentation
- Event markers (GPIO), counters, histograms, trace buffers

7) Mitigate, then fix
- Workarounds to stabilize system (timeouts, backoff, lower bitrate)
- Root-cause and implement permanent fixes

---

## Artifacts and Documentation

- Save capture files with metadata: device firmware version, hardware rev, conditions, and a brief narrative.
- Keep before/after captures when validating fixes.
- Maintain a library of known-good traces for regression comparison.


