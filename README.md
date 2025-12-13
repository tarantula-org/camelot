<div align="center">

<img src="logo.png" alt="Camelot Logo" width="200"/>

# Camelot
Re-Imagining C for modern engineering. A framework enforcing safety, ergonomics, and structural integrity for the C language.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Standard: C23](https://img.shields.io/badge/standard-C23-purple.svg)](https://en.wikipedia.org/wiki/C23_(C_standard_revision))
[![Build: Make](https://img.shields.io/badge/build-makefile-green.svg)](Makefile)

[**Philosophy**](#-philosophy) • [**Features**](#-features) • [**Quick Start**](#-getting-started) • [**Internals**](#-project-structure)

</div>

-----

## 📘 Philosophy

C is often dismissed as "legacy"—a dangerous relic destined to be replaced. **This view ignores reality.**

The world's most critical infrastructure—from the **Linux Kernel** to **Git**, **Redis**, and **SQLite**—is built on C. It remains the gold standard for portability and stability. Experienced engineers don't hate C; they love it for its **honesty**. In C, there is no hidden garbage collector, no massive runtime, and no "magic" slowing you down. It is surprisingly ergonomic when stripped of its historical baggage.

**The "pain" of C comes from the archaic patterns taught in academia.**
Universities train us to manually `free()` every single allocation and wrestle with brittle `scanf` parsing. That isn't "systems programming"—that is just bad engineering. Real-world systems programmers use **Memory Arenas** to handle allocations instantly and **Data-Oriented patterns** to manage state.

**Camelot brings this "Senior Engineer" workflow to the forefront.**
We strip away the friction of the 1989 standard library to reveal the high-performance, expressive language underneath.

**The Goal:** Prove that C is the ultimate tool for modern engineering.
- ❌ **Banned:** No manual `free()` drudgery. No `scanf` traps.
- ✅ **Ergonomic:** Memory handling is O(1) and linear via Arenas (just like modern game engines).
- ✅ **Transparent:** You control the machine, but the machine doesn't fight you.

-----

Yes, you are absolutely correct. Trying to set up a C package manager (like `conan`, `vcpkg`, or `clib`) right now would be **over-engineering** and a massive distraction.

In the C ecosystem, unlike Rust (Cargo) or Node (npm), package managers are not standardized. Forcing a user to install `conan` just to try your experimental framework is a huge barrier to entry.

###Why the "Vendor" (Copy/Paste) approach is better for now:1. **Zero Friction:** The user already has `gcc` and `git`. That is all they should need.
2. **Transparency:** Since your philosophy is "Construction" and "Fortress," users *should* see the `camelot_memory.c` file in their project. It helps them trust the "magic."
3. **Fits Your Structure:** Your current structure is designed to be built easily. Breaking it apart for a package registry adds complexity you don't need yet.

Here is the **refined README** section. It respects your folder structure and provides the exact command a user needs to run to make it work, without assuming they have any fancy tools.

-----

## 🛡️ Features

Camelot enforces modern engineering practices through a combination of compile-time restrictions and runtime helpers.

| Feature | Description | Status |
| :--- | :--- | :--- |
| **Poison Barrier** 🚫 | Blocks `malloc`, `free`, `scanf`, `gets`, `strcpy`. | 🟠 Experimental |
| **Linear Arena** 🧠 | Region-based memory. Allocations are instant pointers bumps. | 🟠 Experimental |
| **Auto-Cleanup** 🧹 | Uses `__attribute__((cleanup))` for scoped deallocation. | 🔴 Planned |
| **Input Sanitizer** 📝 | Replaces `scanf` with buffered, type-safe readers. | 🔴 Planned |

**Lifecycle:** 🔴 **Planned** (Roadmap) • 🟠 **Experimental** (API Unstable) • 🟡 **Preview** (Testing) • 🟢 **Stable** (Production Ready)

-----

## 🚀 Getting Started

Camelot is designed to be **dropped directly into your project**. No complex package managers required.

### Prerequisites
* A C Compiler (GCC/Clang) supporting **C11** or higher.

### Integration

1. **Download the Fortress:**
   Clone this repository into a `lib/` folder inside your project.
   ```bash
   mkdir -p lib
   git clone [https://github.com/AKRiLLiCK/camelot.git](https://github.com/AKRiLLiCK/camelot.git) lib/camelot
   ```

2. **Include the Interface:**
   In your `main.c`, include the single public header.
   ```c
   // main.c
   #include "lib/camelot/include/camelot.h"
   ```

3. **Compile:**
You must compile your code **along with** the Camelot implementation files.
   ```bash
   gcc main.c lib/camelot/src/*.c -I lib/camelot/include -o my_app
   ```

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
