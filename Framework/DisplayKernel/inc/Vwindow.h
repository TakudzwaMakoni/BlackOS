// virses (c) 2020 Takudzwa Makoni
// virtual window Interface

#ifndef BLACKOS_VWINDOW_H
#define BLACKOS_VWINDOW_H

#include <iostream>
#include <vector>

namespace BlackOS {
namespace DisplayKernel {

class Vwindow {
public:
  virtual void display() = 0;
  virtual void borderStyle(int const ch) = 0;
  virtual void borderStyle(int const L, int const R, int const T, int const B,
                           int const TL, int const TR, int const BL,
                           int const BR) = 0;
  virtual void label(std::string const &label) const = 0;
  virtual std::vector<size_t> maxSize() const = 0;
  virtual std::string winType() const = 0;
  virtual std::string name() const = 0;
  virtual void erase(size_t const y1, size_t const x1, size_t const y2,
                     size_t const x2) = 0;
  virtual void eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                           size_t const x2) = 0;
  virtual void erase(std::vector<size_t> const &elements) = 0;
  virtual void erase(bool titlebar) = 0;
  virtual void clear() = 0;
  virtual void eraseExcept(std::vector<size_t> const &elements) = 0;
  virtual void refresh() = 0;
  virtual char getCharFromWin(size_t const y, size_t const x,
                              bool const preserve_cursor_pos) const = 0;
  virtual void fill(char ch, bool titleBar = false) = 0;
  virtual void insert(std::string const &str, size_t const y,
                      size_t const x) = 0;
  virtual void insert(char const *ch, size_t const y, size_t const x) = 0;
  virtual void insert(char const ch, size_t const y, size_t const x) = 0;
  virtual void pause() const = 0;
  virtual bool windowSet() const = 0;
};

} // namespace DisplayKernel
} // namespace BlackOS
#endif // BLACKOS_VWINDOW_H
