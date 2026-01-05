# Avant Systems Canon (ASC-1.1)
**A Universal Standard for Architectural Integrity, Safety, and Engineering Rigor.**

---

## 1. UNIVERSAL APPLICABILITY
The Canon governs by **Architectural Role**, not by language-specific syntax. It ensures that systems software remains deterministic, manageable, and performant regardless of the underlying language (C, C++, Zig, Rust, Odin).

### 1.1 Role Mapping
* **Adoption Requirement:** Any codebase adopting this Canon **MUST** map its source tree to these Architectural Roles (e.g., via a `ROLE_MAP.md`).
* **Governance Rule:** Physical directory structures are implementation details; the Canon governs behavior based on a component's Role.

### 1.2 Canonical Roles
* **Memory Subsystem:** Responsible for raw allocation and region management. This is the only role permitted to perform "unsafe" operations or direct heap interaction.
* **Data Structure Subsystem:** Generic containers and collection primitives. Must be memory-agnostic (borrowing memory from providers).
* **I/O Subsystem:** Explicit boundaries for OS interaction, file systems, and hardware drivers.
* **Application Logic:** High-level composition. Must be platform-independent and rely strictly on the subsystems above.

### 1.3 Single Source of Truth (SSoT)
* **The Law:** Information (constants, logic, configuration) must reside in exactly one location. Redundancy is a violation of architectural integrity.

---

## 2. DEVELOPMENT WORKFLOW
Integrity is maintained through a strict, linear progression of state.

* **Atomic Contributions:** Changes must be scoped to a single logical improvement.
* **Verification:** Contributions are invalid until verified by the local automated test suite.
* **The Automaton:** All submissions are subject to machine-led verification. Human review is a secondary check for intent, never a primary check for syntax or hygiene.

---

## 3. ENGINEERING STANDARDS
* **Naming:** Use consistent, language-idiomatic casing.
* **Immutability:** Data not intended for modification **MUST** be marked immutable at the API boundary.
* **State Isolation:** Global mutable state is **PROHIBITED**. State must be passed explicitly to ensure thread safety and deterministic testing.

---

## 5. SOURCE HYGIENE & TOPOLOGY
Hygiene is strictly enforced to prevent logical collisions and "poisoning" of the system environment.

### 5.1 Hierarchy of Inclusion
To preserve safety boundaries, source files **MUST** follow a strict inclusion order:
1.  **Directives:** Local overrides and permission macros (e.g., `ALLOW_UNSAFE`).
2.  **External/System Headers:** Dependencies provided by the OS or Language Standard.
3.  **Project Headers:** Internal definitions and safety enforcement headers.

### 5.2 The Automaton Rule (Formatting)
Code style is a deterministic output of a machine, not a matter of opinion.
* **Authority:** The repository’s configuration file (e.g., `.clang-format`) is the absolute authority.
* **CI Enforcement:** Contributions with formatting violations **MUST** be automatically rejected.

### 5.3 Documentation as Code
* **The Law:** A feature is not "implemented" until its public interface is documented in-source. Documentation must evolve at the same velocity as the logic.

---

## 6. SAFETY & ERROR PHILOSOPHY

### 6.1 The "Unsafe" Hatch
Direct interaction with unmanaged memory is prohibited in Application-level roles.
* **Authorization:** Subsystems requiring unsafe access must explicitly declare intent via a pre-processor or language-level directive (e.g., `ALLOW_UNSAFE`).
* **Scope:** This declaration **MUST** be localized to the smallest possible scope.

### 6.2 Error Handling
* **The Law:** Errors are **values**, not side-effects. Exceptions are prohibited. If an operation can fail, that failure must be explicitly represented in the return signature to force handling by the caller.

### 6.3 Linter Exemptions
Disabling the Automaton (Formatting) is permitted **only** in specific blocks where manual alignment significantly improves structural clarity (e.g., Matrix math or SIMD blocks).
* **Syntax:** Use explicit markers (e.g., `// format off`).

---

## 7. INTELLECTUAL PROPERTY & LICENSING
By contributing to a project governed by this Canon, the contributor agrees to the licensing terms defined in the root of the specific project.

---
**Avant Systems Canon (ASC-1.1).** *Released into the Public Domain. Formalize the Architecture. Own the Machine.*