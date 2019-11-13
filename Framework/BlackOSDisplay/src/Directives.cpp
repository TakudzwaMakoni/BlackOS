//
// Created by Takudzwa Makoni on 2019-07-13.
//

// test
#include "../inc/Directives.h"
#include <ncurses.h>

namespace Directives {
void doNothing() {}
void exitProgram() {
  endwin();
  exit(0);
}
void writeToFile() {
  std::string buf, fullPath;

  fullPath = "testexample.txt";
  buf = "this is a test!";
  std::ofstream file;
  file.open(fullPath);
  file << buf;
  file.close();
}
void animate(int aniCode) {}

} // namespace Directives
