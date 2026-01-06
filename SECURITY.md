# Security Policy
**Governed by the Avant Systems Canon (ASC-1.1)**

Camelot is a foundational kernel for systems engineering. We hold our **Memory** and **I/O** subsystems to the highest standards of correctness. If you break our safety guarantees, we want to know.

---

## Supported Versions

| Version | Status | Notes |
| :--- | :--- | :--- |
| **Epoch 2 (v0.2.x)** | :white_check_mark: **Supported** | Current Stable Kernel |
| Epoch 1 (v0.1.x) | :x: Deprecated | No longer receiving patches |

---

## Reporting a Vulnerability

**DO NOT open a public GitHub Issue for security vulnerabilities.**

If you discover a safety bypass, memory corruption, or logic error in a **Core Subsystem**, please report it responsibly.

### 1. Contact
Email the maintainers directly at:
**`[YOUR_EMAIL_HERE]`** *(Optional: Encrypt your message using our PGP Key: `[LINK_TO_KEY]`).*

### 2. Format
To expedite triage, your report **MUST** include:
* **Subject:** `[SECURITY] <Subsystem>: <Short Description>`
* **Proof of Concept (PoC):** A minimal, reproducible C file (`repro.c`) demonstrating the exploit.
* **Impact:** Describe how the vulnerability compromises the application (e.g., RCE, Heap Corruption, Info Leak).

---

## Scope & Threat Model

Camelot guarantees safety *only* when usage adheres to the strictures of **ASC-1.1**.

### :dart: In Scope (Valid Vulnerabilities)
* **Arena Metadata Corruption:** If a standard allocation (`arena.alloc`) allows overwriting internal arena headers.
* **Bounds Check Failures:** If `List`, `Table`, or `String` primitives allow out-of-bounds Read/Write without panicking.
* **I/O Safety:** Buffer overflows in `io.scan`, `io.slurp`, or format string vulnerabilities in `io.print`.
* **Type Confusion:** If the `Any` type or generic containers can be coerced into unsafe casting.

### :no_entry_sign: Out of Scope (Invalid Reports)
* **Unsafe Hatches:** Vulnerabilities triggered inside scopes marked with `ALLOW_UNSAFE`.
* **Manual Misuse:** Issues caused by casting away `const`, manually modifying internal struct fields, or ignoring return values.
* **Denial of Service (DoS):** OOM (Out of Memory) or Stack Overflow are considered "Defined Panics," not vulnerabilities, unless they lead to Remote Code Execution.

---

## Response Timeline

We adhere to a strict internal SLA for security reports:

1.  **Acknowledgement:** Within **48 hours**.
2.  **Triage & Validation:** Within **5 business days**.
3.  **Patch Release:** We aim to release a hotfix (`v0.X.Y+1`) within **14 days** of validation.

## Safe Harbor

We pledge not to pursue legal action against researchers who:
* Engage in testing/research without harming Camelot users or infrastructure.
* Adhere to this policy and **Responsible Disclosure** guidelines.
* Do not publish details until a patch is released or the disclosure timeline (90 days) expires.