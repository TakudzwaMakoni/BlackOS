//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KMENU_H
#define BLACKOS_KMENU_H


#include "Kfield.h"
#include "ncurses.h"
#include "Kwindow.h"
#include <string>
#include <vector>
#include <memory>
#include "Eigen/Dense"


namespace BlackOSDisplay{

    
/// BlackOS Menu Object
class Kmenu : public Kwindow {
public:
    virtual  void display() const override;
    /// return Window Object
    virtual WINDOW * window() const override;
/// set animation on start and finish
    virtual void setAnimation(const int &start, const int &finish) const override;
    /// switch for cursor blinking
    virtual void cursor(const int &code) const override;
    /// set style of corners of BlackOS Window
    virtual void setCornerStyle(const char &ch = '*') override;
    /// set style for each corner of BlackOS Window
    virtual void setCornerStyle(const char &ch1 = '*', const char &ch2 = '*',
                                const char &ch3 = '*', const char &ch4 = '*') override ;
    /// set border style
    virtual void setBorderStyle(const char &ch) override;
    /// set corner label of BlackOS Window
    virtual void setLabel(const std::string &label) const override;
    virtual std::vector<int> maxSize() const override ;
    virtual std::string winType() const override;
    virtual std::string name() const override;

  Kmenu(const size_t &id, std::string &name, int sizeY, int sizeX ,int posY, int posX);

  std::vector<Kfield> fields() const;
  std::vector<int> size() const;

  virtual void addDisplayObj(Kwindow obj) const;
  void setPos(Eigen::Vector2i &v);
  Eigen::Vector2i position() const;
    
    ~Kmenu();

private:
    virtual void setWin() override;
    WINDOW * m_privateWin;
    std::vector<int> m_size;
    std::vector<Kfield> m_fields;
    int m_xAlignment = 0;
    int m_yAlignment = 0;
    Eigen::Vector2i m_position;
    std::string m_name;
    const size_t m_id; // TODO: need a NODE MAP TO NAVIGATE BETWEEN MENUS
    mutable int m_startAnim, m_finishAnim;
};

     
     
} // namespace BlackOSObjects

#endif //BLACKOS_KMENU_H

