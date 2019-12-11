//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KWINDOW_H
#define BLACKOS_KWINDOW_H

#include "ncurses.h"
#include <memory>
#include <vector>

#define cursor(x) curs_set(x)

namespace BlackOSDisplay {

typedef WINDOW *cwin;

/// BlackOS Window Interface
class Kwindow {

public:
  virtual void display() = 0;
  /// return window
  virtual WINDOW *window() const = 0;
  /// set style of corners of BlackOS Window
  virtual void borderStyle(const int ch) = 0;
  /// set style for each corner of BlackOS Window
  virtual void borderStyle(const int L, const int R, const int T, const int B,
                           const int TL, const int TR, const int BL,
                           const int BR) = 0;
  /// set corner label of BlackOS Window
  virtual void label(const std::string &label) const = 0;
  virtual std::vector<int> maxSize() const = 0;
  virtual std::string winType() const = 0;
  virtual std::string name() const = 0;
  virtual void setWin(WINDOW *window) = 0;
  virtual void kErase(const int y1, const int x1, const int y2,
                      const int x2) = 0;
  virtual void kEraseExcept(const int y1, const int x1, const int y2,
                            const int x2) = 0;
  virtual void kErase(const std::vector<int> &elements) = 0;
  virtual void kEraseExcept(const std::vector<int> &elements) = 0;
};
} // namespace BlackOSDisplay

#endif // BLACKOS_KWINDOW_H
