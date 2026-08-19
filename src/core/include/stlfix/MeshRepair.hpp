#pragma once

#include "stlfix/Mesh.hpp"

#include <cstddef>

namespace stlfix {

struct MeshRepairReport final {
  std::size_t inputTriangles{};
  std::size_t outputTriangles{};
  std::size_t removedDegenerateTriangles{};
  std::size_t removedExactDuplicateTriangles{};
};

struct MeshRepairResult final {
  Mesh mesh;
  MeshRepairReport report;
};

class MeshRepairer final {
public:
  // Removes only unambiguous defects. A positive tolerance explicitly opts in
  // to treating triangles at or below that area as degenerate.
  [[nodiscard]] MeshRepairResult
  repairConservatively(const Mesh &source, double areaTolerance = 0.0) const;
};

} // namespace stlfix
