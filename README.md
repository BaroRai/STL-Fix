# STLFix

STLFix is a deterministic, local STL analysis and repair engine written in
modern C++. It is intended to diagnose damaged triangle meshes, propose
conservative repairs, validate every applied change, and clearly report cases
where safe automatic repair is impossible.

AI is not part of runtime mesh analysis or repair. AI-assisted development tools
may help implement, test, and review the project, subject to the repository's
[`AGENTS.md`](AGENTS.md) rules.

## Goals

- ASCII and binary STL parsing with automatic format detection;
- deterministic mesh and topology reconstruction;
- duplicate, degenerate, boundary, non-manifold, orientation, and intersection
  diagnostics;
- minimum-destructive, measurable repair;
- watertightness and post-repair validation;
- local/offline CLI operation;
- a later Qt Widgets desktop GUI with a separate DirectX viewport.

## Components

- `stlfix_core` - reusable, UI-independent C++ analysis and repair engine;
- `stlfix` - command-line frontend;
- `stlfix_gui` - planned Qt Widgets desktop frontend;
- DirectX renderer - planned Windows viewport backend, separate from core;
- `tests` - unit, generated-corruption, and regression tests.

The core must remain independent of Qt, DirectX, and frontend behavior so it can
be reused by the CLI, GUI, tests, and other applications.

## Project structure

```text
STL-Fix/
|-- AGENTS.md
|-- CMakeLists.txt
|-- CMakePresets.json
|-- vcpkg.json
|-- README.md
|-- docs/
|-- src/
|   |-- core/
|   `-- cli/
|-- tests/
`-- build/                 generated, not committed
```

The initial core remains deliberately compact. It will be divided into deeper
geometry, I/O, analysis, and repair directories only when those modules grow
enough for the extra structure to improve navigation.

## Documentation

- [Documentation index](docs/README.md)
- [Development setup](docs/development-setup.md)
- [Architecture](docs/architecture.md)
- [Mesh repair policy](docs/mesh-repair-policy.md)
- [SketchUp STL problem map](docs/sketchup-stl-problems.md)
- [Release roadmap](docs/roadmap.md)

The SketchUp document records why apparent duplicate edges and faces occur in
exported triangle meshes, how STLFix can classify them, and how real exports will
be converted into reproducible regression cases.

## Development stack

- C++20
- GCC / MinGW-w64 UCRT64
- CMake and Ninja
- vcpkg
- Catch2
- VS Code
- Qt 6 Widgets and DirectX planned for the desktop frontend

See the [development setup](docs/development-setup.md) for installation,
configuration, build, and test commands.

## Repair workflow

```text
Parse
  -> Analyze
  -> Report
  -> Propose repair
  -> Apply explicit repair
  -> Validate again
  -> Accept or reject the result
```

STLFix does not silently invent substantial missing geometry. Exact, conditional,
and destructive operations are treated as separate repair levels; details are
defined in the [mesh repair policy](docs/mesh-repair-policy.md).

## Release plan

STLFix uses normal product versions rather than portfolio stages:

- `0.2` - geometry and mesh foundation;
- `0.3` - robust ASCII and binary STL input;
- `0.4` - MVP analysis and `stlfix analyze model.stl`;
- `0.5` - conservative repair preview and initial SketchUp corpus;
- `1.0` - stable CLI analysis and conservative repair;
- `1.x` - focused stable diagnostic and repair improvements;
- `2.0` - stable desktop application with a DirectX-backed 3D viewport.

See the [release roadmap](docs/roadmap.md) for scope and acceptance direction.

## MVP command

The first useful CLI workflow will be:

```powershell
stlfix.exe analyze model.stl
```

It reports the detected format, triangle count, and bounding box. Reconstructed
topology statistics and deterministic defect diagnostics will be added in the
next analysis stage.

The first conservative repair workflow is:

```powershell
stlfix.exe repair input.stl repaired.stl
```

It removes only exact degenerate and same-orientation duplicate triangles, writes
an ASCII STL, and reports every removed triangle. Ambiguous topology repair is
not performed by this initial command.
