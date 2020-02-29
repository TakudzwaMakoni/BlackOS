#include "../inc/Kcanvas.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <thread>
#include <vector>

#define WORLD_WIDTH (COLS - 2)
#define WORLD_HEIGHT (LINES - 2)
#define Y_CENTRE (LINES - WORLD_HEIGHT) / 2
#define X_CENTRE (COLS - WORLD_WIDTH) / 2

// namespace BlackOS {
// namespace BlackOSDisplay {

using namespace BlackOS::DisplayKernel;

int main(int argc, const char *argv[]) {
  initscr();
  cbreak();
  cursor(0);

  WINDOW *world = newwin(0, 0, 0, 0);

  std::string canvasName = "BlackOS canvas version 1.0 ";

  Kcanvas canvas(canvasName, WORLD_HEIGHT, WORLD_WIDTH, Y_CENTRE, X_CENTRE);

  canvas.setWin(world);
  canvas.setTitle("test canvas");
  canvas.showTitle(true);
  canvas.borderStyle(0);
  canvas.display();

  canvas.fill('x', true);
  canvas.refresh();

  for (int i = 1; i < WORLD_HEIGHT - 1; i++) {
    for (int j = 1; j < WORLD_WIDTH - 1; j++) {
      canvas.erase(i, j, i, j + 1);
      std::this_thread::sleep_for(std::chrono::seconds(1));
      canvas.refresh();
    }
  }

  wgetch(world);
  delwin(world);
  endwin();

  return 0;
}
//} // namespace BlackOSDisplay
//} // namespace BlackOS
