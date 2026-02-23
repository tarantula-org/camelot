# Contributing to Camelot

This is the contribution guide for developers wanting to help build the Camelot standard library replacement.

## 1. Technical Contract
Camelot enforces strict architectural boundaries. Contributions that violate these constraints will be rejected at the PR level.

* **Memory Sovereignty:** Direct calls to `malloc`, `calloc`, `realloc`, or `free` are prohibited (they are poisoned at the compiler level). All allocations must be scoped to an `Arena` or `Workspace`.
* **RAII Scoping:** Use the provided `__attribute__((cleanup))` macro wrappers for all local Workspaces to ensure automatic memory release.
* **String Views:** Do not use null-terminated strings for internal logic. All string operations must utilize the `String` (ptr + len) view type.
* **Error Values:** Every fallible operation must return a `Result` enum. Silent failures or global error states are prohibited.
* **Namespacing:** Expose public functions via `const struct` subsets (e.g., `io.print`, `string.from`).

## 2. Repository Lifecycle

### Making Commits
* **Sign-off your commits** ([Git flag `-s`](https://git-scm.com/docs/git-commit#Documentation/git-commit.txt---signoff)). We use the [Developer Certificate of Origin (DCO)](https://developercertificate.org/) in place of a heavy CLA. By signing off, you certify that you have the right to submit the code under the project's license.
* **Sign commits** cryptographically where possible ([Learn more](https://docs.github.com/en/authentication/managing-commit-signature-verification/signing-commits)).
* **Conventional Commits:** Use the [Conventional Commit style](https://www.conventionalcommits.org/en/v1.0.0-beta.2/) for all commit messages (`feat:`, `fix:`, `refactor:`, `perf:`).
* **Formatting:** Run the code through `.clang-format` before committing. The CI pipeline will automatically reject styling violations.
* **Atomicity:** Keep each PR bound to a single feature, architectural change, or bug fix. This prevents PRs from getting stuck in review limbo.

### Merging Pull Requests
All PR titles must use Conventional Commit style. We enforce **squash merging** to keep the `main` branch history linear and readable. 

## 3. What can I help with?

The main project board serves as the starting point for all development:

1. **The Tarantula Master Plan:** Check the GitHub Projects tab for active development columns.
2. **Backlog & Free Issues:** Look for issues tagged with `needs-triage` or `enhancement`. These are generally free to pick up.
3. **Strategic Roadmap (`[PLAN]` tags):** Issues marked as Roadmap Directives are prioritized architectural goals. Please comment on the issue before beginning work to ensure your implementation plan aligns with the Steering Committee's constraints.
4. **Proposing Features (`[RFC]` tags):** If you are suggesting a new feature, open a Feature Request issue first. We prioritize keeping the dependency budget at zero; abstractions built solely for hypothetical future use cases will be declined.

Any work on issues marked for future milestones may conflict with ongoing core architectural changes. Please stick to the current active milestone.