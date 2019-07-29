//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KWINDOW_H
#define BLACKOS_KWINDOW_H

#include "ncurses.h"
#include <vector>
#include <memory>

namespace  BlackOSDisplay{

typedef WINDOW *cwin;

    /// BlackOS Window Interface
 class Kwindow {

    public:
      virtual  void display() const = 0;

        virtual WINDOW * window() const = 0;
        virtual void setAnimation(const int &start, const int &finish) const = 0;
        /// switch for cursor blinking
        virtual void cursor(const int &code) const = 0;
        /// set style of corners of BlackOS Window
        virtual void setCornerStyle(const char &ch) = 0;
        /// set style for each corner of BlackOS Window
        virtual void setCornerStyle(const char &ch1 , const char &ch2,
                            const char &ch3, const char &ch4) = 0;
        /// set border style of BlackOS window
        virtual void setBorderStyle(const char &ch) = 0;
        /// set corner label of BlackOS Window
        virtual void setLabel(const std::string &label) const = 0;
        virtual std::vector<int> maxSize() const = 0;
        virtual std::string winType() const = 0;
        virtual std::string name() const = 0;
        virtual void setWin() = 0;
 };
}


#endif //BLACKOS_KWINDOW_H
