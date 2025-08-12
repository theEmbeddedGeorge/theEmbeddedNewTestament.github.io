# Network Protocols for Embedded Systems

Designing networked embedded devices requires balancing deterministic behavior, small memory footprints, and constrained CPU cycles while speaking interoperable protocols. This guide focuses on practical, production-oriented aspects for IPv4/IPv6, ICMP/ARP/ND, UDP/TCP, and application-layer IoT protocols.

---

## Goals
- Understand the TCP/IP stack in embedded contexts
- Choose and configure lightweight stacks (e.g., lwIP)
- Implement robust UDP/TCP clients/servers
- Integrate IoT app protocols (MQTT/CoAP)
- Diagnose problems with a structured checklist

---

## TCP/IP Stack Overview

- Network stack models:
  - OSI (7 layers) vs TCP/IP (4 layers). In practice, embedded stacks follow TCP/IP: Link → Internet → Transport → Application.
- Common link layers: Ethernet, Wi‑Fi, PPP, LTE Cat-x, 802.15.4.
- Internet layer:
  - IPv4 header (20 bytes min), IPv6 (40 bytes). IPv6 removes header checksum and uses Neighbor Discovery (ND) instead of ARP.
  - ICMPv4/ICMPv6 for diagnostics and error reporting.
- Transport layer:
  - UDP: message boundaries preserved, lower overhead, no built-in retransmission.
  - TCP: connection-oriented, ordered, reliable stream, congestion and flow control.
- Application layer:
  - IoT protocols: MQTT (pub/sub over TCP), CoAP (REST over UDP), HTTP/HTTPS, custom binary.

---

## Addressing, Name Resolution, and Configuration

- Address assignment:
  - Static IP, DHCPv4, SLAAC/DHCPv6 for IPv6.
- Name resolution:
  - DNS (A/AAAA), mDNS for local discovery.
- ARP (IPv4) and ND (IPv6) for L2 neighbor resolution.
- NAT traversal considerations for outbound vs inbound connectivity (prefer client-initiated sessions for simplicity).

---

## Embedded Network Stacks

- Common choices: lwIP, uIP (legacy/very small), vendor stacks (e.g., STM32Cube, NXP SDK).
- Key configuration levers (lwIP examples):
  - Memory pools: `MEM_SIZE`, `PBUF_POOL_SIZE`, `TCP_MSS`, `TCP_SND_BUF`, `TCP_WND`.
  - Features: enable only what you need (e.g., disable `LWIP_IPV6` if not used; disable `LWIP_DNS` if static names).
  - Threading: `tcpip_thread` priority vs application tasks; ensure input path is high priority and bounded work in callbacks.
  - Zero-copy path from driver to stack when possible (DMA-safe RX/TX buffers, proper cache maintenance on MCUs with cache).

---

## UDP in Embedded Systems

Use UDP when latency and simplicity outweigh guaranteed delivery.

- Pros: small footprint, no connection state, multicast support.
- Cons: no retransmission, ordering, or congestion control.
- Design patterns:
  - App-level sequence numbers and ACK/NACK where needed.
  - Timeouts and retry policy with capped backoff.
  - Message authentication (HMAC) if security layer is not provided elsewhere.

### Minimal UDP Echo (POSIX-like pseudo C)

```c
int sock = socket(AF_INET, SOCK_DGRAM, 0);
struct sockaddr_in addr = { .sin_family = AF_INET, .sin_port = htons(9000), .sin_addr.s_addr = INADDR_ANY };
bind(sock, (struct sockaddr*)&addr, sizeof(addr));

for (;;) {
  uint8_t buf[1500];
  struct sockaddr_in peer; socklen_t len = sizeof(peer);
  int n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&peer, &len);
  if (n > 0) {
    sendto(sock, buf, n, 0, (struct sockaddr*)&peer, len);
  }
}
```

---

## TCP in Embedded Systems

Use TCP for reliability and compatibility with cloud services.

- Memory planning:
  - Each TCP PCB and each connection consumes buffers; set limits (`MEMP_NUM_TCP_PCB`, backlog).
  - Choose `TCP_MSS` appropriate to link MTU (Ethernet MTU 1500 → MSS 1460 for IPv4, 1440 for IPv6).
- Nagle vs latency:
  - Disable Nagle (`TCP_NODELAY`) for request/response with small payloads to reduce latency.
  - Keep Nagle for bulk transfers; or coalesce at application layer.
- Keepalive:
  - TCP keepalive timers detect dead peers; configure periods conservatively to avoid unnecessary network chatter on cellular.

### Minimal TCP Client (POSIX-like pseudo C)

```c
int sock = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in srv = { .sin_family = AF_INET, .sin_port = htons(1883), .sin_addr.s_addr = inet_addr("192.0.2.10") };
connect(sock, (struct sockaddr*)&srv, sizeof(srv));

const char hello[] = "ping";
send(sock, hello, sizeof(hello)-1, 0);

uint8_t buf[1024];
int n = recv(sock, buf, sizeof(buf), 0);
// handle response
```

---

## IoT Application Protocols

### MQTT (over TCP)
- Pub/Sub with broker; topics, QoS 0/1/2; retain and last will.
- Embedded notes: persistent sessions reduce handshake cost; limit topic and payload size; batch publishes.

### CoAP (over UDP)
- REST-like with confirmable/non-confirmable messages, observe, block-wise transfer.
- Embedded notes: implement retransmission and token matching carefully; DTLS for security if required.

### HTTP/HTTPS
- Ubiquitous, verbose; consider short timeouts and connection pooling if client.
- Prefer HTTP/1.1 keep-alive; HTTP/2 is heavier for MCUs without proper libraries.

---

## Performance Tuning Checklist

- Link MTU and MSS aligned; avoid IP fragmentation.
- Pre-allocate RX/TX buffers; avoid heap in hot path.
- Use DMA and zero-copy pbufs where supported.
- Configure interrupt coalescing (if NIC supports) vs latency goals.
- Pin high-priority threads; bound ISR work; defer to RTOS tasks.
- Use DSCP/ToS to mark latency-sensitive traffic where network honors QoS.

---

## Reliability and Robustness

- Exponential backoff with jitter for reconnects.
- Dead-peer detection (keepalive, application heartbeats).
- Validate all lengths and parse defensively (avoid buffer overruns).
- Implement watchdog resets around networking stalls with recovery paths.
- Persist credentials and broker endpoints in redundant storage.

---

## Diagnostics

- Packet capture:
  - Mirror/SPAN port or inline hub; or software capture from the driver if feasible.
  - Use filters (examples): `arp`, `icmp`, `tcp.port == 1883`, `udp.port == 5683`.
- Health metrics:
  - RX/TX drops, retransmits, RTT, DNS latency, reconnect counts.
- Common issues:
  - ARP/ND resolution failures → check VLAN, gateway, and subnet masks
  - MSS/MTU mismatch → excessive fragmentation, PMTU black holes
  - Head-of-line blocking in single-threaded MQTT clients → worker thread separation

---

## Security Notes

- Prefer TLS/DTLS with modern ciphersuites; validate certificates and time.
- Use hardware RNG and secure key storage if available.
- Restrict inbound listeners; favor outbound client connections.
- Rate-limit and authenticate management endpoints.

---

## Production Readiness Checklist

- IP configuration: static/DHCPv4/IPv6 SLAAC documented
- DNS fallback and caching behavior defined
- Robust reconnect and backoff strategy
- Heartbeats and liveness probes implemented
- TLS/DTLS configuration with strong RNG and key storage
- Watchdog and brown-out recovery paths tested
- Packet captures for success and failure cases recorded


