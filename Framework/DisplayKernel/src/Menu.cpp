/**
 * Menu
 *
 * Copyright (C) 2019-07-27, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "../inc/Menu.h"

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

Menu::Menu(size_t const sizeY, size_t const sizeX, size_t const posY,
           size_t const posX) {
  _winSzY = sizeY;
  _winSzX = sizeX;
  _winPosY = posY;
  _winPosX = posX;
}

size_t Menu::_highlightedMap() const {
  size_t const map = (_pCoeff() * _page) + _highlighted;
  return map;
}

/// ACCESSOR
std::vector<std::string> Menu::fields() const { return _fields; }

size_t Menu::winSzY() const { return _winSzY; }

size_t Menu::winSzX() const { return _winSzX; }

size_t Menu::page() const { return _page; }

size_t Menu::winPosY() const { return _winPosY; }

size_t Menu::winPosX() const { return _winPosX; }

/// ACCESSOR
std::string Menu::winType() const { return "Menu"; }

/// ACCESSOR
std::string Menu::selectedField() const {
  size_t map = _highlightedMap();
  return this->_fields[map];
}

std::vector<int> Menu::cursorPosition() const {
  int x, y;
  getyx(_win, y, x);
  std::vector<int> v{x, y};
  return v;
}

void Menu::bgfg(int const fg, int const bg) {
  start_color();
  // TODO: check if Foreground is the same colour and warn invisibility/deny
  init_pair(1 /*1 reserved for BG/FG pair*/, fg, bg);
  wbkgd(_win, COLOR_PAIR(1));
  wclear(_win);
}

size_t Menu::selectedFieldIndex() const { return _highlightedMap(); }

/// ACCESSOR
std::vector<size_t> Menu::maxSize() const {
  // supposed to be size of COLS and ROWS
  size_t rows, cols;
  getmaxyx(_win, rows, cols);
  std::vector<size_t> termSz{rows, cols};
  return termSz;
}

wchar_t Menu::getCharFromUser() const { return wgetch(_win); }

void Menu::insert(std::string const &str, size_t const y, size_t const x,
                  attr_t style) {
  moveCursor(y, x);
  print(str.c_str(), style);
}

void Menu::insert(char const *ch, size_t const y, size_t const x,
                  attr_t style) {
  moveCursor(y, x);
  print(ch, style);
}

void Menu::insert(char const ch, size_t const y, size_t const x, attr_t style) {
  char const *chstr = &ch;
  moveCursor(y, x);
  print(chstr, style);
}

/// MUTATOR RETROACTIVE
void Menu::borderStyle(int const ch) {
  _showBorder = 1;
  wborder(_win, ch, ch, ch, ch, ch, ch, ch, ch);
}

void Menu::pause() const { wgetch(_win); }

/// MUTATOR RETROACTIVE
void Menu::borderStyle(int const L, int const R, int const T, int const B,
                       int const TL, int const TR, int const BL, int const BR) {
  _showBorder = 1;
  wborder(_win, L, R, T, B, TL, TR, BL, BR);
}

bool Menu::windowSet() const { return _win != nullptr; }

void Menu::_updateF() {
  size_t p = numPages();
  size_t pRem = _pRem();
  size_t pCoeff = _pCoeff();
  if (_page == (p - 1) && pRem != 0)
    _f = pRem;
  else
    _f = pCoeff;
}

/// MUTATOR RETROACTIVE
void Menu::initFields(std::vector<std::string> const &fields) {
  _fields = fields;
  _fieldSz = fields.size();

  // load defaults on new std::strings
  _page = 0;

  _updateM();
  _updateF();
}

int Menu::lastKeyPressed() const { return _lastKeyPressed; }

