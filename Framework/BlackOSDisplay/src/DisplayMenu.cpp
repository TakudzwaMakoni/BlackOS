#include "../inc/Kfield.h"
#include "../inc/Kgrid.h"
#include "../inc/Kmenu.h"
#include "../inc/Kwindow.h"

#include "../inc/Directives.h"

#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

#define WORLD_WIDTH (COLS - 2)
#define WORLD_HEIGHT (LINES - 2)
#define Y_CENTRE (LINES - WORLD_HEIGHT) / 2
#define X_CENTRE (COLS - WORLD_WIDTH) / 2

int main(int argc, const char *argv[]) {

  initscr();
  cbreak();
  cursor(0);
  while (true) {

    // create main menu

    std::string fname1 = "* HOME *";
    std::string fmessage1 = "you selected 'home'.";

    std::string fname2 = "* SETTINGS *";
    std::string fmessage2 = "you selected 'settings'.";

    std::string fname3 = "* PROFILES *";
    std::string fmessage3 = "you selected 'profiles'.";

    std::string fname4 = "* QUIT *";
    std::string fmessage4 = "you selected 'quit'.";

    BlackOSDisplay::Kfield field1(fname1, fmessage1);
    BlackOSDisplay::Kfield field2(fname2, fmessage2);
    BlackOSDisplay::Kfield field3(fname3, fmessage3);
    BlackOSDisplay::Kfield field4(fname4, fmessage4, Directives::exitProgram);

    std::vector<BlackOSDisplay::Kfield> mainFields = {field1, field2, field3,
                                                      field4};
    std::string mainMenuName = "BlackOS version 1.0 ";
    BlackOSDisplay::Kmenu main_menu(1, mainMenuName, WORLD_HEIGHT, WORLD_WIDTH,
                                    Y_CENTRE, X_CENTRE);

    main_menu.setFieldAlign(0, 0);
    main_menu.setBorderStyle(0, 0, 0, 0, '*', '*', '*', '*');
    main_menu.label("main menu");
    main_menu.setFields(mainFields);
    main_menu.fieldPadding(true);
    main_menu.display();
    auto selectedField = main_menu.getSelectedField();

    WINDOW *main_win = main_menu.window();
    std::string msg = selectedField.message();
    int msgLen = msg.length();
    wclear(main_win);
    mvwprintw(main_win, main_menu.centreY(), main_menu.centreX() - msgLen,
              selectedField.message().c_str());

    auto script = selectedField.script();
    script();
    wgetch(main_win);

  } // while true
  endwin();
  return 0;
}