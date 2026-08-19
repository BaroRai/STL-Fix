# SketchUp STL Problem Map

This document is an initial engineering taxonomy for STL files exported from
SketchUp. It is not yet a claim about the frequency of each defect. Findings will
be refined and prioritized using reproducible exports, recorded exporter
settings, and regression fixtures.

## STL limitation: edges are reconstructed

STL stores triangle facets, not independent model edges, component hierarchy,
materials, or standardized units. A reported "duplicate edge" therefore cannot
be read directly from an STL file. STLFix must first reconstruct indexed vertices
and canonical edges from triangle geometry.

After reconstruction, an edge with one incident face is normally a boundary;
two incident faces are the manifold case; and three or more incident faces are
non-manifold. Apparent duplicate edges can instead originate from duplicate
faces, coincident but unwelded vertices, overlapping shells, or internal faces.

## Initial defect taxonomy

### Duplicate and opposite duplicate triangles

The same three geometric vertices can occur repeatedly with equal, cyclically
shifted, or reversed ordering. Exact duplicates are candidates for safe removal.
Oppositely oriented duplicates need contextual analysis before modification.

### Inconsistent or inward orientation

Adjacent triangles may disagree about winding across their shared edge, or a
closed component may be consistently oriented inward. Local adjacency and signed
volume checks are separate diagnostics.

### Boundary and non-manifold edges

Boundary edges expose holes or disconnected seams. Non-manifold edges commonly
indicate duplicate faces, internal walls, touching solids, or overlapping
components. Not every boundary is safe to fill automatically.

### Internal and overlapping faces

Internal walls and partially overlapping coplanar faces can survive when source
components were combined without a clean solid union. Partial coplanar overlap
is more complex than hashing identical triangles and requires a later geometric
overlap stage.

### Degenerate and near-degenerate triangles

Triangles may repeat a vertex, be collinear, or have an area that is negligible
at the model's scale. Exact and tolerance-dependent degeneracy are reported
separately.

### T-junctions and coincident unwelded vertices

A vertex may lie on another triangle's edge without sharing its topology.
Likewise, separate facet vertices may occupy equal or nearly equal positions.
Welding can repair a seam but can also destroy an intentional small gap, so its
tolerance is an explicit policy.

### Disconnected, touching, and nested shells

Nested or transformed source components can become separate shells, mirrored
winding, touching-only components, or overlapping volumes. STL does not retain
enough source hierarchy to reconstruct the original SketchUp component intent.

### Self-intersections

Non-adjacent triangles can intersect, touch, or overlap. Detection must
distinguish shared topology, contact, coplanar overlap, and a proper interior
intersection. Large meshes will require a tested acceleration structure such as
a BVH rather than an all-pairs scan.

### Units, scale, and small geometry

STL does not reliably communicate whether coordinates represent millimeters,
centimeters, meters, or inches. STLFix must not silently rescale a model. It may
report suspicious dimensions and support an explicit unit conversion whose
factor is recorded in the result.

### Softened and hidden source edges

A softened or hidden SketchUp edge is not necessarily a mesh defect. STL
triangulation also introduces diagonals that should not be reported as damaged
geometry. Original visibility and smoothing intent cannot generally be recovered
from STL alone.

## Regression corpus plan

Real cases should be reduced to the smallest redistributable fixture and stored
under a category in `tests/fixtures/sketchup/`. Each fixture needs metadata with:

- SketchUp and exporter version;
- export settings and assumed units;
- a short reproduction description;
- expected deterministic findings;
- licensing or provenance information.

Private models should not be committed. Instead, create a minimal synthetic
reproduction or a deterministic corruptor that derives the defect from a valid
primitive. Planned categories include duplicate faces, reversed faces, internal
faces, holes, T-junctions, non-manifold topology, touching components,
overlapping components, tiny faces, and self-intersections.

