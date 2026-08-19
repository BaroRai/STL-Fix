#include "stlfix/MeshRepair.hpp"
#include "stlfix/StlReader.hpp"
#include "stlfix/StlWriter.hpp"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <sstream>
#include <variant>

TEST_CASE("Synthetic damaged cube is repaired deterministically") {
  const auto fixture = std::filesystem::path(STLFIX_TEST_FIXTURE_DIR) /
                       "synthetic" / "damaged_cube.stl";
  const auto readResult = stlfix::StlReader{}.read(fixture);
  REQUIRE(std::holds_alternative<stlfix::StlReadSuccess>(readResult));

  const auto &input = std::get<stlfix::StlReadSuccess>(readResult);
  REQUIRE(input.format == stlfix::StlFormat::ascii);
  REQUIRE(input.mesh.triangleCount() == 14);
  REQUIRE((input.mesh.boundingBox().minimum() == stlfix::Vec3{0.0, 0.0, 0.0}));
  REQUIRE(
      (input.mesh.boundingBox().maximum() == stlfix::Vec3{10.0, 10.0, 10.0}));

  const auto repaired = stlfix::MeshRepairer{}.repairConservatively(input.mesh);
  REQUIRE(repaired.report.inputTriangles == 14);
  REQUIRE(repaired.report.removedDegenerateTriangles == 1);
  REQUIRE(repaired.report.removedExactDuplicateTriangles == 1);
  REQUIRE(repaired.report.outputTriangles == 12);

  std::ostringstream output;
  REQUIRE(stlfix::StlWriter{}.writeAscii(repaired.mesh, output).success);

  std::istringstream roundTripInput(output.str());
  const auto roundTrip = stlfix::StlReader{}.read(roundTripInput);
  REQUIRE(std::holds_alternative<stlfix::StlReadSuccess>(roundTrip));
  REQUIRE(std::get<stlfix::StlReadSuccess>(roundTrip).mesh.triangleCount() ==
          12);
}
