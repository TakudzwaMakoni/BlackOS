#ifndef BLACKOS_KCANVAS_H
#define BLACKOS_KCANVAS_H

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
