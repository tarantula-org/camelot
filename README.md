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
├── packages/                   # 🎁 BUILD ARTIFACTS
│   ├── test_runner             # Compiled Test Suite
│   └── dist/                   # Production-ready package
│
├── include/                    # 📢 PUBLIC INTERFACE
│   ├── camelot.h               # The Facade (Single include point)
│   │
│   ├── camelot/                # Core Framework
│   │   ├── memory.h            # Arena Allocator & Scopes
│   │   └── io.h                # I/O Namespace & File System
│   │
│   ├── types/                  # 🧱 TYPE DEFINITIONS
│   │   ├── primitives.h        # Rust-style u8, i32, f64, bool
│   │   └── string.h            # 'String' struct & namespace
│   │
│   └── ds/                     # 📦 DATA STRUCTURES
│       ├── list.h              # Paged List (O(1) access & growth)
│       └── table.h             # Hash Table (Linear Probing)
│
├── src/                        # ⚙️ INTERNAL ENGINE
│   ├── io/
│   │   ├── fs.c                # File Stream Logic (Pipes/Sockets)
│   │   └── io.c                # Console Output Formatting
│   │
│   ├── memory/
│   │   └── memory.c            # Linear Memory Management
│   │
│   ├── types/
│   │   └── string.c            # String Implementation
│   │
│   └── ds/
│       ├── list.c              # Paged List Logic
│       └── table.c             # Hash Table Logic
│
└── tests/                      # 🛡️ QUALITY CONTROL
    ├── tests.h                 # Header-Only Test Harness
    ├── main.c                  # Test Runner Entry Point
    ├── test_types.c            # Verifies String construction
    ├── test_memory.c           # Verifies Arena alignment & OOM
    ├── test_io.c               # Visual checks for Output
    ├── test_files.c            # Verifies Slurp/Stream logic
    └── test_ds.c               # Verifies Lists & Hash Tables
```

---

## 📚 Documentation

Detailed architecture, API references, and design principles are available on our official documentation site:

👉 **[Read the Camelot Docs](https://camelot-1.gitbook.io/docs/)**

---

## 📄 License

**MIT** — Use, fork, learn, experiment. See `LICENSE` for details.

## 🎨 Credits

**Author:** Acrilic
**Logo:** [Solarus](https://www.flaticon.com/free-icon/cube_17533083)