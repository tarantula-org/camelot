# Security Policy

We view security as a non-negotiable engineering constraint. We are committed to the immediate triage and remediation of critical vulnerabilities affecting our users or infrastructure.

---

## Maintenance Lifecycle

We adhere to a strict semantic versioning policy for security patches.

| Version Track | Status | Support Level |
| :--- | :--- | :--- |
| **Current Stable** | Active | Critical patches & Backports. |
| **Main / Dev** | Unstable | Immediate fixes; no backports. |
| **Legacy** | End-of-Life | No support. Upgrade required. |

---

## Reporting Protocol

**DO NOT** file public GitHub Issues for security vulnerabilities. Public disclosure places the entire ecosystem at risk before a patch is available.

### Direct Contact
All security vulnerabilities must be reported directly to the Security Team via email.

* **Email:** `camelot.tarantula.tech@atomicmail.io`
* **Subject:** `[SECURITY] <Project Name>: <Vulnerability Class>`

### Required Evidence
To expedite triage, your report **MUST** include:
1. **Impact:** The specific security violation (e.g., Remote Code Execution, Privilege Escalation).
2. **Proof of Concept (PoC):** A minimal reproduction case or code snippet demonstrating the exploit.
3. **Threat Model:** The specific conditions required to trigger the vulnerability.

---

## Threat Model & Scope

We strictly distinguish between **Implementation Flaws** (Maintainer Responsibility) and **Unsafe Usage** (User Responsibility).

### In Scope (Valid Vulnerabilities)
* **Memory Safety Violations:** Heap corruption, Use-After-Free, Double-Free, or Buffer Overflows occurring in safe API contexts.
* **Logic Errors:** Bypassing intended access controls, integrity checks, or authentication mechanisms.
* **Undefined Behavior:** API behaviors that deviate from the specification in a way that compromises system stability or security.
* **Cryptographic Failures:** Weak randomness, timing attacks, or broken protocol implementations.

### Out of Scope (Invalid Reports)
* **Opt-in Unsafe:** Vulnerabilities triggered within scopes explicitly marked as `unsafe`, `unchecked`, or where strict aliasing rules are manually violated by the user.
* **Misconfiguration:** Issues arising from the user's modification of build flags, internal headers, or insecure environment setup.
* **Denial of Service (DoS):** Resource exhaustion (OOM/Stack Overflow) is treated as a stability bug, not a security vulnerability, unless it directly leads to code execution.
* **Social Engineering:** Phishing or physical attacks against maintainers.

---

## Safe Harbor

We pledge not to pursue legal action against researchers who adhere to these **Responsible Disclosure** guidelines:

1. **Authorized Testing:** Research is conducted only on systems you own or have explicit permission to test.
2. **Data Privacy:** No user data is compromised, exfiltrated, or destroyed during testing.
3. **Non-Disclosure:** Vulnerability details are not published until a patch is released or the **90-day disclosure deadline** expires.

