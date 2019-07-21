//
// Created by Takudzwa Makoni on 2019-07-20.
//

#include "../header/BlackOSObjects.h"

namespace BlackOSObjects{

    Kfield::Kfield(std::string &name, directive script)
            : m_name(name), m_script(script) {}
    std::string Kfield::name() const { return m_name; }
    directive Kfield::script() const { return m_script; }

    Kmenu::Kmenu( std::string &name, std::vector<Kfield> &fields, std::vector<int> &size) : m_fields(fields), m_size(size), m_name(name){};
    std::vector<Kfield> Kmenu::fields() const { return m_fields; };
    std::vector<int> Kmenu::size() const { return m_size; }
    std::string Kmenu::name() const { return m_name; }

    Kwindow::Kwindow(std::vector<int> &size, std::unique_ptr<Kmenu> &menu,
                    const std::pair<int,int> &alignment){

         m_menu = std::move(menu);
        curs_set(0);

        char star = '*'; // set custom border
        m_window = newwin(size[0], size[1], size[2], size[3]);
        // box(m_window, 0 , 0;
        wborder(m_window, 0, 0, 0, 0, (int)star, (int)star, (int)star, (int)star);
        refresh();

        if (m_menu != nullptr) {

            // add label
            auto label = m_menu->name();

            int labellocy = size[0] - 1;                         // bottom borderline.
            int labellocx = size[1] - (3 + (int)label.length()); // padding 3.

            mvwprintw(m_window, labellocy, labellocx, label.c_str());
            wrefresh(m_window);

          int yMax, xMax;
          getmaxyx(m_window, yMax, xMax);
          move(yMax / 2, xMax / 2);

          keypad(m_window, true);

          int selection;
          int highlighted = 0;

          std::vector<Kfield> fields = m_menu->fields();
          int numOfFields = fields.size();

          while (true) {
            for (int i = 0; i < fields.size(); i++) {
              if (i == highlighted)
                wattron(m_window, A_REVERSE);

              int yAlign, xAlign;
              int left, right, top, bottom, v_centre, h_centre;

              // left align
              left = 1;

              // right align
              int longest_string_len = 0;
              std::for_each(fields.begin(), fields.end(), [&longest_string_len](const Kfield &field){

                std::string str = field.name();

                int len = str.length();
                if (len > longest_string_len)
                  longest_string_len = len;

              });
              right = xMax - longest_string_len -
                  1;

              // centre align
              v_centre = i + (yMax / 2) - numOfFields;
              h_centre = (xMax - fields[i].name().length()) / 2;

              // top align
              top =  i + 1;
              bottom = yMax - 2  - numOfFields + i;

              if(alignment.first == 1 )
                xAlign = right;
              else if(alignment.first == 0)
                xAlign = h_centre;
              else if(alignment.first == -1)
                  xAlign = left;

              if(alignment.second == 1)
                  yAlign = top;
              else if(alignment.second == 0)
                  yAlign = v_centre;
              else if (alignment.second == -1)
                yAlign = bottom;

              mvwprintw(m_window, yAlign, xAlign, fields[i].name().c_str());
              wattroff(m_window, A_REVERSE);
            }
            selection = wgetch(m_window);

            switch (selection) {
            case KEY_UP:
              highlighted--;
              if (highlighted == -1) {
                highlighted = 0;
              }
              break;
            case KEY_DOWN:
              highlighted++;
              if (highlighted == fields.size()) {
                highlighted = (int)fields.size() - 1;
              }
            default:
              break;
            }

            if (selection == 10) {
              break;
            }
            clear();
            mvwin(m_window, yMax / 2, xMax / 2);
            printw("%s %s", "you selected: ", fields[highlighted].name().c_str());
            auto fieldScript = fields[highlighted].script();
            fieldScript(); // performs the script.
          }

        }

        wrefresh(m_window);
        getch();
    }

    WINDOW *Kwindow::window() const { return m_window; }
    void Kwindow::setAnimation(int start, int finish) {
        m_start = start;
        m_end = finish;
    };

    void Kwindow::cursor(int code) { curs_set(code); }
    Kwindow::~Kwindow() {

        // BOSS_ANIMATION(m_end);

        delwin(m_window);
    };

}