<div align="center">

  <img src="logo.svg" alt="Camelot Logo" width="200" />

  <br />

  <h1>Camelot</h1>
   
  **A C Standard Library Framework**

  <br />
  <br />

  <img src="https://img.shields.io/badge/License-MPL_2.0-brightgreen?style=for-the-badge&logo=opensourceinitiative&logoColor=white" alt="License" />
  <img src="https://img.shields.io/badge/Standard-C23-purple?style=for-the-badge&logo=c&logoColor=white" alt="Standard" />

</div>

<br />

## <img src="https://cdn.simpleicons.org/blueprint/5a5d7c" width="24" style="vertical-align: bottom;" /> Overview

C remains the foundational language for systems engineering due to its minimal runtime, predictable performance, and strict ABI stability. However, the legacy `libc` ecosystem lacks modern safety paradigms, often forcing developers to adopt C++ to manage complexity at scale.

**Camelot** is a standard library replacement built to bridge this gap. It implements the structural guarantees typically sought in higher-level languages—such as RAII, explicit error handling, and safe memory lifetimes—natively within C23. Our goal is to make C a rigorous, viable choice for modern large-scale projects.

## <img src="https://cdn.simpleicons.org/polywork/5a5d7c" width="24" style="vertical-align: bottom;" /> Architecture

The framework functions via four distinct components designed for deterministic execution:

| Component | Stack | Responsibility |
| :--- | :--- | :--- |
| **Memory Engine** | <img src="https://img.shields.io/badge/Arena_Allocator-96bf48?style=flat&logo=c&logoColor=white&labelColor=96bf48" height="20" /> | O(1) linear allocation (Workspaces) with automated `__attribute__((cleanup))` RAII. |
| **Data Layout** | <img src="https://img.shields.io/badge/Zero--Copy_Views-e5a50a?style=flat&logo=buffer&logoColor=white&labelColor=e5a50a" height="20" /> | Immutable string views and slices to prevent redundant memory copying and bounds violations. |
| **Safety Protocol** | <img src="https://img.shields.io/badge/Result_Types-dea584?style=flat&logo=rust&logoColor=white&labelColor=dea584" height="20" /> | Deterministic error handling via explicit `Result` return types; silent failures are prohibited. |
| **Build System** | <img src="https://img.shields.io/badge/Make_Dist-0082fc?style=flat&logo=gnu&logoColor=white&labelColor=0082fc" height="20" /> | Source-level artifact generation and structural isolation. |

## <img src="https://cdn.simpleicons.org/github/ffffff" width="24" style="vertical-align: bottom;" /> Integration

Camelot is distributed via versioned source releases. To integrate the framework, download the latest stable version from the [releases](https://github.com/tarantula-org/camelot/releases) section, compile the static library via `make`, and install it onto your system to link against your projects.