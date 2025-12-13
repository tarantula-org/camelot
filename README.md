<div align="center">

<img src="logo.png" alt="Camelot Logo" width="200"/>

# Camelot
**Re-Imagining C for modern engineering.** A framework enforcing safety, ergonomics, and structural integrity for the C language.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Standard: C23](https://img.shields.io/badge/standard-C23-purple.svg)](https://en.wikipedia.org/wiki/C23_(C_standard_revision))
[![Documentation: GitBook](https://img.shields.io/badge/docs-GitBook-blue.svg)](https://camelot-1.gitbook.io/docs/)

[**Philosophy**](#-philosophy) • [**Quick Start**](#-getting-started) • [**Documentation**](#-documentation)

</div>

-----

## 📘 Philosophy

C is often dismissed as "legacy"—a dangerous relic destined to be replaced. **This view ignores reality.**

The world's most critical infrastructure—from the **Linux Kernel** to **Git**—is built on C. It remains the gold standard for portability. Experienced engineers don't hate C; they love it for its **honesty**.

**Camelot brings the "Senior Engineer" workflow to the forefront.**
We strip away the friction of the 1989 standard library to reveal the high-performance, expressive language underneath.

* ❌ **Banned:** No manual `free()` drudgery. No `scanf` traps.
* ✅ **Ergonomic:** Memory handling is O(1) and linear via Arenas.
* ✅ **Transparent:** You control the machine, but the machine doesn't fight you.

-----

## 📚 Documentation

Detailed architecture, API references, and tutorials are available on our official documentation site:

👉 **[Read the Camelot Docs](https://camelot-1.gitbook.io/docs/)**

-----

## 🚀 Getting Started

### Prerequisites
* A C Compiler (GCC/Clang) supporting **C11** or higher (C23 recommended).

### Integration

1.  **Clone the Repository:**
    ```bash
    git clone [https://github.com/AKRiLLiCK/camelot.git](https://github.com/AKRiLLiCK/camelot.git)
    ```

2.  **Include the Fortress:**
    In your code, simply include the single public header.
    ```c
    #include "camelot/include/camelot.h"
    ```

3.  **Compile:**
    You must compile your code along with the Camelot core modules.
    ```bash
    gcc main.c camelot/src/*.c -o my_game
    ```

### Running Tests
To verify the framework integrity on your machine:
```bash
gcc tests.c src/*.c -std=c2x -o run_tests
./run_tests
```

---

##📂 Project StructureA clean, ISO-standard C structure designed for portability.

```text
Camelot/
├── Makefile                # Build automation
├── include/
│   └── camelot.h           # The Public Interface (and Poison)
├── src/
│   ├── camelot_memory.c    # Linear Arena & Allocator
│   ├── camelot_list.c      # Dynamic List (Vector) logic
│   ├── camelot_string.c    # Safe String Views
│   ├── camelot_table.c     # Hash Map implementation
│   └── camelot_io.c        # File System Helpers
└── tests.c                 # Verification Suite
```

---

##📄 LicenseMIT — Use, fork, learn, experiment.

See `LICENSE` for details.

##🎨 Credits**Author:** Acrilic

**Logo:** [Solarus](https://www.flaticon.com/free-icon/cube_17533083)
