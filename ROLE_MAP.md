# Architectural Role Map
**Repo Compliance:** ASC-1.1
**Governance:** Normative

This document maps the physical source tree of **Camelot** to the **Avant Systems Canon** architectural roles.

### 1. Memory Subsystem
**Responsibilities:** Raw allocation, Arena lifecycle, Pointer arithmetic, Memory safety overrides.
**Privilege:** Authorized to use `malloc`, `free`, `memset` (via `ALLOW_UNSAFE`).
**Scope:**
* `src/memory/`
* `include/camelot/memory.h`

### 2. Data Structure Subsystem
**Responsibilities:** Strings, Containers, Collections, Hashing, Type definitions.
**Privilege:** May apply for "Fundamental Data Structure" exemptions (e.g., custom hashing).
**Scope:**
* `src/ds/` (Lists, Tables)
* `src/types/` (Strings, Primitives)
* `include/ds/`
* `include/types/`

### 3. I/O Subsystem
**Responsibilities:** File System, Stdin/Stdout, OS Descriptors, Formatting.
**Privilege:** Authorized to use `stdio.h`, `unistd.h`, `fcntl.h` (via `ALLOW_UNSAFE`).
**Scope:**
* `src/io/`
* `include/camelot/io.h`

### 4. Application Logic (Default)
**Responsibilities:** Testing, Integration, Examples, Build Artifacts.
**Privilege:** NONE. Must use abstractions provided by other roles.
**Scope:**
* `tests/`
* `include/camelot.h` (Public Entry Point)
* `Makefile`
* `logo.svg`
* Root directory files (`.gitignore`, `LICENSE`, etc.)

---
*Note: Any file not explicitly listed above defaults to **Application Logic**.*