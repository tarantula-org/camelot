<div align="center">

<img src="logo.svg" alt="Camelot Logo" width="180"/>

# Camelot
**A Modern Standard Library for C23.**
Replacing legacy paradigms with safe memory models and strict architectural integrity.

[![License: MPL 2.0](https://img.shields.io/badge/License-MPL_2.0-brightgreen.svg?logo=opensourceinitiative&logoColor=white)](LICENSE)
[![Standard: C23](https://img.shields.io/badge/standard-C23-purple.svg?logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C23_(C_standard_revision))
[![Compliance: ASC-1.2](https://img.shields.io/badge/Compliance-ASC--1.2-firebrick.svg?logo=checkmarx&logoColor=white)](CONTRIBUTING.md)
[![Documentation: GitBook](https://img.shields.io/badge/docs-GitBook-blue.svg?logo=gitbook&logoColor=white)](https://camelot-1.gitbook.io/docs/)

[**Quick Start**](https://camelot-1.gitbook.io/docs/start-here/quick-start) • [**Roadmap**](https://camelot-1.gitbook.io/docs/roadmap) • [**Contribute**](CONTRIBUTING.md)

</div>

---

## <img src="https://cdn.simpleicons.org/c" width="24" style="vertical-align: middle;"> About

**The C language is fine. The standard library is the problem.**

Most bugs in C stem from the outdated design of `libc`—manual memory management (`malloc/free`), null-terminated string risks, and opaque error handling. Camelot is an opinionated framework designed to remove these foot-guns by enforcing modern systems engineering patterns at the core level.

We replace the standard library with a toolkit built on three pragmatic pillars:

1.  **Arena Memory (Workspaces):** We stop heap fragmentation by using scoped linear allocators. Memory is allocated in O(1) and cleaned up automatically when the scope ends (RAII).
2.  **Immutable Views:** We avoid unnecessary data copying. Functions accept "views" (pointer + length) rather than owning raw buffers, making data processing zero-copy by default.
3.  **Explicit Control:** We remove undefined behavior as a "feature." If an allocation fails or a bound is checked, the system handles it deterministically via strict Result types.

For a detailed breakdown of why we chose this architecture over Rust or C++, read our **[Design Philosophy](https://camelot-1.gitbook.io/docs/manifesto)**.

### ⚡ Getting Started
You can get the library compiled and linked to your project in under a minute.

**[Read the Quick Start Guide →](https://camelot-1.gitbook.io/docs/start-here/quick-start)**

---

## <img src="https://cdn.simpleicons.org/gitbook" width="24" style="vertical-align: middle;"> Project Structure

Camelot follows the **Avant Systems Canon (ASC-1.2)** to ensure maintainability.

The code is strictly separated into **Public API** (what you use) and **Internal Implementation** (how it works). Before contributing, please review the directory topology to understand where your code belongs.

**[Explore the Project Structure →](https://camelot-1.gitbook.io/docs/documentation/engineering/project-structure)**

---

## <img src="https://cdn.simpleicons.org/git" width="24" style="vertical-align: middle;"> Roadmap

We are currently in **Epoch 2: Compatibility & Hardening**. Our focus is on achieving feature parity with necessary libc functions while maintaining our safety guarantees.

**[View the Roadmap](https://camelot-1.gitbook.io/docs/roadmap)**
