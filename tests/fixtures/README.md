# STL test fixtures

Fixtures in this directory are small, redistributable inputs used by automated
regression tests and manual CLI checks. Generated repair output must be written
to a build or temporary directory, never back into this directory.

## `synthetic/damaged_cube.stl`

- **Origin:** generated specifically for STLFix; no third-party model data.
- **Units:** unitless STL coordinates; dimensions are `10 x 10 x 10`.
- **Input triangles:** 14.
- **Expected conservative repair:** remove one exact same-orientation duplicate
  and one exactly degenerate triangle, leaving the 12 surface triangles of a
  cube.
- **Purpose:** quick manual test object and automated reader/repair/writer
  regression fixture.

This fixture intentionally covers only defects the current conservative repair
policy can resolve without guessing. Future SketchUp-derived fixtures belong in
`tests/fixtures/sketchup/` and must document the SketchUp version, exporter,
settings, units, expected findings, and provenance.

