#ifndef BLACKOS_KCANVAS_H
#define BLACKOS_KCANVAS_H

/**
 * Kcanvas
 *
 * Copyright (C) 2019, Takudzwa Makoni
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

#include "../inc/Directives.h"
#include "Eigen/Dense"
#include "Kwindow.h"
#include <string>
#include <vector>

namespace BlackOS {
namespace DisplayKernel {
class Kcanvas : public Kwindow {
public:
  Kcanvas(std::string &name, size_t const sizeY, size_t const sizeX,
          size_t const posY, size_t const posX);
  virtual void display() override;
  virtual void borderStyle(int const ch = 0) override;
  virtual void borderStyle(int const L, int const R, int const T, int const B,
                           int const TL, int const TR, int const BL,
                           int const BR) override;
  virtual void label(std::string const &label) const override;
  virtual std::vector<size_t> maxSize() const override;
  virtual std::string winType() const override;
  virtual std::string name() const override;
  virtual void setWin(WINDOW *window) override;
  virtual void erase(size_t const y1, size_t const x1, size_t const y2,
                     size_t const x2) override;
  virtual void eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                           size_t const x2) override;
  virtual void erase(std::vector<size_t> const &elements) override;
  virtual void eraseExcept(std::vector<size_t> const &elements) override;
  virtual void refresh() override;
  virtual size_t getChrfromW(size_t const y, size_t const x,
                             bool const preserve_cursor_pos) const override;
  virtual void fill(char const ch, bool const titleBar = false) override;
  virtual void pause() const override;
  virtual bool windowSet() const override;

  std::vector<size_t> size() const;
  std::vector<size_t> position() const;
  void showTitle(bool const show = true);
  void setTitle(std::string const &title);
  void delWith(std::vector<WINDOW *> windows);
  std::string attributeString();

  ~Kcanvas();

private:
  WINDOW *_win;
  std::vector<WINDOW *> _subwins;
  std::vector<size_t> _size;
  std::vector<size_t> _position;
  std::vector<std::string> _attributes;
  int _xAlign = 0;
  int _yAlign = 0;
  std::string _name;
  bool _showTitle{false};
  std::string _title;
};
} // namespace DisplayKernel
} // namespace BlackOS
#endif
