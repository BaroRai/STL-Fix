#pragma once

#include "stlfix/Mesh.hpp"

#include <filesystem>
#include <ostream>
#include <string>

namespace stlfix {

struct StlWriteResult final {
  bool success;
  std::string message;
};

class StlWriter final {
public:
  [[nodiscard]] StlWriteResult
  writeAscii(const Mesh &mesh, const std::filesystem::path &path) const;

  [[nodiscard]] StlWriteResult writeAscii(const Mesh &mesh,
                                          std::ostream &output) const;
};

} // namespace stlfix
