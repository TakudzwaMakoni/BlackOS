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

namespace BlackOS {
namespace Display {
template <typename dataType, size_t rows, size_t cols>
class Kgrid : public Kwindow {
public:
  Kgrid(std::string &name, size_t sizeY, size_t sizeX, size_t posY,
        size_t posX);
  void write(Eigen::Matrix<dataType, rows, cols> &data);
  void write(std::vector<dataType> &data);
  virtual void setWin(WINDOW *window) override;
  virtual void display() override;
  virtual void borderStyle(int const ch = 0) override;
  virtual void borderStyle(int const L, int const R, int const T, int const B,
                           int const TL, int const TR, int const BL,
                           int const BR) override;
  virtual void label(std::string const &label) const override;
  virtual std::vector<size_t> maxSize() const override;
  virtual std::string winType() const override;
  virtual std::string name() const override;
  virtual void kErase(size_t const y1, size_t const x1, size_t const y2,
                      size_t const x2) override;
  virtual void kEraseExcept(size_t const y1, size_t const x1, size_t const y2,
                            size_t const x2) override;
  virtual void kErase(std::vector<size_t> const &elements) override;
  virtual void kEraseExcept(std::vector<size_t> const &elements) override;
  virtual void refresh() override;
  virtual size_t
  getChrfromW(size_t const y, size_t const x,
              bool const preserve_cursor_pos = true) const override;
  virtual void fill(char const ch, bool const titleBar = false) override;

  Eigen::Matrix<dataType, rows, cols> matrix() const { return _matrix; }
  v void setPrecision(size_t const precision) { _precision = precision; }
  void gridLines(bool const show) { _setGrid = show; }
  void showTitle(bool const show) { _showTitle = show; }
  void setBorderStyle();
  void align(int x, int y) {
    _xAlign = x;
    _yAlign = y;
  }
  dataType selectedRaw() const;
  std::vector<size_t> selectedIndices() const;
  void setTitle(std::string const &title);

  ~Kgrid();

private:
  WINDOW *_win;
  std::string _name;
  Eigen::Matrix<dataType, rows, cols> _matrix;
  std::vector<WINDOW *> _subwins;
  std::vector<size_t> _size;
  std::vector<size_t> _position;
  int _xAlign = 0;
  int _yAlign = 0;
  size_t _precision;
  size_t _vPadding{1};
  std::string _title;
  bool _setGrid{false};
  bool _showTitle{false};
  size_t _highlightedRow{0};
  size_t _highlightedCol{0};
  std::vector<std::string> _attributes;

  std::string attributeString();
  void delWith(std::vector<WINDOW *> windows);
};
} // namespace Display
} // namespace BlackOS
#include "Kgrid.tpp"
#endif // BLACKOS_KGRID_H
