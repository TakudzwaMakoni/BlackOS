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
std::vector<int> blocksFound(const int yValue,
                             const std::vector<int> &elements) {
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

Kmenu::Kmenu(std::string const &name, int sizeY, int sizeX, int posY,
             int posX) {
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
WINDOW *Kmenu::window() const { return this->_win; };

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

/// ACCESSOR
std::string Kmenu::attributeString() {
  std::string str;
  for (std::vector<std::string>::iterator it = _attributes.begin();
       it != _attributes.end(); ++it) {
    str += " " + *it;
  }
  return str;
}

/// MUTATOR RETROACTIVE
void Kmenu::borderStyle(const int ch) {
  _borderStyle = {ch, ch, ch, ch, ch, ch, ch, ch};
}

/// MUTATOR RETROACTIVE
void Kmenu::borderStyle(const int L, const int R, const int T, const int B,
                        const int TL, const int TR, const int BL,
                        const int BR) {
  _borderStyle = {L, R, T, B, TL, TR, BL, BR};
}

/// MUTATOR RETROACTIVE
void Kmenu::setFields(const std::vector<Kfield> &fields) {
  this->_fields = fields;
}

/// MUTATOR RETROACTIVE
void Kmenu::setFieldAlign(int x, int y) {
  _xAlign = x;
  _yAlign = y;
}

/// MUTATOR RETROACTIVE
void Kmenu::setFieldStyle(std::string style) { this->_fieldStyle = style; }

/// MUTATOR RETROACTIVE
void Kmenu::paginate(int divisor) { _pagination = divisor; }

/// MUTATOR RETROACTIVE
void Kmenu::showTitle(bool show) { _showTitle = show; }

/// MUTATOR RETROACTIVE
void Kmenu::setTitle(std::string title) { _title = title; }

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

/// MF ACTIVE
void Kmenu::kErase(const int y1, const int x1, const int y2, const int x2) {
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
void Kmenu::kEraseExcept(const int y1, const int x1, const int y2,
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
  wrefresh(_win);
}

/// MF ACTIVE
void Kmenu::kErase(const std::vector<int> &elements) {
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
void Kmenu::kEraseExcept(const std::vector<int> &elements) {
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

/// MF ACTIVE
void Kmenu::label(const std::string &label) const {
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
  wrefresh(_win);
}

/// MF ACTIVE
void Kmenu::setBorderStyle() {
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
  wrefresh(_win);
}

/// MF ACTIVE
void Kmenu::fill(char ch) {
  std::string fillString(_size[1] - 2, ch);
  for (int i = 1; i <= _size[0]; ++i) {
    mvwprintw(_win, i, 1, fillString.c_str());
  }
  wrefresh(_win);
}

/// MF ACTIVE
void Kmenu::wipe(bool titleBar) {
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

/// MF ACTIVE
void Kmenu::display() {
  keypad(_win, true);
  int selection;
  int highlighted = 0;
  const int numOfFields = _fields.size();
  setBorderStyle();
  int test = _pagination;
  const int pages = numOfFields / _pagination;
  const int residue = numOfFields % _pagination;
  int totalPages = residue == 0 ? pages : pages + 1;
  auto paginatedFields = _paginate(pages, residue);
  int page = 0;
  while (true) {
    if (_showTitle) {
      _attributes = {"page: ", std::to_string(page + 1), " of ",
                     std::to_string(totalPages)};
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
    wipe(_showTitle);
    std::vector<Kfield> fields = paginatedFields[page];
    int numOfDisplayFields = fields.size();
    for (int i = 0; i < numOfDisplayFields; ++i) {
      if (i == highlighted)
        wattron(_win, A_REVERSE);
      int yAlign = 0;
      int xAlign = 0;
      int left, right, top, bottom, v_centre, h_centre;
      left = 1;
      int longest_string_len = 0;
      std::for_each(fields.begin(), fields.end(),
                    [&longest_string_len](const Kfield &field) {
                      std::string str = field.name();
                      int len = str.length();
                      if (len > longest_string_len)
                        longest_string_len = len;
                    });
      right = _size[1] - longest_string_len - 1;
      v_centre = i + (_size[0] - numOfDisplayFields) / 2;
      h_centre = (_size[1] - fields[i].name().length()) / 2;
      top = i + 1;
      bottom = _size[0] - 2 - numOfDisplayFields + i;
      if (_xAlign == 1)
        xAlign = right;
      else if (_xAlign == 0)
        xAlign = h_centre;
      else if (_xAlign == -1)
        xAlign = left;
      if (_yAlign == 1)
        yAlign = top;
      else if (_yAlign == 0)
        yAlign = v_centre;
      else if (_yAlign == -1)
        yAlign = bottom;
      mvwprintw(_win, yAlign, xAlign, fields[i].name().c_str());
      wattroff(_win, A_REVERSE);
      wrefresh(_win);
    }
    selection = wgetch(_win);
    switch (selection) {
    case KEY_LEFT:
      page--;
      highlighted = 0;
      if (page == -1) {
        page = 0;
      }
      kErase(2, 2, _size[0] - 3, _size[1] - 3);
      break;
    case KEY_RIGHT:
      page++;
      highlighted = 0;
      if (page == paginatedFields.size()) {
        page--;
      }
      kErase(2, 2, _size[0] - 3, _size[1] - 3);
      break;
    case KEY_UP:
      highlighted--;
      if (highlighted == -1) {
        highlighted++;
      }
      break;
    case KEY_DOWN:
      highlighted++;
      if (highlighted == fields.size()) {
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
  wrefresh(_win);
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
std::vector<std::vector<Kfield>> Kmenu::_paginate(int const pages,
                                                  int const residue) {
  int max = _fields.size();
  if (_fields.empty())
    throw std::runtime_error("Trying to paginate set of empty fields.");
  bool inRange = 0 < _pagination && _pagination <= max;
  _pagination = inRange ? _pagination : max;
  std::vector<std::vector<Kfield>> paginatedFields;
  for (int page = 0; page < pages; ++page) {
    std::vector<Kfield> sub(_fields.begin() + (page * _pagination),
                            _fields.begin() + ((page + 1) * _pagination));
    paginatedFields.push_back(sub);
  }
  if (residue != 0) {
    std::vector<Kfield> residualSub(_fields.begin() + (_pagination * pages),
                                    _fields.end());
    paginatedFields.push_back(residualSub);
  }
  return paginatedFields;
}

Kmenu::~Kmenu() {
  _delWith(_subwins);
  setWin();
}

} // namespace Display
} // namespace BlackOS
