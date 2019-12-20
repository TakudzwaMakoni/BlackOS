#include "../inc/Kcanvas.h"

#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <unistd.h>
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

  std::string canvasName = "BlackOS canvas version 1.0 ";

  BlackOSDisplay::Kcanvas canvas(canvasName, WORLD_HEIGHT, WORLD_WIDTH,
                                 Y_CENTRE, X_CENTRE);

  canvas.setWin(world);
  canvas.setTitle("test canvas");
  canvas.showTitle(true);
  canvas.borderStyle(0);
  canvas.fill('x');
  for (int i = 1; i < WORLD_HEIGHT - 1; i++) {
    for (int j = 1; j < WORLD_WIDTH - 1; j++) {
      canvas.kErase(i, j, i, j + 1);
      usleep(1000);
      canvas.display();
    }
  }
  wgetch(world);
  delwin(world);
  endwin();
  return 0;
}
