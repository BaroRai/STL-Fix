#pragma once

#include "stlfix/Mesh.hpp"

#include <filesystem>
#include <istream>
#include <string>
#include <variant>

namespace stlfix {

enum class StlFormat { ascii, binary };

enum class StlReadErrorCode {
  ioFailure,
  emptyInput,
  unknownFormat,
  malformedAscii,
  invalidBinarySize,
  nonFiniteCoordinate
};

struct StlReadError final {
  StlReadErrorCode code;
  std::string message;
};

struct StlReadSuccess final {
  Mesh mesh;
  StlFormat format;
};

using StlReadResult = std::variant<StlReadSuccess, StlReadError>;

class StlReader final {
public:
  [[nodiscard]] StlReadResult read(const std::filesystem::path &path) const;

  [[nodiscard]] StlReadResult read(std::istream &input) const;
};

[[nodiscard]] constexpr const char *toString(StlFormat format) noexcept {
  return format == StlFormat::ascii ? "ASCII STL" : "Binary STL";
}

} // namespace stlfix
