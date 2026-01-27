# Avant Systems Canon (ASC-1.3)

---

## 1. UNIVERSAL APPLICABILITY
The Canon governs by **Architectural Role**, not by language-specific syntax. It ensures that systems remain deterministic, manageable, and performant regardless of the underlying technology.

### 1.1 Role Mapping
* **Adoption Requirement:** Stewardship of the source tree is defined strictly by a standardized ownership definition file (specifically `CODEOWNERS`). This file serves as the authoritative map of Architectural Roles.
* **Enforcement:** Every directory in the source tree **MUST** be owned by a specific Role, Team, or Maintainer defined in `CODEOWNERS`. Orphaned code is prohibited.
* **Governance Rule:** Physical directory structures are implementation details; the Canon governs behavior based on a component's Role as defined in the ownership map.

### 1.2 Single Source of Truth (SSoT)
* **The Law:** Information (constants, logic, configuration) must reside in exactly one location. Redundancy is a violation of architectural integrity.

---

## 2. THE STABILITY MANDATE
**Objective: Zero Maintenance.**

The Canon prioritizes **perfect** over **done**. The goal is to reduce future maintenance burden to zero.

* **Architecture Before Implementation:** We reject the philosophy of "Move fast and break things". Code is written once but read and maintained a thousand times. If a feature requires future maintenance to keep it working, it is technically insolvent.
* **The Definition of Done:** A feature is not "done" when it passes tests; it is done when it is structurally optimal. We do not defer refactoring. Technical debt is treated as a critical build failure.
* **Correctness Over Velocity:** It is better to delay a release than to ship an API that requires a breaking change later.

---

## 3. DEVELOPMENT WORKFLOW
Integrity is maintained through a strict, linear progression of state.

* **Atomic Contributions:** Changes must be scoped to a single logical improvement.
* **Verification:** Contributions are invalid until verified by the local automated test suite (e.g., `make test` or equivalent).
* **The Automaton:** All submissions are subject to machine-led verification (CI/CD). Human review is a secondary check for intent, never a primary check for syntax or hygiene.
* **Chain of Command:** Reviews must follow the authority defined in `CODEOWNERS`.

---

## 4. ENGINEERING STANDARDS
* **Naming:** Use consistent, language-idiomatic casing.
* **Immutability:** Data not intended for modification **MUST** be marked immutable at the API boundary.
* **State Isolation:** Global mutable state is **PROHIBITED**. State must be passed explicitly to ensure thread safety and deterministic testing.

---

## 5. COMMUNICATION & MESSAGING
We utilize **Conventional Commits** to ensure the history of the system is machine-readable and logically structured.

* `feat:` (New functionality)
* `fix:` (Correcting erroneous behavior)
* `refactor:` (Structural changes without functional impact)
* `perf:` (Optimization of resource usage)
* `docs:` (Documentation updates)

---

## 6. SOURCE HYGIENE & TOPOLOGY
Hygiene is strictly enforced to prevent logical collisions and "poisoning" of the system environment.

### 6.1 Hierarchy of Inclusion
To preserve safety boundaries, source files **MUST** follow a strict dependency order:
1.  **Directives:** Local overrides and permission flags.
2.  **External Dependencies:** Provided by the System, Language Standard, or Third-Party Packages.
3.  **Internal Project Headers:** Internal definitions and safety enforcement.

### 6.2 The Automaton Rule (Formatting)
Code style is a deterministic output of a machine, not a matter of opinion.
* **Authority:** The repository’s configuration file (e.g., `.clang-format`, `.prettierrc`, `fmt`) is the absolute authority.
* **CI Enforcement:** Contributions with formatting violations **MUST** be automatically rejected.

### 6.3 Documentation as Code
* **The Law:** A feature is not "implemented" until its public interface is documented in-source. Documentation must evolve at the same velocity as the logic.

---

## 7. SAFETY & ERROR PHILOSOPHY

### 7.1 The "Unsafe" Hatch
Direct interaction with memory-unsafe or undefined-behavior regions is prohibited in Application-level roles.
* **Authorization:** Subsystems requiring unsafe access must explicitly declare intent via a language-appropriate directive (e.g., `unsafe`, `ALLOW_UNSAFE`).
* **Scope:** This declaration **MUST** be localized to the smallest possible scope.

### 7.2 Error Handling
* **The Law:** Errors are **values**, not side-effects. Exceptions are discouraged unless the language lacks value-based error handling. If an operation can fail, that failure must be explicitly represented in the return signature to force handling by the caller.

### 7.3 Linter Exemptions
Disabling the Automaton (Formatting) is permitted **only** in specific blocks where manual alignment significantly improves structural clarity (e.g., Matrix math or ASCII diagrams).
* **Syntax:** Use explicit markers (e.g., `// format off`).

---

## 8. SECURITY PROTOCOLS
* **Reporting:** Vulnerabilities must be reported privately via the channels defined in the project's security policy. Public disclosure of exploits without responsible notice is a violation of the Canon.
* **Threat Model:** The "Safe" subset of the API guarantees integrity; "Unsafe" hatches do not. Reports must demonstrate a violation of the Safe API guarantees.

---

## 9. INTELLECTUAL PROPERTY & LICENSING
By contributing to a project governed by this Canon, the contributor agrees to the licensing terms defined in the root of the specific project.

---

**Avant Systems Canon (ASC-1.3).** *Released into the Public Domain.*