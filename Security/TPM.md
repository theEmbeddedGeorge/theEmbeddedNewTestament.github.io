## Trusted Platform Module (TPM)

### **Intro**

The Trusted Computing Group (TCG) has been
addressing the trust issue – and related security
benefits - for PCs, servers, networking gear and
embedded systems for more than a decade, driven
by the Trusted Platform Module (TPM) specification.

The TPM standard defines a hardware root of trust
(HRoT) widely accepted as more secure than software
that can be more easily breached by attackers.

The TPM is used with software to enable features;
open source APIs are available and custom software
can be developed. Additional resources for software
support also are provided later in this paper.

In many systems, the TPM provides integrity
measurements, health checks and authentication
services.

```TPM are passive, low-end devices that offer strong security guarantees.```

**ATTRIBUTES OF THE TPM INCLUDE:**

- Support for bulk (symmetric) encryption
in the platform
- High quality random numbers
- Cryptographic services
- A protected persistent store for small amounts
of data, sticky- bits, monotonic counters and
extendible registers
- A protected pseudo-persistent store for
unlimited amounts of keys and data
- An extensive choice of authorization methods
to access protected keys and data
- Platform identities
- Support for platform privacy
- Signing and verifying digital signatures
(normal, anonymous, pseudonymous)
- Certifying the properties of keys and data
- Auditing the usage of keys and data
IN A TRUSTED PLATFORM
THE TPM ALSO PROVIDES:
- Attestation: reporting platform state
- Sealing: using platform state to authorize
access to keys and data

**IN A TRUSTED PLATFORM THE TPM ALSO PROVIDES:**
- Attestation: reporting platform state
- Sealing: using platform state to authorize
access to keys and data

[***TPM 2.0 - A Brief Introduction***](https://trustedcomputinggroup.org/wp-content/uploads/2019_TCG_TPM2_BriefOverview_DR02web.pdf)

This PDF document gives a very good overview of what is TPM and its attributes. It also gives a brief summary of TPM history, application and types of TPM solutions. A few vaulable links about more TPM details are also given at the end of the document.

### **TPM usage**

```TPMs support two main use-cases: secure key generation and remote system attestation.```

**Secure Key Generation**

TPM is a cryptographic device. It can securely generate new cryptographic keys: the keys are only available to the TPM - private key material never leaves the device in plain form.
TPM can do crypto operations such as encryption and signing. TPM can certify new keys, so in a way a TPM acts as a certificate authority (CA). Trust in these keys is rooted in a primary key provisioned by the manufacturer or the owner of the TPM.

**Remote System Attestation**

TPM can capture the host system state: this is done by storing a sequence of measurements in a special set of registers called Platform Configuration Registers (PCRs). The TPM can later report its PCR values to a remote party. It is done in a secure way such that the remote attester can verify the report is fresh, genuine, and has not been tampered with.

### **TPM types**

**DISCRETE TPM**

Provides the highest level of security, as might be needed for a TPM used to secure
the brake controller in a car. The intent of this level is to ensure that the device it’s protecting does
not get hacked via even sophisticated methods. To accomplish this, a discrete chip is designed, built
and evaluated for the highest level of security that can resist tampering with the chip, including
probing it and freezing it with all sorts of sophisticated attacks.

**INTEGRATED TPM** 

is the next level down in terms of security. This level still has a hardware TPM but
it is integrated into a chip that provides functions other than security. The hardware implementation
makes it resistant to software bugs, however, this level is not designed to be tamper-resistant.

**FIRMWARE TPM**

is implemented in protected software. The code runs on the main CPU, so a
separate chip is not required. While running like any other program, the code is in a protected
execution environment called a trusted execution environment (TEE) that is separated from the rest
of the programs that are running on the CPU. By doing this, secrets like private keys that might be
needed by the TPM but should not be accessed by others can be kept in the TEE creating a more
difficult path for hackers.
In addition to the lack of tamper resistance, the downside to the TEE or firmware TPM is that now
the TPM is dependent on many additional aspects to keep it secure, including the TEE operating
system, bugs in the application code running in the TEE, etc.

**SOFTWARE TPM** 

can be implemented as a software emulator of the TPM. However, a software
TPM is open to many vulnerabilities, not only tampering but also the bugs in any operating system
running it. It does have key applications: it is very good for testing or building a system prototype
with a TPM in it. For testing purposes, a software TPM could provide the right solution/approach.

[***Google TPM-JS Interactive TPM Simulator***](https://google.github.io/tpm-js/)

TPM-JS is a webpage TPM simulator that lets you experiment with a software Trusted Platform Module (TPM) in your browser. TPM-JS includes the following libraries:

Intel TPM2 Software Stack (TSS).
IBM software TPM simulator.
Google BoringSSL.
The TSS library provides high-level APIs to the TPM. It manages TPM resources, marshals command buffers and unmarshals response buffers.

The simulator executes TPM commands. It maintains internal state and uses the BoringSSL library for cryptographic operations.

**Some observations about TPM**
- TPM is a discrete device soldered on the motherboard. It is a cheap (costs less than a dollar to manufacture), low-end device that communicates with the main CPU over a slow, low-bandwidth channel.

- TPM is a passive device: it doesn't monitor the system and it can't halt CPU execution. For it to work, it must be fed data.

- TPM has limited storage for runtime state and persistent data: its non-volatile storage is about 64KB in size. TPM can only hold a limited number of objects at the same time. For this reason, a dedicated software layer on the host (resource manager) loads and unloads session objects at runtime.

- TPM command execution is single-threaded: it executes one command at a time. Commands cannot be queued or batched together - each command must wait for the currently running command to finish. Note that command execution can be canceled.



