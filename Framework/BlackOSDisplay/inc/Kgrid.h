//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KGRID_H
#define BLACKOS_KGRID_H

#include "Eigen/Dense"
#include "Kwindow.h"
#include <iomanip>
#include <memory>
#include <ncurses.h>
#include <sstream>
#include <string>

namespace BlackOSDisplay {

/// BlackOS Grid class Kwindow
template <typename dataType, size_t rows, size_t cols>
class Kgrid : public Kwindow {

public:
  /// constructor for Kgrid
  Kgrid(std::string &name, int sizeY, int sizeX, int posY, int posX);
  /// write eigen matrix data to class matrix
  void write(Eigen::Matrix<dataType, rows, cols> &data);
  /// (overload) write data from vector into matrix
  void write(std::vector<dataType> &data);

  // overrides

  // assign a window Object
  virtual void setWin(WINDOW *window) override;
  /// display Kgrid
  virtual void display() override;
  /// set style of BlackOS Window border
  virtual void borderStyle(const int ch = 0) override;
  /// set style of BlackOS Window border
  virtual void borderStyle(const int L, const int R, const int T, const int B,
                           const int TL, const int TR, const int BL,
                           const int BR) override;
  /// return window
  virtual WINDOW *window() const override;
  /// set window label
  virtual void label(const std::string &label) const override;
  /// return max size of window
  virtual std::vector<int> maxSize() const override;
  /// return type of Kwindow
  virtual std::string winType() const override;
  /// return name of Kwindow
  virtual std::string name() const override;
  ///
  virtual void kErase(const int y1, const int x1, const int y2,
                      const int x2) override;
  ///
  virtual void kEraseExcept(const int y1, const int x1, const int y2,
                            const int x2) override;
  ///
  virtual void kErase(const std::vector<int> &elements) override;
  ///
  virtual void kEraseExcept(const std::vector<int> &elements) override;
  ///
  Eigen::Matrix<dataType, rows, cols> matrix() const { return _matrix; }
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
  dataType selectedRaw() const;
  // get the i and j indices of the t element selected in the matrix
  std::vector<size_t> selectedIndices() const;
  /// set the title
  void setTitle(std::string title);
  /// destructor for Kgrid
  ~Kgrid();

private:
  /// private member variables
  std::string _name;
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
  std::vector<int> _borderStyle{0, 0, 0, 0, 0, 0, 0, 0}; // size = 8.
  std::vector<std::string> _attributes;

  // private member functions

  /// genetrate attribute string
  std::string attributeString();
  void _setBorderStyle();
  /// delete any additionally added windows.
  void delWith(std::vector<WINDOW *> windows);
};
} // namespace BlackOSDisplay

// implementation for template class Kgrid
#include "Kgrid.tpp"

#endif // BLACKOS_KGRID_H
