/**
 * Window
 *
 * Copyright (C) 2019, Takudzwa Makoni
 * <https://github.com/TakudzwaMakoni>
 *
 * This Program is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This Program is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This Program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#include "../inc/Window.h"

namespace {

std::vector<size_t> blocksFound(size_t const yValue,
                                std::vector<size_t> const &elements) {
  size_t numOfBlocks = elements.size() / 4; /*two coordinates per block*/
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

bool inBlocks(size_t const xValue, std::vector<size_t> const &blocks,
              std::vector<size_t> const &elements) {
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

Window::Window(size_t const sizeY, size_t const sizeX, size_t const posY,
               size_t const posX) {
  _winSzY = sizeY;
  _winSzX = sizeX;
  _winPosY = posY;
  _winPosX = posX;
}

size_t Window::winSzY() const { return _winSzY; }

size_t Window::winSzX() const { return _winSzX; }

size_t Window::winPosY() const { return _winPosY; }

size_t Window::winPosX() const { return _winPosX; }

/// ACCESSOR
std::string Window::winType() const { return "Window"; }

/// ACCESSOR
std::vector<size_t> Window::maxSize() const {
  // supposed to be size of COLS and ROWS
  size_t rows, cols;
  getmaxyx(_win, rows, cols);
  std::vector<size_t> termSz{rows, cols};
  return termSz;
}

void Window::moveCursor(int y, int x) {
  int Y, X;
  getmaxyx(_win, Y, X);
  if (y == 0)
    y += _showTitle + _showBorder;
  if (x == 0)
    x += _showBorder;

  if (y == Y)
    Y -= _showBorder;
  if (x == X)
    X -= _showBorder;
  wmove(_win, y, x);
}

void Window::bgfg(int const fg, int const bg) {
  // TODO: check if Foreground is the same colour and warn invisibility/deny
  init_pair(1 /*1 reserved for BG/FG pair*/, fg, bg);
  wbkgd(_win, COLOR_PAIR(1));
  // wclear(_win);
}

void Window::cursorPosition(int &y, int &x) const { getyx(_win, y, x); }

wchar_t Window::getCharFromUser() const { return wgetch(_win); }

void Window::insert(std::string const &str, size_t const y, size_t const x,
                    attr_t style) {
  moveCursor(y, x);
  print(str.c_str(), style);
}

void Window::insert(char const *ch, size_t const y, size_t const x,
                    attr_t style) {
  moveCursor(y, x);
  print(ch, style);
}

void Window::insert(char const ch, size_t const y, size_t const x,
                    attr_t style) {
  char const *chstr = &ch;
  moveCursor(y, x);
  print(chstr, style);
}

/// MUTATOR RETROACTIVE
void Window::borderStyle(int const ch) {
  _showBorder = 1;
  wborder(_win, ch, ch, ch, ch, ch, ch, ch, ch);
}

void Window::pause() const { wgetch(_win); }

/// MUTATOR RETROACTIVE
void Window::borderStyle(int const L, int const R, int const T, int const B,
                         int const TL, int const TR, int const BL,
                         int const BR) {
  _showBorder = 1;
  wborder(_win, L, R, T, B, TL, TR, BL, BR);
}

bool Window::windowSet() const { return _win != nullptr; }

int Window::lastKeyPressed() const { return _lastKeyPressed; }

void Window::_checkRange(size_t const y1, size_t const x1, size_t const y2,
                         size_t const x2) const {

  bool lowerLimitY1 = 0;
  bool upperLimitY1 = 0;
  bool upperLimitY2 = 0;

  bool lowerLimitX1 = 0;
  bool upperLimitX1 = 0;
  bool upperLimitX2 = 0;

  if (_showBorder == 1) {
    lowerLimitY1 = (y1 == 0);
    upperLimitY1 = (y1 >= _winSzY - 1);
    upperLimitY2 = (y2 >= _winSzY - 1);

    lowerLimitX1 = (x1 == 0);
    upperLimitX1 = (x1 >= _winSzX - 1);
    upperLimitX2 = (x2 >= _winSzX - 1);

  } else if (_showBorder == 0) {
    lowerLimitY1 = (y1 < 0);
    upperLimitY1 = (y1 > _winSzY - 1);
    upperLimitY2 = (y2 > _winSzY - 1);

    lowerLimitX1 = (x1 < 0);
    upperLimitX1 = (x1 > _winSzX - 1);
    upperLimitX2 = (x2 > _winSzX - 1);
  }

  // check y values
  if (lowerLimitY1) {
    throw std::runtime_error(
        "invalid range: border conflict in lower limit for y1 position " +
        std::to_string(y1) + " while border flag is " +
        std::to_string(_showBorder));
  } else if (upperLimitY1) {
    throw std::runtime_error(
        "invalid range: border conflict in upper limit for y1 position " +
        std::to_string(y1) + " while border flag is " +
        std::to_string(_showBorder));
  }
  if (y2 < y1) {
    throw std::runtime_error("invalid range: y2 = " + std::to_string(y2) +
                             " cannot be less than y1 = " + std::to_string(y1));
  } else if (upperLimitY2) {
    throw std::runtime_error(
        "invalid range: border conflict in upper limit for y2 position " +
        std::to_string(y2) + " while border flag is " +
        std::to_string(_showBorder));
  }

  // check x values
  if (lowerLimitX1) {
    throw std::runtime_error(
        "invalid range: border conflict in lower limit for x1 position " +
        std::to_string(x1) + " while border flag is " +
        std::to_string(_showBorder));
  } else if (upperLimitX1) {
    throw std::runtime_error(
        "invalid range: border conflict in upper limit for x1 position " +
        std::to_string(x1) + " while border flag is " +
        std::to_string(_showBorder));
  }
  if (x2 < x1) {
    throw std::runtime_error("invalid range: x2 = " + std::to_string(x2) +
                             " cannot be less than x1 = " + std::to_string(x1));
  } else if (upperLimitX2) {
    throw std::runtime_error(
        "invalid range: border conflict in upper limit for x2 position " +
        std::to_string(x2) + " while border flag is " +
        std::to_string(_showBorder));
  }
}

void Window::_checkRange(size_t const y, size_t const x) const {
  bool lowerLimitY = _showBorder ? (y < 0) : (y == 0);
  bool upperLimitY = _showBorder ? (y > _winSzY - 1) : (y >= _winSzY - 1);
  if (lowerLimitY) {
    throw std::runtime_error("invalid range: border conflict: y position " +
                             std::to_string(y) + " <= 0 ");
  } else if (upperLimitY) {
    throw std::runtime_error("invalid range: border conflict: y position " +
                             std::to_string(y) +
                             " >= " + std::to_string(_winSzY - 1));
  }

  bool lowerLimitX = _showBorder ? (x < 0) : (x == 0);
  bool upperLimitX = _showBorder ? (x > _winSzX - 1) : (x >= _winSzX - 1);
  if (lowerLimitX) {
    throw std::runtime_error("invalid range: border conflict: y position " +
                             std::to_string(x) + " <= 0 ");
  } else if (upperLimitX) {
    throw std::runtime_error("invalid range: border conflict: y position " +
                             std::to_string(x) +
                             " >= " + std::to_string(_winSzX - 1));
  }
}

/// MUTATOR RETROACTIVE
void Window::hideTitle() {
  std::string tPadding(_winSzX, ' ');
  mvwprintw(_win, _winPosY, _winPosX, tPadding.c_str());
  // metadata tells other functions to now disregard title space
  _showTitle = 0;
}

void Window::hideBorder() {
  borderStyle(' ');
  _showBorder = 0;
}

std::vector<std::string> Window::splitString(std::string str,
                                             std::string const &delimiter) {
  size_t pos = 0;
  std::string token;
  std::vector<std::string> words;
  while ((pos = str.find(delimiter)) != std::string::npos) {
    token = str.substr(0, pos);
    words.push_back(token);
    str = str.substr(pos + delimiter.length());
  }
  words.push_back(str);
  return words;
}

std::vector<std::string> Window::splitString(std::string str,
                                             char const delimiter) {
  std::string delimStr;
  delimStr += delimiter;
  auto const words = splitString(str, delimStr);
  return words;
}

void Window::newLine(bool newlineAtBeginning) {
  int x;
  int y;
  getyx(_win, y, x);

  print("\n");
  if (!newlineAtBeginning) {
    wmove(_win, y, x);
  }
  wrefresh(_win);
}

void Window::newLines(int n, bool newlineAtBeginning) {
  int x;
  int y;
  getyx(_win, y, x);

  for (int i = 1; i <= n; i++)
    print("\n");
  if (!newlineAtBeginning) {
    wmove(_win, y, x);
  }
  wrefresh(_win);
}

void Window::addChar(char const ch) { waddch(_win, ch); }
void Window::deleteChar() { wdelch(_win); }

void Window::printLines(std::string const &str, bool newlineAtBeginning) {
  auto const &words = splitString(str, "\n");
  if (words.empty()) {
    wprintw(_win, str.c_str());
    return;
  }
  for (auto const &word : words) {
    wprintw(_win, word.c_str());
    newLine(newlineAtBeginning);
  }
}

void Window::print(std::string const &str, attr_t style) {
  int y, x, Y, X;
  getyx(_win, y, x);
  getmaxyx(_win, Y, X);

  if (y == 0)
    y += _showTitle + _showBorder;
  if (x == 0)
    x += _showBorder;

  if (y == Y)
    Y -= _showBorder;
  if (x == X)
    X -= _showBorder;

  wmove(_win, y, x);
  wattron(_win, style);
  wprintw(_win, str.c_str());
  wattroff(_win, style);
}
void Window::print(std::string const &format, std::string const &str,
                   attr_t style) {
  int y, x;
  getyx(_win, y, x);

  if (y == 0)
    y += _showTitle + _showBorder;
  if (x == 0)
    x += _showBorder;

  wmove(_win, y, x);
  wattron(_win, style);
  wprintw(_win, format.c_str(), str.c_str());
  wattroff(_win, style);
}

/// sets the title for the window with an optional style option (default none).
/// This will not show the title to screen on window refresh if the the tite is
/// hidden.
void Window::loadTitle(std::string const &title, attr_t const style) {
  _title = title;
  _titleStyle = style;
}

void Window::loadTitleStyle(attr_t style) { _titleStyle = style; }

void Window::showTitle() {

  _showTitle = 1; // make title attributes visible again

  std::string headerTitle = _title;
  size_t y, x;

  size_t paddingCorrection;
  size_t headerTitleLen = headerTitle.length();

  // add title to window

  if (_showBorder) {
    y = 1;
    x = 1;
    if (headerTitleLen + 1 < _winSzX) {
      paddingCorrection = _winSzX - headerTitleLen - 2;
    } else {
      throw std::runtime_error("title header is longer than window width.");
    }
  } else {
    y = 0;
    x = 0;
    if (headerTitleLen - 1 < _winSzX) {
      paddingCorrection = _winSzX - headerTitleLen;
    } else {
      throw std::runtime_error("title header is longer than window width.");
    }
  }
  std::string header(paddingCorrection, ' ');
  header.insert(0, headerTitle);
  insert(header, y, x, _titleStyle);
}

/// MF
char Window::getCharFromWin(size_t const y, size_t const x,
                            bool const save_cursor) const {
  _checkRange(y, x);
  size_t curr_y, curr_x;
  getyx(_win, curr_y, curr_x);
  auto ch = (char)((A_CHARTEXT)&mvwinch(_win, y, x));
  if (save_cursor)
    wmove(_win, curr_y, curr_x);
  return ch;
}

/// MF ACTIVE
void Window::erase(size_t const y1, size_t const x1, size_t const y2,
                   size_t const x2) {

  //_checkRange(y1, x1, y2, x2);

  // TODO: this may replace checkRange()
  int y, x, Y, X;
  getyx(_win, y, x);
  getmaxyx(_win, Y, X);

  if (y1 == 0)
    y += _showTitle + _showBorder;
  if (x1 == 0)
    x += _showBorder;

  if (y2 == Y)
    Y -= _showBorder;
  if (x2 == X)
    X -= _showBorder;

  size_t width = x2 - x1 + 1;
  std::string fill(width, ' ');
  for (size_t i = y1; i <= y2; ++i) {
    mvwprintw(_win, i, x1, fill.c_str());
  }
  wrefresh(_win);
}

void Window::erase(bool titleBar) { fill(' ', titleBar); }

void Window::clear() {
  wclear(_win);
  wrefresh(_win);
}

/// MF RETROACTIVE
void Window::fill(char const ch, bool const titleBar) {

  int y, x;
  getyx(_win, y, x);

  y += _showTitle + _showBorder;
  x += _showBorder;

  size_t width = _winSzX - (2 * _showBorder);

  std::string fillString(width, ch);
  for (size_t i = y; i <= _winSzY - _showBorder; ++i) {
    mvwprintw(_win, i, 1, fillString.c_str());
  }
  refresh();
}

void Window::eraseWin() { werase(_win); }

/// MF ACTIVE
void Window::eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                         size_t const x2) {

  //_checkRange(y1, x1, y2, x2);

  int y, x, Y, X;
  getyx(_win, y, x);
  getmaxyx(_win, Y, X);

  if (y1 == 0)
    y += _showTitle + _showBorder;
  if (x1 == 0)
    x += _showBorder;

  if (y2 == Y)
    Y -= _showBorder;
  if (x2 == X)
    X -= _showBorder;

  size_t start = _showTitle + _showBorder;
  size_t end = _winSzY - (2 * _showBorder) - _showTitle;
  size_t width = _winSzX - (2 * _showBorder);

  std::string fill(width, ' ');
  std::string space = " ";
  for (size_t i = start; i <= end; ++i) {
    if (i <= y2 && i >= y1) {
      for (size_t j = _showBorder; j <= width; ++j) {
        if (!(j <= x2 && j >= x1)) {
          mvwprintw(_win, i, j, space.c_str());
        }
      }
    } else {
      mvwprintw(_win, i, _showBorder, fill.c_str());
    }
  }
  wrefresh(_win);
}

