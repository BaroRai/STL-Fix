#include "stlfix/MeshRepair.hpp"
#include "stlfix/StlReader.hpp"
#include "stlfix/StlWriter.hpp"

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <variant>

TEST_CASE("Conservative repair removes exact safe defects") {
  const stlfix::Triangle valid{
      {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
  stlfix::Mesh mesh;
  mesh.addTriangle(valid);
  mesh.addTriangle({valid.vertex(1), valid.vertex(2), valid.vertex(0)});
  mesh.addTriangle({valid.vertex(0), valid.vertex(2), valid.vertex(1)});
  mesh.addTriangle({{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}});

  const auto result = stlfix::MeshRepairer{}.repairConservatively(mesh);

  REQUIRE(result.report.inputTriangles == 4);
  REQUIRE(result.report.removedExactDuplicateTriangles == 1);
  REQUIRE(result.report.removedDegenerateTriangles == 1);
  REQUIRE(result.report.outputTriangles == 2);
  REQUIRE(result.mesh.triangles()[0] == valid);
  REQUIRE(result.mesh.triangles()[1] != valid);
}

TEST_CASE("ASCII writer output can be read again") {
  stlfix::Mesh mesh;
  mesh.addTriangle({{0.0, 0.0, 0.0}, {2.0, 0.0, 0.0}, {0.0, 3.0, 0.0}});

  std::ostringstream output;
  const auto writeResult = stlfix::StlWriter{}.writeAscii(mesh, output);
  REQUIRE(writeResult.success);

  std::istringstream input(output.str());
  const auto readResult = stlfix::StlReader{}.read(input);
  REQUIRE(std::holds_alternative<stlfix::StlReadSuccess>(readResult));
  REQUIRE(std::get<stlfix::StlReadSuccess>(readResult).mesh.triangleCount() ==
          1);
}
