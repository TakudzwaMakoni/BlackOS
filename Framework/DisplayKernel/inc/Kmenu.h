#ifndef BLACKOS_KMENU_H
#define BLACKOS_KMENU_H

/**
 * Kmenu
 *
 * Copyright (C) 2019-07-27, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "Kwindow.h"
#include "ncurses.h"
#include <memory>
#include <string>
#include <vector>

namespace BlackOS {
namespace DisplayKernel {

enum class TitleStyle : size_t { underline, highlight, none };

class Kmenu : public Kwindow {
public:
  Kmenu(std::string const &name, size_t const sizeY, size_t const sizeX,
        size_t const posY, size_t const posX);

  virtual void display(std::vector<int> const &breakConditions = {
                           (int)'q', 10 /*ENTER key*/,
                           27 /*ESC key*/}) override;
  virtual void borderStyle(int const ch = 0) override;
  virtual void borderStyle(int const L, int const R, int const T, int const B,
                           int const TL, int const TR, int const BL,
                           int const BR) override;
  virtual void label(std::string const &label) const override;
  virtual std::vector<size_t> maxSize() const override;
  virtual std::string winType() const override;
  virtual std::string name() const override;
  virtual void setWin(bool const initWin) override;
  virtual void erase(size_t const y1, size_t const x1, size_t const y2,
                     size_t const x2) override;
  virtual void erase(bool titleBar) override;
  virtual void eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                           size_t const x2) override;
  virtual void erase(std::vector<size_t> const &elements) override;
  virtual void clear() override;
  virtual void eraseExcept(std::vector<size_t> const &elements) override;
  virtual void refresh() override;
  virtual char getCharFromWin(size_t const y, size_t const x,
                              bool const save_cursor = true) const override;
  virtual void fill(char const ch, bool const titleBar = false) override;
  virtual void insert(std::string const &str, size_t const y,
                      size_t const x) override;
  virtual void insert(char const *ch, size_t const y, size_t const x) override;
  virtual void insert(char const ch, size_t const y, size_t const x) override;
  virtual void pause() const override;
  virtual bool windowSet() const override;
  virtual int resize(size_t const y, size_t const x) override;
  virtual int reposition(size_t const y, size_t const x) override;

  virtual void loadFields(std::vector<std::string> const &fields);
  virtual std::string selectedField() const;
  size_t selectedFieldIndex() const;

  void loadFieldAlignment(int const x, int const y);
  void fieldStyle(std::string const &style);
  void paginate(size_t const entriesPerPage);

  std::vector<std::string> fields() const;
  size_t winSzY() const;
  size_t winSzX() const;
  size_t winPosY() const;
  size_t winPosX() const;
  std::vector<size_t> position() const;
  void hideBorder();
  void hideTitle();
  void loadTitle(std::string const &title,
                 TitleStyle const titleStyle = TitleStyle::none);
  void loadTitleStyle(TitleStyle style);
  void showTitle();
  int lastKeyPressed() const;

  ~Kmenu();

private:
  WINDOW *_win = nullptr;
  size_t _winSzY;
  size_t _winSzX;
  size_t _winPosY;
  size_t _winPosX;
  std::vector<std::string> _fields;
  size_t _page;    // current partition to load
  size_t _fieldSz; // total number of Fields
  size_t _entriesPerPage = 0;
  size_t _f; // number of fields in current partition
  size_t _m = 0;
  int _xAlign;
  int _yAlign;
  std::string _fieldStyle;
  std::string _name;
  std::string _title;
  size_t _highlighted;
  bool _showTitle = 1;
  bool _showBorder = 1;
  int _lastKeyPressed;
  TitleStyle _titleStyle;

  size_t _pCoeff() const; // typical fields per page
  size_t _pQuot() const;  // quotient
  size_t _pRem() const;   // remainder
  size_t _p() const;      // total number of partitions
  std::vector<std::string> _loadPage();
  std::string _addFieldPadding(std::string const &fieldName);
  void _loadFields();
  void _updateF();
  void _addToTitleHeader(std::string const &title, size_t y, size_t x);
  void addTitleStyle(TitleStyle style = TitleStyle::none);
  void _updateM();
  size_t _highlightedMap() const;
  void _checkRange(size_t const y1, size_t const x1, size_t const y2,
                   size_t const x2) const;
  void _checkRange(size_t const y, size_t const x) const;
};
} // namespace DisplayKernel
} // namespace BlackOS

#endif // BLACKOS_KMENU_H
