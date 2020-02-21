//
// Created by Takudzwa Makoni on 2019-07-27.
//

#include "../inc/Kmenu.h"
#include "../inc/Directives.h"
#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

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

Kmenu::Kmenu(std::string const &name, size_t const sizeY, size_t const sizeX,
             size_t const posY, size_t const posX) {
  _name = name;
  _winSzY = sizeY;
  _winSzX = sizeX;
  _winPosY = posY;
  _winPosX = posX;
}

size_t Kmenu::_highlightedMap() const {
  size_t const map = (_pCoeff() * _page) + _highlighted;
  return map;
}

/// ACCESSOR
std::vector<Kfield> Kmenu::fields() const { return _fields; }

size_t Kmenu::winSzY() const { return _winSzY; }

size_t Kmenu::winSzX() const { return _winSzX; }

size_t Kmenu::winPosY() const { return _winPosY; }

size_t Kmenu::winPosX() const { return _winPosX; }

/// ACCESSOR
std::string Kmenu::winType() const { return "Kmenu"; }

/// ACCESSOR
std::string Kmenu::name() const { return _name; }

/// ACCESSOR
Kfield Kmenu::selectedField() const {
  size_t map = _highlightedMap();
  return this->_fields[map];
}

/// ACCESSOR
std::vector<size_t> Kmenu::maxSize() const {
  // supposed to be size of COLS and ROWS
  size_t rows, cols;
  getmaxyx(_win, rows, cols);
  std::vector<size_t> termSz{rows, cols};
  return termSz;
}

void Kmenu::insert(std::string const &str, size_t const y, size_t const x) {
  mvwprintw(_win, y, x, str.c_str());
}
void Kmenu::insert(char const *ch, size_t const y, size_t const x) {
  mvwprintw(_win, y, x, ch);
}
void Kmenu::insert(char const ch, size_t const y, size_t const x) {
  char const *chstr = &ch;
  mvwprintw(_win, y, x, chstr);
}
/// MUTATOR RETROACTIVE
void Kmenu::borderStyle(int const ch) {
  _showBorder = 1;
  wborder(_win, ch, ch, ch, ch, ch, ch, ch, ch);
}

void Kmenu::pause() const { wgetch(_win); }

/// MUTATOR RETROACTIVE
void Kmenu::borderStyle(int const L, int const R, int const T, int const B,
                        int const TL, int const TR, int const BL,
                        int const BR) {
  _showBorder = 1;
  wborder(_win, L, R, T, B, TL, TR, BL, BR);
}

bool Kmenu::windowSet() const { return _win != nullptr; }

void Kmenu::_updateF() {
  size_t p = _p();
  size_t pRem = _pRem();
  size_t pCoeff = _pCoeff();
  if (_page == (p - 1) && pRem != 0)
    _f = pRem;
  else
    _f = pCoeff;
}

/// MUTATOR RETROACTIVE
void Kmenu::loadFields(std::vector<Kfield> const &fields) {
  _fields = fields;
  _fieldSz = fields.size();

  // load defaults on new kFields
  _page = 0;

  _updateM();
  _updateF();
}

