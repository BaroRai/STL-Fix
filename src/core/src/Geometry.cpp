#include "stlfix/Geometry.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace stlfix {

double Vec3::length() const noexcept { return std::sqrt(squaredLength()); }

double Triangle::area() const noexcept {
  return geometricNormal().length() * 0.5;
}

bool Triangle::isDegenerate(double areaTolerance) const {
  if (areaTolerance < 0.0 || !std::isfinite(areaTolerance)) {
    throw std::invalid_argument(
        "Triangle area tolerance must be finite and non-negative");
  }

  return area() <= areaTolerance;
}

void BoundingBox::expand(const Vec3 &point) noexcept {
  if (empty()) {
    minimum_ = point;
    maximum_ = point;
    return;
  }

  minimum_ = Vec3{std::min(minimum_->x(), point.x()),
                  std::min(minimum_->y(), point.y()),
                  std::min(minimum_->z(), point.z())};
  maximum_ = Vec3{std::max(maximum_->x(), point.x()),
                  std::max(maximum_->y(), point.y()),
                  std::max(maximum_->z(), point.z())};
}

} // namespace stlfix
