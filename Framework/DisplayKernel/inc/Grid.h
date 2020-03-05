#ifndef DISPLAY_KERNEL_GRID_H
#define DISPLAY_KERNEL_GRID_H

/**
 * Grid
 *
 * Copyright (C) 2019-07-27, Takudzwa Makoni
 * <https://github.com/TakudzwaMakoni>
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

#include "DisplayObject.h"
#include "Eigen/Dense"
#include <iomanip>
#include <memory>
#include <ncurses.h>
#include <sstream>
#include <string>

namespace BlackOS {
namespace DisplayKernel {
template <typename dataType, size_t rows, size_t cols>
class Grid : public DisplayObject {
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
  virtual void erase(size_t const y1, size_t const x1, size_t const y2,
                     size_t const x2) override;
  virtual void eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                           size_t const x2) override;
  virtual void erase(std::vector<size_t> const &elements) override;
  virtual void eraseExcept(std::vector<size_t> const &elements) override;
  virtual void refresh() override;
  virtual size_t
  getChrfromW(size_t const y, size_t const x,
              bool const preserve_cursor_pos = true) const override;
  virtual void fill(char const ch, bool const titleBar = false) override;
  virtual bool windowSet() const override;

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

  ~Grid();

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
} // namespace DisplayKernel
} // namespace BlackOS
#include "Grid.tpp"
#endif // BLACKOS_KGRID_H
