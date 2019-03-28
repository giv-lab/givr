#include "grid2.h"

namespace geometry {

using point = Grid2::point;

Grid2::Grid2(int width, int height) : m_width(width), m_height(height) {
  m_points = points(size());
}

int Grid2::width() const { return m_width; }

int Grid2::height() const { return m_height; }

int Grid2::size() const { return width() * height(); }

point const &Grid2::operator()(int x, int y) const { return at(x, y); }

int Grid2::index(int x, int y) const { return indexOf_RowMajor(x, y, width()); }

point &Grid2::operator()(int x, int y) {
  // should do some sort of error checking
  return at(x, y);
}

point &Grid2::operator[](int index) {
  // should do some sort of error checking
  return at(index);
}

// private
point &Grid2::at(int x, int y) { return m_points[index(x, y)]; }

point &Grid2::at(int index) { return m_points[index]; }

point const &Grid2::at(int x, int y) const { return at(x, y); }

point const &Grid2::at(int index) const { return at(index); }

// free functions

int indexOf_RowMajor(int x, int y, int width) { return width * y + x; }

} // namespace geometry
