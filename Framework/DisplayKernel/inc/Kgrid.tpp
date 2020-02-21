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
#include <numeric>
#include <sstream>
#include <string>

namespace {
std::vector<size_t> blocksFound(size_t const yValue, size_t const numOfBlocks,
                                std::vector<size_t> const &elements) {
  std::vector<size_t> iteratorList(numOfBlocks);
  std::iota(iteratorList.begin(), iteratorList.end(), 0);
  std::vector<size_t> _linesUnclear;
  for (size_t const block : iteratorList) {
    size_t y1 = elements[0 + (block * 4)];
    size_t y2 = elements[2 + (block * 4)];
    if (y1 <= yValue && yValue <= y2)
      _linesUnclear.push_back(block);
  }
  return _linesUnclear;
}
bool inBlocks(size_t const xValue, std::vector<size_t> const &blocks,
              std::vector<size_t> const &elements) {
  for (size_t const block : blocks) {
    size_t x1 = elements[1 + (block * 4)];
    size_t x2 = elements[3 + (block * 4)];
    if (x1 <= xValue && xValue <= x2)
      return true;
  }
  return false;
}
} // namespace

namespace BlackOS {
namespace DisplayKernel {
template <typename dataType, size_t rows, size_t cols>
Kgrid<dataType, rows, cols>::Kgrid(std::string const &name, size_t const sizeY,
                                   size_t const sizeX, size_t const posY,
                                   size_t const posX) {
  _name = name;
  _size = {sizeY, sizeX};
  _position = {posY, posX};
}

template <typename dataType, size_t const rows, size_t const cols>
void Kgrid<dataType, rows, cols>::setWin(WINDOW *window) {
  _win = window;
  wresize(_win, _size[0], _size[1]);
  mvwin(_win, _position[0], _position[1]);
  wrefresh(_win);
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::borderStyle(size_t const ch) {
  wborder(_win, ch, ch, ch, ch, ch, ch, ch, ch);
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::borderStyle(size_t const L, size_t const R,
                                              size_t const T, size_t const B,
                                              size_t const TL, size_t const TR,
                                              size_t const BL,
                                              size_t const BR) {
  wborder(_win, L, R, T, B, TL, TR, BL, BR);
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::label(std::string const &label) const {
  size_t labellocy = _size[0] - 1;
  size_t labellocx = _size[1] - (3 + (size_t)label.length());
  mvwaddstr(_win, labellocy, labellocx, label.c_str());
}

template <typename dataType, size_t rows, size_t cols>
std::vector<size_t> Kgrid<dataType, rows, cols>::maxSize() const {
  size_t yMax, xMax;
  getmaxyx(_win, yMax, xMax);
  std::vector<size_t> size{yMax, xMax};
  return size;
}

template <typename dataType, size_t rows, size_t cols>
std::string Kgrid<dataType, rows, cols>::winType() const {
  return "Kgrid";
}

template <typename dataType, size_t rows, size_t cols>
std::string Kgrid<dataType, rows, cols>::name() const {
  return this->_name;
}

template <typename dataType, size_t rows, size_t cols>
dataType Kgrid<dataType, rows, cols>::selectedRaw() const {
  return this->_matrix(_highlightedRow, _highlightedCol);
}

template <typename dataType, size_t rows, size_t cols>
std::vector<size_t> Kgrid<dataType, rows, cols>::selectedIndices() const {
  return std::vector<size_t>{_highlightedRow, _highlightedCol};
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::setTitle(std::string const &title) {
  _title = title;
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::write(
    Eigen::Matrix<dataType, rows, cols> const &data) {
  _matrix = data;
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::refresh() {
  wrefresh(_win);
}
template <typename dataType, size_t rows, size_t cols>
size_t
Kgrid<dataType, rows, cols>::getChrfromW(size_t const y, size_t const x,
                                         bool const preserve_cursor_pos) const {
  size_t curr_y, curr_x;
  getyx(_win, curr_y, curr_x);
  size_t wch = mvwinch(_win, y, x);
  if (preserve_cursor_pos)
    wmove(_win, curr_y, curr_x);
  return wch;
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::write(std::vector<dataType> const &data) {
  if (data.size() != rows * cols) {
    std::string message = "expected vector size: " + std::to_string(rows) +
                          " x " + std::to_string(cols);
    throw std::invalid_argument(message);
  }
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      _matrix(i, j) = data[(i * rows) + j];
    }
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::erase(size_t const y1, size_t const x1,
                                        size_t const y2, size_t const x2) {
  size_t a, b, c, d;
  size_t borderY = _size[0];
  size_t borderX = _size[1];
  a = y1 == 0 ? 1 : y1;
  b = x1 == 0 ? 1 : x1;
  c = y2 == borderY ? y2 - 1 : y2;
  d = x2 == borderX ? x2 - 1 : x2;
  size_t width = d - b + 1;
  std::string fill(width, ' ');
  for (size_t i = a; i <= c; ++i) {
    mvwprintw(_win, i, b, fill.c_str());
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::eraseExcept(size_t const y1, size_t const x1,
                                              size_t const y2,
                                              size_t const x2) {
  size_t borderY = _size[0] - 2;
  size_t borderX = _size[1] - 2;
  std::string fill(borderX, ' ');
  std::string space = " ";
  for (size_t i = 1; i <= borderY; ++i) {
    if (i < y1 || i > y2) {
      mvwprintw(_win, i, 1, fill.c_str());
    } else {
      for (size_t j = 1; j <= borderX; ++j) {
        if ((j > x2 || j < x1)) {
          mvwprintw(_win, i, j, space.c_str());
        }
      }
    }
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::fill(char const ch, bool const titleBar) {
  size_t start = titleBar ? 2 : 1;
  size_t end = _size[0] - 2;

  std::string fillString(_size[1] - 2, ch);
  for (size_t i = start; i <= end; ++i) {
    mvwprintw(_win, i, 1, fillString.c_str());
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::erase(std::vector<size_t> const &elements) {
  size_t numOfAreas = elements.size() / 4; /*two coordinates per block*/
  for (size_t areaIdx = 0; areaIdx < numOfAreas; ++areaIdx) {
    size_t y1, x1, y2, x2;
    y1 = elements[0 + (areaIdx * 4)];
    x1 = elements[1 + (areaIdx * 4)];
    y2 = elements[2 + (areaIdx * 4)];
    x2 = elements[3 + (areaIdx * 4)];
    erase(y1, x1, y2, x2);
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::eraseExcept(
    std::vector<size_t> const &elements) {
  size_t numOfBlocks = elements.size() / 4; /*two coordinates per block*/
  size_t borderY = _size[0];
  size_t borderX = _size[1];
  size_t width = borderX - 2;
  size_t height = borderY - 2;
  std::string fill(width, ' ');
  std::string space = " ";
  for (size_t i = 1; i <= height; ++i) {
    std::vector<size_t> blocks = blocksFound(i, numOfBlocks, elements);
    if (blocks.empty()) {
      mvwprintw(_win, i, 1, fill.c_str());
    } else {
      for (size_t j = 1; j <= width; ++j) {
        bool _inBlocks = inBlocks(j, blocks, elements);
        if (!_inBlocks) {
          mvwprintw(_win, i, j, space.c_str());
        }
      }
    }
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::display() {
  keypad(_win, true);
  size_t selection;
  size_t highlightedRow = 0;
  size_t highlightedCol = 0;
  size_t topPad = 1;
  setBorderStyle();
  if (_showTitle) {
    _attributes = {"RAD", "PREC: " + std::to_string(_precision)};
    wattron(_win, A_REVERSE);
    std::string tPadding(_size[1], ' ');
    size_t correction = _size[1] - _title.length() - 3;
    std::string titleString = attributeString();
    size_t titleStrLength = titleString.size();
    tPadding.replace(0, titleStrLength, titleString);
    tPadding.replace(correction, _size[1], _title + " ");
    mvwprintw(_win, _position[0], _position[1], tPadding.c_str());
    wattroff(_win, A_REVERSE);
  }
  size_t displayPrecision = 5;
  while (true) {
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        if (j == highlightedCol && i == highlightedRow) {
          wattron(_win, A_REVERSE);
        }
        auto element = _matrix(highlightedRow, highlightedCol);
        std::stringstream elementStream;
        elementStream << std::setprecision(_precision) << element;
        const std::string elementStr = elementStream.str();
        size_t const elementStrSize = elementStr.size();
        std::string elementDisplay(_size[1] / 2, ' ');
        elementDisplay.replace(0, elementStrSize, elementStr);
        std::stringstream numStream;
        std::string str(8, ' ');
        size_t strSize = str.size();
        if (_matrix.coeff(i, j) > 99.99) {
          numStream << std::scientific << std::setprecision(2)
                    << _matrix.coeff(i, j);
        } else {
          numStream << std::setprecision(displayPrecision) << std::fixed
                    << _matrix.coeff(i, j);
        }
        std::string numStr = numStream.str();
        size_t numStrSize = numStr.size();
        size_t correction = strSize - numStrSize;
        str.replace(correction, numStrSize, numStr);
        int yAlign = 0;
        int xAlign = 0;
        size_t left, right, top, bottom, v_centre, h_centre;
        left = 2;
        size_t hPadding{4};
        if (_setGrid) {
          hPadding = 6;
          str = "|" + str + "|";
        }
        size_t gridWidth = (cols * 8) + (cols - 2);
        size_t gridHeight = rows * _vPadding; // avoid collision with status bar
        size_t bottomPad = -gridHeight + i;
        size_t rightPad = gridWidth - 1;
        size_t hCentrePad = gridWidth;
        size_t vCentrPad = gridHeight;
        right = _size[1];
        v_centre = i + (_size[0] / 2) - vCentrPad;
        h_centre = (_size[1] - hCentrePad) / 2;
        top = i + topPad;
        bottom = _size[0] - 2;
        if (_xAlign == 1)
          xAlign = right + rightPad;
        else if (_xAlign == 0)
          xAlign = h_centre;
        else if (_xAlign == -1)
          xAlign = left;
        if (_yAlign == 1)
          yAlign = top;
        else if (_yAlign == 0)
          yAlign = v_centre;
        else if (_yAlign == -1)
          yAlign = bottom + bottomPad;
        mvwprintw(_win, yAlign + i * _vPadding,
                  xAlign + (displayPrecision + hPadding) * j, (str).c_str());
        wattroff(_win, A_REVERSE);
        mvwprintw(_win, bottom, left, elementDisplay.c_str());
        wrefresh(_win);
      }
    }
    selection = wgetch(_win);
    switch (selection) {
    case KEY_UP:
      highlightedRow--;
      if (highlightedRow == -1) {
        highlightedRow = 0;
      }
      break;
    case KEY_DOWN:
      highlightedRow++;
      if (highlightedRow == rows) {
        highlightedRow = rows - 1;
      }
      break;
    case KEY_LEFT:
      highlightedCol--;
      if (highlightedCol == -1) {
        highlightedCol = 0;
      }
      break;
    case KEY_RIGHT:
      highlightedCol++;
      if (highlightedCol == cols) {
        highlightedCol = cols - 1;
      }
      break;
    default:
      break;
    }
    if (selection == 10) {
      break;
    }
  }
  _highlightedRow = highlightedRow;
  _highlightedCol = highlightedCol;
  wrefresh(_win);
}

template <typename dataType, size_t rows, size_t cols>
std::string Kgrid<dataType, rows, cols>::attributeString() {
  std::string str;
  for (std::vector<std::string>::iterator it = _attributes.begin();
       it != _attributes.end(); ++it) {
    str += " " + *it;
  }
  return str;
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::delWith(std::vector<WINDOW *> windows) {
  if (!windows.empty())
    for (std::vector<WINDOW *>::iterator it = windows.begin();
         it != windows.end(); ++it) {
      delwin(*it);
    }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::pause() const {
  wgetch(_win);
}

template <typename dataType, size_t rows, size_t cols>
bool Kgrid<dataType, rows, cols>::windowSet() const {
  return _win != nullptr;
}

template <typename dataType, size_t rows, size_t cols>
Kgrid<dataType, rows, cols>::~Kgrid() {
  delWith(_subwins);
  wclear(_win);
}
} // namespace DisplayKernel
} // namespace BlackOS
#endif // BLACKOS_KGRID_TPP
