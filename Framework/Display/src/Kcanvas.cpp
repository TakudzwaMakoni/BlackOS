
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
const int PADDING = 1;
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
Kcanvas::Kcanvas(std::string &name, int sizeY, int sizeX, int posY, int posX) {
  _size = {sizeY, sizeX};
  _position = {posY, posX};
}
WINDOW *Kcanvas::window() const { return this->_win; };
void Kcanvas::borderStyle(const int ch) {
  _borderStyle = {ch, ch, ch, ch, ch, ch, ch, ch};
}
void Kcanvas::borderStyle(const int L, const int R, const int T, const int B,
                          const int TL, const int TR, const int BL,
                          const int BR) {
  _borderStyle = {L, R, T, B, TL, TR, BL, BR};
}
void Kcanvas::label(const std::string &label) const {
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
void Kcanvas::kErase(const int y1, const int x1, const int y2, const int x2) {
  int borderY = _size[0];
  int borderX = _size[1];
  int _y1 = y1 <= 0 ? 1 : y1;
  int _x1 = x1 <= 0 ? 1 : x1;
  int _y2 = y2 >= borderY ? borderY - 1 : y2;
  int _x2 = x2 >= borderX ? borderX - 1 : x2;
  int width = _x2 - _x1;
  std::string fill(width, ' ');
  for (int i = _y1; i <= _y2; ++i) {
    mvwprintw(_win, i, _x1, fill.c_str());
  }
}
void Kcanvas::kEraseExcept(const int y1, const int x1, const int y2,
                           const int x2) {
  int borderY = _size[0];
  int borderX = _size[1];
  int width = borderX - 2;
  int height = borderY - 2;
  int _y1 = y1 <= 0 ? 1 : y1;
  int _x1 = x1 <= 0 ? 1 : x1;
  int _y2 = y2 >= borderY ? borderY - 1 : y2;
  int _x2 = x2 >= borderX ? borderX - 1 : x2;
  std::string fill(width, ' ');
  std::string space = " ";
  for (int i = 1; i <= height; ++i) {
    if (i <= _y2 && i >= _y1) {
      for (int j = 1; j <= width; ++j) {
        if (!(j <= _x2 && j >= _x1)) {
          mvwprintw(_win, i, j, space.c_str());
        }
      }
    } else {
      mvwprintw(_win, i, 1, fill.c_str());
    }
  }
}
void Kcanvas::kErase(const std::vector<int> &elements) {
  int numOfAreas = elements.size() / 4; /*two coordinates per block*/
  for (int areaIdx = 0; areaIdx < numOfAreas; ++areaIdx) {
    int borderY = _size[0];
    int borderX = _size[1];
    int width = borderX - 2;
    int height = borderY - 2;
    int y1 = elements[0 + (areaIdx * 4)];
    int x1 = elements[1 + (areaIdx * 4)];
    int y2 = elements[2 + (areaIdx * 4)];
    int x2 = elements[3 + (areaIdx * 4)];
    int _y1 = y1 <= 0 ? 1 : y1;
    int _x1 = x1 <= 0 ? 1 : x1;
    int _y2 = y2 >= borderY ? borderY - 1 : y2;
    int _x2 = x2 >= borderX ? borderX - 1 : x2;
    kErase(_y1, _x1, _y2, _x2);
  }
}
void Kcanvas::kEraseExcept(const std::vector<int> &elements) {
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
int Kcanvas::centreX() const { return _size[1] / 2; }
int Kcanvas::centreY() const { return _size[0] / 2; }
std::vector<int> Kcanvas::size() const { return _size; }
Eigen::Vector2i Kcanvas::position() const { return _position; }
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

void Kcanvas::_setBorderStyle() {
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
void Kcanvas::fill(char ch) {
  std::string fillString(_size[1] - 2, ch);
  for (int i = 1; i <= _size[0]; ++i) {
    mvwprintw(_win, i, 1, fillString.c_str());
  }
  wrefresh(_win);
}
void Kcanvas::wipe(bool titleBar) {
  if (titleBar) {
    std::string fillString(_size[1] - 2, ' ');
    for (int i = 2; i <= _size[0] - 3; ++i) {
      mvwprintw(_win, i, 1, fillString.c_str());
    }
  } else {
    fill(' ');
  }
  wrefresh(_win);
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
} // namespace Display
} // namespace BlackOS
