//
// Created by Takudzwa Makoni on 2019-07-27.
//

#include "../inc/Kmenu.h"
#include "../inc/Directives.h"
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace {
const int PADDING = 1;
// WINDOW * DEFAULT_WINDOW = newwin(0,0,0,0);
std::string paddedStr(const std::string &str, const int len) {
  std::string newstr = str;
  const int oldStrLen = str.size();
  int correction = len - oldStrLen;
  std::string padding(" ", correction);
  newstr.insert(0, padding);
  newstr.insert(oldStrLen, padding);
  return newstr;
}

} // namespace

using namespace BlackOSDisplay;

Kmenu::Kmenu(const size_t &id, std::string &name, int sizeY, int sizeX,
             int posY, int posX)
    : _id(id) {
  _size = {sizeY, sizeX};
  _position = {posY, posX};
  setWin();
}

/// return Window Object
WINDOW *Kmenu::window() const { return _win; }
/// return fields in Kmenu instance
std::vector<Kfield> Kmenu::fields() const { return _fields; }
/// return window size of instance
std::vector<int> Kmenu::size() const { return _size; }
/// return window centre x position
int Kmenu::centreX() const { return _size[1] / 2; }
int Kmenu::centreY() const { return _size[0] / 2; }
/// return type of Kwindow instance
std::string Kmenu::winType() const { return "Kmenu"; }
/// return name of Kwindow instance
std::string Kmenu::name() const { return _name; }
/// set style of corners of BlackOS Window
void Kmenu::borderStyle(const int &ch) {
  wborder(_win, ch, ch, ch, ch, ch, ch, ch, ch);
  wrefresh(_win);
}
/// set style for each corner of BlackOS Window
void Kmenu::setBorderStyle(const int &L, const int &R, const int &T,
                           const int &B, const int &TL, const int &TR,
                           const int &BL, const int &BR) {
  wborder(_win, L, R, T, B, TL, TR, BL, BR);
  wrefresh(_win);
}

void Kmenu::setFields(const std::vector<Kfield> &fields) {
  this->_fields = fields;
}
/// get selected field
Kfield Kmenu::getSelectedField() const { return this->_fields[_highlighted]; }
/// set corner label of BlackOS Window
void Kmenu::label(const std::string &label) const {
  int labellocy = _size[0] - 1;
  int labellocx = _size[1] - (3 + (int)label.length());
  mvwaddstr(_win, labellocy, labellocx, label.c_str());
}
size_t Kmenu::getID() const { return this->_id; }
/// return the maximum size of the local window
std::vector<int> Kmenu::maxSize() const {
  int yMax, xMax;
  getmaxyx(_win, yMax, xMax);
  std::vector<int> size{yMax, xMax};
  return size;
}
/// set the position of the menu inside the local window
void Kmenu::setWinPos(int x, int y) {}
/// set field alignment
void Kmenu::setFieldAlign(int x, int y) {
  _xAlign = x;
  _yAlign = y;
}

void Kmenu::fieldPadding(bool paddingOn) {
  if (paddingOn) {
    // establish length of longest field name
    int longest_field_name_len = 0;
    for (const Kfield field : _fields) {
      int len = field.name().size();
      if (len > longest_field_name_len)
        longest_field_name_len = len;
    }
    std::for_each(_fields.begin(), _fields.end(), [&](Kfield f) {
      f.setName(paddedStr(f.name(), longest_field_name_len));
    });
  }
}
/// set animation codes for window start and finish
void Kmenu::setAnimation(const int &start, const int &finish) const {
  _startAnim = start;
  _finishAnim = finish;
}
/// get position of window top left corner
Eigen::Vector2i Kmenu::position() const { return _position; }
/// set the private window to a window
void Kmenu::setWin() {
  _win = newwin(_size[0], _size[1], _position[0], _position[1]);
}
/// TODO : Not in use
void Kmenu::addDisplayObj(BlackOSDisplay::Kwindow &obj) const {
  std::vector<int> childSize = _size;
  std::for_each(childSize.begin(), childSize.end(),
                [](const int &i) { return i - PADDING; });
}
/// display the menu onto private window
void Kmenu::display() {

  setAnimation(0, 0); // TODO: not in use

  keypad(_win, true);
  int selection;
  int highlighted = 0;
  int numOfFields = _fields.size();

  while (true) {
    for (int i = 0; i < numOfFields; i++) {
      if (i == highlighted)
        wattron(_win, A_REVERSE);

      int yAlign = 0;
      int xAlign = 0;
      int left, right, top, bottom, v_centre, h_centre;

      // left align
      left = 1;
      // right align
      int longest_string_len = 0;
      std::for_each(_fields.begin(), _fields.end(),
                    [&longest_string_len](const Kfield &field) {
                      std::string str = field.name();
                      int len = str.length();
                      if (len > longest_string_len)
                        longest_string_len = len;
                    });
      right = _size[1] - longest_string_len - 1;

      // centre align
      v_centre = i + (_size[0] / 2) - numOfFields;
      h_centre = (_size[1] - _fields[i].name().length()) / 2;

      // top align
      top = i + 1;
      bottom = _size[0] - 2 - numOfFields + i;

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

      mvwprintw(_win, yAlign, xAlign, _fields[i].name().c_str());
      wattroff(_win, A_REVERSE);
    }

    selection = wgetch(_win);
    switch (selection) {
    case KEY_UP:
      highlighted--;
      if (highlighted == -1) {
        highlighted = 0;
      }
      break;
    case KEY_DOWN:
      highlighted++;
      if (highlighted == _fields.size()) {
        highlighted = (int)_fields.size() - 1;
      }
    default:
      break;
    }
    if (selection == 10) {
      break;
    }
  }
  _highlighted = highlighted;
  clear();
  wrefresh(_win);
}
Kmenu::~Kmenu() { delwin(_win); }
