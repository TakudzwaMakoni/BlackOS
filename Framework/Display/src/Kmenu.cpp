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
namespace Display {

Kmenu::Kmenu(std::string const &name, size_t const sizeY, size_t const sizeX,
             size_t const posY, size_t const posX) {
  _size = {sizeY, sizeX};
  _position = {posY, posX};
}

size_t Kmenu::_highlightedMap() const {
  size_t map = (_pCoeff * _page) + _highlighted;
  return map;
}

/// ACCESSOR
std::vector<Kfield> Kmenu::fields() const { return _fields; }

/// ACCESSOR
std::vector<size_t> Kmenu::size() const { return _size; }

/// ACCESSOR
std::vector<size_t> Kmenu::position() const { return _position; }

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
/// MUTATOR RETROACTIVE
void Kmenu::borderStyle(int const ch) {
  wborder(_win, ch, ch, ch, ch, ch, ch, ch, ch);
}

void Kmenu::pause() const { wgetch(_win); }

/// MUTATOR RETROACTIVE
void Kmenu::borderStyle(int const L, int const R, int const T, int const B,
                        int const TL, int const TR, int const BL,
                        int const BR) {
  wborder(_win, L, R, T, B, TL, TR, BL, BR);
}

bool Kmenu::windowSet() const { return _win != nullptr; }

void Kmenu::_updateF() {
  if (_page == (_p - 1) && _pRem != 0)
    _f = _pRem;
  else
    _f = _pCoeff;
}

/// MUTATOR RETROACTIVE
void Kmenu::setFields(std::vector<Kfield> const &fields) {
  _fields = fields;
  _fieldSz = fields.size();

  // load defaults on new kFields
  _page = 0;
  _pCoeff = _fieldSz;
  _pQuot = 1;
  _pRem = 0;
  _p = _pQuot + _pRem;
  _updateF();
}

void Kmenu::_setFields() {

  _updateF();
  std::vector<Kfield> subFields = _loadPage();
  size_t xCorrect;
  size_t yCorrect;

  if (_showTitle) {
    std::string pageInfo = "page: " + std::to_string(_page + 1) + " of " +
                           std::to_string(_p) + " * ";
    wattron(_win, A_REVERSE);
    mvwprintw(_win, _position[0], _position[1], pageInfo.c_str());
    wattroff(_win, A_REVERSE);
  }

  for (size_t i = 0; i < _f; ++i) {

    std::string fieldName = subFields[i].name();

    if (_xAlign == 1) {
      // is to be subtracted by longest string of field length
      size_t right = _size[1] - 1 - fieldName.length();
      xCorrect = right;
    } else if (_xAlign == 0) {
      // is to be subtracted by ( fields[i].name().length() ) / 2
      size_t h_centre = (_size[1] - fieldName.length()) / 2;
      xCorrect = h_centre;
    } else if (_xAlign == -1)
      xCorrect = 1; // assume border
    if (_yAlign == 1) {
      // is to be added by i
      size_t top = _showTitle ? 2 : 1;
      yCorrect = top + i;
    } else if (_yAlign == 0) {
      // is to be added by i
      size_t v_centre = (_size[0] - _f) / 2;
      yCorrect = v_centre + i;
    } else if (_yAlign == -1) {
      // is to be added by i
      size_t bottom = _size[0] - 2 - _f + i;
      yCorrect = bottom + i;
    }

    if (i == _highlighted)
      wattron(_win, A_REVERSE);
    mvwprintw(_win, yCorrect, xCorrect, fieldName.c_str());
    wattroff(_win, A_REVERSE);
  }
}

/// MUTATOR RETROACTIVE
void Kmenu::alignFields(int const x, int const y) {
  _xAlign = x;
  _yAlign = y;
}

/// MUTATOR RETROACTIVE
void Kmenu::setFieldStyle(std::string const &style) { _fieldStyle = style; }

/// MUTATOR RETROACTIVE
void Kmenu::paginate(size_t const entriesPerPage) {
  if (entriesPerPage < 0)
    throw std::runtime_error(
        "Cannot have negative value number of entries per page.");
  if (_fieldSz == 0)
    throw std::runtime_error("no fields were set.");

  // _pCoeff means display all on window.
  _pCoeff = entriesPerPage == 0 ? _fieldSz : entriesPerPage;
  _pQuot = _fieldSz / _pCoeff;
  _pRem = (size_t)(_fieldSz % _pCoeff != 0);
  _p = _pQuot + _pRem;
  _updateF();
}

/// MUTATOR RETROACTIVE
void Kmenu::hideTitle() {
  std::string tPadding(_size[1], ' ');
  mvwprintw(_win, _position[0], _position[1], tPadding.c_str());
  // metadata tells other functions to now disregard title space
  _showTitle = false;
}

void Kmenu::_addTitle() {
  int correction = _size[1] - _title.length() - 3;
  std::string tPadding(_size[1], ' ');
  tPadding.replace(correction, _size[1], _title + " ");
  wattron(_win, A_REVERSE);
  mvwprintw(_win, _position[0], _position[1], tPadding.c_str());
  wattroff(_win, A_REVERSE);
};

/// MUTATOR RETROACTIVE
void Kmenu::addTitle(std::string const &title) {
  _title = title;
  _addTitle();
}

/// MUTATOR RETROACTIVE
void Kmenu::addFieldPadding() {
  int maxLen = 1;
  for (const Kfield field : _fields) {
    int len = field.name().size();
    if (len > maxLen)
      maxLen = len;
  }
  for (std::vector<Kfield>::iterator it = _fields.begin(); it != _fields.end();
       ++it) {
    std::string str = it->name(); // retrieve field name
    const int newStrLen = maxLen % 2 == 0 ? maxLen : maxLen + 1;
    const int oldStrLen = str.size();   // retrieve size of field name
    std::string newStr(newStrLen, ' '); // make empty new field
    int correction = (newStrLen - oldStrLen) / 2;
    newStr.replace(correction, oldStrLen, str);
    std::string front = _fieldStyle + " ";
    std::string back = " " + _fieldStyle;
    newStr.insert(0, front);
    newStr.append(back);
    it->setName(newStr); // assign padded name to field
  }
}

/// MF
int Kmenu::getChrfromW(size_t const y, size_t const x,
                       bool const save_cursor) const {
  size_t curr_y, curr_x;
  getyx(_win, curr_y, curr_x);
  int ch = mvwinch(_win, y, x);
  if (save_cursor)
    wmove(_win, curr_y, curr_x);
  return ch;
}

/// MF ACTIVE
void Kmenu::kErase(size_t const y1, size_t const x1, size_t const y2,
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
  wrefresh(_win);
}

/// MF ACTIVE
void Kmenu::kEraseExcept(size_t const y1, size_t const x1, size_t const y2,
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
  wrefresh(_win);
}

/// MF ACTIVE
void Kmenu::kErase(std::vector<size_t> const &elements) {
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
    kErase(_y1, _x1, _y2, _x2);
  }
}

