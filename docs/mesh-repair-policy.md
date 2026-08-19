# Mesh Repair Policy

## Principles

STLFix preserves the source model whenever possible. Analysis never mutates a
mesh, and a finding does not automatically authorize a repair.

```text
Parse -> Analyze -> Report -> Propose -> Apply -> Validate -> Accept or reject
```

Every accepted repair must report:

- the operation and policy that authorized it;
- affected source elements where they are known;
- measurable before and after values;
- diagnostics remaining after validation.

An operation that cannot make a safe, deterministic choice reports the ambiguity
instead of guessing.

## Repair levels

### Safe

Operations with an unambiguous local intent, such as removing an exact duplicate
triangle or a triangle with repeated vertex identifiers. Safe does not mean
unchecked: the complete result is validated again.

### Conditional

Operations whose correctness depends on an explicit tolerance or model policy,
such as welding nearby vertices, removing an isolated component, or filling a
small planar boundary. They are opt-in and include the policy in the report.

### Destructive

Operations that can substantially reconstruct the model, including broad
retriangulation, boolean reconstruction, or voxel/SDF-based replacement. These
remain separate from normal repair and require explicit selection.

## Tolerances

Diagnostics distinguish exact equality from tolerance-dependent proximity. A
tolerance has a name, unit, documented purpose, and configuration source. The
implementation must account for model scale and must not merge nearby surfaces
merely because a global epsilon happens to include both.

## Determinism

Reports and repair plans have a canonical order independent of hash-table order,
thread scheduling, GUI state, or progress callbacks. The same input,
configuration, and STLFix version must produce the same output.

