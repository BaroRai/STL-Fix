# Release Roadmap

Version numbers describe user-visible capability, not portfolio stages. Scope is
kept incremental and a version is considered stable only after its required
tests and documentation pass.

The project is currently in the `0.x` development series. Release `1.0` is the
first stable command-line analysis and conservative repair release. Minor `1.x`
releases extend that stable core without waiting for the GUI. Release `2.0`
introduces the stable desktop application and GPU-backed viewport.

## 0.1 - Project foundation (current)

- C++20 CMake project;
- GCC/MinGW and Ninja workflow;
- reusable core, CLI, and Catch2 test targets;
- architecture, repair policy, and agent instructions.

## 0.2 - Geometry and mesh foundation

- `Vec3`, `Triangle`, `BoundingBox`, and `Mesh`;
- explicit ownership and read-only traversal;
- unit tests for geometry invariants and numerical edge cases.

## 0.3 - STL input

- binary and ASCII readers;
- automatic format detection;
- structured parse errors;
- malformed and truncated input tests.

## 0.4 - MVP analysis

- indexed topology reconstruction;
- mesh statistics and bounding box;
- exact duplicate and degenerate triangles;
- boundary and non-manifold edges;
- orientation and connected-component findings;
- deterministic text and JSON reports;
- `stlfix analyze model.stl`.

## 0.5 - Conservative repair preview

- explicit repair plans and reports;
- safe duplicate and degenerate cleanup;
- unambiguous orientation repair;
- STL output and complete post-repair validation;
- initial SketchUp regression corpus.

## 1.0 - Stable CLI analysis and repair

- stable documented CLI and core API surface;
- deterministic analysis and conservative repair workflow;
- binary and ASCII round-trip coverage;
- regression, sanitizer, fuzz, and performance baselines;
- clear handling of ambiguous and destructive cases.

## 1.1 - Expanded SketchUp diagnostics

- larger reproducible export corpus;
- T-junction and coincident-seam diagnostics;
- improved component and shell classification.

## 1.2 - Advanced intersections

- accelerated self-intersection detection;
- coplanar overlap classification;
- scalable spatial indexing and benchmarks.

## 1.3 - Conditional repair tools

- policy-controlled vertex welding;
- small-boundary repair where intent is unambiguous;
- before-and-after quality measurements.

Additional `1.x` releases may be added as focused stable increments. The roadmap
does not reserve empty version numbers merely to reach a predetermined count.

## 2.0 - Stable desktop application

- Qt Widgets application shell;
- Windows-native 3D viewport with a separate DirectX renderer;
- background analysis with cancellation and progress;
- issue selection and visualization;
- before-and-after comparison and repaired STL export.

GPU acceleration initially belongs to visualization. Moving a core algorithm to
the GPU requires deterministic CPU reference results, profiling evidence, and an
architecture decision that preserves a non-GUI core.

