// navigate-dir 2020 by takudzwa Makoni (c)

#include "inc/NavigationHelpers.h"
#include "inc/Navigator.h"

#include "ncurses.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace BlackOS::Trinkets;

int main(int argc, char const *argv[]) {
  std::string test;
  bool showHidden = 0;
  BlackOS::DisplayKernel::Kmenu menu(test, 1, 1, 1, 1);
  std::string parentPath = "/home/takudzwa";

  Navigator testNav(parentPath, 0);

  auto children = testNav.children();
  auto fields = loadFields(children, parentPath);

  // enter curses mode
  initscr();
  cbreak();
  cursor(0);

  // get current cursor position from system call

  endwin();
  return 0;
}
