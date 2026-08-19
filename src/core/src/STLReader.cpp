#include "stlfix/StlReader.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>

namespace stlfix {
namespace {

using Bytes = std::vector<std::byte>;

constexpr std::size_t binaryHeaderSize = 80;
constexpr std::size_t binaryPrefixSize = 84;
constexpr std::size_t binaryTriangleSize = 50;

[[nodiscard]] StlReadError error(StlReadErrorCode code, std::string message) {
  return {code, std::move(message)};
}

[[nodiscard]] std::uint32_t readUint32(const std::byte *data) noexcept {
  return std::to_integer<std::uint32_t>(data[0]) |
         (std::to_integer<std::uint32_t>(data[1]) << 8U) |
         (std::to_integer<std::uint32_t>(data[2]) << 16U) |
         (std::to_integer<std::uint32_t>(data[3]) << 24U);
}

[[nodiscard]] float readFloat(const std::byte *data) noexcept {
  return std::bit_cast<float>(readUint32(data));
}

[[nodiscard]] bool hasBinarySize(const Bytes &bytes) noexcept {
  if (bytes.size() < binaryPrefixSize) {
    return false;
  }

  const auto triangleCount = readUint32(bytes.data() + binaryHeaderSize);
  if (triangleCount >
      (std::numeric_limits<std::size_t>::max() - binaryPrefixSize) /
          binaryTriangleSize) {
    return false;
  }

  return bytes.size() ==
         binaryPrefixSize +
             static_cast<std::size_t>(triangleCount) * binaryTriangleSize;
}

[[nodiscard]] StlReadResult readBinary(const Bytes &bytes) {
  if (!hasBinarySize(bytes)) {
    return error(StlReadErrorCode::invalidBinarySize,
                 "Binary STL size does not match its triangle count");
  }

  const auto triangleCount = readUint32(bytes.data() + binaryHeaderSize);
  Mesh mesh;
  mesh.reserve(triangleCount);

  for (std::uint32_t index = 0; index < triangleCount; ++index) {
    const auto offset =
        binaryPrefixSize + static_cast<std::size_t>(index) * binaryTriangleSize;
    const auto *record = bytes.data() + offset;

    // The first 12 bytes contain the exported normal. Geometry-derived normals
    // remain authoritative, so the reader currently skips that advisory value.
    const auto readVertex = [record](std::size_t vertexIndex) {
      const auto base = 12 + vertexIndex * 12;
      return Vec3{static_cast<double>(readFloat(record + base)),
                  static_cast<double>(readFloat(record + base + 4)),
                  static_cast<double>(readFloat(record + base + 8))};
    };

    Triangle triangle{readVertex(0), readVertex(1), readVertex(2)};
    if (!triangle.isFinite()) {
      return error(StlReadErrorCode::nonFiniteCoordinate,
                   "Binary STL triangle " + std::to_string(index) +
                       " contains a non-finite coordinate");
    }
    mesh.addTriangle(std::move(triangle));
  }

  return StlReadSuccess{std::move(mesh), StlFormat::binary};
}

[[nodiscard]] bool readExpected(std::istream &input, std::string_view expected,
                                std::string &actual) {
  return static_cast<bool>(input >> actual) && actual == expected;
}

[[nodiscard]] bool readVec3(std::istream &input, Vec3 &result) {
  double x{};
  double y{};
  double z{};
  if (!(input >> x >> y >> z)) {
    return false;
  }
  result = Vec3{x, y, z};
  return true;
}

[[nodiscard]] StlReadResult readAscii(const Bytes &bytes) {
  const std::string text(reinterpret_cast<const char *>(bytes.data()),
                         bytes.size());
  std::istringstream input(text);
  std::string token;

  if (!readExpected(input, "solid", token)) {
    return error(StlReadErrorCode::malformedAscii,
                 "ASCII STL must begin with 'solid'");
  }

  Mesh mesh;
  bool parsedFacet = false;
  while (input >> token) {
    if (token == "endsolid") {
      return StlReadSuccess{std::move(mesh), StlFormat::ascii};
    }

    // Tokens before the first facet form the optional solid name.
    if (token != "facet" && !parsedFacet) {
      continue;
    }
    if (token != "facet") {
      return error(StlReadErrorCode::malformedAscii,
                   "Expected 'facet' or 'endsolid' in ASCII STL");
    }

    Vec3 exportedNormal;
    if (!readExpected(input, "normal", token) ||
        !readVec3(input, exportedNormal) ||
        !readExpected(input, "outer", token) ||
        !readExpected(input, "loop", token)) {
      return error(StlReadErrorCode::malformedAscii,
                   "Malformed ASCII STL facet header");
    }

    std::array<Vec3, 3> vertices;
    for (auto &vertex : vertices) {
      if (!readExpected(input, "vertex", token) || !readVec3(input, vertex)) {
        return error(StlReadErrorCode::malformedAscii,
                     "Malformed ASCII STL vertex");
      }
    }

    if (!readExpected(input, "endloop", token) ||
        !readExpected(input, "endfacet", token)) {
      return error(StlReadErrorCode::malformedAscii,
                   "Malformed ASCII STL facet terminator");
    }

    Triangle triangle{vertices[0], vertices[1], vertices[2]};
    if (!exportedNormal.isFinite() || !triangle.isFinite()) {
      return error(StlReadErrorCode::nonFiniteCoordinate,
                   "ASCII STL facet contains a non-finite number");
    }
    mesh.addTriangle(std::move(triangle));
    parsedFacet = true;
  }

  return error(StlReadErrorCode::malformedAscii,
               "ASCII STL is missing 'endsolid'");
}

[[nodiscard]] bool beginsWithSolid(const Bytes &bytes) {
  const auto first =
      std::find_if_not(bytes.begin(), bytes.end(), [](std::byte value) {
        const auto character =
            static_cast<unsigned char>(std::to_integer<unsigned int>(value));
        return std::isspace(character) != 0;
      });
  constexpr std::string_view prefix = "solid";
  return static_cast<std::size_t>(bytes.end() - first) >= prefix.size() &&
         std::memcmp(&*first, prefix.data(), prefix.size()) == 0;
}

} // namespace

StlReadResult StlReader::read(const std::filesystem::path &path) const {
  std::ifstream input(path, std::ios::binary);
  if (!input) {
    return error(StlReadErrorCode::ioFailure,
                 "Unable to open STL file: " + path.string());
  }
  return read(input);
}

StlReadResult StlReader::read(std::istream &input) const {
  const std::vector<char> characters((std::istreambuf_iterator<char>(input)),
                                     std::istreambuf_iterator<char>());
  if (input.bad()) {
    return error(StlReadErrorCode::ioFailure, "Failed while reading STL input");
  }
  if (characters.empty()) {
    return error(StlReadErrorCode::emptyInput, "STL input is empty");
  }

  Bytes bytes(characters.size());
  std::memcpy(bytes.data(), characters.data(), characters.size());

  if (hasBinarySize(bytes)) {
    return readBinary(bytes);
  }
  if (beginsWithSolid(bytes)) {
    return readAscii(bytes);
  }
  if (bytes.size() >= binaryPrefixSize) {
    return error(StlReadErrorCode::invalidBinarySize,
                 "Binary STL size does not match its triangle count");
  }
  return error(StlReadErrorCode::unknownFormat,
               "Input is neither a recognized ASCII nor binary STL");
}

} // namespace stlfix
