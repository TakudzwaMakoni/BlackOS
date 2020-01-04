//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KWINDOW_H
#define BLACKOS_KWINDOW_H

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
