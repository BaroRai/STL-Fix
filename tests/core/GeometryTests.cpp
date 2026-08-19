#include "stlfix/Geometry.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <limits>
#include <stdexcept>

TEST_CASE("Vec3 supports deterministic vector operations") {
  constexpr stlfix::Vec3 first{1.0, 2.0, 3.0};
  constexpr stlfix::Vec3 second{-4.0, 5.0, 2.0};

  STATIC_REQUIRE((first + second == stlfix::Vec3{-3.0, 7.0, 5.0}));
  STATIC_REQUIRE((first - second == stlfix::Vec3{5.0, -3.0, 1.0}));
  STATIC_REQUIRE((2.0 * first == stlfix::Vec3{2.0, 4.0, 6.0}));
  STATIC_REQUIRE(stlfix::dot(first, second) == 12.0);
  STATIC_REQUIRE(
      (stlfix::cross(first, second) == stlfix::Vec3{-11.0, -14.0, 13.0}));
  REQUIRE(first.length() == Catch::Approx(std::sqrt(14.0)));
}

TEST_CASE("Vec3 identifies non-finite coordinates") {
  REQUIRE(stlfix::Vec3{}.isFinite());
  REQUIRE_FALSE(stlfix::Vec3{std::numeric_limits<double>::infinity(), 0.0, 0.0}
                    .isFinite());
  REQUIRE_FALSE(stlfix::Vec3{0.0, std::numeric_limits<double>::quiet_NaN(), 0.0}
                    .isFinite());
}

TEST_CASE("Triangle calculates geometry and validates explicit tolerance") {
  const stlfix::Triangle triangle{
      {0.0, 0.0, 0.0}, {2.0, 0.0, 0.0}, {0.0, 3.0, 0.0}};

  REQUIRE((triangle.geometricNormal() == stlfix::Vec3{0.0, 0.0, 6.0}));
  REQUIRE(triangle.area() == 3.0);
  REQUIRE_FALSE(triangle.isDegenerate());
  REQUIRE(triangle.isDegenerate(3.0));
  REQUIRE_THROWS_AS(triangle.isDegenerate(-1.0), std::invalid_argument);
}

TEST_CASE("BoundingBox represents empty and expanded bounds") {
  stlfix::BoundingBox box;
  REQUIRE(box.empty());
  REQUIRE_FALSE(box.minimum().has_value());
  REQUIRE_FALSE(box.maximum().has_value());

  box.expand({3.0, -2.0, 5.0});
  box.expand({-1.0, 4.0, 2.0});

  REQUIRE_FALSE(box.empty());
  REQUIRE((box.minimum() == stlfix::Vec3{-1.0, -2.0, 2.0}));
  REQUIRE((box.maximum() == stlfix::Vec3{3.0, 4.0, 5.0}));
}
