//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KGRID_H
#define BLACKOS_KGRID_H

#include "Eigen/Dense"
#include "Kwindow.h"
#include <iomanip>
#include <sstream>
#include <string>

namespace BlackOSDisplay {

/// BlackOS Grid Object
template <typename dataType, size_t rows, size_t cols>
class Kgrid : public Kwindow {
private:
  const std::string _name;
  Eigen::Matrix<dataType, rows, cols> _matrix;
  WINDOW *_win;
  std::vector<WINDOW *> _subwins;
  std::vector<int> _size;
  int _xAlign = 0;
  int _yAlign = 0;
  Eigen::Vector2i _position;
  int _precision;
  int _vPadding{1};
  std::string _title;
  bool _setGrid{false};
  bool _showTitle{false};
  size_t _highlightedRow{0};
  size_t _highlightedCol{0};
  // degrees, precision,
  std::vector<std::string> _attributes;
  const size_t _id; // TODO: need a NODE MAP TO NAVIGATE BETWEEN MENUS
  mutable int m_startAnim, m_finishAnim;
  virtual void setWin() override {
    _win = newwin(_size[0], _size[1], _position[0], _position[1]);
  }
  std::string attributeString() {
    std::string str;
    for (std::vector<std::string>::iterator it = _attributes.begin();
         it != _attributes.end(); ++it) {
      str += " " + *it;
    }
    return str;
  }
  // delete any additionally added windows. TODO: possibility for implementation
  // of externally adding windows?
  void delWith(std::vector<WINDOW *> windows) {
    if (!windows.empty())
      for (std::vector<WINDOW *>::iterator it = windows.begin();
           it != windows.end(); ++it) {
        delwin(*it);
      }
  };
  ///
  void write(Eigen::Matrix<dataType, rows, cols> &data) { _matrix = data; }
  void write(std::vector<dataType> &data) {
    if (data.size() != rows * cols) {
      std::string message = "expected vector size: " + std::to_string(rows) +
                            " x " + std::to_string(cols);
      throw std::invalid_argument(message);
    }
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        _matrix(i, j) = data[(i * rows) + j]; // testing output
      }
    }
  }

