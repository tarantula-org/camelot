# Avant Systems Canon (ASC-1.1)

---

## EXECUTIVE PRINCIPLE — COGNITIVE DURABILITY

This Canon defines the non-negotiable engineering standards governing all software produced under this organization.

Source code is a long-lived cognitive asset. It is not disposable, not ephemeral, and not owned by its original author. The primary objective of this Canon is **Cognitive Durability**: the ability for systems to be understood, verified, and safely modified by competent engineers who did not write them.

This Canon explicitly rejects hero-centric development in favor of tiered, verifiable engineering discipline. Compliance is mandatory.

---

## 1. UNIVERSAL APPLICABILITY BY ARCHITECTURAL ROLE

To remain valid across radically different systems—from kernels and allocators to services and applications—this Canon governs by **Architectural Role**, not by hard-coded file paths, directory names, or toolchains.

### 1.1 Role Mapping
* **Adoption Requirement:** Each repository **MUST** map its source tree to the Canon’s Architectural Roles (for example, in a `ROLE_MAP.md`).
* **Governance Rule:** The Canon governs behavior by Role. File names and directory structures are implementation details.

### 1.2 Canonical Roles
The following roles are normative. Projects may refine or extend them, but their responsibilities may not be weakened.

* **Memory Subsystem** Responsible for raw allocation, region management, allocator policy, and pointer-level operations.

* **Data Structure Subsystem** Responsible for generic containers, hashing, indexing, and collection primitives.

* **I/O Subsystem** Responsible for operating system interaction, files, sockets, devices, and external boundaries.

* **Application Logic** All remaining business logic, orchestration, tests, and feature code.

---

## 2. DEFINITIONS & GLOSSARY

* **2.1 Logical Unit** — A function, module, class, or file with a single coherent responsibility.
* **2.2 Public Interface** — Any API, type, function, or symbol intended for use beyond its immediate maintenance context.
* **2.3 Competent Engineer** — An engineer proficient in the relevant domain and language, capable of forming a correct mental model of a unit.
* **2.4 Five-Minute Rule** — A Logical Unit must be understandable by a Competent Engineer within approximately five minutes of focused reading. Failure implies excessive complexity.
* **2.5 Systemic Issue** — A design flaw spanning multiple units (e.g., circular dependencies, violated layering, pervasive global state).

---

## 3. COMPLIANCE TIERS

Enforcement is tiered by lifecycle stage to preserve rigor without paralyzing progress.

### 3.1 Tier A — Greenfield (New Code)
* **Definition:** New files, modules, or logically independent subsystems.
* **Mandate:** Tier A code **MUST** comply fully with this Canon at merge time. No technical debt may be introduced.

### 3.2 Tier B — Touched Legacy (Refactoring by Contact)
* **Definition:** Existing code modified during feature work or defect correction.
* **Mandate:** The specific Logical Unit being modified **MUST** be elevated to Tier A standards (naming, tests, contracts).
* **Scope Control:** Cleanup is intentionally time-bounded.
* **Escalation Rule:** Discovery of a Systemic Issue **MUST** result in an Escalation Ticket rather than an ad-hoc workaround.

### 3.3 Tier C — Frozen Legacy
* **Definition:** Code not under active modification.
* **Mandate:** Exempt from proactive refactoring. Any defect fix promotes the affected unit to Tier B.

---

## 4. MEMORY DISCIPLINE

### 4.1 Prohibition
Direct interaction with system allocators (e.g., `malloc`, `free`, `mmap`, `new`) is **PROHIBITED** in Application Logic, I/O, or general-purpose subsystems.

### 4.2 Role-Restricted Exception
Direct interaction with platform memory APIs is permitted **only** within the **Memory Subsystem**.

### 4.3 Enforcement Implications
* Any change requiring dynamic allocation **MUST** use abstractions exposed by the Memory Subsystem.
* Pull Requests introducing allocator calls outside the Memory Subsystem **MUST** be rejected.

---