/// MF ACTIVE
void Window::erase(std::vector<size_t> const &elements) {
  size_t numOfAreas = elements.size() / 4; /*two coordinates per block*/
  for (size_t areaIdx = 0; areaIdx < numOfAreas; ++areaIdx) {

    size_t y1 = elements[0 + (areaIdx * 4)];
    size_t x1 = elements[1 + (areaIdx * 4)];
    size_t y2 = elements[2 + (areaIdx * 4)];
    size_t x2 = elements[3 + (areaIdx * 4)];

    erase(y1, x1, y2, x2);
  }
}

/// MF ACTIVE
void Window::eraseExcept(std::vector<size_t> const &elements) {

  size_t start = _showTitle + _showBorder;
  size_t end = _winSzY - (2 * _showBorder) - _showTitle;
  size_t width = _winSzX - (2 * _showBorder);

  std::string fill(width, ' ');
  for (size_t i = start; i <= end; ++i) {
    std::vector<size_t> blocks = blocksFound(i, elements);
    if (blocks.empty()) {
      mvwprintw(_win, i, _showBorder, fill.c_str());
    } else {
      for (size_t j = _showBorder; j <= width; ++j) {
        bool _inBlocks = inBlocks(j, blocks, elements);
        if (!_inBlocks) {
          mvwprintw(_win, i, j, " ");
        }
      }
    }
  }
  wrefresh(_win);
}

