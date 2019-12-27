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
  Kmenu(std::string const &name, int sizeY, int sizeX, int posY, int posX);
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
  virtual void setWin(WINDOW *window = nullptr) override;
  virtual void kErase(const int y1, const int x1, const int y2,
                      const int x2) override;
  virtual void kEraseExcept(const int y1, const int x1, const int y2,
                            const int x2) override;
  virtual void kErase(const std::vector<int> &elements) override;
  virtual void kEraseExcept(const std::vector<int> &elements) override;

  virtual void setFields(const std::vector<Kfield> &fields);
  virtual Kfield getSelectedField() const;

  void setFieldAlign(int x, int y);
  void addFieldPadding();
  void setFieldStyle(std::string style);
  void paginate(int divisor);
  std::vector<Kfield> fields() const;
  std::vector<int> size() const;
  Eigen::Vector2i position() const;
  void showTitle(bool show = true);
  void setTitle(std::string title);
  void setBorderStyle();

  std::string attributeString();
  void fill(char ch);
  void wipe(bool titleBar);

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
