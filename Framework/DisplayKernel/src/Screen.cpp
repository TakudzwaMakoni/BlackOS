/**
 * Screen
 *
 * Copyright (C) 2019-07-13, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "../inc/Screen.h"

namespace {

/// SUBROUTINE ANONYMOUS
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

/// SUBROUTINE ANONYMOUS
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

size_t Screen::winSzY() const { return _termSzY; }

size_t Screen::winSzX() const { return _termSzX; }

size_t Screen::winPosY() const { return 0; }

size_t Screen::winPosX() const { return 0; }

/// ACCESSOR
std::string Screen::winType() const { return "Screen"; }

/// ACCESSOR
std::vector<size_t> Screen::maxSize() const { return TERMINAL_SIZE(); }

void Screen::insert(std::string const &str, size_t const y, size_t const x,
                    TextStyle style) {
  if (style == TextStyle::highlight) {
    attron(A_REVERSE);
    mvprintw(y, x, str.c_str());
    attroff(A_REVERSE);
  } else if (style == TextStyle::underline) {
    attron(A_UNDERLINE);
    mvprintw(y, x, str.c_str());
    attroff(A_UNDERLINE);
  } else /*none*/ {
    mvprintw(y, x, str.c_str());
  }
}

void Screen::setCursorColour(char const *colour) {
  printf("\e]12;%s\a", colour);
  fflush(stdout);
}

void Screen::insert(char const *ch, size_t const y, size_t const x,
                    TextStyle style) {
  if (style == TextStyle::highlight) {
    attron(A_REVERSE);
    mvprintw(y, x, ch);
    attroff(A_REVERSE);
  } else if (style == TextStyle::underline) {
    attron(A_UNDERLINE);
    mvprintw(y, x, ch);
    attroff(A_UNDERLINE);
  } else /*none*/ {
    mvprintw(y, x, ch);
  }
}

std::vector<int> Screen::cursorPosition() const {
  int x, y;
  getsyx(y, x);
  std::vector<int> v{y, x};
  return v;
}

char Screen::getCharFromUser() const { return getch(); }

void Screen::insert(char const ch, size_t const y, size_t const x,
                    TextStyle style) {
  char const *chstr = &ch;
  if (style == TextStyle::highlight) {
    attron(A_REVERSE);
    mvprintw(y, x, chstr);
    attroff(A_REVERSE);
  } else if (style == TextStyle::underline) {
    attron(A_UNDERLINE);
    mvprintw(y, x, chstr);
    attroff(A_UNDERLINE);
  } else /*none*/ {
    mvprintw(y, x, chstr);
  }
}

/// MUTATOR RETROACTIVE
void Screen::borderStyle(int const ch) {
  _showBorder = 1;
  border(ch, ch, ch, ch, ch, ch, ch, ch);
}

void Screen::pause() const { getch(); }

/// MUTATOR RETROACTIVE
void Screen::borderStyle(int const L, int const R, int const T, int const B,
                         int const TL, int const TR, int const BL,
                         int const BR) {
  _showBorder = 1;
  border(L, R, T, B, TL, TR, BL, BR);
}

bool Screen::windowSet() const { /*TODO: need?*/
  return _screenInitialised;
}

int Screen::lastKeyPressed() const { return _lastKeyPressed; }

