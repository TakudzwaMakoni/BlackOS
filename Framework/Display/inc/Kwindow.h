#ifndef BLACKOS_KWINDOW_H
#define BLACKOS_KWINDOW_H

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

#include "ncurses.h"
#include <memory>
#include <vector>

#define cursor(x) curs_set(x)

namespace BlackOS {
namespace Display {
typedef WINDOW *cwin;
class Kwindow {
public:
  virtual void display() = 0;
  virtual WINDOW *window() const = 0;
  virtual void borderStyle(int const ch) = 0;
  virtual void borderStyle(int const L, int const R, int const T, int const B,
                           int const TL, int const TR, int const BL,
                           int const BR) = 0;
  virtual void label(std::string const &label) const = 0;
  virtual std::vector<int> maxSize() const = 0;
  virtual std::string winType() const = 0;
  virtual std::string name() const = 0;
  virtual void setWin(WINDOW *window) = 0;
  virtual void kErase(int const y1, int const x1, int const y2,
                      int const x2) = 0;
  virtual void kEraseExcept(int const y1, int const x1, int const y2,
                            int const x2) = 0;
  virtual void kErase(std::vector<int> const &elements) = 0;
  virtual void kEraseExcept(std::vector<int> const &elements) = 0;
  virtual void refresh() = 0;
  virtual int getChrfromW(int const y, int const x,
                          bool const preserve_cursor_pos) const = 0;
};
} // namespace Display
} // namespace BlackOS
#endif // BLACKOS_KWINDOW_H
