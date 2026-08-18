# Development Setup

## Toolchain

The primary Windows environment uses:

- C++20;
- GCC/MinGW-w64 UCRT64 from MSYS2;
- CMake 3.25 or newer;
- Ninja;
- vcpkg manifest mode;
- Catch2 for tests;
- VS Code with the Microsoft C/C++ and CMake Tools extensions.

Qt 6 Widgets and DirectX are planned frontend dependencies. They are not core
dependencies.

## Windows installation

Install CMake and MSYS2:

```powershell
winget install Kitware.CMake
winget install MSYS2.MSYS2
```

In an **MSYS2 UCRT64** terminal, update packages and install GCC and Ninja:

```bash
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-ninja
```

Add the following directory to the Windows user `PATH`, then restart VS Code:

```text
C:\msys64\ucrt64\bin
```

Verify the tools from the VS Code PowerShell terminal:

```powershell
cmake --version
gcc --version
g++ --version
ninja --version
where.exe g++
where.exe ninja
```

The compiler should normally resolve to
`C:\msys64\ucrt64\bin\g++.exe`.

## vcpkg

Install vcpkg outside this repository:

```powershell
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd C:\vcpkg
.\bootstrap-vcpkg.bat
```

Set `VCPKG_ROOT` to that installation. For one PowerShell session:

```powershell
$env:VCPKG_ROOT = "C:\vcpkg"
Test-Path "$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
```

The expected result is `True`. Configure the variable permanently for normal
development and restart VS Code after changing it.

Production core currently has no mandatory third-party dependency. Catch2 is
provided by the optional `tests` feature in `vcpkg.json`. The Windows presets use
the `x64-mingw-dynamic` target and host triplets so dependencies match the GCC
toolchain.

## Configure and build

Shared presets are stored in `CMakePresets.json`; machine-only overrides belong
in the ignored `CMakeUserPresets.json`.

```powershell
cmake --preset debug
cmake --build --preset debug

cmake --preset release
cmake --build --preset release
```

From VS Code, open the complete repository folder, select a configure preset with
**CMake: Select Configure Preset**, then run **CMake: Configure** and
**CMake: Build**.

Generated output belongs under `build/` and is not committed.

## Clean configure

CMake caches compiler, generator, dependency, and toolchain information. Delete
the affected build directory after changing any of those inputs:

```powershell
Remove-Item -Recurse -Force .\build\debug -ErrorAction SilentlyContinue
cmake --preset debug
cmake --build --preset debug
```

Use `build\release` and the release preset for the corresponding release clean.

## Tests

When the test target is enabled and built:

```powershell
ctest --test-dir build/debug --output-on-failure
```

The suite will combine unit tests, generated corruptions, and small regression
fixtures. Each fixed parser or mesh defect should gain a regression test where
practical.

## Run the CLI

After a Windows release build:

```powershell
.\build\release\src\cli\stlfix.exe
```