void Menu::_checkRange(size_t const y1, size_t const x1, size_t const y2,
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

void Menu::_checkRange(size_t const y, size_t const x) const {
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

void Menu::loadFields() {

  _updateF();
  std::vector<std::string> subFields = _loadPage();

  size_t xCorrect;
  size_t yCorrect;
  size_t correction = _showBorder;

  if (_entriesPerPage <= _fieldSz && _showPages) {
    std::string pageInfo = " page: " + std::to_string(_page + 1) + " of " +
                           std::to_string(numPages());
    insert(pageInfo, _winSzY - 1 - correction,
           _winSzX - correction - pageInfo.length(), _titleStyle);
  }

  for (size_t i = 0; i < _f; ++i) {
    std::string fieldName = subFields[i];

    if (_showBorder == 0) {
      correction = 0;
    } else if (_showBorder == 1) {
      correction = 1;
    }

    if (_xAlign == 1) {
      // is to be subtracted by longest string of field length
      size_t right = _winSzX - correction - fieldName.length();
      xCorrect = right;
    } else if (_xAlign == 0) {
      // is to be subtracted by ( fields[i].length() ) / 2
      size_t h_centre = (_winSzX - fieldName.length()) / 2;
      xCorrect = h_centre;
    } else if (_xAlign == -1)
      xCorrect = correction; // assume border
    if (_yAlign == 1) {
      // is to be added by i
      size_t top = _showTitle ? 2 : 1;
      yCorrect = top + i - (size_t)(!_showBorder);
    } else if (_yAlign == 0) {
      // is to be added by i
      size_t v_centre = (_winSzY - _f) / 2;
      yCorrect = v_centre + i;
    } else if (_yAlign == -1) {
      // is to be added by i
      size_t bottom = _winSzY - correction - _f + i;
      yCorrect = bottom;
    }

    if (i == _highlighted)
      wattron(_win, A_REVERSE);
    mvwprintw(_win, yCorrect, xCorrect, _addFieldPadding(fieldName).c_str());
    wattroff(_win, A_REVERSE);
  }
}

/// MUTATOR RETRO1ACTIVE
void Menu::loadFieldAlignment(int const x, int const y) {
  _xAlign = x;
  _yAlign = y;
}

/// MUTATOR RETROACTIVE
void Menu::fieldStyle(std::string const &style) { _fieldStyle = style; }

/// MUTATOR RETROACTIVE
void Menu::paginate(size_t const entriesPerPage, bool showPages) {
  _showPages = showPages;
  if (entriesPerPage < 0)
    throw std::runtime_error(
        "Cannot have negative value number of entries per page.");
  _entriesPerPage = entriesPerPage;
}

size_t Menu::_pCoeff() const {
  size_t pCoeff = _entriesPerPage == 0 ? _fieldSz : _entriesPerPage;
  return pCoeff;
}
size_t Menu::_pQuot() const {
  size_t pQuot = _fieldSz / _pCoeff();
  return pQuot;
}
size_t Menu::_pRem() const {
  size_t pRem = (size_t)(_fieldSz % _pCoeff());
  return pRem;
}
size_t Menu::numPages() const {
  size_t p = _pQuot() + (_pRem() != 0);
  return p;
}

/// MUTATOR RETROACTIVE
void Menu::hideTitle() {
  std::string tPadding(_winSzX, ' ');
  mvwprintw(_win, _winPosY, _winPosX, tPadding.c_str());
  // metadata tells other functions to now disregard title space
  _showTitle = 0;
}

void Menu::hideBorder() {
  // TODO: if title is showing, move the title up one level.
  borderStyle(' ');
  _showBorder = 0;
}

/// sets the title for the window with an optional style option (default none).
/// This will not show the title to screen on window refresh if the the tite is
/// hidden.
void Menu::loadTitle(std::string const &title, attr_t const style) {
  _title = title;
  _titleStyle = style;
}

void Menu::loadTitleStyle(attr_t style) { _titleStyle = style; }

void Menu::showTitle() {

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
  print(header, _titleStyle);
};

void Menu::_updateM() {
  for (const std::string field : _fields) {
    size_t len = field.length();
    if (len > _m)
      _m = len;
  }
}

std::string Menu::_addFieldPadding(std::string const &fieldName) {
  const size_t newStrLen = _m % 2 == 0 ? _m : _m + 1;
  const size_t oldStrLen = fieldName.size(); // retrieve size of field name
  std::string newStr(newStrLen, ' ');        // make empty new field
  size_t correction = (newStrLen - oldStrLen) / 2;
  newStr.replace(correction, oldStrLen, fieldName);
  newStr.insert(0, _fieldStyle);
  newStr.append(_fieldStyle);
  return newStr;
}

/// MF
char Menu::getCharFromWin(size_t const y, size_t const x,
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
void Menu::erase(size_t const y1, size_t const x1, size_t const y2,
                 size_t const x2) {
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

void Menu::erase(bool titleBar) { fill(' ', titleBar); }

void Menu::clear() {
  wclear(_win);
  wrefresh(_win);
}

/// MF RETROACTIVE
void Menu::fill(char const ch, bool const titleBar) {
  size_t start = titleBar + _showBorder;
  size_t end = _winSzY - (2 * _showBorder) - titleBar;
  size_t width = _winSzX - (2 * _showBorder);

  std::string fillString(width, ch);
  for (size_t i = start; i <= end; ++i) {
    mvwprintw(_win, i, _showBorder, fillString.c_str());
  }
  wrefresh(_win);
}

/// MF ACTIVE
void Menu::eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                       size_t const x2) {

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
void Menu::erase(std::vector<size_t> const &elements) {
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
void Menu::eraseExcept(std::vector<size_t> const &elements) {
  size_t numOfAreas = elements.size() / 4; /*two coordinates per block*/
  for (size_t areaIdx = 0; areaIdx < numOfAreas; ++areaIdx) {

    size_t y1 = elements[0 + (areaIdx * 4)];
    size_t x1 = elements[1 + (areaIdx * 4)];
    size_t y2 = elements[2 + (areaIdx * 4)];
    size_t x2 = elements[3 + (areaIdx * 4)];

    eraseExcept(y1, x1, y2, x2);
  }
}

void Menu::refresh() { wrefresh(_win); }

void Menu::setWin(WIN_SET_CODE const init) {

  if (init == WIN_SET_CODE::INIT_PARENT) {
    initscr();
    curs_set(0);
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

int Menu::resize(size_t const y, size_t const x) {
  _winSzY = y;
  _winSzX = x;
  wresize(_win, _winSzY, _winSzX);
  return 1;
}

int Menu::reposition(size_t const y, size_t const x) {
  _winPosY = y;
  _winPosX = x;
  mvwin(_win, _winPosY, _winPosX);
  return 1;
}

void Menu::print(std::string const &str, attr_t style) {
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

void Menu::print(std::string const &format, std::string const &str,
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

void Menu::moveCursor(int y, int x) {
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

void Menu::setKeypad(bool x) { keypad(_win, x); }

void Menu::resetHighlighted() { _highlighted = 0; }

void Menu::backPage() {
  _page--;
  resetHighlighted();
};

void Menu::forwardPage() {
  _page++;
  resetHighlighted();
};

void Menu::moveHighlightDown() { _highlighted++; }

void Menu::moveHighlightUp() { _highlighted--; }

size_t Menu::numFieldsThisPage() const { return _f; }

size_t Menu::highlighted() const { return _highlighted; }

void Menu::eraseWin() { werase(_win); }

void Menu::display(std::vector<int> const &breakConditions,
                   std::vector<size_t> const &ignoreBlocks) {
  setKeypad(true); // enaable keypad
  int selection;
  resetHighlighted();

  while (true) {

    loadFields(); // <-
    // refresh before getch
    wrefresh(_win);              // <- refresh
    selection = wgetch(_win);    // getCharFromUser()
    _lastKeyPressed = selection; // external
    switch (selection) {
    case KEY_LEFT:
      if (_page != 0) {   // page()
        _page--;          // backPage()
        _highlighted = 0; // part of backPage()
        eraseExcept(ignoreBlocks);
      }
      break;
    case KEY_RIGHT:
      if (_page != numPages() - 1) { // page() numPartitions()
        _page++;                     // forwardPage()
        _highlighted = 0;            // part of forwardPage
        eraseExcept(ignoreBlocks);
      }
      break;
    case KEY_UP:
      _highlighted--;
      if (_highlighted < 0) { // highlighted() NOT highlighted map
        _highlighted++;       // moveHighlightUp()
      }
      break;
    case KEY_DOWN:
      _highlighted++;           // moveHighlightDown()
      if (_highlighted == _f) { // numFieldsThisPage()
        _highlighted--;         // moveHighlightUp()
      }
      break;
    default:
      break;
    }

    bool exitStatus = 0;
    for (const int i : breakConditions) {
      if ((int)selection == i) {
        exitStatus = 1;
      }
    }
    if (exitStatus) {
      break;
    }
  }
}

/// MF RETROACTIVE PRIVATE
std::vector<std::string> Menu::_loadPage() {

  if (_fieldSz == 0)
    throw std::runtime_error("No fields were set.");
  if (numPages() == 1)
    return _fields;

  std::vector<std::string>::const_iterator first_iterator;
  std::vector<std::string>::const_iterator last_iterator;

  size_t remainder = _pRem();

  size_t first = _pCoeff() * _page;
  first_iterator = _fields.begin() + first;
  if (_f != remainder) {
    size_t last = first + _f;
    last_iterator = _fields.begin() + last;
  } else {
    last_iterator = _fields.end();
  }

  /*
  may need to make this check if page is navigated by command in future
  implementation. But current implementation is increment only, and checks are
  already made for bounds.

  size_t const max = _fields.size();
  size_t const pages = max / _pCoeff;

    if (first > pages)
      first = pages * _pCoeff;
    if (last > max)
      last = max;
  */

  // extract subvector. TODO: workaround O(N) for large data?
  // extracts up to but not including last selected.

  std::vector<std::string> subFields(first_iterator, last_iterator);
  return subFields;
}

Menu::~Menu() {
  // TODO: store init mode and kill correspondingly
  if (windowSet())
    setWin(WIN_SET_CODE::KILL_PARENT);
}

} // namespace DisplayKernel
} // namespace BlackOS
