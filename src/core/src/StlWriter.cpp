#include "stlfix/StlWriter.hpp"

#include <fstream>
#include <iomanip>

namespace stlfix {

StlWriteResult StlWriter::writeAscii(const Mesh &mesh,
                                     const std::filesystem::path &path) const {
  std::ofstream output(path, std::ios::binary | std::ios::trunc);
  if (!output) {
    return {false, "Unable to open output STL file: " + path.string()};
  }
  return writeAscii(mesh, output);
}

StlWriteResult StlWriter::writeAscii(const Mesh &mesh,
                                     std::ostream &output) const {
  if (!mesh.isFinite()) {
    return {false, "Cannot write a mesh containing non-finite coordinates"};
  }

  output << std::setprecision(17) << "solid stlfix\n";
  for (const Triangle &triangle : mesh.triangles()) {
    const Vec3 geometricNormal = triangle.geometricNormal();
    const double normalLength = geometricNormal.length();
    const Vec3 normal =
        normalLength == 0.0 ? Vec3{} : geometricNormal / normalLength;
    output << "  facet normal " << normal.x() << ' ' << normal.y() << ' '
           << normal.z() << "\n"
           << "    outer loop\n";
    for (const Vec3 &vertex : triangle.vertices()) {
      output << "      vertex " << vertex.x() << ' ' << vertex.y() << ' '
             << vertex.z() << "\n";
    }
    output << "    endloop\n"
           << "  endfacet\n";
  }
  output << "endsolid stlfix\n";

  if (!output) {
    return {false, "Failed while writing ASCII STL output"};
  }
  return {true, {}};
}

} // namespace stlfix
