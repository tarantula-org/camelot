# The Ada Engineering Codex (AEC-1.0)

### EXECUTIVE PRINCIPLE: COGNITIVE DURABILITY
This Codex defines the non-negotiable engineering standards governing all software produced under this organization.

Source code is treated as a long-term cognitive asset, not a disposable artifact. The primary objective of this Codex is **Cognitive Durability**: the ability for software to be understood, verified, and safely modified by competent engineers who did not author it.

This Codex explicitly rejects hero-centric development practices in favor of tiered, verifiable engineering discipline. Compliance is mandatory.

---

### 1. APPLICABILITY AND COMPLIANCE TIERS

#### 1.1 Lifecycle-Aware Enforcement
To prevent process paralysis, enforcement is tiered based on the lifecycle stage of the artifact.

**1.1.1 Tier A — Greenfield (New Code)**
* **Definition:** New files, modules, or logically independent subsystems.
* **Mandate:** All Tier A code MUST fully comply with every requirement of this Codex. No technical debt may be introduced at merge time.

**1.1.2 Tier B — Touched Legacy (Refactoring by Contact)**
* **Definition:** Existing code modified as part of feature work or bug fixing.
* **Mandate:**
    * The specific function or logical unit being modified MUST be brought to full Tier A compliance.
    * Improvements MUST include naming clarity, error handling, and tests.
    * **Scope Control:** Cleanup is time-bounded (expected effort: minutes, not days).
* **Escalation:** If improving the touched unit reveals systemic issues (e.g., circular dependencies), the change MUST be escalated for broader refactoring approval rather than hacked in.

**1.1.3 Tier C — Frozen Legacy**
* **Definition:** Existing code not under active modification.
* **Mandate:** Tier C code is exempt from proactive refactoring.
* **Exception:** Any defect fix promotes the affected unit to Tier B for the duration of the change.

---

### 2. STRUCTURAL CLARITY AND COGNITIVE LOAD

#### 2.1 The Complexity Boundary
* **Principle:** A competent engineer familiar with the domain must be able to understand a unit of logic within a bounded period of focused reading.
* **The Five-Minute Rule:** If a function or algorithm cannot be understood within approximately five minutes of uninterrupted review, it is considered **excessively complex** and MUST be addressed by:
    1.  Decomposition.
    2.  Refactoring.
    3.  Or formal exemption.

**Complexity Exemptions (Governed)**
Only the following categories may qualify:
* Cryptographic primitives.
* Optimized DSP or numerical kernels.
* Hardware-constrained drivers.

**Exemption Requirements:**
* Refactoring must be demonstrably infeasible.
* **Double Sign-off:** Two reviewers must explicitly approve the exemption.
* **Deep Documentation** (Clause 3.2) is mandatory.

#### 2.2 Naming as Executable Documentation
Identifiers MUST describe intent, not implementation detail.
* **Prohibited:** `data`, `tmp`, `doWork()`, `flag`
* **Required:** `retry_backoff_schedule`, `validate_packet_crc()`, `is_cache_stale`

**Magic Values Prohibited:** All literals influencing logic MUST be named constants.

#### 2.3 File Organization
* **Responsibility:** Files MUST adhere to a single, coherent responsibility.
* **Navigability:** Files exceeding ~300 lines MUST include visual structure markers (e.g., `// --- PUBLIC API ---`) to aid orientation.

---

### 3. DOCUMENTATION AND CONTRACTS

#### 3.1 Public Interface Contract
The Public Interface is the binding contract. Every exposed element MUST include structured documentation specifying:
1.  **Intent**
2.  **Usage Example**
3.  **Constraints/Invariants**
4.  **Failure Modes** (Explicit error semantics)

#### 3.2 Deep Documentation (Mandatory for Exemptions)
For logic exempted under Clause 2.1, inline comments are insufficient.
* **Location:** `/docs/algorithms/<component>.md`
* **Content:** Problem statement, theoretical approach, references (papers/datasheets), edge cases, and performance characteristics.
* **Linkage:** Code MUST explicitly reference this document.

---

### 4. VERIFICATION AND TESTING

#### 4.1 Tests as Functional Requirements
Tests are mandatory artifacts, not optional add-ons.
* **Mandate:** All Tier A public interfaces MUST be unit tested.
* **Coverage Targets:**
    * **Scope:** Targets apply strictly to the *module or unit under test*, not the global codebase.
    * **Core Logic:** ≥80% Branch Coverage.
    * **Utilities:** ≥60% Statement Coverage.

#### 4.2 Defect Handling
* **Reproduction First:** Every bug fix MUST begin with a failing reproduction test.
* **Definition of Done:** A fix is complete only when the reproduction test passes.

---

### 5. AUTOMATION AND TOOLING

#### 5.1 Machine-Enforced Standards
Human review is reserved for reasoning, not formatting.
* **Formatters:** Mandatory and enforced in CI (e.g., `clang-format`, `rustfmt`, `black`).
* **Static Analysis:** CI MUST fail on any new warnings introduced in Tier A/B code.
* **Gate:** Changes failing automation are not eligible for human review.

---

### 6. REVIEW AUTHORITY

#### 6.1 Rejection Criteria
A Change Request MUST be rejected if:
* Required tests are missing.
* The reviewer cannot understand the logic (Five-Minute Rule).
* Errors are silently ignored.
* Magic values are present.

#### 6.2 Acceptance Criteria
A Change Request MAY be accepted only if:
* All automated checks pass.
* Tier obligations are satisfied.
* Intent is evident without verbal explanation.

---

### 7. METRICS AND MONITORING
To ensure this standard improves quality rather than adding friction, the following metrics shall be tracked:
1.  **Tier A Compliance Rate:** % of new code meeting full standards (Target: 100%).
2.  **Defect Escape Rate:** Bugs found in Tier A/B code after merge (Target: <0.1 per KLOC).
3.  **Review Cycle Time:** Time from submission to merge (Target: <24h).
4.  **Complexity Trend:** Average Cyclomatic Complexity per function (Target: Stable or Decreasing).

---

### 8. ADOPTION STRATEGY
* **Phase 1 (Education):** Tooling setup (Linters/Formatters) and team alignment.
* **Phase 2 (Greenfield):** AEC applied strictly to new repos and modules.
* **Phase 3 (Brownfield):** "Boy Scout" rule (Tier B) activated for legacy codebases.

---
*Reference: AEC-1.0. Released into the Public Domain.*