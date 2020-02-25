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

  std::string trueVal = "True";
  bool const showHidden = argv[1] == trueVal ? 1 : 0;
  long cursor_start = strtol(argv[2], NULL, 10);
  std::string parentPath = argv[3];

  // remove trailing '/' if any
  if (parentPath.back() == '/') {
    parentPath.pop_back();
  }

  Navigator testNav(parentPath, showHidden);
  auto fields = testNav.generateFields();

  // enter curses mode
  initscr();
  cbreak();
  cursor(0);

  std::string title = testNav.generateTitle();
  size_t fieldSz = fields.size();

  size_t width = title.length() + 1;
  size_t height = fieldSz + 2;

  std::string const menuID = "Navigator";
  size_t pagination = fieldSz;

  if (height > LINES) {
    cursor_start = 0;
    pagination = LINES - 2;
    height = LINES;
  }

  BlackOS::DisplayKernel::Kmenu NavigationMenu(menuID, height, width,
                                               cursor_start, 0);

  NavigationMenu.loadTitle(title,
                           BlackOS::DisplayKernel::TitleStyle::underline);
  NavigationMenu.loadFields(fields);
  NavigationMenu.loadFieldAlignment(-1, 1);
  NavigationMenu.paginate(pagination);

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
