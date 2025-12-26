<div align="center">

<img src="logo.svg" alt="Camelot Logo" width="180"/>

# Camelot
**The Senior Engineer's C Framework.**
A modern standard library replacement for C23 enforcing safety, ergonomics, and architectural integrity.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Standard: C23](https://img.shields.io/badge/standard-C23-purple.svg)](https://en.wikipedia.org/wiki/C23_(C_standard_revision))
[![Documentation: GitBook](https://img.shields.io/badge/docs-GitBook-blue.svg)](https://camelot-1.gitbook.io/docs/)

[**Quick Start**](https://camelot-1.gitbook.io/docs/start-here/quick-start) • [**Documentation**](https://camelot-1.gitbook.io/docs/) • [**Contribute**](CONTRIBUTING.md)

</div>

---

## 📘 Mission

**"C is legacy".** We reject this statement.

The problem isn't the language; it is the **Standard Library**. Camelot bridges the gap by bringing modern systems engineering concepts into the raw power of C:

1.  **Workspaces, not Malloc:** Memory is managed in scopes (Arenas), not individual pointers.
2.  **Views, not Copies:** Data is immutable and shared by default.
3.  **Transparency:** No hidden allocations. No magic.

---

## 🚀 Getting Started

We provide a comprehensive guide to get you from zero to a running kernel in 30 seconds.

👉 **[Read the Quick Start Guide](https://camelot-1.gitbook.io/docs/start-here/quick-start)**

---

## ⚔️ The Manifesto

**"Why still use C?"**

It is the industry's favorite question. The answer is not nostalgia; it is architectural necessity. We have compiled a comprehensive technical defense of C, dismantling the "Legacy" myth with architectural proofs.

👉 **[Read the Full Manifesto](https://camelot-1.gitbook.io/docs/manifesto)**

---

## 📂 Structure

```text
Camelot/
├── include/       # 📢 Public Interface
├── src/           # ⚙️ Internal Engine
├── tests/         # 🛡️ Unit Test Suite
├── packages/      # 🎁 Build Artifacts
└── Makefile       #    Build Automation
```