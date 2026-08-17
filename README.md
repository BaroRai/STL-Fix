# STLFix

Deterministic local STL analysis and repair engine written in modern C++.

The repair pipeline is intentionally fully deterministic. AI is not part of runtime mesh analysis or repair; AI-assisted development tools may only be used during development to help implement, test, and review the code.

## Goals

* ASCII and binary STL parsing
* deterministic mesh analysis
* topology reconstruction
* anomaly detection
* minimum-destructive repair
* watertightness validation
* self-intersection detection
* local/offline operation
* later Qt Widgets desktop GUI

## Components

* `stlfix_core` – reusable C++ STL analysis and repair engine
* `stlfix` – command-line interface
* `stlfix_gui` – Qt Widgets desktop application (planned)
* `tests` – unit and regression tests

The core engine should remain independent of Qt so it can be reused by the CLI, GUI, tests, or other applications.

## Project Structure

```text
STL-Fix/
├── CMakeLists.txt
├── CMakePresets.json
├── vcpkg.json
├── README.md
│
├── src/
│   ├── core/
│   │   ├── CMakeLists.txt
│   │   └── src/
│   │
│   └── cli/
│       ├── CMakeLists.txt
│       └── main.cpp
│
├── tests/
│
└── build/
```

`build/` contains generated files and is not committed to Git.

## Development Stack

Current Windows development environment:

* C++20
* GCC / MinGW-w64 UCRT64
* MSYS2
* CMake
* Ninja
* vcpkg
* Catch2 for tests
* VS Code
* Qt 6 Widgets planned for GUI

Current tested compiler:

```text
GCC / G++ 16.2.0
```

The exact compiler version is not intended to be a hard requirement unless compatibility problems are discovered.

## VS Code Extensions

Minimal intended VS Code setup:

* C/C++ – Microsoft
* CMake Tools – Microsoft
* Qt C++ – Qt Group

Qt Creator is not required.

Qt will later be used only as the GUI framework and related tooling.

---

# Windows Development Setup

## 1. CMake

Install CMake:

```powershell
winget install Kitware.CMake
```

Verify:

```powershell
cmake --version
```

After installation, restart VS Code so it receives the updated Windows `PATH`.

## 2. MSYS2

Install MSYS2:

```powershell
winget install MSYS2.MSYS2
```

Open:

```text
MSYS2 UCRT64
```

Do not use the generic `MSYS` terminal for the compiler toolchain.

The terminal prompt should contain:

```text
UCRT64
```

Update packages:

```bash
pacman -Syu
```

Install GCC/G++:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

Install Ninja:

```bash
pacman -S mingw-w64-ucrt-x86_64-ninja
```

Verify:

```bash
gcc --version
g++ --version
which g++
ninja --version
```

The compiler should resolve approximately to:

```text
/ucrt64/bin/g++
```

## 3. Add MSYS2 UCRT64 to Windows PATH

Add:

```text
C:\msys64\ucrt64\bin
```

to the Windows user `PATH`.

Restart VS Code afterwards.

Verify from the VS Code PowerShell terminal:

```powershell
gcc --version
g++ --version

where.exe g++
where.exe ninja
```

The expected compiler location is:

```text
C:\msys64\ucrt64\bin\g++.exe
```

## 4. vcpkg

Install vcpkg outside the STLFix repository:

```powershell
cd C:\

git clone https://github.com/microsoft/vcpkg.git

cd C:\vcpkg

.\bootstrap-vcpkg.bat
```

The project expects:

```text
VCPKG_ROOT
```

to point to the vcpkg installation.

For the current PowerShell session:

```powershell
$env:VCPKG_ROOT="C:\vcpkg"
```

Verify:

```powershell
$env:VCPKG_ROOT

Test-Path "$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
```

Expected result:

```text
True
```

For normal development, `VCPKG_ROOT` should eventually be configured permanently as a Windows environment variable.

Restart VS Code after changing environment variables.

---

# Dependency Management

Dependencies are defined in:

```text
vcpkg.json
```

The production STLFix core currently has no mandatory third-party runtime dependency.

Catch2 is an optional dependency used by the test suite.

Conceptually:

```text
base STLFix
    └── no external dependency

tests
    └── Catch2
```

The MinGW vcpkg triplet currently used by the project is:

```text
x64-mingw-dynamic
```

Both target and host triplets use MinGW so that vcpkg dependencies match the GCC/MSYS2 toolchain.

---

# CMake Presets

Shared project build configuration is stored in:

```text
CMakePresets.json
```

The project currently uses:

```text
debug
release
```

presets.

Generated files are stored in:

```text
build/debug/
build/release/
```

`CMakePresets.json` is committed to Git because it defines the shared build workflow.

Machine-specific configuration should preferably go into:

