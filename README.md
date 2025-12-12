<div align="center">

<img src="logo.png" alt="Camelot Logo" width="200"/>

# Camelot Framework

**Re-Imagining C for Modern Engineering.** *A modern fortress for a foundational language. 🏰*

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Standard: C23](https://img.shields.io/badge/standard-C23-purple.svg)](https://en.wikipedia.org/wiki/C23_(C_standard_revision))
[![Build: Make](https://img.shields.io/badge/build-makefile-green.svg)](Makefile)

[**Philosophy**](#-philosophy) • [**The Barrier**](#-the-barrier) • [**Quick Start**](#-getting-started) • [**Internals**](#-project-structure)

</div>

---

## 📘 Philosophy: The Wilderness vs. The Fortress

University teaches us to survive in the **Wilderness**.
We are taught to manually manage memory with `malloc` and `free`, parse raw text with `scanf`, and juggle `char*` pointers without safety nets. This leads to memory leaks, buffer overflows, and undefined behavior.

**Camelot builds a Fortress.**
This framework enforces modern engineering practices on top of C. It "poisons" legacy unsafe functions at the compiler level, forcing the developer to use safer, region-based memory management and sanitized I/O.

**The Goal:** Write C that feels like a modern language.
- ❌ **Banned:** No manual `free()` calls. No `scanf` loops.
- ✅ **Safe:** Memory is handled by Linear Arenas (O(1) allocation).
- ✅ **Typed:** Strings are structs, not raw pointers.

---

## 🛡️ The Barrier

Camelot uses GCC preprocessor directives to **physically prevent** unsafe code from compiling.

| Feature | Description | Status |
| :--- | :--- | :--- |
| **Poison Barrier** 🚫 | Blocks `malloc`, `free`, `scanf`, `gets`, `strcpy`. | 🟢 Active |
| **Linear Arena** 🧠 | Region-based memory. Allocations are instant pointers bumps. | 🟢 Active |
| **Auto-Cleanup** 🧹 | Uses `__attribute__((cleanup))` for scoped deallocation. | 🟡 WIP |
| **Input Sanitizer** 📝 | Replaces `scanf` with buffered, type-safe readers. | 🟢 Active |

---

## 🚀 Getting Started

### Prerequisites
* GCC or Clang
* Make

### Workflow

1. **Clone the repo:**
   ```bash
   git clone https://github.com/AKRiLLiCK/camelot
   cd camelot
   ```

2.  **Build the framework:**

    ```bash
    make
    ```

3.  **Start Coding:**
    Link your `main.c` against the Camelot object files. If you try to use banned functions like `malloc` or `scanf`, the compiler will throw an error and stop the build.

-----

## 📂 Project Structure

A clean, ISO-standard C structure designed for portability.

```text
Camelot/
├── Makefile                # Build automation
├── include/
│   └── camelot.h           # The Public Interface (and Poison)
├── src/
│   ├── camelot_memory.c    # Linear Arena implementation
│   ├── camelot_io.c        # Input Sanitizer implementation
│   └── camelot_string.c    # String type logic
└── main.c                  # Showcase Application
```

-----

## 📄 License

MIT — Use, fork, learn, experiment.  
See [LICENSE](https://www.google.com/search?q=LICENSE) for details.

## 🎨 Credits

**Author:** Acrilic

### Assets

  * Logo made by [Solarus](https://www.flaticon.com/free-icon/cube_17533083)
