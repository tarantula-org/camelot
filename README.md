<div align="center">

<img src="logo.png" alt="Camelot Logo" width="200"/>

# Camelot
**Re-Imagining C for modern engineering.**
A framework enforcing safety, ergonomics, and structural integrity for the C language.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Standard: C23](https://img.shields.io/badge/standard-C23-purple.svg)](https://en.wikipedia.org/wiki/C23_(C_standard_revision))
[![Documentation: GitBook](https://img.shields.io/badge/docs-GitBook-blue.svg)](https://camelot-1.gitbook.io/docs/)

[**Philosophy**](#-philosophy) • [**Documentation**](#-documentation)

</div>

-----

## 📘 Philosophy

C is often dismissed as "legacy"—destined to be replaced. **This view ignores reality.**

The world's critical infrastructure is built on C. Experienced engineers don't hate C; they love it for its honesty. **Camelot** brings the "Senior Engineer" workflow to the forefront, stripping away the friction of the 1989 standard library to reveal the high-performance language underneath.

We believe in a transparent relationship between the engineer and the machine: strict control, zero hidden costs, and architectural clarity.

-----

## 📂 Project Structure

A clean, modular architecture designed for portability and logical separation of concerns.

```text
Camelot/
├── LICENSE                     # MIT License
├── README.md                   # Documentation & Manifesto
├── Makefile                    # Build Automation
│
├── include/                    # 📢 PUBLIC INTERFACE
│   └── camelot/                # Namespace
│       ├── camelot.h           # The Facade (Single include point)
│       ├── memory.h            # Arena, Allocators, & RAII Scopes
│       ├── io.h                # File System & I/O
│       │
│       ├── types/              # 🧱 TYPES MODULE
│       │   ├── primitives.h    # u8, i32, f64, bool, Error codes
│       │   └── string.h        # Str type & operations
│       │
│       └── ds/                 # 📦 DATA STRUCTURES MODULE
│           ├── array.h         # Dynamic Array definitions
│           └── table.h         # Hash Map definitions
│
├── src/                        # ⚙️ INTERNAL ENGINE
│   ├── internal.h              # Private shared helpers
│   │
│   ├── memory/
│   │   ├── arena.c             # The Linear Allocator logic
│   │   └── scope.c             # Auto-cleanup logic
│   │
│   ├── types/
│   │   └── string.c            # String implementation
│   │
│   ├── ds/
│   │   ├── array.c             # Array implementation
│   │   └── table.c             # Hash Map implementation
│   │
│   └── io/
│       └── fs.c                # File System implementation
│
└── tests/                      # 🛡️ QUALITY CONTROL
    ├── tests.h                 # The Header-Only Test Harness
    ├── main.c                  # Test Runner Entry Point
    ├── test_types.c            # Tests for Strings & Primitives
    ├── test_memory.c           # Tests for Arena & Scopes
    ├── test_ds.c               # Tests for Arrays & Tables
    └── test_io.c               # Tests for Files

```

---

##📚 DocumentationDetailed architecture, API references, and design principles are available on our official documentation site:

👉 **[Read the Camelot Docs](https://camelot-1.gitbook.io/docs/)**

---

##📄 License**MIT** — Use, fork, learn, experiment. See `LICENSE` for details.

##🎨 Credits**Author:** Acrilic
**Logo:** [Solarus](https://www.flaticon.com/free-icon/cube_17533083)