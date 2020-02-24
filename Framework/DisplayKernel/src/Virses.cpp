#include "../inc/Virses.h"

#include <iostream>
#include <string>

namespace BlackOS {
namespace DisplayKernel {

void Vcontroller::moveCursor(int const rows, int const cols) {
  printf("\033[%d;%dH", rows, cols);
  _cursorY = rows;
  _cursorX = cols;
}
void Vcontroller::insert(std::string const &str) {
  std::cout << str;
  _cursorX += str.length();
}
} // namespace DisplayKernel
} // namespace BlackOS
