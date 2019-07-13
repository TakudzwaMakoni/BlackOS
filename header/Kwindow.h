//
// Created by Takudzwa Makoni on 2019-07-12.
//

#ifndef BLACKOS_KWINDOW_H
#define BLACKOS_KWINDOW_H

#include "Kmenu.h"
#include "ncurses.h"
#include <string>

class Kwindow {

public:
    explicit Kwindow(std::vector<int> size,
            Kmenu *menu = nullptr,char alignment = 'c',
            directive init_directive = nullptr,
            directive end_directive = nullptr);

    WINDOW *window() const;
    void setAnimation(int start, int finish);
    Kmenu *menu() const;
    void cursor(int code);

    ~Kwindow();
private:
    WINDOW *m_window;
    int m_start, m_end;
    Kmenu *m_menu;

};

#endif //BLACKOS_KWINDOW_H