void Kmenu::_checkRange(size_t const y1, size_t const x1, size_t const y2,
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

void Kmenu::_checkRange(size_t const y, size_t const x) const {
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

void Kmenu::_loadFields() {

  _updateF();
  std::vector<Kfield> subFields = _loadPage();
  size_t xCorrect;
  size_t yCorrect;

  if (_showTitle) {
    std::string pageInfo = "page: " + std::to_string(_page + 1) + " of " +
                           std::to_string(_p()) + " * ";
    _addToTitleHeader(pageInfo);
  }

  for (size_t i = 0; i < _f; ++i) {

    std::string fieldName = subFields[i].name();

    size_t correction = _showBorder ? 1 : 0;

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
      // is to be subtracted by ( fields[i].name().length() ) / 2
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
void Kmenu::alignFields(int const x, int const y) {
  _xAlign = x;
  _yAlign = y;
}

/// MUTATOR RETROACTIVE
void Kmenu::fieldStyle(std::string const &style) { _fieldStyle = style; }

/// MUTATOR RETROACTIVE
void Kmenu::paginate(size_t const entriesPerPage) {
  if (entriesPerPage < 0)
    throw std::runtime_error(
        "Cannot have negative value number of entries per page.");
  if (_fieldSz == 0)
    throw std::runtime_error("field size is zero.");
  // set entriesPerPage member variable
  _entriesPerPage = entriesPerPage;
}

size_t Kmenu::_pCoeff() const {
  size_t pCoeff = _entriesPerPage == 0 ? _fieldSz : _entriesPerPage;
  return pCoeff;
}
size_t Kmenu::_pQuot() const {
  size_t pQuot = _fieldSz / _pCoeff();
  return pQuot;
}
size_t Kmenu::_pRem() const {
  size_t pRem = (size_t)(_fieldSz % _pCoeff() != 0);
  return pRem;
}
size_t Kmenu::_p() const {
  size_t p = _pQuot() + _pRem();
  return p;
}

/// MUTATOR RETROACTIVE
void Kmenu::hideTitle() {
  std::string tPadding(_winSzX, ' ');
  mvwprintw(_win, _winPosY, _winPosX, tPadding.c_str());
  // metadata tells other functions to now disregard title space
  _showTitle = 0;
}

void Kmenu::hideBorder() {
  borderStyle(' ');
  _showBorder = 0;
}

void Kmenu::_addToTitleHeader(std::string const &title) {

  size_t y;
  size_t x;

  if (_showBorder == 0) {
    y = 0;
    x = 0;
  } else if (_showBorder == 1) {
    y = 1;
    x = 1;
  }

  if (_titleStyle == TitleStyle::highlight) {
    wattron(_win, A_REVERSE);
    mvwprintw(_win, y, x, title.c_str());
    wattroff(_win, A_REVERSE);
  } else if (_titleStyle == TitleStyle::underline) {
    wattron(_win, A_UNDERLINE);
    mvwprintw(_win, y, x, title.c_str());
    wattroff(_win, A_UNDERLINE);
  } else /*none*/ {
    mvwprintw(_win, y, x, title.c_str());
  }
}

/// sets the title for the window with an optional style option (default none).
/// This will not show the title to screen on window refresh if the the tite is
/// hidden.
void Kmenu::loadTitle(std::string const &title, TitleStyle const style) {
  _title = title;
  _titleStyle = style;
}

void Kmenu::loadTitleStyle(TitleStyle style) { _titleStyle = style; }

void Kmenu::showTitle() {

  _showTitle = 1; // make title attributes visible again

  // add title to window
  if (_showBorder) {
    size_t correction = _winSzX - _title.length() - 3;
    std::string tPadding(_winSzX, ' ');
    tPadding.replace(correction, _winSzX, _title + " ");
    _addToTitleHeader(tPadding);
  } else {
    size_t correction = _winSzX - _title.length() - 1;
    std::string tPadding(_winSzX, ' ');
    tPadding.replace(correction, _winSzX, _title + " ");
    _addToTitleHeader(tPadding);
  }
};

void Kmenu::_updateM() {
  for (const Kfield field : _fields) {
    size_t len = field.name().size();
    if (len > _m)
      _m = len;
  }
}

std::string Kmenu::_addFieldPadding(std::string const &fieldName) {
  const size_t newStrLen = _m % 2 == 0 ? _m : _m + 1;
  const size_t oldStrLen = fieldName.size(); // retrieve size of field name
  std::string newStr(newStrLen, ' ');        // make empty new field
  size_t correction = (newStrLen - oldStrLen) / 2;
  newStr.replace(correction, oldStrLen, fieldName);
  std::string front = _fieldStyle + " ";
  std::string back = " " + _fieldStyle;
  newStr.insert(0, front);
  newStr.append(back);
  return newStr;
}

/// MF
char Kmenu::getCharFromWin(size_t const y, size_t const x,
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
void Kmenu::erase(size_t const y1, size_t const x1, size_t const y2,
                  size_t const x2) {

  _checkRange(y1, x1, y2, x2);
  size_t width = x2 - x1 + 1;
  std::string fill(width, ' ');
  for (size_t i = y1; i <= y2; ++i) {
    mvwprintw(_win, i, x1, fill.c_str());
  }
  wrefresh(_win);
}

void Kmenu::erase(bool titleBar) { fill(' ', titleBar); }

void Kmenu::clear() { wclear(_win); }

/// MF RETROACTIVE
void Kmenu::fill(char const ch, bool const titleBar) {
  size_t correction = (size_t)(!_showBorder);
  size_t start = titleBar ? 2 : 1;
  size_t end = _winSzY - 2;
  size_t width = _winSzX - 2;

  std::string fillString(width, ch);
  for (size_t i = start; i <= end; ++i) {
    mvwprintw(_win, i, 1, fillString.c_str());
  }
}

/// MF ACTIVE
void Kmenu::eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                        size_t const x2) {

  _checkRange(y1, x1, y2, x2);

  size_t width = _winSzX - 2;
  size_t height = _winSzY - 2;

  std::string fill(width, ' ');
  std::string space = " ";
  for (size_t i = 1; i <= height; ++i) {
    if (i <= y2 && i >= y1) {
      for (size_t j = 1; j <= width; ++j) {
        if (!(j <= x2 && j >= x1)) {
          mvwprintw(_win, i, j, space.c_str());
        }
      }
    } else {
      mvwprintw(_win, i, 1, fill.c_str());
    }
  }
  wrefresh(_win);
}

/// MF ACTIVE
void Kmenu::erase(std::vector<size_t> const &elements) {
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
void Kmenu::eraseExcept(std::vector<size_t> const &elements) {
  size_t width = _winSzX - 2;
  size_t height = _winSzY - 2;
  std::string fill(width, ' ');
  std::string space = " ";
  for (size_t i = 1; i <= height; ++i) {
    std::vector<size_t> blocks = blocksFound(i, elements);
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
  wrefresh(_win);
}

void Kmenu::refresh() { wrefresh(_win); }

/// MF ACTIVE
void Kmenu::label(std::string const &label) const {
  size_t labellocy = _winSzY - 1;
  size_t labellocx = _winSzX - (3 + (size_t)label.length());
  mvwaddstr(_win, labellocy, labellocx, label.c_str());
}

/// MF ACTIVE
void Kmenu::setWin(WINDOW *window) {
  _win = window;
  if (window != nullptr) {
    wresize(_win, _winSzY, _winSzX);
    mvwin(_win, _winPosY, _winPosX);
  }
}

/// MF ACTIVE
void Kmenu::display() {
  keypad(_win, true);
  size_t selection;

  _highlighted = 0;

  while (true) {

    _loadFields();
    // refresh before getch
    wrefresh(_win);
    selection = wgetch(_win);

    switch (selection) {
    case KEY_LEFT:
      if (_page != 0) {
        _page--;
        _highlighted = 0;
        erase(1 + (size_t)(_showBorder), 0 + (size_t)(_showBorder),
              _winSzY - 1 - (size_t)(_showBorder),
              _winSzX - 1 - (size_t)(_showBorder));
      }
      break;
    case KEY_RIGHT:
      if (_page != _p() - 1) {
        _page++;
        _highlighted = 0;

        erase(1 + (size_t)(_showBorder), 0 + (size_t)(_showBorder),
              _winSzY - 1 - (size_t)(_showBorder),
              _winSzX - 1 - (size_t)(_showBorder));
      }
      break;
    case KEY_UP:
      if (_highlighted != 0) {
        _highlighted--;
      }
      break;
    case KEY_DOWN:
      _highlighted++;
      if (_highlighted == _f) {
        _highlighted--;
      }
      break;
    default:
      break;
    }
    if (selection == 10) {
      break;
    }
  }
}

/// MF ACTIVE PRIVATE
void Kmenu::_delWith(std::vector<WINDOW *> windows) {
  if (!windows.empty())
    for (std::vector<WINDOW *>::iterator it = windows.begin();
         it != windows.end(); ++it) {
      delwin(*it);
    }
}

/// MF RETROACTIVE PRIVATE
std::vector<Kfield> Kmenu::_loadPage() {

  if (_fieldSz == 0)
    throw std::runtime_error("No fields were set.");
  if (_p() == 1)
    return _fields;

  std::vector<Kfield>::const_iterator first_iterator;
  std::vector<Kfield>::const_iterator last_iterator;

  size_t first = _pCoeff() * _page;
  first_iterator = _fields.begin() + first;
  if (_f != _pRem()) {
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

  std::vector<Kfield> subFields(first_iterator, last_iterator);
  return subFields;
}

Kmenu::~Kmenu() {
  _delWith(_subwins); // delete windows created by Kmenu
  setWin();           // diisociate with window
}

} // namespace DisplayKernel
} // namespace BlackOS
