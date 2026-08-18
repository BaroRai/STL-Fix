# Architecture

## Design goals

STLFix should provide deterministic STL parsing, analysis, and conservative
repair as a reusable C++20 core. Frontends present core operations; they do not
reimplement or influence them.

```text
                    stlfix_core
                         ^
              +----------+----------+
              |          |          |
             CLI       Tests       GUI
                                      |
                                Renderer API
                                      |
                               DirectX backend
```

## Minimal initial layout

The first implementation deliberately avoids a directory and interface for
every small type.

```text
src/core/
|-- include/stlfix/
|   |-- Geometry.hpp
|   |-- Mesh.hpp
|   |-- StlReader.hpp
|   |-- MeshAnalyzer.hpp
|   `-- Diagnostics.hpp
`-- src/
    |-- Geometry.cpp
    |-- Mesh.cpp
    |-- StlReader.cpp
    `-- MeshAnalyzer.cpp
```

The layout may be split into `geometry/`, `io/`, `analysis/`, and `repair/`
subdirectories when those areas contain enough code to make that division
useful. Public names should remain under the `stlfix` namespace so that a
physical reorganization does not force an API redesign.

## Domain model

`Vec3`, `Triangle`, `BoundingBox`, and `Mesh` are value/domain types. They do not
share a universal polymorphic `Object` base. `Mesh` owns its geometry and exposes
read-only traversal rather than its mutable container.

Runtime polymorphism is reserved for demonstrated boundaries such as multiple
mesh readers, writers, or repair operations. Constructor injection is preferred
when a use case must depend on such a boundary. STLFix does not need a dependency
injection framework or service locator.

Non-member `friend` functions are appropriate for symmetric value operations,
such as `Vec3` arithmetic, when they preserve encapsulation. Broad `friend class`
access is not a substitute for a small public API.

Lambdas, ranges, concepts, and callbacks should express a concrete algorithm or
policy. They should not be introduced only to demonstrate language features.
Callbacks may observe progress but must not affect analysis or repair results.

## Frontend and renderer boundary

The planned GUI may use Qt Widgets for the application shell and a dedicated
DirectX backend for its 3D viewport. Neither dependency belongs in
`stlfix_core`.

The renderer owns its GPU resources and derives a render-oriented mesh from the
core mesh. This permits the core to retain the numerical representation required
by geometry algorithms while the renderer uses GPU-friendly vertex buffers.
The initial renderer should favor the smallest Windows API that meets the viewer
requirements; a move from DirectX 11 to DirectX 12 requires a measured benefit
and a separate design decision.

GPU work begins with rendering. Analysis and repair remain CPU implementations
until tests, profiles, and deterministic reference results identify a worthwhile
acceleration target.

