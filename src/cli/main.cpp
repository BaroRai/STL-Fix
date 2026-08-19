#include "stlfix/MeshRepair.hpp"
#include "stlfix/StlReader.hpp"
#include "stlfix/StlWriter.hpp"

#include <filesystem>
#include <iostream>
#include <string_view>
#include <variant>

namespace {

void printUsage(const char *executable) {
  std::cout << "STLFix 0.1\n\n"
            << "Usage:\n"
            << "  " << executable << " analyze <model.stl>\n"
            << "  " << executable << " repair <input.stl> <output.stl>\n";
}

} // namespace

int main(int argc, char **argv) {
  const bool analyze = argc == 3 && std::string_view(argv[1]) == "analyze";
  const bool repair = argc == 4 && std::string_view(argv[1]) == "repair";
  if (!analyze && !repair) {
    printUsage(argv[0]);
    return argc == 1 ? 0 : 2;
  }

  const std::filesystem::path path(argv[2]);
  stlfix::StlReader reader;
  auto result = reader.read(path);
  if (const auto *failure = std::get_if<stlfix::StlReadError>(&result)) {
    std::cerr << "Error: " << failure->message << '\n';
    return 1;
  }

  const auto &success = std::get<stlfix::StlReadSuccess>(result);
  if (repair) {
    const auto repaired =
        stlfix::MeshRepairer{}.repairConservatively(success.mesh);
    const auto writeResult =
        stlfix::StlWriter{}.writeAscii(repaired.mesh, argv[3]);
    if (!writeResult.success) {
      std::cerr << "Error: " << writeResult.message << '\n';
      return 1;
    }

    std::cout << "Input: " << path.string() << '\n'
              << "Output: " << argv[3] << '\n'
              << "Input triangles: " << repaired.report.inputTriangles << '\n'
              << "Removed degenerate triangles: "
              << repaired.report.removedDegenerateTriangles << '\n'
              << "Removed exact duplicate triangles: "
              << repaired.report.removedExactDuplicateTriangles << '\n'
              << "Output triangles: " << repaired.report.outputTriangles
              << '\n';
    return 0;
  }

  const auto bounds = success.mesh.boundingBox();
  std::cout << "File: " << path.string() << '\n'
            << "Format: " << stlfix::toString(success.format) << '\n'
            << "Triangles: " << success.mesh.triangleCount() << '\n';

  if (!bounds.empty()) {
    const auto &minimum = *bounds.minimum();
    const auto &maximum = *bounds.maximum();
    std::cout << "Bounding Box:\n"
              << "  X: " << minimum.x() << " .. " << maximum.x() << '\n'
              << "  Y: " << minimum.y() << " .. " << maximum.y() << '\n'
              << "  Z: " << minimum.z() << " .. " << maximum.z() << '\n';
  }

  return 0;
}
