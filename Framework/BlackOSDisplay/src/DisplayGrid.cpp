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

  std::string name = "test";

  Eigen::Matrix<double, 3, 3> testData;
  testData << 11.089, 2.436, 3.5268, 4.721346, 5.8654, 6.345, 7.1743, 8.13673,
      9100.4564;
  std::vector<double> testData2{34.2,   21.25, 37.2,    54.2, 5.2,
                                6546.2, 7.2,   75358.2, 9.2};
  BlackOSDisplay::Kgrid<double, 3, 3> grid(name, testData2, WORLD_HEIGHT,
                                           WORLD_WIDTH, Y_CENTRE, X_CENTRE);
  grid.setPrecision(2);
  grid.borderStyle();
  grid.gridLines(false);
  grid.label(grid.name());
  grid.setTitle("Matrix Editor Program");
  grid.showTitle(true);
  grid.align(0, 0);
  auto mat = grid.matrix();

  while (true) {
    //  create main window

    printw(std::to_string(mat.coeff(0, 0)).c_str());

    grid.display();
    auto selectedElement = grid.selectedRaw();
    WINDOW *grid_window = grid.window();
    wclear(grid_window);
    mvwprintw(grid_window, grid.centreY(), grid.centreX(),
              std::to_string(selectedElement).c_str());
    wgetch(grid_window);

    wclear(grid_window);
  } // while true
  endwin();
  return 0;
}
