# Embedded Security Fundamentals

> **Building Trustworthy Systems in an Insecure World**  
> Understanding the fundamental principles of embedded security and why they matter

---

## 📋 **Table of Contents**

- [Security Philosophy](#security-philosophy)
- [Threat Models](#threat-models)
- [Attack Vectors](#attack-vectors)
- [Security Principles](#security-principles)
- [Practical Implementation](#practical-implementation)

---

## 🛡️ **Security Philosophy**

### **Why Security Matters in Embedded Systems**

Imagine you're designing a medical device that controls insulin delivery. If an attacker can compromise this device, they could:
- **Overdose a patient** with insulin
- **Deny treatment** by disabling the device
- **Steal sensitive medical data**
- **Cause system failures** that put lives at risk

This isn't science fiction—it's the reality of modern embedded systems.

**The Security Mindset**

Security isn't about making your system "hacker-proof"—it's about making attacks expensive enough that they're not worth attempting. Think of it like a house: you can't make it impossible to break into, but you can make it hard enough that burglars look for easier targets.

---

## 🎯 **Threat Models**

### **Understanding Your Adversaries**

A threat model is like a security assessment that asks: "Who might attack us, and how?" This understanding drives all your security decisions.

#### **Types of Attackers**

**1. Script Kiddies**
- **Motivation**: Boredom, curiosity, bragging rights
- **Capability**: Low - uses existing tools and scripts
- **Resources**: Minimal - just a computer and internet
- **Risk**: Low to medium - can cause disruption but not sophisticated attacks

**2. Cybercriminals**
- **Motivation**: Financial gain through theft, ransomware, or selling stolen data
- **Capability**: Medium to high - can develop custom tools
- **Resources**: Significant - can invest in attack development
- **Risk**: High - motivated and well-funded

**3. Nation-State Actors**
- **Motivation**: Espionage, sabotage, or military advantage
- **Capability**: Very high - access to zero-day exploits and advanced techniques
- **Resources**: Extensive - unlimited budget and time
- **Risk**: Very high - sophisticated and persistent

**4. Insiders**
- **Motivation**: Revenge, financial gain, or ideology
- **Capability**: High - has legitimate access to systems
- **Resources**: High - knows system internals and has access
- **Risk**: Very high - hardest to defend against

#### **Attack Scenarios**

**Scenario 1: Medical Device Compromise**
```
Attacker Goal: Disable insulin pump
Attack Vector: Wireless communication exploit
Impact: Patient harm or death
Difficulty: Medium (requires wireless access)
```

**Scenario 2: Industrial Control System Sabotage**
```
Attacker Goal: Disrupt manufacturing process
Attack Vector: Network infiltration
Impact: Production downtime, safety incidents
Difficulty: High (requires network access)
```

**Scenario 3: Automotive System Takeover**
```
Attacker Goal: Control vehicle systems
Attack Vector: OBD-II port or wireless
Impact: Vehicle hijacking, safety compromise
Difficulty: Medium to high (depends on access)
```

---

## 🚪 **Attack Vectors**

### **How Attackers Get In**

Attack vectors are the paths attackers use to compromise your system. Understanding these helps you build appropriate defenses.

#### **Physical Attack Vectors**

**1. Direct Hardware Access**
```
┌─────────────────────────────────────┐
│           Device Enclosure          │
│  ┌─────────────────────────────┐    │
│  │      Circuit Board          │    │
│  │  ┌─────────────────────┐    │    │
│  │  │   Microcontroller   │    │    │
│  │  │  ┌───────────────┐  │    │    │
│  │  │  │   CPU Core    │  │    │    │
│  │  │  └───────────────┘  │    │    │
│  │  └─────────────────────┘    │    │
│  └─────────────────────────────┘    │
└─────────────────────────────────────┘
```

**Attack Methods:**
- **Chip removal** and replacement
- **Bus tapping** to monitor communications
- **Power analysis** to extract cryptographic keys
- **Fault injection** using voltage glitches or laser pulses

**2. Interface Exploitation**
```
Common Interfaces:
┌─────────────┬─────────────┬─────────────┐
│   USB Port  │  Serial Port│  JTAG Port  │
├─────────────┼─────────────┼─────────────┤
│   Easy      │   Medium    │   Hard      │
│  Access     │   Access    │   Access    │
└─────────────┴─────────────┴─────────────┘
```

**USB Attack Example:**
```c
// Vulnerable USB handling code
void handle_usb_command(uint8_t* buffer, int length) {
    if (length > 0) {
        // No validation of command type
        uint8_t command = buffer[0];
        
        switch (command) {
            case CMD_READ_MEMORY:
                // Allows reading any memory location!
                read_memory(buffer[1], buffer[2]);
                break;
            case CMD_WRITE_MEMORY:
                // Allows writing to any memory location!
                write_memory(buffer[1], buffer[2], buffer[3]);
                break;
        }
    }
}
```

**Secure Version:**
```c
// Secure USB handling with validation
void handle_usb_command_secure(uint8_t* buffer, int length) {
    if (length < MIN_COMMAND_LENGTH) {
        return; // Invalid command length
    }
    
    uint8_t command = buffer[0];
    
    // Validate command is allowed in current mode
    if (!is_command_allowed(command, current_security_mode)) {
        return; // Command not allowed
    }
    
    // Validate parameters before execution
    if (!validate_command_parameters(buffer, length)) {
        return; // Invalid parameters
    }
    
    switch (command) {
        case CMD_READ_MEMORY:
            if (is_memory_region_readable(buffer[1], buffer[2])) {
                read_memory(buffer[1], buffer[2]);
            }
            break;
        case CMD_WRITE_MEMORY:
            if (is_memory_region_writable(buffer[1], buffer[2])) {
                write_memory(buffer[1], buffer[2], buffer[3]);
            }
            break;
    }
}
```

#### **Network Attack Vectors**

**1. Wireless Communication**
```
Wireless Attack Types:
┌─────────────────┬─────────────────┬─────────────────┐
│   Eavesdropping │   Replay Attack │   Man-in-Middle │
├─────────────────┼─────────────────┼─────────────────┤
│  Intercept      │  Record &       │  Intercept &    │
│  communications │  replay later   │  modify traffic │
└─────────────────┴─────────────────┴─────────────────┘
```

**2. Network Protocol Vulnerabilities**
```c
// Vulnerable network code
void handle_network_packet(uint8_t* packet, int length) {
    if (length >= 4) {
        uint32_t command = *(uint32_t*)packet;
        
        // No authentication - anyone can send commands!
        if (command == CMD_SHUTDOWN) {
            system_shutdown();
        } else if (command == CMD_FACTORY_RESET) {
            factory_reset();
        }
    }
}
```

**Secure Network Handling:**
```c
// Secure network handling with authentication
void handle_network_packet_secure(uint8_t* packet, int length) {
    if (length < MIN_PACKET_SIZE) {
        return; // Packet too short
    }
    
    // Extract and verify authentication
    uint8_t* auth_data = packet;
    uint8_t* payload = packet + AUTH_SIZE;
    
    if (!verify_authentication(auth_data, payload, length - AUTH_SIZE)) {
        return; // Authentication failed
    }
    
    // Check if sender is authorized
    if (!is_sender_authorized(auth_data)) {
        return; // Sender not authorized
    }
    
    // Now process the authenticated command
    uint32_t command = *(uint32_t*)payload;
    if (command == CMD_SHUTDOWN && is_shutdown_allowed()) {
        system_shutdown();
    }
}
```

---

## 🔐 **Security Principles**

### **The Foundation of Secure Design**

These principles guide all security decisions and help you build systems that are secure by design.

#### **1. Defense in Depth**

Don't rely on a single security measure. Build multiple layers of protection:

```
Security Layers:
┌─────────────────────────────────────┐
│        Application Security         │ ← Input validation, authentication
├─────────────────────────────────────┤
│         Runtime Protection          │ ← Memory protection, sandboxing
├─────────────────────────────────────┤
│         Operating System            │ ← Process isolation, file permissions
├─────────────────────────────────────┤
│         Hardware Security           │ ← Secure boot, hardware isolation
├─────────────────────────────────────┤
│         Physical Security           │ ← Tamper detection, secure enclosure
└─────────────────────────────────────┘
```

**Example: Multi-Layer Authentication**
```c
// Multiple authentication factors
typedef struct {
    uint8_t password_hash[32];      // Something you know
    uint8_t device_token[16];       // Something you have
    uint8_t biometric_template[64]; // Something you are
} MultiFactorAuth;

bool authenticate_user(const char* password, 
                      const uint8_t* device_token,
                      const uint8_t* biometric_data) {
    
    // Layer 1: Password verification
    if (!verify_password_hash(password, auth.password_hash)) {
        return false;
    }
    
    // Layer 2: Device token verification
    if (!verify_device_token(device_token, auth.device_token)) {
        return false;
    }
    
    // Layer 3: Biometric verification
    if (!verify_biometric(biometric_data, auth.biometric_template)) {
        return false;
    }
    
    return true; // All layers passed
}
```

#### **2. Principle of Least Privilege**

Give each component only the permissions it absolutely needs:

```c
// Before: Everything runs with full privileges
void process_sensor_data() {
    // Can access any memory, any peripheral
    read_sensor();
    write_to_display();
    modify_system_config();
    access_network();
}

// After: Limited privileges for each function
void process_sensor_data() {
    // Only sensor access allowed
    read_sensor();
    
    // Request permission for other operations
    if (request_permission(PERM_DISPLAY_WRITE)) {
        write_to_display();
    }
    
    if (request_permission(PERM_CONFIG_MODIFY)) {
        modify_system_config();
    }
    
    if (request_permission(PERM_NETWORK_ACCESS)) {
        access_network();
    }
}
```

#### **3. Fail Secure**

When security mechanisms fail, the system should fail to a secure state:

```c
// Fail-secure authentication
bool authenticate_user(const char* username, const char* password) {
    // If any step fails, deny access
    if (!username || !password) {
        return false; // Fail secure: deny access
    }
    
    if (!validate_input(username) || !validate_input(password)) {
        return false; // Fail secure: deny access
    }
    
    if (!check_credentials(username, password)) {
        return false; // Fail secure: deny access
    }
    
    // Only grant access if everything succeeds
    return true;
}
```

---

## 🛠️ **Practical Implementation**

### **Building Security Into Your System**

#### **Secure Boot Implementation**

Secure boot ensures only trusted code runs on your system:

```c
// Secure boot sequence
typedef struct {
    uint8_t signature[64];      // Digital signature
    uint8_t hash[32];          // Code hash
    uint32_t version;          // Version number
    uint32_t flags;            // Security flags
} SecureBootHeader;

bool verify_boot_image(uint8_t* image, uint32_t size) {
    SecureBootHeader* header = (SecureBootHeader*)image;
    
    // Step 1: Verify signature
    if (!verify_signature(header->signature, 
                         header->hash, 
                         sizeof(header->hash))) {
        return false; // Signature verification failed
    }
    
    // Step 2: Verify code hash
    uint8_t calculated_hash[32];
    calculate_hash(image + sizeof(SecureBootHeader), 
                  size - sizeof(SecureBootHeader), 
                  calculated_hash);
    
    if (memcmp(calculated_hash, header->hash, 32) != 0) {
        return false; // Code hash mismatch
    }
    
    // Step 3: Check version and flags
    if (header->version < MIN_SUPPORTED_VERSION) {
        return false; // Version too old
    }
    
    if (header->flags & FLAG_DEBUG_ENABLED) {
        return false; // Debug mode not allowed in production
    }
    
    return true; // All checks passed
}
```

#### **Memory Protection**

Protect sensitive data in memory:

```c
// Memory protection example
typedef struct {
    uint8_t encryption_key[32];
    uint8_t user_credentials[64];
    uint8_t system_config[128];
} SecureData;

// Mark sensitive data as non-swappable and encrypted
void initialize_secure_memory() {
    // Lock pages in memory (prevent swapping)
    mlock(&secure_data, sizeof(secure_data));
    
    // Encrypt sensitive data
    encrypt_data(&secure_data.encryption_key, sizeof(secure_data.encryption_key));
    encrypt_data(&secure_data.user_credentials, sizeof(secure_data.user_credentials));
    
    // Set memory as read-only after initialization
    mprotect(&secure_data, sizeof(secure_data), PROT_READ);
}

// Clear sensitive data when no longer needed
void clear_secure_memory() {
    // Decrypt data first
    decrypt_data(&secure_data.encryption_key, sizeof(secure_data.encryption_key));
    
    // Overwrite with random data
    secure_random_fill(&secure_data, sizeof(secure_data));
    
    // Overwrite with zeros
    memset(&secure_data, 0, sizeof(secure_data));
    
    // Unlock pages
    munlock(&secure_data, sizeof(secure_data));
}
```

#### **Input Validation**

Always validate and sanitize input data:

```c
// Comprehensive input validation
typedef enum {
    VALIDATION_OK,
    VALIDATION_TOO_LONG,
    VALIDATION_INVALID_CHARS,
    VALIDATION_OUT_OF_RANGE,
    VALIDATION_NULL_POINTER
} ValidationResult;

ValidationResult validate_user_input(const char* input, int max_length) {
    // Check for null pointer
    if (!input) {
        return VALIDATION_NULL_POINTER;
    }
    
    // Check length
    int length = strlen(input);
    if (length > max_length) {
        return VALIDATION_TOO_LONG;
    }
    
    // Check for valid characters (alphanumeric only)
    for (int i = 0; i < length; i++) {
        if (!isalnum(input[i]) && input[i] != '_' && input[i] != '-') {
            return VALIDATION_INVALID_CHARS;
        }
    }
    
    return VALIDATION_OK;
}

// Safe string copying with validation
bool safe_string_copy(char* dest, const char* src, int dest_size) {
    if (!dest || !src || dest_size <= 0) {
        return false;
    }
    
    ValidationResult result = validate_user_input(src, dest_size - 1);
    if (result != VALIDATION_OK) {
        return false;
    }
    
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0'; // Ensure null termination
    
    return true;
}
```

---

## 🎯 **Key Takeaways**

### **Fundamental Principles**

1. **Security is a mindset** - Think like an attacker
2. **Defense in depth** - Multiple layers of protection
3. **Least privilege** - Give only necessary permissions
4. **Fail secure** - Default to secure state
5. **Input validation** - Never trust external data

### **Security Checklist**

- [ ] **Threat model** completed and documented
- [ ] **Attack vectors** identified and mitigated
- [ ] **Secure boot** implemented
- [ ] **Memory protection** enabled
- [ ] **Input validation** on all inputs
- [ ] **Authentication** required for sensitive operations
- [ ] **Encryption** for data at rest and in transit
- [ ] **Secure communication** protocols implemented
- [ ] **Physical security** measures in place
- [ ] **Security testing** performed

### **Common Mistakes to Avoid**

1. **Security through obscurity** - Don't rely on hiding things
2. **Single point of failure** - Build redundancy into security
3. **Ignoring physical security** - Hardware access can bypass software
4. **Weak authentication** - Use strong, multi-factor authentication
5. **No security updates** - Plan for security maintenance

---

## 📚 **Additional Resources**

- **"Security Engineering" by Ross Anderson** - Comprehensive security guide
- **"Building Secure and Reliable Systems" by Google** - Production security practices
- **OWASP Embedded Application Security** - Embedded security guidelines

---

**Next Topic**: [Secure Boot and Chain of Trust](./Secure_Boot_Chain_Trust.md) → [TPM 2.0 Basics](./TPM2_Basics.md)
