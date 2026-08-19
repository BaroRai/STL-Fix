#include "stlfix/MeshRepair.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <set>
#include <stdexcept>
#include <utility>

namespace stlfix {
namespace {

[[nodiscard]] bool less(const Vec3 &lhs, const Vec3 &rhs) noexcept {
  if (lhs.x() != rhs.x()) {
    return lhs.x() < rhs.x();
  }
  if (lhs.y() != rhs.y()) {
    return lhs.y() < rhs.y();
  }
  return lhs.z() < rhs.z();
}

using TriangleKey = std::array<Vec3, 3>;

struct TriangleKeyLess final {
  [[nodiscard]] bool operator()(const TriangleKey &lhs,
                                const TriangleKey &rhs) const noexcept {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end(), less);
  }
};

// Cyclic rotations preserve winding, while reversed triangles intentionally
// remain distinct because removing opposite faces requires topology context.
[[nodiscard]] TriangleKey orientedKey(const Triangle &triangle) {
  TriangleKey key{triangle.vertex(0), triangle.vertex(1), triangle.vertex(2)};
  const auto first = std::min_element(key.begin(), key.end(), less);
  std::rotate(key.begin(), first, key.end());
  return key;
}

} // namespace

MeshRepairResult
MeshRepairer::repairConservatively(const Mesh &source,
                                   double areaTolerance) const {
  if (areaTolerance < 0.0 || !std::isfinite(areaTolerance)) {
    throw std::invalid_argument(
        "Repair area tolerance must be finite and non-negative");
  }
  if (!source.isFinite()) {
    throw std::invalid_argument(
        "Cannot repair a mesh with non-finite coordinates");
  }

  Mesh repaired;
  repaired.reserve(source.triangleCount());
  std::set<TriangleKey, TriangleKeyLess> retainedTriangles;

  MeshRepairReport report;
  report.inputTriangles = source.triangleCount();

  for (const Triangle &triangle : source.triangles()) {
    if (triangle.isDegenerate(areaTolerance)) {
      ++report.removedDegenerateTriangles;
      continue;
    }

    if (!retainedTriangles.insert(orientedKey(triangle)).second) {
      ++report.removedExactDuplicateTriangles;
      continue;
    }

    repaired.addTriangle(triangle);
  }

  report.outputTriangles = repaired.triangleCount();
  return {std::move(repaired), report};
}

} // namespace stlfix
