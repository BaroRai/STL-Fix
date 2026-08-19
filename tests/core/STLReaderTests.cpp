#include "stlfix/StlReader.hpp"

#include <catch2/catch_test_macros.hpp>

#include <bit>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <variant>

namespace {

void appendUint32(std::string &data, std::uint32_t value) {
  for (unsigned shift = 0; shift < 32; shift += 8) {
    data.push_back(static_cast<char>((value >> shift) & 0xffU));
  }
}

void appendFloat(std::string &data, float value) {
  appendUint32(data, std::bit_cast<std::uint32_t>(value));
}

std::string oneTriangleBinaryStl() {
  std::string data(80, '\0');
  appendUint32(data, 1);
  for (const float value : {0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F,
                            0.0F, 0.0F, 1.0F, 0.0F}) {
    appendFloat(data, value);
  }
  data.push_back('\0');
  data.push_back('\0');
  return data;
}

} // namespace

TEST_CASE("StlReader reads ASCII STL") {
  std::istringstream input(R"(solid triangle
facet normal 0 0 1
  outer loop
    vertex 0 0 0
    vertex 1 0 0
    vertex 0 1 0
  endloop
endfacet
endsolid triangle
)");

  const auto result = stlfix::StlReader{}.read(input);
  REQUIRE(std::holds_alternative<stlfix::StlReadSuccess>(result));
  const auto &success = std::get<stlfix::StlReadSuccess>(result);
  REQUIRE(success.format == stlfix::StlFormat::ascii);
  REQUIRE(success.mesh.triangleCount() == 1);
}

TEST_CASE("StlReader reads binary STL even when its header begins with solid") {
  auto data = oneTriangleBinaryStl();
  std::memcpy(data.data(), "solid binary", 12);
  std::istringstream input(data);

  const auto result = stlfix::StlReader{}.read(input);
  REQUIRE(std::holds_alternative<stlfix::StlReadSuccess>(result));
  const auto &success = std::get<stlfix::StlReadSuccess>(result);
  REQUIRE(success.format == stlfix::StlFormat::binary);
  REQUIRE(success.mesh.triangleCount() == 1);
}

TEST_CASE("StlReader reports malformed and empty inputs") {
  std::istringstream empty;
  const auto emptyResult = stlfix::StlReader{}.read(empty);
  REQUIRE(std::get<stlfix::StlReadError>(emptyResult).code ==
          stlfix::StlReadErrorCode::emptyInput);

  std::istringstream malformed("solid broken facet normal 0 0 1");
  const auto malformedResult = stlfix::StlReader{}.read(malformed);
  REQUIRE(std::get<stlfix::StlReadError>(malformedResult).code ==
          stlfix::StlReadErrorCode::malformedAscii);
}
