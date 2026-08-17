# STLFix

Deterministic local STL analysis and repair engine.

## Goals

- ASCII and binary STL parsing
- deterministic mesh analysis
- topology reconstruction
- anomaly detection
- minimum-destructive repair
- watertightness validation
- self-intersection detection
- local/offline operation

## Components

- `stlfix_core` – C++ repair engine
- `stlfix` – command-line interface
- `stlfix_gui` – Qt desktop application (planned)

## Build

```bash
cmake --preset debug
cmake --build --preset debug