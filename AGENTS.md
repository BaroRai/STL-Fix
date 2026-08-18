# STLFix Agent Instructions

## Purpose

STLFix is a deterministic, local STL analysis and repair engine. Runtime mesh
analysis and repair must not depend on AI services.

## Architecture

- Keep `stlfix_core` independent of the CLI, Qt, DirectX, and other UI code.
- Keep analysis separate from modification.
- Do not put mesh-repair algorithms in a frontend or renderer.
- Prefer value types for geometry and domain data.
- Add polymorphic interfaces only at demonstrated variation points.
- Prefer constructor injection to service locators, global state, or a DI
  framework.
- Do not expose mutable internal containers.
- Use `friend` only for tightly scoped operators or invariant-preserving
  collaboration.

## Mesh and numerical rules

- The same input and configuration must produce the same ordered result.
- Preserve source triangle identifiers in diagnostics whenever possible.
- Never silently discard, invent, or rewrite geometry.
- Every repair must describe what it changed and must be followed by validation.
- Use named, configurable tolerances; do not scatter unexplained epsilon values.
- Distinguish exact defects from tolerance-dependent findings.
- Treat ambiguous repairs as findings unless an explicit repair policy permits
  the operation.

## C++ and dependencies

- Use C++20 and follow the target settings defined by CMake.
- Keep the production core free of mandatory third-party runtime dependencies
  unless an architectural decision explicitly changes this rule.
- Use const-correct APIs and apply `[[nodiscard]]`, `constexpr`, and `noexcept`
  where their contracts are accurate.
- Do not add Qt, Windows, or DirectX headers to `src/core`.
- Do not wrap imports or includes in exception handling.

## Testing

- Add focused tests with every production change.
- Add a regression test or fixture for every corrected defect where practical.
- Test parsers with empty, malformed, truncated, and adversarial input.
- Keep committed fixtures small and document their origin and expected findings.
- Run configure, build, tests, and relevant static or runtime checks before
  committing.

## Workflow

- Keep each change scoped to one roadmap milestone or one architectural concern.
- Do not combine parser, topology, repair, GUI, and renderer implementations in
  one change.
- Update documentation when public behavior, architecture, or repair policy
  changes.
- Record significant design decisions under `docs/` before introducing costly
  dependencies or irreversible architecture.