void Screen::_checkRange(size_t const y1, size_t const x1, size_t const y2,
                         size_t const x2) const {

  bool lowerLimitY1 = 0;
  bool upperLimitY1 = 0;
  bool upperLimitY2 = 0;

  bool lowerLimitX1 = 0;
  bool upperLimitX1 = 0;
  bool upperLimitX2 = 0;

  if (_showBorder == 1) {
    lowerLimitY1 = (y1 == 0);
    upperLimitY1 = (y1 >= _termSzY - 1);
    upperLimitY2 = (y2 >= _termSzY - 1);

    lowerLimitX1 = (x1 == 0);
    upperLimitX1 = (x1 >= _termSzX - 1);
    upperLimitX2 = (x2 >= _termSzX - 1);

  } else if (_showBorder == 0) {
    lowerLimitY1 = (y1 < 0);
    upperLimitY1 = (y1 > _termSzY - 1);
    upperLimitY2 = (y2 > _termSzY - 1);

    lowerLimitX1 = (x1 < 0);
    upperLimitX1 = (x1 > _termSzX - 1);
    upperLimitX2 = (x2 > _termSzX - 1);
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

void Screen::_checkRange(size_t const y, size_t const x) const {
  bool lowerLimitY = _showBorder ? (y < 0) : (y == 0);
  bool upperLimitY = _showBorder ? (y > _termSzY - 1) : (y >= _termSzY - 1);
  if (lowerLimitY) {
    throw std::runtime_error("invalid range: border conflict: y position " +
                             std::to_string(y) + " <= 0 ");
  } else if (upperLimitY) {
    throw std::runtime_error("invalid range: border conflict: y position " +
                             std::to_string(y) +
                             " >= " + std::to_string(_termSzY - 1));
  }

  bool lowerLimitX = _showBorder ? (x < 0) : (x == 0);
  bool upperLimitX = _showBorder ? (x > _termSzX - 1) : (x >= _termSzX - 1);
  if (lowerLimitX) {
    throw std::runtime_error("invalid range: border conflict: y position " +
                             std::to_string(x) + " <= 0 ");
  } else if (upperLimitX) {
    throw std::runtime_error("invalid range: border conflict: y position " +
                             std::to_string(x) +
                             " >= " + std::to_string(_termSzX - 1));
  }
}

/// MUTATOR RETROACTIVE
void Screen::hideTitle() {
  std::string tPadding(_termSzX, ' ');
  mvprintw(0, 0, tPadding.c_str());
  // metadata tells other functions to now disregard title space
  _showTitle = 0;
}

void Screen::hideBorder() {
  borderStyle(' ');
  _showBorder = 0;
}

/// sets the title for the window with an optional style option (default none).
/// This will not show the title to screen on window refresh if the the tite is
/// hidden.
void Screen::loadTitle(std::string const &title, TextStyle const style) {
  _title = title;
  _titleStyle = style;
}

void Screen::loadTitleStyle(TextStyle style) { _titleStyle = style; }

void Screen::showTitle() {

  _showTitle = 1; // make title attributes visible again

  std::string headerTitle = _title;
  size_t y, x;

  size_t paddingCorrection;
  size_t headerTitleLen = headerTitle.length();

  // add title to window

  if (_showBorder) {
    y = 1;
    x = 1;
    if (headerTitleLen + 1 < _termSzX) {
      paddingCorrection = _termSzX - headerTitleLen - 2;
    } else {
      throw std::runtime_error("title header is longer than window width.");
    }
  } else {
    y = 0;
    x = 0;
    if (headerTitleLen - 1 < _termSzX) {
      paddingCorrection = _termSzX - headerTitleLen;
    } else {
      throw std::runtime_error("title header is longer than window width.");
    }
  }

  std::string header(paddingCorrection, ' ');
  header.insert(0, headerTitle);
  insert(header, y, x, _titleStyle);
};

/// MF
char Screen::getCharFromWin(size_t const y, size_t const x,
                            bool const save_cursor) const {
  _checkRange(y, x);
  size_t curr_y, curr_x;
  getsyx(curr_y, curr_x);
  auto ch = (char)((A_CHARTEXT)&mvinch(y, x));
  if (save_cursor)
    move(curr_y, curr_x);
  return ch;
}

/// MF ACTIVE
void Screen::erase(size_t const y1, size_t const x1, size_t const y2,
                   size_t const x2) {
  _checkRange(y1, x1, y2, x2);
  size_t width = x2 - x1 + 1;
  std::string fill(width, ' ');
  for (size_t i = y1; i <= y2; ++i) {
    mvprintw(i, x1, fill.c_str());
  }
  refresh();
}

void Screen::erase(bool titleBar) { fill(' ', titleBar); }

/// MF RETROACTIVE
void Screen::fill(char const ch, bool const titleBar) {

  size_t start = titleBar + _showBorder;
  size_t end = _termSzY - (2 * _showBorder) - titleBar;
  size_t width = _termSzX - (2 * _showBorder);

  std::string fillString(width, ch);
  for (size_t i = start; i <= end; ++i) {
    mvprintw(i, _showBorder, fillString.c_str());
  }
  refresh();
}

/// MF ACTIVE
void Screen::eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                         size_t const x2) {

  _checkRange(y1, x1, y2, x2);

  size_t width = _termSzX - 2;
  size_t height = _termSzY - 2;

  std::string fill(width, ' ');
  std::string space = " ";
  for (size_t i = 1; i <= height; ++i) {
    if (i <= y2 && i >= y1) {
      for (size_t j = 1; j <= width; ++j) {
        if (!(j <= x2 && j >= x1)) {
          mvprintw(i, j, space.c_str());
        }
      }
    } else {
      mvprintw(i, 1, fill.c_str());
    }
  }
  refresh();
}

/// MF ACTIVE
void Screen::erase(std::vector<size_t> const &elements) {
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
void Screen::eraseExcept(std::vector<size_t> const &elements) {
  size_t width = _termSzX - 2;
  size_t height = _termSzY - 2;
  std::string fill(width, ' ');
  std::string space = " ";
  for (size_t i = 1; i <= height; ++i) {
    std::vector<size_t> blocks = blocksFound(i, elements);
    if (blocks.empty()) {
      mvprintw(i, 1, fill.c_str());
    } else {
      for (size_t j = 1; j <= width; ++j) {
        bool _inBlocks = inBlocks(j, blocks, elements);
        if (!_inBlocks) {
          mvprintw(i, j, space.c_str());
        }
      }
    }
  }
  refresh();
}

/// MF ACTIVE
void Screen::label(std::string const &label) const {
  size_t labellocy = _termSzY - 1;
  size_t labellocx = _termSzX - (3 + (size_t)label.length());
  mvaddstr(labellocy, labellocx, label.c_str());
}

/// MF ACTIVE
void Screen::setWin(bool const init) {
  if (init == 1) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    auto termSz = TERMINAL_SIZE();
    _termSzY = termSz[0];
    _termSzX = termSz[1];
    _screenInitialised = 1;

  } else if (init == 0) {
    _screenInitialised = 0;
    endwin();
  }
}

Screen::~Screen() {
  // if last screen state was initialised
  if (windowSet())
    setWin(0);
}

} // namespace DisplayKernel
} // namespace BlackOS
