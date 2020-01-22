
#include "../inc/Directives.h"
#include "../inc/Kfield.h"
#include "../inc/Kmenu.h"
#include "../inc/Kwindow.h"

#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

#define WORLD_WIDTH (COLS - 2)
#define WORLD_HEIGHT (LINES - 2)
#define Y_CENTRE (LINES - WORLD_HEIGHT) / 2
#define X_CENTRE (COLS - WORLD_WIDTH) / 2

// namespace BlackOS {
// namespace Display {

using namespace BlackOS::Display;

int main(int argc, const char *argv[]) {
  initscr();
  cbreak();
  cursor(0);

  WINDOW *world = newwin(0, 0, 0, 0);

  std::string const fname0 = "FIELD 1";
  std::string const fmessage0 = "you selected 'FIELD 1'.";
  std::string const fname1 = "FIELD 2";
  std::string const fmessage1 = "you selected 'FIELD 2'.";
  std::string const fname2 = "FIELD 3";
  std::string const fmessage2 = "you selected 'FIELD 3'.";
  std::string const fname3 = "FIELD 4";
  std::string const fmessage3 = "you selected 'FIELD 4'.";
  std::string const fname4 = "FIELD 5";
  std::string const fmessage4 = "you selected 'FIELD 5'.";
  std::string const fname5 = "FIELD 6";
  std::string const fmessage5 = "you selected 'FIELD 6'.";
  std::string const fname6 = "FIELD 7";
  std::string const fmessage6 = "you selected 'FIELD 7'.";
  std::string const fname7 = "QUIT";

  Kfield field0(fname0, Directives::doNothing, fmessage0);
  Kfield field1(fname1, Directives::doNothing, fmessage1);
  Kfield field2(fname2, Directives::doNothing, fmessage2);
  Kfield field3(fname3, Directives::doNothing, fmessage3);
  Kfield field4(fname4, Directives::doNothing, fmessage4);
  Kfield field5(fname5, Directives::doNothing, fmessage5);
  Kfield field6(fname6, Directives::doNothing, fmessage6);
  Kfield field7(fname7, Directives::exitProgram);

  std::vector<Kfield> test_fields = {field0, field1, field2, field3,
                                     field4, field5, field6, field7};

  std::string mainMenuName = "BlackOS version 1.0 ";

  Kmenu main_menu(mainMenuName, WORLD_HEIGHT, WORLD_WIDTH, Y_CENTRE, X_CENTRE);

  main_menu.setWin(world); // must set the window!

  int pagination = 3;
  main_menu.alignFields(0, 0);

  main_menu.borderStyle('o', 'o', 'o', 'o', 'o', 'o', 'o', 'o');
  main_menu.addTitle("TEST_MENU with " + std::to_string(pagination) +
                     " fields per page.");
  main_menu.setFieldStyle("!"); // must do this before padding
  main_menu.addFieldPadding();  // must do this before displaying

  main_menu.setFields(test_fields);
  main_menu.paginate(pagination);

  main_menu.display();
  auto message = main_menu.selectedField().message();
  auto winSz = main_menu.size();

  size_t centreY = winSz[0] / 2;
  size_t centreX = (winSz[1] - message.length()) / 2;

  main_menu.fill(' ', true);
  main_menu.insert(message, centreY, centreX);
  main_menu.refresh();
  main_menu.pause();

  delwin(world);
  endwin();
  return 0;
}
//} // namespace Display
//} // namespace BlackOS
