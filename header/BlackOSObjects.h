//
// Created by Takudzwa Makoni on 2019-07-20.
//

#ifndef BLACKOS_BLACKOSOBJECTS_H
#define BLACKOS_BLACKOSOBJECTS_H

#include "ncurses.h"
#include "BlackOSScripts.h"

#include <string>
#include <vector>
#include <memory>

namespace BlackOSObjects{

    typedef void (*directive)();

    class Kfield {

    public:
        Kfield(std::string &name, directive script);

        std::string name() const;
        directive script() const;

    private:
        const std::string m_name;
        const directive m_script;
    };


    class Kmenu {

    public:
        Kmenu(std::string &name, std::vector<Kfield> &fields, std::vector<int> &size);

        std::vector<Kfield> fields() const;
        std::vector<int> size() const;
        std::string name() const;

    private:
        const std::vector<Kfield> m_fields;
        const std::vector<int> m_size;
        const std::string m_name;
    };


    class Kwindow {

    public:
        explicit Kwindow(std::vector<int> &size, std::unique_ptr<Kmenu> &menu,
                         char alignment = 'c');

        WINDOW *window() const;
        void setAnimation(int start, int finish);
        static void cursor(int code);
      static void loadMenu(WINDOW *window, const std::unique_ptr<Kmenu> menu,
                             const char &alignment);

        ~Kwindow();

    private:
        WINDOW *m_window;
        int m_start, m_end;
      std::unique_ptr<Kmenu>  m_menu;

    };

} // BlackOSObjects

#endif //BLACKOS_BLACKOSOBJECTS_H