## 5. COMPLEXITY EXEMPTIONS (ROLE-AWARE)

### 5.1 Principle
The Canon favors decomposition. Exemptions to the Five-Minute Rule are granted only when decomposition would compromise correctness, performance, or hardware constraints.

### 5.2 Universal Exemption Categories
The following categories **MAY** qualify for exemption **only** within their appropriate Architectural Role:

1. **Cryptographic Primitives** — Proof sketch required.
2. **Optimized DSP or Numerical Kernels** — Benchmarks required.
3. **Fundamental Data Structures** — Only within the Data Structure Subsystem.
4. **Memory Allocator Internals** — Only within the Memory Subsystem.

### 5.3 Mandatory Controls
All exemptions require:
* Demonstrated infeasibility of safe decomposition.
* **Double Sign-off** by reviewers with domain expertise.
* **Deep Documentation** as defined in Section 6.

---

## 6. DEEP DOCUMENTATION & CO-LOCATION

### 6.1 Co-Location Rule
Documentation for complex or exempt logic **MUST** be co-located within a `docs/` hierarchy mirroring the source tree.

*Example:* `src/ds/map.c` → `docs/ds/map.md`

### 6.2 Required Contents
Deep Documentation **MUST** include:
1. Problem Statement
2. Rejected Design Alternatives
3. Correctness Arguments or Invariants
4. Failure Modes and Edge Cases
5. Performance Characteristics or Microbenchmarks (where applicable)

### 6.3 Source Linkage
Source files implementing complex logic **MUST** include a header reference to the corresponding Deep Document.

---

## 7. PUBLIC INTERFACE CONTRACTS

### 7.1 Intent-First Requirement
Every Public Interface **MUST** include a structured comment block adhering to the following strict format. The **USAGE** section must include a Markdown code block.

**Required Format:**
```c
/*
 * INTENT: <Why this interface exists>
 * USAGE:
 * ```
 * <Code Example>
 * ```
 * INVARIANTS: <Preconditions and Postconditions>
 * FAILURE MODES: <Explicit Error Semantics>
 */

```

### 7.2 Machine Readability

Projects *SHOULD* include a machine-readable (YAML/JSON) block alongside interface documentation to enable automated analysis.

---

## 8. TOOLING & AUTOMATION

### 8.1 Machine-Verifiable Configuration

Each repository **MUST** define a root-level configuration file (e.g., `.asc_requirements.toml`) declaring required tooling and versions.

The Canon intentionally names no tools. The configuration file is authoritative.

### 8.2 CI Enforcement

Continuous Integration **MUST** enforce formatting, static analysis, and testing.

* Failing automation **PROHIBITS** human review.
* CI **MUST** fail on any new warnings introduced in Tier A or Tier B code.

---

## 9. GOVERNANCE & METRICS

### 9.1 Exemption Registry

All exemptions **MUST** be recorded in a central log (e.g., `docs/exemptions/`) including rationale, reviewers, and a sunset or re-evaluation date.

### 9.2 Review Authority

A Change Request **MUST** be rejected if:

* Required tests are missing.
* The logic violates the Five-Minute Rule.
* Errors are silently ignored.
* Magic values exist without named constants.

### 9.3 Health Indicators

Projects *SHOULD* track:

* Tier A Compliance Rate (Target: 100%)
* Defect Escape Rate (Target: <0.1 per KLOC)
* Review Cycle Time (Target: <24h)

---

## 10. CONTRIBUTOR CHECKLIST

Before opening a Pull Request:

* [ ] Change is mapped to an Architectural Role
* [ ] Tier obligations are satisfied
* [ ] No direct allocation outside the Memory Subsystem
* [ ] Public interfaces include Intent-First contracts with Code Blocks
* [ ] Complex logic has co-located Deep Documentation
* [ ] Automation configuration is respected
* [ ] Defects include reproduction tests

---

*Reference: Avant Systems Canon (ASC-1.1). Released into the Public Domain.*