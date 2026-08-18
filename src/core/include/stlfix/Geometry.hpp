#pragma once

#include <array>
#include <cstddef>
#include <limits>
#include <optional>
#include <span>

namespace stlfix {

class Vec3 final {
public:
  constexpr Vec3() noexcept = default;
  constexpr Vec3(double x, double y, double z) noexcept : x_(x), y_(y), z_(z) {}

  [[nodiscard]] constexpr double x() const noexcept { return x_; }
  [[nodiscard]] constexpr double y() const noexcept { return y_; }
  [[nodiscard]] constexpr double z() const noexcept { return z_; }

  [[nodiscard]] constexpr double squaredLength() const noexcept;
  [[nodiscard]] double length() const noexcept;
  [[nodiscard]] constexpr bool isFinite() const noexcept;

  friend constexpr bool operator==(const Vec3 &,
                                   const Vec3 &) noexcept = default;
  friend constexpr Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) noexcept;
  friend constexpr Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs) noexcept;
  friend constexpr Vec3 operator-(const Vec3 &value) noexcept;
  friend constexpr Vec3 operator*(const Vec3 &value, double scalar) noexcept;
  friend constexpr Vec3 operator*(double scalar, const Vec3 &value) noexcept;
  friend constexpr Vec3 operator/(const Vec3 &value, double scalar) noexcept;

private:
  double x_{};
  double y_{};
  double z_{};
};

[[nodiscard]] constexpr double dot(const Vec3 &lhs, const Vec3 &rhs) noexcept;
[[nodiscard]] constexpr Vec3 cross(const Vec3 &lhs, const Vec3 &rhs) noexcept;

class Triangle final {
public:
  constexpr Triangle(Vec3 first, Vec3 second, Vec3 third) noexcept
      : vertices_{first, second, third} {}

  [[nodiscard]] constexpr const Vec3 &vertex(std::size_t index) const;
  [[nodiscard]] constexpr std::span<const Vec3, 3> vertices() const noexcept;
  [[nodiscard]] constexpr Vec3 geometricNormal() const noexcept;
  [[nodiscard]] double area() const noexcept;
  [[nodiscard]] bool isDegenerate(double areaTolerance = 0.0) const;
  [[nodiscard]] constexpr bool isFinite() const noexcept;

  friend constexpr bool operator==(const Triangle &,
                                   const Triangle &) noexcept = default;

private:
  std::array<Vec3, 3> vertices_;
};

class BoundingBox final {
public:
  constexpr BoundingBox() noexcept = default;

  void expand(const Vec3 &point) noexcept;

  [[nodiscard]] constexpr bool empty() const noexcept {
    return !minimum_.has_value();
  }
  [[nodiscard]] constexpr const std::optional<Vec3> &minimum() const noexcept {
    return minimum_;
  }
  [[nodiscard]] constexpr const std::optional<Vec3> &maximum() const noexcept {
    return maximum_;
  }

private:
  std::optional<Vec3> minimum_;
  std::optional<Vec3> maximum_;
};

constexpr double Vec3::squaredLength() const noexcept {
  return x_ * x_ + y_ * y_ + z_ * z_;
}

constexpr bool Vec3::isFinite() const noexcept {
  // A finite value is neither NaN nor positive/negative infinity. This form is
  // constexpr in C++20, unlike std::isfinite on the supported toolchains.
  constexpr double maximum = std::numeric_limits<double>::max();
  return x_ == x_ && y_ == y_ && z_ == z_ && x_ <= maximum && x_ >= -maximum &&
         y_ <= maximum && y_ >= -maximum && z_ <= maximum && z_ >= -maximum;
}

constexpr Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) noexcept {
  return {lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_};
}

constexpr Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs) noexcept {
  return {lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_};
}

constexpr Vec3 operator-(const Vec3 &value) noexcept {
  return {-value.x_, -value.y_, -value.z_};
}

constexpr Vec3 operator*(const Vec3 &value, double scalar) noexcept {
  return {value.x_ * scalar, value.y_ * scalar, value.z_ * scalar};
}

constexpr Vec3 operator*(double scalar, const Vec3 &value) noexcept {
  return value * scalar;
}

constexpr Vec3 operator/(const Vec3 &value, double scalar) noexcept {
  return {value.x_ / scalar, value.y_ / scalar, value.z_ / scalar};
}

constexpr double dot(const Vec3 &lhs, const Vec3 &rhs) noexcept {
  return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

constexpr Vec3 cross(const Vec3 &lhs, const Vec3 &rhs) noexcept {
  return {lhs.y() * rhs.z() - lhs.z() * rhs.y(),
          lhs.z() * rhs.x() - lhs.x() * rhs.z(),
          lhs.x() * rhs.y() - lhs.y() * rhs.x()};
}

constexpr const Vec3 &Triangle::vertex(std::size_t index) const {
  return vertices_.at(index);
}

constexpr std::span<const Vec3, 3> Triangle::vertices() const noexcept {
  return vertices_;
}

constexpr Vec3 Triangle::geometricNormal() const noexcept {
  return cross(vertices_[1] - vertices_[0], vertices_[2] - vertices_[0]);
}

constexpr bool Triangle::isFinite() const noexcept {
  return vertices_[0].isFinite() && vertices_[1].isFinite() &&
         vertices_[2].isFinite();
}

} // namespace stlfix