```text
CMakeUserPresets.json
```

which should not be committed.

---

# VS Code Workflow

Open the complete project folder in VS Code:

```text
D:\STL-Fix
```

Do not open only individual source files.

## Configure Using VS Code

Open Command Palette:

```text
Ctrl + Shift + P
```

Run:

```text
CMake: Select Configure Preset
```

Choose:

```text
STLFix Debug
```

or:

```text
STL-Fix Release
```

Then run:

```text
CMake: Configure
```

## Build Using VS Code

After a successful configure:

```text
Ctrl + Shift + P
```

Run:

```text
CMake: Build
```

---

# Terminal Workflow

The same workflow can be run directly from the integrated PowerShell terminal.

## Release Build

```powershell
cmake --preset release

cmake --build --preset release
```

## Debug Build

```powershell
cmake --preset debug

cmake --build --preset debug
```

---

# Clean Configure

CMake caches information about compilers, generators, dependencies, and toolchains.

After changing:

* compiler
* Ninja installation
* vcpkg configuration
* CMake presets
* toolchain settings

delete the relevant build directory before configuring again.

Release:

```powershell
Remove-Item -Recurse -Force .\build\release -ErrorAction SilentlyContinue

cmake --preset release

cmake --build --preset release
```

Debug:

```powershell
Remove-Item -Recurse -Force .\build\debug -ErrorAction SilentlyContinue

cmake --preset debug
```

---

# Running STLFix

After a successful release build:

```powershell
.\build\release\src\cli\stlfix.exe
```

The current executable is the CLI frontend for `stlfix_core`.

---

# Tests

Tests are located under:

```text
tests/
```

Catch2 is managed through the `tests` feature in `vcpkg.json`.

Once the test configuration is enabled and successfully built:

```powershell
ctest --test-dir build/debug --output-on-failure
```

The test suite will eventually contain both ordinary unit tests and deliberately corrupted STL fixtures.

Examples:

```text
valid_cube.stl
zero_area_triangle.stl
duplicate_faces.stl
open_boundary.stl
reversed_faces.stl
non_manifold_edge.stl
self_intersection.stl
```

Every repaired bug or edge case should ideally gain a regression test.

---

# Planned Qt GUI

The desktop GUI will be added later using:

```text
C++
Qt 6 Widgets
CMake
stlfix_core
```

The intended architecture is:

```text
             stlfix_core
             pure C++
                 ▲
          ┌──────┼──────┐
          │      │      │
         CLI   Tests   Qt GUI
```

Qt must not become a dependency of `stlfix_core`.

The GUI should only expose operations provided by the core engine.

Planned GUI capabilities include:

* open STL
* inspect model
* display mesh statistics
* visualize detected anomalies
* analyze model
* repair model
* compare before/after state
* export repaired STL

---

# Repair Philosophy

STLFix should prioritize preserving the original model.

The preferred repair order is:

```text
Analyze
   ↓
Detect
   ↓
Repair topology where possible
   ↓
Perform minimal geometric modification
   ↓
Validate again
```

The program should not silently invent substantial missing geometry.

If safe deterministic repair is impossible, STLFix should report the problem instead of guessing.

A future explicitly destructive reconstruction mode may use volumetric or SDF-based reconstruction, but it should remain separate from normal repair.

---

# Development Principles

1. Runtime repair logic must be deterministic.
2. Analysis and modification must remain separate.
3. Preserve original geometry whenever possible.
4. Every destructive modification should be measurable.
5. Validate after repair operations.
6. The core engine must remain independent of Qt.
7. Invalid or ambiguous geometry should be reported rather than silently guessed.
8. Regression tests should accompany repaired failure cases.
9. Same input and configuration should produce the same output.
10. GUI behavior must not influence repair results.

---

# Current Build Status

The base Windows development environment is operational:

```text
Git       OK
CMake     OK
Ninja     OK
GCC/G++   OK
vcpkg     OK
Release   OK
CLI link  OK
```

A successful release build currently ends with:

```text
Configuring done
Generating done
Build files have been written to: .../build/release
Linking CXX executable src\cli\stlfix.exe
```

---

# Next Milestone

The next implementation stage is the fundamental STL/mesh layer:

```text
Vec3
Triangle
Mesh
BoundingBox
```

followed by:

```text
Binary STL reader
ASCII STL reader
automatic STL format detection
basic validation
```

The first useful CLI target should eventually support:

```powershell
stlfix.exe analyze model.stl
```

and return basic information such as:

```text
File: model.stl
Format: Binary STL
Triangles: 15234

Bounding Box:
X: ...
Y: ...
Z: ...
```

More advanced topology reconstruction and actual repair logic will be implemented only after the parser and mesh representation are tested and stable.
