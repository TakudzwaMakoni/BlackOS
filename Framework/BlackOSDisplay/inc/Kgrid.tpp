//
// created by Takudzwa Makoni on 2019-11-22
//

#ifndef BLACKOS_KGRID_TPP
#define BLACKOS_KGRID_TPP

#include "Eigen/Dense"
#include "Kgrid.h"
#include "Kwindow.h"
#include <iomanip>
#include <memory>
#include <ncurses.h>
#include <sstream>
#include <string>

namespace BlackOSDisplay {

// constructor for Kgrid;
template <typename dataType, size_t rows, size_t cols>
Kgrid<dataType, rows, cols>::Kgrid(std::string &name, int sizeY, int sizeX,
                                   int posY, int posX) {
  _name = name;
  _size = {sizeY, sizeX};
  _position = {posY, posX};
}

// overrides

// assign a window object
template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::setWin(WINDOW *window) {
  _win = window;
  wresize(_win, _size[0], _size[1]);
  mvwin(_win, _position[0], _position[1]);
  wrefresh(_win);
}
/// set style of BlackOS Window border
template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::setBorderStyle(const int &ch) {
  _borderStyle = {ch, ch, ch, ch, ch, ch, ch, ch};
}
/// set style of BlackOS Window border
template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::setBorderStyle(const int &L, const int &R,
                                                 const int &T, const int &B,
                                                 const int &TL, const int &TR,
                                                 const int &BL, const int &BR) {
  _borderStyle = {L, R, T, B, TL, TR, BL, BR};
}
/// return window
template <typename dataType, size_t rows, size_t cols>
WINDOW *Kgrid<dataType, rows, cols>::window() const {
  return this->_win;
};
///
template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::label(const std::string &label) const {
  int labellocy = _size[0] - 1;
  int labellocx = _size[1] - (3 + (int)label.length());
  mvwaddstr(_win, labellocy, labellocx, label.c_str());
}
/// return max size of window
template <typename dataType, size_t rows, size_t cols>
std::vector<int> Kgrid<dataType, rows, cols>::maxSize() const {
  int yMax, xMax;
  getmaxyx(_win, yMax, xMax);
  std::vector<int> size{yMax, xMax};
  return size;
}
/// return type of Kwindow
template <typename dataType, size_t rows, size_t cols>
std::string Kgrid<dataType, rows, cols>::winType() const {
  return "Kgrid";
}
/// return name of Kwindow
template <typename dataType, size_t rows, size_t cols>
std::string Kgrid<dataType, rows, cols>::name() const {
  return this->_name;
}
// member functions

/// write Eigen Matrix to class matrix
template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::write(
    Eigen::Matrix<dataType, rows, cols> &data) {
  _matrix = data;
}
/// write data from vector to class matrix
template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::write(std::vector<dataType> &data) {
  if (data.size() != rows * cols) {
    std::string message = "expected vector size: " + std::to_string(rows) +
                          " x " + std::to_string(cols);
    throw std::invalid_argument(message);
  }
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      _matrix(i, j) = data[(i * rows) + j];
    }
  }
}

} // namespace BlackOSDisplay

#endif // BLACKOS_KGRID_TPP
