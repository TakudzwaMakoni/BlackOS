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
  Kmenu(std::string const &name, size_t const sizeY, size_t const sizeX,
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
  virtual void setWin(WINDOW *window = nullptr) override;
  virtual void erase(size_t const y1, size_t const x1, size_t const y2,
                     size_t const x2) override;
  virtual void eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                           size_t const x2) override;
  virtual void erase(std::vector<size_t> const &elements) override;
  virtual void eraseExcept(std::vector<size_t> const &elements) override;
  virtual void refresh() override;
  virtual int getChrfromW(size_t const y, size_t const x,
                          bool const save_cursor = true) const override;
  virtual void fill(char const ch, bool const titleBar = false) override;
  virtual void insert(std::string const &str, size_t const y,
                      size_t const x) override;
  virtual void insert(char const *ch, size_t const y, size_t const x) override;
  virtual void pause() const override;
  virtual bool windowSet() const override;

  virtual void setFields(std::vector<Kfield> const &fields);
  virtual Kfield selectedField() const;

  void alignFields(int const x, int const y);
  void addFieldPadding();
  void setFieldStyle(std::string const &style);
  void paginate(size_t const entriesPerPage);

  std::vector<Kfield> fields() const;
  std::vector<size_t> size() const;
  std::vector<size_t> position() const;
  void hideTitle();
  void addTitle(std::string const &title);

  ~Kmenu();

private:
  WINDOW *_win = nullptr;
  std::vector<WINDOW *> _subwins;
  std::vector<size_t> _size;
  std::vector<size_t> _position;
  std::vector<Kfield> _fields;
  size_t _page;    // current partition to load
  size_t _pCoeff;  // typical fields per page
  size_t _pQuot;   // quotient
  size_t _pRem;    // remainder
  size_t _p;       // total number of partitions
  size_t _fieldSz; // total number of Fields
  size_t _f;       // number of fields in current partition
  int _xAlign;
  int _yAlign;
  std::string _fieldStyle;
  std::string _name;
  size_t _highlighted;
  bool _showTitle = 1;
  std::string _title;

  void _delWith(std::vector<WINDOW *> windows);
  std::vector<BlackOS::Display::Kfield> _loadPage();
  void _addTitle();
  void _setFields();
  void _updateF();
  size_t _highlightedMap() const;
};
} // namespace Display
} // namespace BlackOS

#endif // BLACKOS_KMENU_H
