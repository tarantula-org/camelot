# Pull Request

## 1. Context & Intent
[REPLACE THIS TEXT WITH YOUR CONTEXT]

## 2. Architectural Role
**Target Subsystem:** [ENTER ROLE HERE]

## 3. Type of Change
- [ ] `feat` (New functionality)
- [ ] `fix` (Bug fix)
- [ ] `refactor` (Refactoring without functional change)
- [ ] `perf` (Optimization)
- [ ] `docs` (Documentation update)
- [ ] `chore` (Build scripts, configs, maintenance)

## 4. Compliance Checklist
- [ ] **Role Mapping:** Changes strictly adhere to the responsibilities of the target subsystem.
- [ ] **Safety:** No `unsafe` / `malloc` usage outside authorized roles.
- [ ] **Hygiene:** `make test` passes locally.
- [ ] **Automaton:** Code is formatted via `clang-format`.

## 5. Verification
```text
[PASTE TERMINAL OUTPUT HERE]