void Window::refresh() { wrefresh(_win); }

void Window::setScroll(bool x) { scrollok(_win, x); }

void Window::setKeypad(bool x) { keypad(_win, x); }

void Window::setWin(WIN_SET_CODE const init) {

  if (init == WIN_SET_CODE::INIT_PARENT) {
    initscr();
    keypad(_win, TRUE);

    _win = newwin(0, 0, 0, 0);
    wresize(_win, _winSzY, _winSzX);
    mvwin(_win, _winPosY, _winPosX);
  } else if (init == WIN_SET_CODE::INIT_CHILD) {
    _win = newwin(0, 0, 0, 0);
    wresize(_win, _winSzY, _winSzX);
    mvwin(_win, _winPosY, _winPosX);
  } else if (init == WIN_SET_CODE::KILL_PARENT) {
    delwin(_win);
    endwin();
    _win = nullptr;
  } else if (init == WIN_SET_CODE::KILL_CHILD) {
    delwin(_win);
    _win = nullptr;
  }
}

int Window::resize(size_t const y, size_t const x) {
  _winSzY = y;
  _winSzX = x;
  wresize(_win, _winSzY, _winSzX);
  return 1;
}

int Window::reposition(size_t const y, size_t const x) {
  _winPosY = y;
  _winPosX = x;
  mvwin(_win, _winPosY, _winPosX);
  return 1;
}

Window::~Window() {
  // TODO: store init mode and kill correspondingly
  if (windowSet())
    setWin(WIN_SET_CODE::KILL_PARENT);
}

} // namespace DisplayKernel
} // namespace BlackOS
