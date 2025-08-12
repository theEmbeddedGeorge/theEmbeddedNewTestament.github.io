# Embedded Security Guide

## Overview
Embedded security is critical for protecting devices, data, and systems from unauthorized access, tampering, and attacks. This guide covers essential security concepts, implementations, and best practices for embedded systems.

---

## Concept → Why it matters → Minimal example → Try it → Takeaways

**Concept**: Embedded security is about protecting devices and data from unauthorized access, tampering, and attacks. It's not just about encryption, but about building a comprehensive security architecture that includes secure boot, cryptographic implementations, side-channel attack prevention, and secure communication protocols.

**Why it matters**: Security breaches in embedded systems can have serious consequences, from data theft to physical harm. Many embedded devices operate in critical infrastructure, medical devices, or automotive systems where security failures can be catastrophic. Understanding embedded security is essential for building trustworthy systems.

**Minimal example**: A simple secure boot implementation that verifies the integrity of firmware before execution, demonstrating the chain of trust concept.

**Try it**: Implement a basic AES encryption function and then modify it to be resistant to timing attacks, observing how security considerations affect implementation complexity.

**Takeaways**: Embedded security requires a defense-in-depth approach, understanding of cryptographic principles, and awareness of various attack vectors. Security should be designed into the system from the beginning, not added as an afterthought.

---

