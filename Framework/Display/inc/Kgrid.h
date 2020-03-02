#ifndef BLACKOS_KGRID_H
#define BLACKOS_KGRID_H

/**
 *  Copyright 2019-07-27 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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
  Kgrid(std::string &name, int sizeY, int sizeX, int posY, int posX);
  void write(Eigen::Matrix<dataType, rows, cols> &data);
  void write(std::vector<dataType> &data);
  virtual void setWin(WINDOW *window) override;
  virtual void display() override;
  virtual void borderStyle(const int ch = 0) override;
  virtual void borderStyle(const int L, const int R, const int T, const int B,
                           const int TL, const int TR, const int BL,
                           const int BR) override;
  virtual WINDOW *window() const override;
  virtual void label(const std::string &label) const override;
  virtual std::vector<int> maxSize() const override;
  virtual std::string winType() const override;
  virtual std::string name() const override;
  virtual void kErase(const int y1, const int x1, const int y2,
                      const int x2) override;
  virtual void kEraseExcept(const int y1, const int x1, const int y2,
                            const int x2) override;
  virtual void kErase(const std::vector<int> &elements) override;
  virtual void kEraseExcept(const std::vector<int> &elements) override;
  virtual void refresh() override;
  virtual int getChrfromW(int const y, int const x,
                          bool const preserve_cursor_pos = true) const override;

  Eigen::Matrix<dataType, rows, cols> matrix() const { return _matrix; }
  void setPrecision(int precision) { _precision = precision; }
  void gridLines(bool show) { _setGrid = show; }
  void showTitle(bool show) { _showTitle = show; }
  void setBorderStyle();
  void align(int x, int y) {
    _xAlign = x;
    _yAlign = y;
  }
  dataType selectedRaw() const;
  std::vector<size_t> selectedIndices() const;
  void setTitle(std::string title);

  ~Kgrid();

private:
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

  std::string attributeString();
  void delWith(std::vector<WINDOW *> windows);
};
} // namespace Display
} // namespace BlackOS
#include "Kgrid.tpp"
#endif // BLACKOS_KGRID_H
