#include "stlfix/Mesh.hpp"

#include <algorithm>
#include <utility>

namespace stlfix {

void Mesh::reserve(std::size_t triangleCount) {
  triangles_.reserve(triangleCount);
}

void Mesh::addTriangle(Triangle triangle) {
  triangles_.push_back(std::move(triangle));
}

bool Mesh::empty() const noexcept { return triangles_.empty(); }

std::size_t Mesh::triangleCount() const noexcept { return triangles_.size(); }

std::span<const Triangle> Mesh::triangles() const noexcept {
  return triangles_;
}

BoundingBox Mesh::boundingBox() const noexcept {
  BoundingBox result;
  std::ranges::for_each(triangles_, [&result](const Triangle &triangle) {
    std::ranges::for_each(triangle.vertices(), [&result](const Vec3 &vertex) {
      result.expand(vertex);
    });
  });
  return result;
}

bool Mesh::isFinite() const noexcept {
  return std::ranges::all_of(
      triangles_, [](const Triangle &triangle) { return triangle.isFinite(); });
}

} // namespace stlfix
