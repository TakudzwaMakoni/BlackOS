#ifndef VIRSES_H
#define VIRSES_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace BlackOS {
namespace DisplayKernel {
int const LINES = atoi(getenv("LINES"));
int const COLS = atoi(getenv("COLS"));

class Vcontroller {
public:
  void moveCursor(int const rows, int const cols);
  void insert(std::string const &str);

private:
  size_t _cursorX;
  size_t _cursorY;
};

} // namespace DisplayKernel
} // namespace BlackOS
#endif
