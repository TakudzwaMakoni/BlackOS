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
std::vector<int> blocksFound(const int yValue, const int numOfBlocks,
                             const std::vector<int> &elements) {
  std::vector<int> iteratorList(numOfBlocks);
  std::iota(iteratorList.begin(), iteratorList.end(), 0);
  std::vector<int> _linesUnclear;
  for (const int block : iteratorList) {
    int y1 = elements[0 + (block * 4)];
    int y2 = elements[2 + (block * 4)];
    if (y1 <= yValue && yValue <= y2)
      _linesUnclear.push_back(block);
  }
  return _linesUnclear;
}
bool inBlocks(const int xValue, const std::vector<int> &blocks,
              const std::vector<int> &elements) {
  for (const int block : blocks) {
    int x1 = elements[1 + (block * 4)];
    int x2 = elements[3 + (block * 4)];
    if (x1 <= xValue && xValue <= x2)
      return true;
  }
  return false;
}
} // namespace

namespace BlackOS {
namespace Display {
template <typename dataType, size_t rows, size_t cols>
Kgrid<dataType, rows, cols>::Kgrid(std::string &name, int sizeY, int sizeX,
                                   int posY, int posX) {
  _name = name;
  _size = {sizeY, sizeX};
  _position = {posY, posX};
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::setWin(WINDOW *window) {
  _win = window;
  wresize(_win, _size[0], _size[1]);
  mvwin(_win, _position[0], _position[1]);
  wrefresh(_win);
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::borderStyle(const int ch) {
  _borderStyle = {ch, ch, ch, ch, ch, ch, ch, ch};
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::borderStyle(const int L, const int R,
                                              const int T, const int B,
                                              const int TL, const int TR,
                                              const int BL, const int BR) {
  _borderStyle = {L, R, T, B, TL, TR, BL, BR};
}

template <typename dataType, size_t rows, size_t cols>
WINDOW *Kgrid<dataType, rows, cols>::window() const {
  return this->_win;
};

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::label(const std::string &label) const {
  int labellocy = _size[0] - 1;
  int labellocx = _size[1] - (3 + (int)label.length());
  mvwaddstr(_win, labellocy, labellocx, label.c_str());
}

template <typename dataType, size_t rows, size_t cols>
std::vector<int> Kgrid<dataType, rows, cols>::maxSize() const {
  int yMax, xMax;
  getmaxyx(_win, yMax, xMax);
  std::vector<int> size{yMax, xMax};
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
void Kgrid<dataType, rows, cols>::setTitle(std::string title) {
  _title = title;
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::write(
    Eigen::Matrix<dataType, rows, cols> &data) {
  _matrix = data;
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::refresh() {
  wrefresh(_win);
}
template <typename dataType, size_t rows, size_t cols>
int Kgrid<dataType, rows, cols>::getChrfromW(
    int const y, int const x, bool const preserve_cursor_pos) const {
  int curr_y, curr_x;
  getyx(_win, curr_y, curr_x);
  int wch = mvwinch(_win, y, x);
  if (preserve_cursor_pos)
    wmove(_win, curr_y, curr_x);
  return wch;
}

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

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::kErase(const int y1, const int x1,
                                         const int y2, const int x2) {
  int a, b, c, d;
  int borderY = _size[0];
  int borderX = _size[1];
  a = y1 == 0 ? 1 : y1;
  b = x1 == 0 ? 1 : x1;
  c = y2 == borderY ? y2 - 1 : y2;
  d = x2 == borderX ? x2 - 1 : x2;
  int width = d - b + 1;
  std::string fill(width, ' ');
  for (int i = a; i <= c; ++i) {
    mvwprintw(_win, i, b, fill.c_str());
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::kEraseExcept(const int y1, const int x1,
                                               const int y2, const int x2) {
  int borderY = _size[0] - 2;
  int borderX = _size[1] - 2;
  std::string fill(borderX, ' ');
  std::string space = " ";
  for (int i = 1; i <= borderY; ++i) {
    if (i < y1 || i > y2) {
      mvwprintw(_win, i, 1, fill.c_str());
    } else {
      for (int j = 1; j <= borderX; ++j) {
        if ((j > x2 || j < x1)) {
          mvwprintw(_win, i, j, space.c_str());
        }
      }
    }
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::kErase(const std::vector<int> &elements) {
  int numOfAreas = elements.size() / 4; /*two coordinates per block*/
  for (int areaIdx = 0; areaIdx < numOfAreas; ++areaIdx) {
    int y1, x1, y2, x2;
    y1 = elements[0 + (areaIdx * 4)];
    x1 = elements[1 + (areaIdx * 4)];
    y2 = elements[2 + (areaIdx * 4)];
    x2 = elements[3 + (areaIdx * 4)];
    kErase(y1, x1, y2, x2);
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::kEraseExcept(
    const std::vector<int> &elements) {
  int numOfBlocks = elements.size() / 4; /*two coordinates per block*/
  int borderY = _size[0];
  int borderX = _size[1];
  int width = borderX - 2;
  int height = borderY - 2;
  std::string fill(width, ' ');
  std::string space = " ";
  for (int i = 1; i <= height; ++i) {
    std::vector<int> blocks = blocksFound(i, numOfBlocks, elements);
    if (blocks.empty()) {
      mvwprintw(_win, i, 1, fill.c_str());
    } else {
      for (int j = 1; j <= width; ++j) {
        bool _inBlocks = inBlocks(j, blocks, elements);
        if (!_inBlocks) {
          mvwprintw(_win, i, j, space.c_str());
        }
      }
    }
  }
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::setBorderStyle() {
  int L, R, T, B, TL, TR, BL, BR;
  L = _borderStyle[0];
  R = _borderStyle[1];
  T = _borderStyle[2];
  B = _borderStyle[3];
  TL = _borderStyle[4];
  TR = _borderStyle[5];
  BL = _borderStyle[6];
  BR = _borderStyle[7];
  wborder(_win, L, R, T, B, TL, TR, BL, BR);
}

template <typename dataType, size_t rows, size_t cols>
void Kgrid<dataType, rows, cols>::display() {
  keypad(_win, true);
  int selection;
  size_t highlightedRow = 0;
  size_t highlightedCol = 0;
  int topPad = 1;
  setBorderStyle();
  if (_showTitle) {
    _attributes = {"RAD", "PREC: " + std::to_string(_precision)};
    wattron(_win, A_REVERSE);
    std::string tPadding(_size[1], ' ');
    int correction = _size[1] - _title.length() - 3;
    std::string titleString = attributeString();
    int titleStrLength = titleString.size();
    tPadding.replace(0, titleStrLength, titleString);
    tPadding.replace(correction, _size[1], _title + " ");
    mvwprintw(_win, _position[0], _position[1], tPadding.c_str());
    wattroff(_win, A_REVERSE);
  }
  int displayPrecision = 5;
  while (true) {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        if (j == highlightedCol && i == highlightedRow) {
          wattron(_win, A_REVERSE);
        }
        auto element = _matrix(highlightedRow, highlightedCol);
        std::stringstream elementStream;
        elementStream << std::setprecision(_precision) << element;
        const std::string elementStr = elementStream.str();
        const int elementStrSize = elementStr.size();
        std::string elementDisplay(_size[1] / 2, ' ');
        elementDisplay.replace(0, elementStrSize, elementStr);
        std::stringstream numStream;
        std::string str(8, ' ');
        int strSize = str.size();
        if (_matrix.coeff(i, j) > 99.99) {
          numStream << std::scientific << std::setprecision(2)
                    << _matrix.coeff(i, j);
        } else {
          numStream << std::setprecision(displayPrecision) << std::fixed
                    << _matrix.coeff(i, j);
        }
        std::string numStr = numStream.str();
        int numStrSize = numStr.size();
        int correction = strSize - numStrSize;
        str.replace(correction, numStrSize, numStr);
        int yAlign = 0;
        int xAlign = 0;
        int left, right, top, bottom, v_centre, h_centre;
        left = 2;
        int hPadding{4};
        if (_setGrid) {
          hPadding = 6;
          str = "|" + str + "|";
        }
        int gridWidth = (cols * 8) + (cols - 2);
        int gridHeight = rows * _vPadding; // avoid collision with status bar
        int bottomPad = -gridHeight + i;
        int rightPad = gridWidth - 1;
        int hCentrePad = gridWidth;
        int vCentrPad = gridHeight;
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
Kgrid<dataType, rows, cols>::~Kgrid() {
  delWith(_subwins);
  wclear(_win);
}
} // namespace Display
} // namespace BlackOS
#endif // BLACKOS_KGRID_TPP
