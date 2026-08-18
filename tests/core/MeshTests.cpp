#include "stlfix/Mesh.hpp"

#include <catch2/catch_test_macros.hpp>

#include <limits>
#include <type_traits>

TEST_CASE("Mesh owns triangles and exposes read-only traversal") {
  stlfix::Mesh mesh;
  mesh.reserve(2);
  mesh.addTriangle({{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}});
  mesh.addTriangle({{-2.0, 0.0, 1.0}, {0.0, 4.0, 0.0}, {0.0, 0.0, 3.0}});

  STATIC_REQUIRE(std::is_const_v<
                 std::remove_reference_t<decltype(mesh.triangles().front())>>);
  REQUIRE_FALSE(mesh.empty());
  REQUIRE(mesh.triangleCount() == 2);
  REQUIRE(mesh.triangles().size() == 2);

  const auto bounds = mesh.boundingBox();
  REQUIRE((bounds.minimum() == stlfix::Vec3{-2.0, 0.0, 0.0}));
  REQUIRE((bounds.maximum() == stlfix::Vec3{1.0, 4.0, 3.0}));
  REQUIRE(mesh.isFinite());
}

TEST_CASE("Empty mesh has empty bounds") {
  const stlfix::Mesh mesh;
  REQUIRE(mesh.empty());
  REQUIRE(mesh.boundingBox().empty());
  REQUIRE(mesh.isFinite());
}

TEST_CASE("Mesh reports a non-finite triangle") {
  stlfix::Mesh mesh;
  mesh.addTriangle({{0.0, 0.0, 0.0},
                    {1.0, 0.0, 0.0},
                    {0.0, std::numeric_limits<double>::quiet_NaN(), 0.0}});

  REQUIRE_FALSE(mesh.isFinite());
}
