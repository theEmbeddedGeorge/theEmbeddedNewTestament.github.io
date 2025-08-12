# Secure Communication for Embedded Systems

Security must be engineered end-to-end: device identity, key storage, protocol selection, and operational processes. This guide focuses on practical, resource-aware approaches.

---

## Threat Model and Goals

- Threats: eavesdropping, MITM, replay, injection, firmware tampering, key extraction
- Goals: confidentiality, integrity, authenticity, freshness, availability
- Trust anchors: device-unique keys, certificates, secure boot chain

---

## Cryptographic Primitives

- Symmetric: AES-GCM/CCM for AEAD
- Asymmetric: ECDSA/ECDH (P-256) for signatures and key exchange
- Hash: SHA-256 for digests and HMAC
- RNG: TRNG preferred; DRBG seeded from hardware entropy

---

## Transport Security

- TLS 1.2/1.3 over TCP; DTLS 1.2/1.3 over UDP
- Client authentication options:
  - PSK: smallest footprint; careful key management
  - Certificates: scalable identity; mutual TLS for device auth
- Library options: mbedTLS, wolfSSL; configure for minimal footprint (disable unused ciphers, curves, features)

### Minimal TLS Client Considerations

- Session resumption to reduce handshake cost
- Keep-alive and backoff for lossy links
- Validate server cert chain and hostname; ensure RTC/time source for validity checks

---

## Key Management

- Generation: on-device with TRNG; avoid provisioning plaintext keys over insecure links
- Storage: secure element/TPM/TrustZone or encrypted flash with anti-rollback
- Rotation: periodic update with remote command; support multiple key slots
- Attestation: device proves identity and firmware state to backend using signed claims

---

## Application-Layer Security

- If transport security is unavailable (e.g., constrained links), use AEAD at message layer (AES-CCM or AES-GCM) with nonce management and sequence numbers
- Include device ID, timestamp, and counter in AAD; reject duplicates (replay protection)
- Authenticate control messages; separate read vs write permissions

### Example: Authenticated UART framing (conceptual)

```text
|LEN|TYPE|CTR|PAYLOAD|TAG|
  ^   ^    ^    ^       ^
  |   |    |    |       +-- 16B authentication tag (AES-CCM)
  |   |    |    +---------- encrypted payload
  |   |    +--------------- 64-bit monotonic counter
  |   +-------------------- message type
  +------------------------ payload length
```

---

## Performance and Footprint

- Use hardware accelerators if present (AES, SHA, PKA)
- Prefer curves with hardware support (e.g., P-256) and compact encodings
- Limit certificate chain length and key sizes appropriate to threat and device lifetime

---

## Common Pitfalls

- Weak RNG → predictable keys and nonces
- Certificate validation disabled or hostname not checked
- Reuse of nonces in AEAD → catastrophic compromise
- Insecure time source → cert validity and replay protections fail
- Silent downgrade to weak ciphersuites

---

## Operational Controls

- Secure boot: verify firmware before execution; anti-rollback
- Secure update: signed (and optionally encrypted) firmware; fail-safe recovery
- Logging and audit: record security events and key operations
- Device offboarding: remote certificate revocation, credential wipe

---

## Security Checklist

- Strong entropy and DRBG health checks
- Private keys generated and stored on device; no plaintext at rest
- TLS/DTLS with modern ciphersuites; mutual auth if needed
- Replay protection and freshness at app layer as required
- Secure boot and anti-rollback enabled
- Secure update path verified end-to-end


