# STLFix Documentation

The project documentation is split by concern so that the repository landing
page remains concise while design rules and domain research can evolve without
turning it into one large document.

- [Development setup](development-setup.md) describes the supported CMake,
  Ninja, GCC/MSYS2, vcpkg, VS Code, and test workflow.
- [Architecture](architecture.md) defines component boundaries and the intended
  path from the minimal core to the Windows GUI and renderer.
- [Mesh repair policy](mesh-repair-policy.md) defines how STLFix classifies,
  proposes, applies, and validates repairs.
- [SketchUp STL problems](sketchup-stl-problems.md) is the initial problem
  taxonomy and the plan for a regression corpus based on real exports.
- [Roadmap](roadmap.md) defines stable release targets and incremental versions.

