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

  WINDOW *world = newwin(WORLD_HEIGHT, WORLD_WIDTH, Y_CENTRE, X_CENTRE);

  std::string fname0 = "SOMETHING";
  std::string fmessage0 = "you selected 'something'.";
  std::string fname1 = "HOME";
  std::string fmessage1 = "you selected 'home'.";
  std::string fname2 = "SETTINGS";
  std::string fmessage2 = "you selected 'settings'.";
  std::string fname3 = "PROFILE";
  std::string fmessage3 = "you selected 'profiles'.";
  std::string fname4 = "QUIT";
  std::string fmessage4 = "you selected 'quit'.";
  std::string fname5 = "SOMETHING ELSE";
  std::string fmessage5 = "you selected 'something else'.";

  BlackOSDisplay::Kfield field0(fname0, Directives::doNothing, fmessage0);
  BlackOSDisplay::Kfield field1(fname1, Directives::doNothing, fmessage1);
  BlackOSDisplay::Kfield field2(fname2, Directives::doNothing, fmessage2);
  BlackOSDisplay::Kfield field3(fname3, Directives::doNothing, fmessage3);
  BlackOSDisplay::Kfield field4(fname4, Directives::exitProgram);
  BlackOSDisplay::Kfield field5(fname5, Directives::doNothing, fmessage5);

  std::vector<BlackOSDisplay::Kfield> mainFields = {
      field1, field2, field3, field4, field5,
      field0, field0, field0, field0, field0,
      /*field5, field0, field0, field0*/};

  std::string mainMenuName = "BlackOS version 1.0 ";
  while (true) {

    BlackOSDisplay::Kmenu main_menu(mainMenuName, WORLD_HEIGHT, WORLD_WIDTH,
                                    Y_CENTRE, X_CENTRE);
    main_menu.setWin(world); // must set the window!
    main_menu.setFieldAlign(0, 0);
    main_menu.borderStyle(' ', ' ', ' ', ' ', '*', '*', '*', '*');
    main_menu.paginate(4);
    main_menu.setFields(mainFields);
    main_menu.setTitle("BlackOS Menu");
    main_menu.showTitle();
    main_menu.setFieldStyle("="); // must do this before padding
    main_menu.addFieldPadding();  // must do this before displaying
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
  delwin(world);
  endwin();
  return 0;
}
