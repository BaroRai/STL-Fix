#pragma once

#include "stlfix/Geometry.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace stlfix {

class Mesh final {
public:
  using Container = std::vector<Triangle>;

  void reserve(std::size_t triangleCount);
  void addTriangle(Triangle triangle);

  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] std::size_t triangleCount() const noexcept;
  [[nodiscard]] std::span<const Triangle> triangles() const noexcept;
  [[nodiscard]] BoundingBox boundingBox() const noexcept;
  [[nodiscard]] bool isFinite() const noexcept;

private:
  Container triangles_;
};

} // namespace stlfix
