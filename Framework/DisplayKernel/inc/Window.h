#ifndef DISPLAY_KERNEL_WINDOW_H
#define DISPLAY_KERNEL_WINDOW_H

/**
 * Window
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

#include "DisplayHelpers.h"
#include "DisplayObject.h"

#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace BlackOS {
namespace DisplayKernel {

class Window : public DisplayObject {
public:
  Window(size_t const sizeY, size_t const sizeX, size_t const posY,
         size_t const posX);

  virtual void borderStyle(int const ch = 0) override;
  virtual void borderStyle(int const L, int const R, int const T, int const B,
                           int const TL, int const TR, int const BL,
                           int const BR) override;
  virtual std::vector<size_t> maxSize() const override;
  virtual std::string winType() const override;
  virtual void setWin(WIN_SET_CODE const init) override;
  virtual void erase(size_t const y1, size_t const x1, size_t const y2,
                     size_t const x2) override;
  virtual void erase(bool titleBar) override;
  virtual void eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                           size_t const x2) override;
  virtual void erase(std::vector<size_t> const &elements) override;
  virtual void eraseExcept(std::vector<size_t> const &elements) override;
  virtual char getCharFromWin(size_t const y, size_t const x,
                              bool const save_cursor = true) const override;
  virtual void fill(char const ch, bool const titleBar = false) override;
  virtual void insert(std::string const &str, size_t const y, size_t const x,
                      attr_t style = A_NORMAL) override;
  virtual void insert(char const *ch, size_t const y, size_t const x,
                      attr_t style = A_NORMAL) override;
  virtual void insert(char const ch, size_t const y, size_t const x,
                      attr_t style = A_NORMAL) override;
  virtual void pause() const override;
  virtual bool windowSet() const override;

  size_t winSzY() const;
  size_t winSzX() const;
  size_t winPosY() const;
  size_t winPosX() const;
  void hideBorder();
  void hideTitle();
  void loadTitle(std::string const &title, attr_t const titleStyle = A_NORMAL);
  void loadTitleStyle(attr_t style);
  void showTitle();
  int lastKeyPressed() const;
  void clear();
  void eraseWin();
  void refresh();
  wchar_t getCharFromUser() const;
  int resize(size_t const y, size_t const x);
  int reposition(size_t const y, size_t const x);
  void cursorPosition(int &, int &) const;
  void bgfg(int const fg, int const bg); // TODO: rename
  void setScroll(bool);
  void setKeypad(bool);

  void newLine(bool newlineAtBeginning = true);
  void newLines(int n, bool newlineAtBeginning = true);
  void print(std::string const &str, attr_t style = A_NORMAL);
  void print(std::string const &format, std::string const &str,
             attr_t style = A_NORMAL);
  std::vector<std::string> splitString(std::string, std::string const &);
  std::vector<std::string> splitString(std::string, char const);
  void printLines(std::string const &str, bool newlineAtBeginning = true);
  void addChar(char const);
  void deleteChar();
  void moveCursor(int, int);

  ~Window();

private:
  WINDOW *_win = nullptr;
  size_t _winSzY;
  size_t _winSzX;
  size_t _winPosY;
  size_t _winPosX;
  std::string _title;
  bool _showTitle = 0;
  bool _showBorder = 0;
  int _lastKeyPressed;
  attr_t _titleStyle;

  void _checkRange(size_t const y1, size_t const x1, size_t const y2,
                   size_t const x2) const;
  void _checkRange(size_t const y, size_t const x) const;
};
} // namespace DisplayKernel
} // namespace BlackOS
#endif
