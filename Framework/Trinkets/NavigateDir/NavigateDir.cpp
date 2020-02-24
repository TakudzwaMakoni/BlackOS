// navigate-dir 2020 by takudzwa Makoni (c)

#include "inc/NavigationHelpers.h"
#include "inc/Navigator.h"

#include "ncurses.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace BlackOS::Trinkets;

int main(int argc, char const *argv[]) {

  long const cursor_start = strtol(argv[1], NULL, 10);

  bool const showHidden = 0;
  std::string const parentPath = "/home/takudzwa";

  Navigator testNav(parentPath, 0);

  auto fields = testNav.generateFields();

  // enter curses mode
  initscr();
  cbreak();
  cursor(0);

  size_t const width = fields[0].name().length(); // same length for all fields
  size_t const height = fields.size() + 1;

  std::string const menuID = "Navigator";
  BlackOS::DisplayKernel::Kmenu NavigationMenu(menuID, height, width + 1,
                                               cursor_start, 0);

  NavigationMenu.loadTitle(testNav.generateTitle(),
                           BlackOS::DisplayKernel::TitleStyle::underline);
  NavigationMenu.loadFields(fields);
  NavigationMenu.loadFieldAlignment(-1, 1);

  NavigationMenu.setWin(1);
  NavigationMenu.hideBorder();
  NavigationMenu.showTitle();
  NavigationMenu.display();

  size_t const fieldIdx = NavigationMenu.selectedFieldIndex();

  std::string chosenPath = testNav.children()[fieldIdx];
  std::string type = pathType(chosenPath);
  std::ofstream navigationDir("/tmp/navigationDirectory.txt");
  navigationDir << chosenPath;
  NavigationMenu.setWin(0);
  endwin();

  return 0;
}
