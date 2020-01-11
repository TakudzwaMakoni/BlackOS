//
// Created by Takudzwa Makoni on 2019-07-27.
//

#include "../inc/Kmenu.h"
#include "../inc/Directives.h"
#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace {
const int PADDING = 1;

/// SUBROUTINE ANONYMOUS
std::vector<int> blocksFound(int const yValue,
                             std::vector<int> const &elements) {
  int numOfBlocks = elements.size() / 4; /*two coordinates per block*/
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

/// SUBROUTINE ANONYMOUS
bool inBlocks(int const xValue, std::vector<int> const &blocks,
              std::vector<int> const &elements) {
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

Kmenu::Kmenu(std::string const &name, int const sizeY, int const sizeX,
             int const posY, int const posX) {
  _size = {sizeY, sizeX};
  _position = {posY, posX};
}

/// ACCESSOR
std::vector<Kfield> Kmenu::fields() const { return _fields; }

/// ACCESSOR
std::vector<int> Kmenu::size() const { return _size; }

/// ACCESSOR
Eigen::Vector2i Kmenu::position() const { return _position; }

/// ACCESSOR
std::string Kmenu::winType() const { return "Kmenu"; }

/// ACCESSOR
std::string Kmenu::name() const { return _name; }

/// ACCESSOR
Kfield Kmenu::getSelectedField() const { return this->_fields[_highlighted]; }

/// ACCESSOR
std::vector<int> Kmenu::maxSize() const {
  int winSzY, winSzX;
  winSzX = getmaxx(_win);
  std::vector<int> winSz{winSzY, winSzX};
  return winSz;
}

/// MUTATOR RETROACTIVE
void Kmenu::borderStyle(int const ch) {
  _borderStyle = {ch, ch, ch, ch, ch, ch, ch, ch};
  wborder(_win, ch, ch, ch, ch, ch, ch, ch, ch);
}

/// MUTATOR RETROACTIVE
void Kmenu::borderStyle(int const L, int const R, int const T, int const B,
                        int const TL, int const TR, int const BL,
                        int const BR) {
  _borderStyle = {L, R, T, B, TL, TR, BL, BR};
  wborder(_win, L, R, T, B, TL, TR, BL, BR);
}

/// MUTATOR RETROACTIVE
void Kmenu::setFields(std::vector<Kfield> const &fields) {
  _fields = fields;
  _p_fields = {fields};
  if (_pagination != 1)
    _paginate();
}

/// MUTATOR RETROACTIVE
void Kmenu::setFieldAlign(int const x, int const y) {

  int left, right, top, bottom, v_centre, h_centre;
  left = 1;
  int longest_string_len = 0;
  std::for_each(subFields.begin(), subFields.end(),
                [&longest_string_len](const Kfield &subField) {
                  std::string str = subField.name();
                  int len = str.length();
                  if (len > longest_string_len)
                    longest_string_len = len;
                });
  right = _size[1] - longest_string_len - 1;
  v_centre = i + (_size[0] - numOfSubFields) / 2;
  h_centre = (_size[1] - subFields[i].name().length()) / 2;
  top = i + 1;
  bottom = _size[0] - 2 - numOfSubFields + i;
  if (x == 1)
    _xAlign = right;
  else if (x == 0)
    _xAlign = h_centre;
  else if (x == -1)
    _xAlign = left;
  if (y == 1)
    _yAlign = top;
  else if (y == 0)
    _yAlign = v_centre;
  else if (y == -1)
    _yAlign = bottom;
}

/// MUTATOR RETROACTIVE
void Kmenu::setFieldStyle(std::string const &style) { _fieldStyle = style; }

/// MUTATOR RETROACTIVE
void Kmenu::paginate(int const divisor) {
  _pagination = divisor;
  _paginate();
}

/// MUTATOR RETROACTIVE
void Kmenu::showTitle(bool const show) { _showTitle = show; }

/// MUTATOR RETROACTIVE
void Kmenu::setTitle(std::string const &title) {
  _title = title;
  if (_showTitle) {
    int correction = _size[1] - _title.length() - 3;
    std::string tPadding(_size[1], ' ');
    tPadding.replace(correction, _size[1], _title + " ");
    wattron(_win, A_REVERSE);
    mvwprintw(_win, _position[0], _position[1], tPadding.c_str());
    wattroff(_win, A_REVERSE);
  }
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
int Kmenu::getChrfromW(int const y, int const x,
                       bool preserve_cursor_pos) const {
  int curr_y, curr_x;
  getyx(_win, curr_y, curr_x);
  int wch = mvwinch(_win, y, x);
  if (preserve_cursor_pos)
    wmove(_win, curr_y, curr_x);
  return wch;
}

/// MF ACTIVE
void Kmenu::kErase(int const y1, int const x1, int const y2, int const x2) {
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
  wrefresh(_win);
}

/// MF ACTIVE
void Kmenu::kEraseExcept(int const y1, int const x1, int const y2,
                         int const x2) {
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
  wrefresh(_win);
}

/// MF ACTIVE
void Kmenu::kErase(std::vector<int> const &elements) {
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

/// MF ACTIVE
void Kmenu::kEraseExcept(std::vector<int> const &elements) {
  int borderY = _size[0];
  int borderX = _size[1];
  int width = borderX - 2;
  int height = borderY - 2;
  std::string fill(width, ' ');
  std::string space = " ";
  for (int i = 1; i <= height; ++i) {
    std::vector<int> blocks = blocksFound(i, elements);
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
  wrefresh(_win);
}

void Kmenu::refresh() { wrefresh(_win); }

/// MF ACTIVE
void Kmenu::label(std::string const &label) const {
  int labellocy = _size[0] - 1;
  int labellocx = _size[1] - (3 + (int)label.length());
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
void Kmenu::fill(char ch, bool titleBar) {
  int start = titleBar ? 2 : 1;
  int end = _size[0] - 2;

  std::string fillString(_size[1] - 2, ch);
  for (int i = start; i <= end; ++i) {
    mvwprintw(_win, i, 1, fillString.c_str());
  }
}

/// MF ACTIVE
void Kmenu::display() {
  keypad(_win, true);
  int selection;
  int highlighted = 0;

  // pagination

  while (true) {
    if (_showTitle) {
      std::string pageInfo = "page: " + std::to_string(_page + 1) + " of " +
                             std::to_string(totalPages) + " * ";
      wattron(_win, A_REVERSE);
      mvwprintw(_win, _position[0], _position[1], pageInfo.c_str());
      wattroff(_win, A_REVERSE);
    }
    fill(' ', _showTitle);
    std::vector<Kfield> subFields = _fields[page];
    int numOfSubFields = subFields.size();
    for (int i = 0; i < numOfSubFields; ++i) {
      if (i == highlighted)
        wattron(_win, A_REVERSE);
      mvwprintw(_win, _yAlign, _xAlign, subFields[i].name().c_str());
      wattroff(_win, A_REVERSE);
    }
    // refresh before getch
    wrefresh(_win);
    selection = wgetch(_win);

    switch (selection) {
    case KEY_LEFT:
      _page--;
      highlighted = 0;
      if (page == -1) {
        _page = 0;
      }
      fill(' ', _showTitle);
      break;
    case KEY_RIGHT:
      _page++;
      highlighted = 0;
      if (_page == _fields.size()) {
        _page--;
      }
      fill(' ', _showTitle);
      break;
    case KEY_UP:
      highlighted--;
      if (highlighted == -1) {
        highlighted++;
      }
      break;
    case KEY_DOWN:
      highlighted++;
      if (highlighted == subFields.size()) {
        highlighted--;
      }
      break;
    default:
      break;
    }
    if (selection == 10) {
      break;
    }
  }
  _highlighted = highlighted + (_pagination * page);
}

/// MF ACTIVE PRIVATE
void Kmenu::_delWith(std::vector<WINDOW *> windows) {
  if (!windows.empty())
    for (std::vector<WINDOW *>::iterator it = windows.begin();
         it != windows.end(); ++it) {
      delwin(*it);
    }
  wrefresh(_win);
}

/// MF RETROACTIVE PRIVATE
std::vector<std::vector<Kfield>> Kmenu::_loadPage(int const page) {

  if (_fields.empty())
    throw std::runtime_error("Trying to paginate set of empty fields.");
  if (_pages.empty())
    return _fields;
  if (std::accumulate(_pages.begin(), _pages.end(), (int)0) == _fields.size())
    throw std::runtime_error("pagination does not match size of fields.");

  // extract subvector. TODO: O(N) for large data?? this may be expensive -
  // maybe even pagination is better after all
  int ignore =
      std::accumulate(_pages.begin, _pages.begin() + _pages[page - 1], (int)0);
}

Kmenu::~Kmenu() {
  _delWith(_subwins); // delete windows created by Kmenu
  setWin();           // diisociate with window
}

} // namespace Display
} // namespace BlackOS
