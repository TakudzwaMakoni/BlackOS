//
// Created by Takudzwa Makoni on 2019-07-12.
//

#include "../header/Kwindow.h"
#include "../header/BlackOSScripts.h"

Kwindow::Kwindow(std::vector<int> size,
                 Kmenu *menu, char alignment,
                 directive init_directive,
                 directive end_directive) : m_menu(menu) {

    curs_set(0);

    char star = '*'; //set custom border
    m_window = newwin(size[0], size[1], size[2], size[3]);
    //box(m_window, 0 , 0;
    wborder(m_window, 0, 0, 0, 0, (int)star, (int)star, (int)star, (int)star);
    refresh();

    if(m_menu != nullptr){

        // add label
        auto label = m_menu->name();

        int labellocy = size[0] - 1; // bottom borderline.
        int labellocx = size[1] - (3 + (int)label.length()); // padding 3.

        mvwprintw(m_window,labellocy,labellocx,label.c_str());
        wrefresh(m_window);

        loadMenu(m_window, m_menu, alignment);

    }

    wrefresh(m_window);
    getch();

}

WINDOW *Kwindow::window() const { return m_window; }
void Kwindow::setAnimation(int start, int finish){ m_start = start; m_end = finish; };
Kmenu *Kwindow::menu() const { return m_menu; }
void Kwindow::cursor(int code) { curs_set(code); }
void Kwindow::loadMenu(WINDOW *window, const Kmenu *menu, const char &alignment){

  int yMax,xMax;
  getmaxyx(window, yMax, xMax);
  move(yMax/2,xMax/2);

  keypad(window, true);

  int selection;
  int highlighted = 0;

  fieldVector fields = menu->fields();
  int numOfFields = fields.size();

  while(true){
    for(int i = 0; i < fields.size(); i++ ){
      if(i == highlighted)
        wattron(window, A_REVERSE);
      int yAlign, xAlign;
      switch(alignment){
      case  'c' :
        yAlign =  i + (yMax/2) - numOfFields;
        xAlign = (xMax - fields[i].name().length()) / 2;
        break;
      case 'l' : yAlign = i + (yMax/2) - numOfFields;
        xAlign = 1;
        break;
      default:
        yAlign =  i + (yMax/2) - numOfFields;
        xAlign = (xMax - fields[i].name().length()) / 2;
      }
      mvwprintw(window, yAlign, xAlign,
                fields[i].name().c_str());
      wattroff(window, A_REVERSE);

    }
    selection = wgetch(window);

    switch (selection) {
    case KEY_UP:
      highlighted--;
      if(highlighted == -1){
        highlighted = 0;
      }
      break;
    case KEY_DOWN:
      highlighted++;
      if(highlighted == fields.size() ){
        highlighted = (int)fields.size()- 1;
      }
    default:
      break;
    }

    if(selection == 10){
      break;
    }
    clear();
    mvwin(window,yMax/2,xMax/2);
    printw( "%s %s", "you selected: ",fields[highlighted].name().c_str() );
    auto fieldScript = fields[highlighted].script();
    fieldScript(); // performs the script.

  }
}

Kwindow::~Kwindow(){

    // BOSS_ANIMATION(m_end);

    delwin(m_window);

};