public:
  /// display Kgrid
  virtual void display() override {
    /// TODO MAKE GRID

    setAnimation(0, 0); // TODO: not in use
    keypad(_win, true);
    int selection;
    size_t highlightedRow = 0;
    size_t highlightedCol = 0;
    int topPad = 1;

    // title bar
    if (_showTitle) {
      _attributes = {"RAD", "PREC: " + std::to_string(_precision)};
      // an extra space below for system / window messages.
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

    // precision of entries shown in matrix
    int displayPrecision = 5;

    while (true) {

      // navigate through elements in matrix.
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
          if (j == highlightedCol && i == highlightedRow) {
            wattron(_win, A_REVERSE);
          }

          auto element = _matrix(highlightedRow, highlightedCol);
          std::stringstream elementStream;
          elementStream << std::setprecision(_precision) << element;
          const std::string elementStr = elementStream.str();
          const int elementStrSize = elementStr.size();
          std::string elementDisplay(_size[1] / 2, ' ');
          elementDisplay.replace(0, elementStrSize, elementStr);

          std::stringstream numStream;

          // length of matrix cells set to 8.
          std::string str(8, ' ');
          int strSize = str.size();

          // if number is greater than two digits, express in scientific format,
          // which requires 4 spaces for exponent, two for front digit and
          // decimal point, allowing two d.p for total width 8.

          if (_matrix.coeff(i, j) > 99.99) {
            numStream << std::scientific << std::setprecision(2)
                      << _matrix.coeff(i, j);
          } else {
            // three spaces reserved for decimal and front digits, max width
            // is 8.
            // displayPrecision = _precision > 5 ? 5 : _precision;
            numStream << std::setprecision(displayPrecision) << std::fixed
                      << _matrix.coeff(i, j);
          }

          std::string numStr = numStream.str();
          int numStrSize = numStr.size();

          int correction = strSize - numStrSize;
          str.replace(correction, numStrSize, numStr);

          int yAlign = 0;
          int xAlign = 0;
          int left, right, top, bottom, v_centre, h_centre;

          // left align
          left = 2;

          int hPadding{4};
          // auto str{stream.str()};
          if (_setGrid) {
            hPadding = 6;
            str = "|" + str + "|";
          }
          int gridWidth = (cols * 8) + (cols - 2);
          int gridHeight = rows * _vPadding; // avoid collision with status bar

          int bottomPad = -gridHeight + i;
          int rightPad = gridWidth - 1;
          int hCentrePad = gridWidth;
          int vCentrPad = gridHeight;

          // right align
          right = _size[1];

          // centre align
          v_centre = i + (_size[0] / 2) - vCentrPad;
          h_centre = (_size[1] - hCentrePad) / 2;

          // top align
          top = i + topPad;
          bottom = _size[0] - 2;

          if (_xAlign == 1)
            xAlign = right + rightPad;
          else if (_xAlign == 0)
            xAlign = h_centre;
          else if (_xAlign == -1)
            xAlign = left;

          if (_yAlign == 1)
            yAlign = top;
          else if (_yAlign == 0)
            yAlign = v_centre;
          else if (_yAlign == -1)
            yAlign = bottom + bottomPad;
          mvwprintw(_win, yAlign + i * _vPadding,
                    xAlign + (displayPrecision + hPadding) * j, (str).c_str());
          wattroff(_win, A_REVERSE);

          // display the element highlighted to full user precision.
          mvwprintw(_win, bottom, left, elementDisplay.c_str());
          wrefresh(_win);
        }
      }
      selection = wgetch(_win);
      switch (selection) {
      case KEY_UP:
        highlightedRow--;
        if (highlightedRow == -1) {
          highlightedRow = 0;
        }
        break;
      case KEY_DOWN:
        highlightedRow++;
        if (highlightedRow == rows) {
          highlightedRow = rows - 1;
        }
        break;
      case KEY_LEFT:
        highlightedCol--;
        if (highlightedCol == -1) {
          highlightedCol = 0;
        }
        break;
      case KEY_RIGHT:
        highlightedCol++;
        if (highlightedCol == cols) {
          highlightedCol = cols - 1;
        }
        break;
      default:
        break;
      }
      if (selection == 10) {
        break;
      }
    }
    _highlightedRow = highlightedRow;
    _highlightedCol = highlightedCol;
    clear();
    wrefresh(_win);
  }
  /// return Window Object
  virtual WINDOW *window() const override { return this->_win; }
  /// set animation on start and finish
  virtual void setAnimation(const int &start,
                            const int &finish) const override {
    m_startAnim = start;
    m_finishAnim = finish;
  }
  /// set style of BlackOS Window border
  virtual void borderStyle(const int &ch = 0) override {
    wborder(_win, ch, ch, ch, ch, ch, ch, ch, ch);
    wrefresh(_win);
  }
  /// set style of BlackOS Window border
  virtual void setBorderStyle(const int &L, const int &R, const int &T,
                              const int &B, const int &TL, const int &TR,
                              const int &BL, const int &BR) override {
    wborder(_win, L, R, T, B, TL, TR, BL, BR);
    wrefresh(_win);
  }
  ///
  virtual void label(const std::string &label) const override {
    int labellocy = _size[0] - 1;
    int labellocx = _size[1] - (3 + (int)label.length());
    mvwaddstr(_win, labellocy, labellocx, label.c_str());
  }
  ///
  virtual std::vector<int> maxSize() const override {
    int yMax, xMax;
    getmaxyx(_win, yMax, xMax);
    std::vector<int> size{yMax, xMax};
    return size;
  }
  virtual std::string winType() const override { return "Kgrid"; }
  virtual std::string name() const override { return this->_name; }
  ///
  Kgrid(std::string &name, Eigen::Matrix<dataType, rows, cols> &data, int sizeY,
        int sizeX, int posY, int posX)
      : _name(name), _id(0) {
    this->write(data);
    _size = {sizeY, sizeX};
    _position = {posY, posX};
    setWin();
  }
  ///
  Kgrid(std::string &name, std::vector<dataType> &data, int sizeY, int sizeX,
        int posY, int posX)
      : _name(name), _id(0) {
    this->write(data);
    _size = {sizeY, sizeX};
    _position = {posY, posX};
    setWin();
  }
  ///
  Eigen::Matrix<dataType, rows, cols> matrix() const { return _matrix; }
  ///
  size_t getID() const { return this->_id; }
  ///
  int centreX() const { return _size[1] / 2; }
  int centreY() const { return _size[0] / 2; }
  void setPrecision(int precision) { _precision = precision; }
  void gridLines(bool show) { _setGrid = show; }
  void showTitle(bool show) { _showTitle = show; }
  void align(int x, int y) {
    _xAlign = x;
    _yAlign = y;
  }
  // get the raw element selected from matrix
  dataType selectedRaw() const {
    return this->_matrix(_highlightedRow, _highlightedCol);
  }
  // get the i and j indices of the t element selected in the matrix
  std::vector<size_t> selectedIndices() const {
    return std::vector<size_t>{_highlightedRow, _highlightedCol};
  }
  /// set the title
  void setTitle(std::string title) { _title = title; }

  ~Kgrid() {
    delWith(_subwins);
    delwin(_win);
  }
};
} // namespace BlackOSDisplay

#endif // BLACKOS_KGRID_H
