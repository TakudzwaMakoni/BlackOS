//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KWINDOW_H
#define BLACKOS_KWINDOW_H

#include "ncurses.h"
#include <vector>
#include <memory>

#define cursor(x) curs_set(x)

namespace  BlackOSDisplay{

typedef WINDOW *cwin;

    /// BlackOS Window Interface
 class Kwindow {

    public:
     
        virtual void display() = 0;
        virtual WINDOW * window() const = 0;
        virtual void setAnimation(const int &start, const int &finish) const = 0;
        /// switch for cursor blinking
        /// set style of corners of BlackOS Window
        virtual void setBorderStyle(const int &ch) = 0;
        /// set style for each corner of BlackOS Window
     virtual void setBorderStyle(const int &L, const int &R,
                                 const int &T, const int &B,const int &TL, const int &TR,
                                 const int &BL, const int &BR) = 0;
        /// set corner label of BlackOS Window
        virtual void setLabel(const std::string &label) const = 0;
        virtual std::vector<int> maxSize() const = 0;
        virtual std::string winType() const = 0;
        virtual std::string name() const = 0;
        virtual void setWin() = 0;
 };
}


#endif //BLACKOS_KWINDOW_H
