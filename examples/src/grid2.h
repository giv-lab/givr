#pragma once

#include "glm/glm.hpp"
#include <vector>

namespace geometry {

class Grid2 {
public:
  using point = float;
  using points = std::vector<point>;

public:
  Grid2() = default;
  Grid2(int width, int height);

  int width() const;
  int height() const;
  int size() const;

  point const &operator()(int x, int y) const;
  point const &operator[](int index) const;
  int index(int x, int y) const;

  point &operator()(int x, int y);
  point &operator[](int index);

private:
  point &at(int x, int y);
  point &at(int index);

  point const &at(int x, int y) const;
  point const &at(int index) const;

private:
  int m_width = 0;
  int m_height = 0;
  points m_points;
};

// free functions
int indexOf_RowMajor(int x, int y, int width);

} // namespace geometry