/// MF ACTIVE
void Kmenu::kEraseExcept(std::vector<size_t> const &elements) {
  size_t borderY = _size[0];
  size_t borderX = _size[1];
  size_t width = borderX - 2;
  size_t height = borderY - 2;
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
  size_t labellocy = _size[0] - 1;
  size_t labellocx = _size[1] - (3 + (size_t)label.length());
  mvwaddstr(_win, labellocy, labellocx, label.c_str());
}

/// MF ACTIVE
void Kmenu::setWin(WINDOW *window) {
  _win = window;
  if (window != nullptr) {
    wresize(_win, _size[0], _size[1]);
    mvwin(_win, _position[0], _position[1]);
  }
}

/// MF RETROACTIVE
void Kmenu::fill(char const ch, bool const titleBar) {
  size_t start = titleBar ? 2 : 1;
  size_t end = _size[0] - 2;

  std::string fillString(_size[1] - 2, ch);
  for (size_t i = start; i <= end; ++i) {
    mvwprintw(_win, i, 1, fillString.c_str());
  }
}

/// MF ACTIVE
void Kmenu::display() {
  keypad(_win, true);
  size_t selection;

  _highlighted = 0;

  while (true) {

    _setFields();
    // refresh before getch
    wrefresh(_win);
    selection = wgetch(_win);

    switch (selection) {
    case KEY_LEFT:
      if (_page != 0) {
        _page--;
        _highlighted = 0;
        fill(' ', _showTitle);
      }
      break;
    case KEY_RIGHT:
      if (_page != _p - 1) {
        _page++;
        _highlighted = 0;
        fill(' ', _showTitle);
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
  if (_p == 0)
    return _fields;

  std::vector<Kfield>::const_iterator first_iterator;
  std::vector<Kfield>::const_iterator last_iterator;

  size_t first = _pCoeff * _page;
  first_iterator = _fields.begin() + first;
  if (_f != _pRem) {
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

} // namespace Display
} // namespace BlackOS
