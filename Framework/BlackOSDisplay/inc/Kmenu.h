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

namespace BlackOSDisplay {

namespace {
std::string emptyField;
}

/// BlackOS Menu Object
class Kmenu : public Kwindow {
public:
  Kmenu(const size_t &id, std::string &name, int sizeY, int sizeX, int posY,
        int posX);

  virtual void display() override;
  /// return Window Object
  virtual WINDOW *window() const override;
  /// set animation on start and finish
  virtual void setAnimation(const int &start, const int &finish) const override;
  /// set style of BlackOS Window border
  virtual void borderStyle(const int &ch = 0) override;
  /// set style of BlackOS Window border
  virtual void setBorderStyle(const int &L, const int &R, const int &T,
                              const int &B, const int &TL, const int &TR,
                              const int &BL, const int &BR) override;
  virtual void label(const std::string &label) const override;
  virtual std::vector<int> maxSize() const override;
  virtual std::string winType() const override;
  virtual std::string name() const override;

  virtual void setFields(const std::vector<Kfield> &fields);
  virtual void addDisplayObj(Kwindow &obj) const;
  virtual Kfield getSelectedField() const;

  size_t getID() const;
  void setWinPos(int x, int y);
  void setFieldAlign(int x, int y);
  void fieldPadding(bool paddingOn = true);
  int centreX() const;
  int centreY() const;
  std::vector<Kfield> fields() const;
  std::vector<int> size() const;
  Eigen::Vector2i position() const;

  ~Kmenu();

private:
  virtual void setWin() override;
  WINDOW *_win;
  std::vector<int> _size;
  std::vector<Kfield> _fields;
  int _xAlign = 0;
  int _yAlign = 0;
  Eigen::Vector2i _position;
  std::string _name;
  int _highlighted{-1};
  const size_t _id; // TODO: need a NODE MAP TO NAVIGATE BETWEEN MENUS
  mutable int _startAnim, _finishAnim;
};

} // namespace BlackOSDisplay

#endif // BLACKOS_KMENU_H
