//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KMENU_H
#define BLACKOS_KMENU_H

#include "Eigen/Dense"
#include "Kfield.h"
#include "Kwindow.h"
#include "ncurses.h"
#include <memory>
#include <string>
#include <vector>

namespace BlackOS {
namespace Display {
class Kmenu : public Kwindow {
public:
  Kmenu(std::string const &name, int const sizeY, int const sizeX,
        int const posY, int const posX);
  virtual void display() override;
  virtual WINDOW *window() const override;
  virtual void borderStyle(int const ch = 0) override;
  virtual void borderStyle(int const L, int const R, int const T, int const B,
                           int const TL, int const TR, int const BL,
                           int const BR) override;
  virtual void label(std::string const &label) const override;
  virtual std::vector<int> maxSize() const override;
  virtual std::string winType() const override;
  virtual std::string name() const override;
  virtual void setWin(WINDOW *window = nullptr) override;
  virtual void kErase(int const y1, int const x1, int const y2,
                      int const x2) override;
  virtual void kEraseExcept(int const y1, int const x1, int const y2,
                            int const x2) override;
  virtual void kErase(std::vector<int> const &elements) override;
  virtual void kEraseExcept(std::vector<int> const &elements) override;
  virtual void refresh() override;
  virtual int getChrfromW(int const y, int const x,
                          bool const preserve_cursor_pos = true) const override;

  virtual void setFields(std::vector<Kfield> const &fields);
  virtual Kfield getSelectedField() const;

  void setFieldAlign(int const x, int const y);
  void addFieldPadding();
  void setFieldStyle(std::string const &style);
  void paginate(int const divisor);
  std::vector<Kfield> fields() const;
  std::vector<int> size() const;
  Eigen::Vector2i position() const;
  void showTitle(bool const show = true);
  void setTitle(std::string const &title);
  void setBorderStyle();

  std::string attributeString();
  void fill(char ch, bool titleBar = false);

  ~Kmenu();

private:
  WINDOW *_win = nullptr;
  std::vector<WINDOW *> _subwins;
  std::vector<int> _size;
  std::vector<Kfield> _fields;
  std::vector<std::string> _attributes;
  int _xAlign = 0;
  int _yAlign = 0;
  std::string _fieldStyle;
  Eigen::Vector2i _position;
  std::string _name;
  int _highlighted{-1};
  int _pagination = 1;
  bool _showTitle{false};
  std::string _title;
  std::vector<int> _borderStyle{0, 0, 0, 0, 0, 0, 0, 0}; // size = 8.

  void _delWith(std::vector<WINDOW *> windows);
  std::vector<std::vector<BlackOS::Display::Kfield>>
  _paginate(int const pages, int const residue);
};
} // namespace Display
} // namespace BlackOS

#endif // BLACKOS_KMENU_H
