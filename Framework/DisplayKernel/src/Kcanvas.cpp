
#include "../inc/Kcanvas.h"
#include "../inc/Directives.h"
#include "../inc/Kwindow.h"
#include "Eigen/Dense"
#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace {

std::vector<size_t> blocksFound(const size_t yValue, const size_t numOfBlocks,
                                const std::vector<size_t> &elements) {
  std::vector<size_t> iteratorList(numOfBlocks);
  std::iota(iteratorList.begin(), iteratorList.end(), 0);
  std::vector<size_t> _linesUnclear;
  for (const size_t block : iteratorList) {
    size_t y1 = elements[0 + (block * 4)];
    size_t y2 = elements[2 + (block * 4)];
    if (y1 <= yValue && yValue <= y2)
      _linesUnclear.push_back(block);
  }
  return _linesUnclear;
}
bool inBlocks(const size_t xValue, const std::vector<size_t> &blocks,
              const std::vector<size_t> &elements) {
  for (const size_t block : blocks) {
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
Kcanvas::Kcanvas(std::string &name, size_t sizeY, size_t sizeX, size_t posY,
                 size_t posX) {
  _size = {sizeY, sizeX};
  _position = {posY, posX};
}
void Kcanvas::borderStyle(int const ch) {
  _borderStyle = {ch, ch, ch, ch, ch, ch, ch, ch};
}
void Kcanvas::borderStyle(int const L, int const R, int const T, int const B,
                          int const TL, int const TR, int const BL,
                          int const BR) {
  _borderStyle = {L, R, T, B, TL, TR, BL, BR};
}
void Kcanvas::label(std::string const &label) const {
  int labellocy = _size[0] - 1;
  int labellocx = _size[1] - (3 + (int)label.length());
  mvwaddstr(_win, labellocy, labellocx, label.c_str());
}
std::vector<int> Kcanvas::maxSize() const {
  int yMax, xMax;
  getmaxyx(_win, yMax, xMax);
  std::vector<int> size{yMax, xMax};
  return size;
}
std::string Kcanvas::winType() const { return "Kcanvas"; }
std::string Kcanvas::name() const { return _name; }
void Kcanvas::setWin(WINDOW *window) {
  _win = window;
  wresize(_win, _size[0], _size[1]);
  mvwin(_win, _position[0], _position[1]);
  wrefresh(_win);
}
void Kcanvas::erase(size_t const y1, size_t const x1, size_t const y2,
                    size_t const x2) {
  size_t borderY = _size[0];
  size_t borderX = _size[1];
  size_t _y1 = y1 <= 0 ? 1 : y1;
  size_t _x1 = x1 <= 0 ? 1 : x1;
  size_t _y2 = y2 >= borderY ? borderY - 1 : y2;
  size_t _x2 = x2 >= borderX ? borderX - 1 : x2;
  size_t width = _x2 - _x1;
  std::string fill(width, ' ');
  for (size_t i = _y1; i <= _y2; ++i) {
    mvwprintw(_win, i, _x1, fill.c_str());
  }
}
void Kcanvas::eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                          size_t const x2) {
  size_t borderY = _size[0];
  size_t borderX = _size[1];
  size_t width = borderX - 2;
  size_t height = borderY - 2;
  size_t _y1 = y1 <= 0 ? 1 : y1;
  size_t _x1 = x1 <= 0 ? 1 : x1;
  size_t _y2 = y2 >= borderY ? borderY - 1 : y2;
  size_t _x2 = x2 >= borderX ? borderX - 1 : x2;
  std::string fill(width, ' ');
  std::string space = " ";
  for (size_t i = 1; i <= height; ++i) {
    if (i <= _y2 && i >= _y1) {
      for (size_t j = 1; j <= width; ++j) {
        if (!(j <= _x2 && j >= _x1)) {
          mvwprintw(_win, i, j, space.c_str());
        }
      }
    } else {
      mvwprintw(_win, i, 1, fill.c_str());
    }
  }
}
void Kcanvas::erase(std::vector<int> const &elements) {
  size_t numOfAreas = elements.size() / 4; /*two coordinates per block*/
  for (size_t areaIdx = 0; areaIdx < numOfAreas; ++areaIdx) {
    size_t borderY = _size[0];
    size_t borderX = _size[1];
    size_t width = borderX - 2;
    size_t height = borderY - 2;
    size_t y1 = elements[0 + (areaIdx * 4)];
    size_t x1 = elements[1 + (areaIdx * 4)];
    size_t y2 = elements[2 + (areaIdx * 4)];
    size_t x2 = elements[3 + (areaIdx * 4)];
    size_t _y1 = y1 <= 0 ? 1 : y1;
    size_t _x1 = x1 <= 0 ? 1 : x1;
    size_t _y2 = y2 >= borderY ? borderY - 1 : y2;
    size_t _x2 = x2 >= borderX ? borderX - 1 : x2;
    erase(_y1, _x1, _y2, _x2);
  }
}
void Kcanvas::eraseExcept(const std::vector<size_t> &elements) {
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
void Kcanvas::refresh() { wrefresh(_win); }
std::vector<size_t> Kcanvas::size() const { return _size; }
std::vector<size_t> Kcanvas::position() const { return _position; }
void Kcanvas::showTitle(bool show) { _showTitle = show; }
void Kcanvas::setTitle(std::string title) { _title = title; }
std::string Kcanvas::attributeString() {
  std::string str;
  for (std::vector<std::string>::iterator it = _attributes.begin();
       it != _attributes.end(); ++it) {
    str += " " + *it;
  }
  return str;
}
void Kcanvas::delWith(std::vector<WINDOW *> windows) {
  if (!windows.empty())
    for (std::vector<WINDOW *>::iterator it = windows.begin();
         it != windows.end(); ++it) {
      delwin(*it);
    }
}

/// MF
int Kcanvas::getChrfromW(size_t const y, size_t const x,
                         bool const preserve_cursor_pos) const {
  size_t curr_y, curr_x;
  getyx(_win, curr_y, curr_x);
  int ch = mvwinch(_win, y, x);
  if (preserve_cursor_pos)
    wmove(_win, curr_y, curr_x);
  return ch;
}

void Kcanvas::pause() const { wgetch(_win); }

bool Kcanvas::windowSet() const { return _win != nullptr; }

void Kcanvas::fill(char const ch, bool const titleBar) {
  size_t start = titleBar ? 2 : 1;
  int end = _size[0] - 2;

  std::string fillString(_size[1] - 2, ch);
  for (int i = start; i <= end; ++i) {
    mvwprintw(_win, i, 1, fillString.c_str());
  }
}
void Kcanvas::display() {
  keypad(_win, true);
  _setBorderStyle();
  if (_showTitle) {
    _attributes = {"Kcanvas"};
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
  wrefresh(_win);
}
Kcanvas::~Kcanvas() { delWith(_subwins); }
} // namespace DisplayKernel
} // namespace BlackOS
