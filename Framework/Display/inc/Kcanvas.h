#ifndef BLACKOS_KCANVAS_H
#define BLACKOS_KCANVAS_H

/**
 *  Copyright 2019-07-13 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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
namespace Display {
class Kcanvas : public Kwindow {
public:
  Kcanvas(std::string &name, int sizeY, int sizeX, int posY, int posX);
  virtual void display() override;
  virtual WINDOW *window() const override;
  virtual void borderStyle(const int ch = 0) override;
  virtual void borderStyle(const int L, const int R, const int T, const int B,
                           const int TL, const int TR, const int BL,
                           const int BR) override;
  virtual void label(const std::string &label) const override;
  virtual std::vector<int> maxSize() const override;
  virtual std::string winType() const override;
  virtual std::string name() const override;
  virtual void setWin(WINDOW *window) override;
  virtual void kErase(const int y1, const int x1, const int y2,
                      const int x2) override;
  virtual void kEraseExcept(const int y1, const int x1, const int y2,
                            const int x2) override;
  virtual void kErase(const std::vector<int> &elements) override;
  virtual void kEraseExcept(const std::vector<int> &elements) override;
  virtual void refresh() override;
  virtual int getChrfromW(int const y, int const x,
                          bool const preserve_cursor_pos) const override;

  std::vector<int> size() const;
  Eigen::Vector2i position() const;
  void showTitle(bool show = true);
  void setTitle(std::string title);
  void delWith(std::vector<WINDOW *> windows);
  std::string attributeString();
  void fill(char ch, bool titleBar = false);

  ~Kcanvas();

private:
  WINDOW *_win;
  std::vector<WINDOW *> _subwins;
  std::vector<int> _size;
  std::vector<std::string> _attributes;
  int _xAlign = 0;
  int _yAlign = 0;
  Eigen::Vector2i _position;
  std::string _name;
  bool _showTitle{false};
  std::string _title;
  std::vector<int> _borderStyle{0, 0, 0, 0, 0, 0, 0, 0}; // size = 8.

  void _setBorderStyle();
};
} // namespace Display
} // namespace BlackOS
#endif
