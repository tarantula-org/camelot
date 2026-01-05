# Architectural Role Map
**Standard:** ASC-1.1

This document formally maps the physical source tree of **Camelot** to the **Avant Systems Canon (ASC-1.1)** architectural roles. Compliance is verified by the CI Automaton.

---

### 1. Memory Subsystem

* **Responsibilities:** Raw allocation, Arena lifecycle, Pointer arithmetic, Memory safety overrides.
* **Privilege:** Authorized for `malloc`, `free`, `memset`, and `uintptr_t` manipulation.
* **Invariant:** Must have **Zero Dependencies** on other internal subsystems. It is the root of the tree.
* **Scope:**
* `src/memory/`
* `include/camelot/memory.h`

### 2. Data Structure Subsystem

* **Responsibilities:** String views, Paged Lists, Hash Tables, Primitives.
* **Privilege:** Authorized for custom hashing and bitwise manipulation.
* **Dependency:** May depend strictly on **Memory Subsystem**.
* **Scope:**
* `src/ds/` (Lists, Tables)
* `src/types/` (Strings, Primitives)
* `include/ds/`
* `include/types/`

### 3. I/O Subsystem

* **Responsibilities:** File System, Streams, OS Descriptors, Type-safe formatting.
* **Privilege:** Authorized for `stdio.h`, `unistd.h`, `fcntl.h`, and `stdarg.h`.
* **Dependency:** May depend on **Memory** (for buffers) and **Data Structures** (for String views).
* **Scope:**
* `src/io/`
* `include/camelot/io.h`

### 4. Application Logic (The "Boundary")

* **Responsibilities:** Testing, Integration, Public API entry points.
* **Privilege:** **NONE.** Explicitly poisoned against all libc allocation and string functions.
* **Dependency:** Must use high-level abstractions. Direct access to "Internal" static functions of other subsystems is a violation of the Canon.
* **Scope:**
* `tests/`
* `include/camelot.h`
* `Makefile`

---

## Dependency Matrix

| From \ To | Memory | Data Structures | I/O | App Logic |
| --- | --- | --- | --- | --- |
| **Memory** | - | ❌ | ❌ | ❌ |
| **Data Structures** | ✅ | - | ❌ | ❌ |
| **I/O** | ✅ | ✅ | - | ❌ |
| **App Logic** | ✅ | ✅ | ✅ | - |

---

*Note: Any file or directory not explicitly mapped above is governed by the rules of **Application Logic**.*