## Table of Contents
1. [Secure Boot and Chain of Trust](#secure-boot-and-chain-of-trust)
2. [Cryptographic Implementations](#cryptographic-implementations)
3. [Side-channel Attack Prevention](#side-channel-attack-prevention)
4. [Hardware Security Modules (HSM)](#hardware-security-modules-hsm)
5. [Secure Communication Protocols](#secure-communication-protocols)
6. [ARM TrustZone](#arm-trustzone)
7. [Encryption Algorithms](#encryption-algorithms)

---

## Secure Boot and Chain of Trust

### What is Secure Boot?
Secure boot ensures that only trusted, authenticated code can execute on the device. It establishes a chain of trust from the hardware root of trust to the application software.

### Chain of Trust Components

#### 1. Hardware Root of Trust
```c
// Example: Hardware root of trust implementation
typedef struct {
    uint8_t public_key[256];
    uint8_t device_id[32];
    uint8_t secure_flags;
} hw_root_of_trust_t;

// Hardware root of trust verification
int verify_hw_root_of_trust(void) {
    hw_root_of_trust_t *hw_rot = (hw_root_of_trust_t*)HW_ROT_ADDRESS;
    
    // Verify hardware root of trust signature
    if (!verify_signature(hw_rot->public_key, hw_rot, sizeof(hw_root_of_trust_t))) {
        return -1;  // Verification failed
    }
    
    return 0;  // Verification successful
}
```

#### 2. Bootloader Verification
```c
// Example: Secure bootloader implementation
int secure_bootloader(void) {
    // 1. Verify hardware root of trust
    if (verify_hw_root_of_trust() != 0) {
        return -1;
    }
    
    // 2. Verify bootloader signature
    if (verify_bootloader_signature() != 0) {
        return -1;
    }
    
    // 3. Verify application signature
    if (verify_application_signature() != 0) {
        return -1;
    }
    
    // 4. Jump to verified application
    jump_to_application();
    
    return 0;
}

// Bootloader signature verification
int verify_bootloader_signature(void) {
    uint8_t *bootloader_addr = (uint8_t*)BOOTLOADER_ADDRESS;
    uint8_t *signature = (uint8_t*)BOOTLOADER_SIGNATURE_ADDRESS;
    
    // Calculate hash of bootloader
    uint8_t hash[SHA256_DIGEST_SIZE];
    sha256_calculate(bootloader_addr, BOOTLOADER_SIZE, hash);
    
    // Verify signature using public key
    return verify_signature(hash, signature, SHA256_DIGEST_SIZE);
}
```

#### 3. Application Verification
```c
// Example: Application signature verification
int verify_application_signature(void) {
    uint8_t *app_addr = (uint8_t*)APPLICATION_ADDRESS;
    uint8_t *signature = (uint8_t*)APP_SIGNATURE_ADDRESS;
    
    // Calculate hash of application
    uint8_t hash[SHA256_DIGEST_SIZE];
    sha256_calculate(app_addr, APPLICATION_SIZE, hash);
    
    // Verify signature
    return verify_signature(hash, signature, SHA256_DIGEST_SIZE);
}
```

### Secure Boot Implementation

#### Boot Sequence
```c
// Example: Complete secure boot sequence
void secure_boot_sequence(void) {
    // 1. Initialize security hardware
    security_hw_init();
    
    // 2. Verify hardware root of trust
    if (verify_hw_root_of_trust() != 0) {
        security_failure_handler();
    }
    
    // 3. Verify bootloader
    if (verify_bootloader_signature() != 0) {
        security_failure_handler();
    }
    
    // 4. Verify application
    if (verify_application_signature() != 0) {
        security_failure_handler();
    }
    
    // 5. Set secure flags
    set_secure_flags();
    
    // 6. Jump to application
    jump_to_application();
}

// Security failure handler
void security_failure_handler(void) {
    // Clear sensitive data
    clear_sensitive_data();
    
    // Disable debug interfaces
    disable_debug_interfaces();
    
    // Enter secure failure mode
    while (1) {
        // Wait for reset or secure recovery
        watchdog_reset();
    }
}
```

---

## Cryptographic Implementations

### Symmetric Encryption

#### AES Implementation
```c
// Example: AES-128 encryption implementation
typedef struct {
    uint32_t round_keys[44];
} aes_context_t;

// AES key expansion
void aes_key_expansion(const uint8_t *key, aes_context_t *ctx) {
    uint32_t *rk = ctx->round_keys;
    uint32_t temp;
    int i = 0;
    
    // Copy initial key
    for (i = 0; i < 4; i++) {
        rk[i] = ((uint32_t)key[4*i] << 24) |
                ((uint32_t)key[4*i+1] << 16) |
                ((uint32_t)key[4*i+2] << 8) |
                ((uint32_t)key[4*i+3]);
    }
    
    // Generate round keys
    for (i = 4; i < 44; i++) {
        temp = rk[i-1];
        if (i % 4 == 0) {
            temp = sub_word(rot_word(temp)) ^ rcon[i/4];
        }
        rk[i] = rk[i-4] ^ temp;
    }
}

// AES encryption
void aes_encrypt(const aes_context_t *ctx, const uint8_t *input, uint8_t *output) {
    uint8_t state[16];
    int round;
    
    // Copy input to state
    memcpy(state, input, 16);
    
    // Add round key
    add_round_key(state, ctx->round_keys, 0);
    
    // Main rounds
    for (round = 1; round < 10; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, ctx->round_keys, round);
    }
    
    // Final round
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, ctx->round_keys, 10);
    
    // Copy state to output
    memcpy(output, state, 16);
}
```

#### AES Decryption
```c
// Example: AES-128 decryption implementation
void aes_decrypt(const aes_context_t *ctx, const uint8_t *input, uint8_t *output) {
    uint8_t state[16];
    int round;
    
    // Copy input to state
    memcpy(state, input, 16);
    
    // Add round key
    add_round_key(state, ctx->round_keys, 10);
    
    // Main rounds
    for (round = 9; round > 0; round--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, ctx->round_keys, round);
        inv_mix_columns(state);
    }
    
    // Final round
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, ctx->round_keys, 0);
    
    // Copy state to output
    memcpy(output, state, 16);
}
```

### Asymmetric Encryption

#### RSA Implementation
```c
// Example: RSA key generation and encryption
typedef struct {
    uint32_t n[64];  // Modulus
    uint32_t e[64];  // Public exponent
    uint32_t d[64];  // Private exponent
    uint32_t p[32];  // Prime p
    uint32_t q[32];  // Prime q
    uint32_t dp[32]; // d mod (p-1)
    uint32_t dq[32]; // d mod (q-1)
    uint32_t qinv[32]; // q^(-1) mod p
} rsa_context_t;

// RSA key generation
int rsa_generate_key(rsa_context_t *ctx, int bits) {
    // Generate two large prime numbers
    if (generate_prime(ctx->p, bits/2) != 0) {
        return -1;
    }
    
    if (generate_prime(ctx->q, bits/2) != 0) {
        return -1;
    }
    
    // Calculate n = p * q
    multiply(ctx->n, ctx->p, ctx->q);
    
    // Calculate φ(n) = (p-1) * (q-1)
    uint32_t phi[64];
    subtract(phi, ctx->p, 1);
    uint32_t temp[64];
    subtract(temp, ctx->q, 1);
    multiply(phi, phi, temp);
    
    // Choose public exponent e
    ctx->e[0] = 65537;  // Common choice
    
    // Calculate private exponent d
    mod_inverse(ctx->d, ctx->e, phi);
    
    return 0;
}

// RSA encryption
int rsa_encrypt(const rsa_context_t *ctx, const uint8_t *input, uint8_t *output) {
    uint32_t message[64];
    uint32_t result[64];
    
    // Convert input to integer
    bytes_to_int(message, input, 32);
    
    // Encrypt: c = m^e mod n
    mod_exp(result, message, ctx->e, ctx->n);
    
    // Convert result to bytes
    int_to_bytes(output, result, 64);
    
    return 0;
}
```

### Hash Functions

#### SHA-256 Implementation
```c
// Example: SHA-256 hash function
typedef struct {
    uint32_t state[8];
    uint32_t count[2];
    uint8_t buffer[64];
} sha256_context_t;

// SHA-256 initialization
void sha256_init(sha256_context_t *ctx) {
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
    
    ctx->count[0] = 0;
    ctx->count[1] = 0;
}

// SHA-256 update
void sha256_update(sha256_context_t *ctx, const uint8_t *data, uint32_t len) {
    uint32_t i;
    
    for (i = 0; i < len; i++) {
        ctx->buffer[ctx->count[0]] = data[i];
        ctx->count[0]++;
        
        if (ctx->count[0] == 64) {
            sha256_transform(ctx, ctx->buffer);
            ctx->count[0] = 0;
        }
    }
}

// SHA-256 finalization
void sha256_final(sha256_context_t *ctx, uint8_t *hash) {
    uint8_t finalcount[8];
    int i;
    
    // Convert count to bytes
    for (i = 0; i < 8; i++) {
        finalcount[i] = (uint8_t)((ctx->count[(i >= 4 ? 0 : 1)] >> ((3-(i & 3)) * 8)) & 255);
    }
    
    // Add padding
    sha256_update(ctx, (uint8_t*)"\x80", 1);
    while (ctx->count[0] != 56) {
        sha256_update(ctx, (uint8_t*)"\x00", 1);
    }
    
    // Add length
    sha256_update(ctx, finalcount, 8);
    
    // Convert state to hash
    for (i = 0; i < 8; i++) {
        hash[i*4] = (uint8_t)((ctx->state[i] >> 24) & 255);
        hash[i*4+1] = (uint8_t)((ctx->state[i] >> 16) & 255);
        hash[i*4+2] = (uint8_t)((ctx->state[i] >> 8) & 255);
        hash[i*4+3] = (uint8_t)(ctx->state[i] & 255);
    }
}
```

---

## Side-channel Attack Prevention

### What are Side-channel Attacks?
Side-channel attacks exploit information leaked through physical characteristics of the system, such as:
- Power consumption
- Electromagnetic emissions
- Timing variations
- Cache behavior

### Power Analysis Attacks

#### Simple Power Analysis (SPA)
```c
// Vulnerable implementation - leaks information through power consumption
int vulnerable_aes_encrypt(const uint8_t *key, const uint8_t *input, uint8_t *output) {
    aes_context_t ctx;
    aes_key_expansion(key, &ctx);
    
    // This leaks information about the key through power consumption
    for (int i = 0; i < 16; i++) {
        output[i] = input[i] ^ key[i];  // Power consumption varies with key bits
    }
    
    return 0;
}

// Protected implementation - constant power consumption
int protected_aes_encrypt(const uint8_t *key, const uint8_t *input, uint8_t *output) {
    aes_context_t ctx;
    aes_key_expansion(key, &ctx);
    
    // Use constant-time operations
    for (int i = 0; i < 16; i++) {
        // Constant-time XOR operation
        output[i] = constant_time_xor(input[i], key[i]);
    }
    
    return 0;
}
```

#### Differential Power Analysis (DPA)
```c
// DPA-resistant implementation
int dpa_resistant_aes_encrypt(const uint8_t *key, const uint8_t *input, uint8_t *output) {
    aes_context_t ctx;
    uint8_t masked_key[16];
    uint8_t masked_input[16];
    
    // Apply random masking
    for (int i = 0; i < 16; i++) {
        uint8_t mask = generate_random_byte();
        masked_key[i] = key[i] ^ mask;
        masked_input[i] = input[i] ^ mask;
    }
    
    // Perform encryption with masked values
    aes_key_expansion(masked_key, &ctx);
    aes_encrypt(&ctx, masked_input, output);
    
    // Remove masking from output
    for (int i = 0; i < 16; i++) {
        output[i] ^= masked_key[i];
    }
    
    return 0;
}
```

### Timing Attack Prevention

#### Constant-Time Operations
```c
// Vulnerable comparison - leaks timing information
int vulnerable_compare(const uint8_t *a, const uint8_t *b, int len) {
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return 0;  // Early exit leaks timing information
        }
    }
    return 1;
}

// Constant-time comparison
int constant_time_compare(const uint8_t *a, const uint8_t *b, int len) {
    uint8_t result = 0;
    
    for (int i = 0; i < len; i++) {
        result |= a[i] ^ b[i];  // Always perform all operations
    }
    
    return result == 0;
}

// Constant-time conditional copy
void constant_time_conditional_copy(uint8_t *dst, const uint8_t *src, int len, int condition) {
    uint8_t mask = condition ? 0xFF : 0x00;
    
    for (int i = 0; i < len; i++) {
        dst[i] = (dst[i] & ~mask) | (src[i] & mask);
    }
}
```

### Cache Attack Prevention

#### Cache-Timing Resistant Implementation
```c
// Cache-timing resistant AES S-box lookup
uint8_t cache_resistant_sbox_lookup(uint8_t index) {
    uint8_t sbox[256];
    uint8_t result = 0;
    
    // Load entire S-box into cache
    for (int i = 0; i < 256; i++) {
        // Always access all elements to prevent cache timing
        uint8_t temp = sbox[i];
        if (i == index) {
            result = temp;
        }
    }
    
    return result;
}
```

---

## Hardware Security Modules (HSM)

### What is an HSM?
A Hardware Security Module (HSM) is a physical device that provides secure storage and processing of cryptographic keys and operations.

### HSM Integration

#### HSM Interface
```c
// Example: HSM interface implementation
typedef struct {
    uint32_t hsm_id;
    uint8_t public_key[256];
    uint8_t certificate[1024];
} hsm_info_t;

// HSM initialization
int hsm_init(void) {
    // Initialize HSM hardware
    if (hsm_hw_init() != 0) {
        return -1;
    }
    
    // Verify HSM integrity
    if (hsm_verify_integrity() != 0) {
        return -1;
    }
    
    // Load HSM configuration
    if (hsm_load_config() != 0) {
        return -1;
    }
    
    return 0;
}

// HSM key generation
int hsm_generate_key(uint32_t key_id, uint32_t key_type, uint32_t key_size) {
    hsm_command_t cmd;
    hsm_response_t resp;
    
    cmd.command = HSM_CMD_GENERATE_KEY;
    cmd.key_id = key_id;
    cmd.key_type = key_type;
    cmd.key_size = key_size;
    
    return hsm_send_command(&cmd, &resp);
}

// HSM encryption
int hsm_encrypt(uint32_t key_id, const uint8_t *input, uint8_t *output, uint32_t len) {
    hsm_command_t cmd;
    hsm_response_t resp;
    
    cmd.command = HSM_CMD_ENCRYPT;
    cmd.key_id = key_id;
    cmd.data_len = len;
    memcpy(cmd.data, input, len);
    
    if (hsm_send_command(&cmd, &resp) != 0) {
        return -1;
    }
    
    memcpy(output, resp.data, resp.data_len);
    return 0;
}
```

### TPM Integration

#### TPM 2.0 Implementation
```c
// Example: TPM 2.0 integration
typedef struct {
    uint32_t tpm_id;
    uint8_t ek_public[256];
    uint8_t srk_public[256];
} tpm_context_t;

// TPM initialization
int tpm_init(tpm_context_t *ctx) {
    // Initialize TPM hardware
    if (tpm_hw_init() != 0) {
        return -1;
    }
    
    // Start TPM
    if (tpm_startup() != 0) {
        return -1;
    }
    
    // Get TPM capabilities
    if (tpm_get_capabilities() != 0) {
        return -1;
    }
    
    return 0;
}

// TPM key creation
int tpm2_create_key(tpm_context_t *ctx, uint32_t key_handle, uint32_t key_type) {
    tpm2_command_t cmd;
    tpm2_response_t resp;
    
    cmd.header.tag = TPM2_ST_NO_SESSIONS;
    cmd.header.command_code = TPM2_CC_CreatePrimary;
    cmd.header.param_size = sizeof(cmd);
    
    cmd.primary_handle = TPM2_RH_ENDORSEMENT;
    cmd.in_sensitive.sensitive.user_auth.size = 0;
    cmd.in_public.public_area.type = key_type;
    cmd.in_public.public_area.parameters.rsa_detail.key_bits = 2048;
    
    return tpm2_send_command(&cmd, &resp);
}
```

---

## Secure Communication Protocols

### TLS/SSL Implementation

#### TLS Handshake
```c
// Example: Simplified TLS handshake
typedef struct {
    uint8_t client_random[32];
    uint8_t server_random[32];
    uint8_t master_secret[48];
    uint8_t session_key[32];
} tls_session_t;

// TLS handshake
int tls_handshake(tls_session_t *session) {
    // 1. Client Hello
    if (tls_client_hello(session) != 0) {
        return -1;
    }
    
    // 2. Server Hello
    if (tls_server_hello(session) != 0) {
        return -1;
    }
    
    // 3. Certificate exchange
    if (tls_certificate_exchange(session) != 0) {
        return -1;
    }
    
    // 4. Key exchange
    if (tls_key_exchange(session) != 0) {
        return -1;
    }
    
    // 5. Finished
    if (tls_finished(session) != 0) {
        return -1;
    }
    
    return 0;
}

// TLS encryption
int tls_encrypt(tls_session_t *session, const uint8_t *input, uint8_t *output, uint32_t len) {
    uint8_t iv[16];
    uint8_t mac[32];
    
    // Generate IV
    generate_random(iv, 16);
    
    // Calculate MAC
    hmac_calculate(session->session_key, input, len, mac);
    
    // Encrypt data
    aes_encrypt_cbc(session->session_key, iv, input, output, len);
    
    // Append MAC
    memcpy(output + len, mac, 32);
    
    return 0;
}
```

### Secure Bootstrapping

#### Device Authentication
```c
// Example: Device authentication protocol
typedef struct {
    uint8_t device_id[32];
    uint8_t challenge[32];
    uint8_t response[64];
    uint8_t session_key[32];
} device_auth_t;

// Device authentication
int device_authenticate(device_auth_t *auth) {
    // 1. Generate challenge
    generate_random(auth->challenge, 32);
    
    // 2. Send challenge to device
    if (send_challenge(auth->challenge) != 0) {
        return -1;
    }
    
    // 3. Receive response
    if (receive_response(auth->response) != 0) {
        return -1;
    }
    
    // 4. Verify response
    if (verify_device_response(auth->challenge, auth->response) != 0) {
        return -1;
    }
    
    // 5. Generate session key
    generate_session_key(auth->session_key);
    
    return 0;
}
```

---

## ARM TrustZone

### What is ARM TrustZone?
ARM TrustZone provides hardware-based security by creating secure and non-secure worlds within the same processor.

### TrustZone Implementation

#### Secure World Setup
```c
// Example: TrustZone secure world implementation
typedef struct {
    uint32_t secure_flags;
    uint8_t secure_key[32];
    uint8_t secure_data[1024];
} secure_world_t;

// Secure world initialization
void secure_world_init(void) {
    // Set up secure world
    configure_secure_world();
    
    // Initialize secure peripherals
    init_secure_peripherals();
    
    // Load secure applications
    load_secure_applications();
}

// Secure world entry
void secure_world_entry(void) {
    // Switch to secure world
    smc_instruction(SMC_SECURE_ENTRY);
    
    // Handle secure world operations
    while (1) {
        uint32_t command = get_secure_command();
        handle_secure_command(command);
    }
}

// Secure world command handler
void handle_secure_command(uint32_t command) {
    switch (command) {
        case SECURE_CMD_ENCRYPT:
            secure_encrypt();
            break;
        case SECURE_CMD_DECRYPT:
            secure_decrypt();
            break;
        case SECURE_CMD_KEY_GEN:
            secure_key_generation();
            break;
        default:
            secure_error_handler();
            break;
    }
}
```

#### Non-Secure World Interface
```c
// Example: Non-secure world interface
int non_secure_encrypt(const uint8_t *input, uint8_t *output, uint32_t len) {
    // Prepare parameters for secure world
    secure_params_t params;
    params.command = SECURE_CMD_ENCRYPT;
    params.input = input;
    params.output = output;
    params.length = len;
    
    // Call secure world
    smc_instruction(SMC_SECURE_CALL, &params);
    
    return params.result;
}

// SMC (Secure Monitor Call) implementation
void smc_instruction(uint32_t function_id, void *params) {
    // Set up registers for SMC
    __asm volatile (
        "mov r0, %0\n"
        "mov r1, %1\n"
        "smc #0\n"
        : : "r" (function_id), "r" (params) : "r0", "r1"
    );
}
```

---

## Encryption Algorithms

### AES-GCM Implementation

#### GCM Mode
```c
// Example: AES-GCM implementation
typedef struct {
    aes_context_t aes_ctx;
    uint8_t h[16];
    uint8_t j0[16];
    uint8_t s[16];
} aes_gcm_context_t;

// AES-GCM initialization
void aes_gcm_init(aes_gcm_context_t *ctx, const uint8_t *key, const uint8_t *iv) {
    // Initialize AES context
    aes_key_expansion(key, &ctx->aes_ctx);
    
    // Calculate H = E(K, 0^128)
    uint8_t zero[16] = {0};
    aes_encrypt(&ctx->aes_ctx, zero, ctx->h);
    
    // Calculate J0
    if (iv_len == 12) {
        memcpy(ctx->j0, iv, 12);
        ctx->j0[15] = 1;
    } else {
        // GHASH(H, 0^s || IV || 0^t)
        ghash(ctx->h, iv, iv_len, ctx->j0);
    }
    
    // Initialize S
    memset(ctx->s, 0, 16);
}

// AES-GCM encryption
void aes_gcm_encrypt(aes_gcm_context_t *ctx, const uint8_t *input, uint8_t *output, uint32_t len) {
    uint8_t y[16];
    uint32_t i;
    
    // Y0 = E(K, J0)
    aes_encrypt(&ctx->aes_ctx, ctx->j0, y);
    
    // Encrypt data
    for (i = 0; i < len; i++) {
        if (i % 16 == 0 && i > 0) {
            // Increment counter
            increment_counter(ctx->j0);
            aes_encrypt(&ctx->aes_ctx, ctx->j0, y);
        }
        output[i] = input[i] ^ y[i % 16];
    }
    
    // Calculate authentication tag
    ghash_calculate(ctx->h, output, len, ctx->s);
    aes_encrypt(&ctx->aes_ctx, ctx->j0, y);
    xor_bytes(ctx->s, y, ctx->s, 16);
}
```

### ChaCha20-Poly1305 Implementation

#### ChaCha20 Stream Cipher
```c
// Example: ChaCha20 implementation
typedef struct {
    uint32_t state[16];
    uint8_t key[32];
    uint8_t nonce[12];
} chacha20_context_t;

// ChaCha20 initialization
void chacha20_init(chacha20_context_t *ctx, const uint8_t *key, const uint8_t *nonce) {
    // Initialize state
    ctx->state[0] = 0x61707865;  // "expa"
    ctx->state[1] = 0x3320646e;  // "nd 3"
    ctx->state[2] = 0x79622d32;  // "2-by"
    ctx->state[3] = 0x6b206574;  // "te k"
    
    // Load key
    for (int i = 0; i < 8; i++) {
        ctx->state[4+i] = ((uint32_t)key[4*i] << 24) |
                          ((uint32_t)key[4*i+1] << 16) |
                          ((uint32_t)key[4*i+2] << 8) |
                          ((uint32_t)key[4*i+3]);
    }
    
    // Load nonce
    ctx->state[12] = 0;
    ctx->state[13] = 0;
    ctx->state[14] = ((uint32_t)nonce[0] << 24) |
                     ((uint32_t)nonce[1] << 16) |
                     ((uint32_t)nonce[2] << 8) |
                     ((uint32_t)nonce[3]);
    ctx->state[15] = ((uint32_t)nonce[4] << 24) |
                     ((uint32_t)nonce[5] << 16) |
                     ((uint32_t)nonce[6] << 8) |
                     ((uint32_t)nonce[7]);
}

// ChaCha20 encryption
void chacha20_encrypt(chacha20_context_t *ctx, const uint8_t *input, uint8_t *output, uint32_t len) {
    uint8_t keystream[64];
    uint32_t block_counter = 0;
    
    for (uint32_t i = 0; i < len; i += 64) {
        // Generate keystream block
        chacha20_block(ctx, block_counter, keystream);
        
        // XOR with input
        uint32_t block_len = (len - i < 64) ? (len - i) : 64;
        for (uint32_t j = 0; j < block_len; j++) {
            output[i + j] = input[i + j] ^ keystream[j];
        }
        
        block_counter++;
    }
}
```

---

## Security Best Practices

### General Security Guidelines
1. **Use secure random number generators** - avoid predictable values
2. **Implement proper key management** - secure storage and rotation
3. **Use constant-time operations** - prevent timing attacks
4. **Validate all inputs** - prevent buffer overflows and injection attacks
5. **Implement secure communication** - use TLS/SSL for data transmission

### Security Checklist
- [ ] Implement secure boot and chain of trust
- [ ] Use cryptographic algorithms correctly
- [ ] Protect against side-channel attacks
- [ ] Implement secure key storage
- [ ] Use secure communication protocols
- [ ] Validate all inputs and outputs
- [ ] Implement secure update mechanisms
- [ ] Monitor for security events

### Common Security Mistakes
1. **Using weak cryptographic algorithms** - avoid deprecated algorithms
2. **Hardcoding secrets** - use secure key storage
3. **Ignoring side-channel attacks** - implement constant-time operations
4. **Not validating inputs** - always validate and sanitize inputs
5. **Using predictable random numbers** - use cryptographically secure RNGs

---

## Resources

### Standards and Specifications
- [NIST Cryptographic Standards](https://www.nist.gov/cryptography)
- [FIPS 140-2](https://csrc.nist.gov/publications/detail/fips/140/2/final) - Security Requirements for Cryptographic Modules
- [Common Criteria](https://www.commoncriteriaportal.org/) - Security evaluation framework

### Tools and Libraries
- [mbed TLS](https://tls.mbed.org/) - Open-source SSL/TLS library
- [OpenSSL](https://www.openssl.org/) - Open-source cryptography library
- [wolfSSL](https://www.wolfssl.com/) - Embedded SSL/TLS library

### Books and References
- "Applied Cryptography" by Bruce Schneier
- "Cryptography Engineering" by Ferguson, Schneier, and Kohno
- "The Art of Deception" by Kevin Mitnick

### Online Resources
- [Cryptography Stack Exchange](https://crypto.stackexchange.com/)
- [Security Stack Exchange](https://security.stackexchange.com/)
- [OWASP Embedded Application Security](https://owasp.org/www-project-embedded-application-security/)

---

## Guided Labs

### Lab 1: Secure Boot Implementation
**Objective**: Implement a basic secure boot mechanism that verifies firmware integrity before execution.

**Setup**: Create a simple bootloader that verifies a digital signature before jumping to the main application.

**Steps**:
1. Implement a basic SHA-256 hash function
2. Create a simple digital signature verification function
3. Modify the bootloader to verify the application signature
4. Test with valid and invalid signatures
5. Observe the system behavior when verification fails

**Expected Outcome**: Understanding of how secure boot establishes a chain of trust and prevents unauthorized code execution.

### Lab 2: Side-Channel Attack Prevention
**Objective**: Learn how to implement cryptographic functions that are resistant to timing attacks.

**Setup**: Implement AES encryption and then modify it to be constant-time.

**Steps**:
1. Implement basic AES encryption with lookup tables
2. Profile the function to identify timing variations
3. Implement constant-time operations for sensitive operations
4. Re-profile to verify timing consistency
5. Test with various input patterns to ensure resistance

**Expected Outcome**: Understanding of how side-channel attacks work and how to implement countermeasures.

### Lab 3: Secure Communication Protocol
**Objective**: Implement a basic secure communication protocol using symmetric encryption.

**Setup**: Create a simple client-server communication system with encryption and authentication.

**Steps**:
1. Implement AES encryption for data confidentiality
2. Add HMAC for message authentication
3. Implement a simple key exchange protocol
4. Test the protocol with various attack scenarios
5. Analyze the security properties of the implementation

**Expected Outcome**: Understanding of how to build secure communication protocols and the importance of proper key management.

---

## Check Yourself

### Understanding Check
- [ ] Can you explain the concept of a chain of trust in secure boot?
- [ ] Do you understand the difference between symmetric and asymmetric encryption?
- [ ] Can you explain how side-channel attacks work and how to prevent them?
- [ ] Do you understand the role of cryptographic hash functions in security?
- [ ] Can you explain the concept of secure communication protocols?

### Application Check
- [ ] Can you implement a basic secure boot mechanism?
- [ ] Do you know how to implement constant-time cryptographic operations?
- [ ] Can you design a secure key management system?
- [ ] Do you understand how to implement secure communication protocols?
- [ ] Can you assess the security implications of different design choices?

### Analysis Check
- [ ] Can you analyze a security architecture for potential vulnerabilities?
- [ ] Do you understand the trade-offs between security and performance?
- [ ] Can you evaluate the security strength of different cryptographic algorithms?
- [ ] Do you know how to implement secure update mechanisms?
- [ ] Can you assess the impact of security failures on system reliability?

---

## Cross-links

### Related Topics
- **[System Integration](./../System_Integration/Build_Systems.md)**: Integrating security into the build and deployment process
- **[Performance Optimization](./../Performance/performance_optimization.md)**: Balancing security requirements with performance constraints
- **[Communication Protocols](./../Communication_Protocols/Secure_Communication.md)**: Implementing secure communication protocols
- **[Real-Time Systems](./../Real_Time_Systems/FreeRTOS_Basics.md)**: Integrating security with real-time constraints

### Further Reading
- **NIST Cryptographic Standards**: Official cryptographic standards and guidelines
- **Common Criteria**: International standard for security evaluation
- **OWASP Embedded Security**: Security guidelines for embedded systems
- **ARM Security Documentation**: TrustZone and security features documentation

### Industry Standards
- **FIPS 140-2**: Security requirements for cryptographic modules
- **Common Criteria**: Security evaluation framework
- **ISO 27001**: Information security management systems
- **IEC 62443**: Industrial automation and control systems security
- **SAE J3061**: Cybersecurity guidebook for cyber-physical vehicle systems
