<div align="center">

  <img src="logo.svg" alt="Camelot Logo" width="200" />

  <br />

  <h1>Camelot</h1>
   
  **The Senior Engineer's C Framework**

  <br />
  <br />

  <img src="https://img.shields.io/badge/License-MPL_2.0-brightgreen?style=for-the-badge&logo=opensourceinitiative&logoColor=white" alt="License" />
  <img src="https://img.shields.io/badge/Standard-C23-purple?style=for-the-badge&logo=c&logoColor=white" alt="Standard" />
  <img src="https://img.shields.io/badge/Compliance-ASC_1.3-firebrick?style=for-the-badge&logo=checkmarx&logoColor=white" alt="Compliance" />

</div>

<br />

## <img src="https://cdn.simpleicons.org/blueprint/5a5d7c" width="24" style="vertical-align: bottom;" /> Overview

**Camelot** is a hardened standard library replacement for C23.

Engineered to eliminate the fragility of legacy `libc` paradigms, Camelot prioritizes memory safety, architectural rigor, and zero-cost abstractions. It serves as a foundational kernel for high-reliability software, replacing manual management with strict automated lifecycles. It is not merely a library, but a rigorous standard for the modern C systems engineer.

## <img src="https://cdn.simpleicons.org/polywork/5a5d7c" width="24" style="vertical-align: bottom;" /> Architecture

The framework functions via four distinct, non-overlapping pillars:

| Component | Stack | Responsibility |
| :--- | :--- | :--- |
| **Memory Engine** | <img src="https://img.shields.io/badge/Arena_Allocator-96bf48?style=flat&logo=c&logoColor=white&labelColor=96bf48" height="20" /> | O(1) linear allocation (Workspaces) with automated RAII cleanup. |
| **Data Layout** | <img src="https://img.shields.io/badge/Zero--Copy_Views-e5a50a?style=flat&logo=buffer&logoColor=white&labelColor=e5a50a" height="20" /> | Immutable string views and slices to eliminate redundant copying. |
| **Safety Protocol** | <img src="https://img.shields.io/badge/Result_Types-dea584?style=flat&logo=rust&logoColor=white&labelColor=dea584" height="20" /> | Deterministic error handling and compiler-enforced state checks. |
| **Build System** | <img src="https://img.shields.io/badge/Make_Dist-0082fc?style=flat&logo=gnu&logoColor=white&labelColor=0082fc" height="20" /> | Portable artifact generation and strict ASC-1.2 separation. |

## <img src="https://cdn.simpleicons.org/github/ffffff" width="24" style="vertical-align: bottom;" /> Integration

Camelot is a developer-first, source-available kernel. We do not ship pre-compiled binaries; you compile the framework directly alongside your project to ensure ABI compatibility and optimization.

[![Read Quick Start](https://img.shields.io/badge/READ-QUICK_START-5a5d7c?style=for-the-badge&logo=gitbook&logoColor=white&labelColor=181717)](https://camelot-1.gitbook.io/docs/documentation)
