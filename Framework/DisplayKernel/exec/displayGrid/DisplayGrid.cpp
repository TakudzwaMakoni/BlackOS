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

// namespace BlackOS {
// namespace DisplayKernel {

using namespace BlackOS::DisplayKernel;

int main(int argc, const char *argv[]) {
  initscr();
  cbreak();
  cursor(0);

  WINDOW *world = newwin(0, 0, 0, 0);

  std::string name = "Matrix A";
  Eigen::Matrix<double, 4, 4> testData;
  testData << 11.089, 2.436, 3.5268, 4.721346, 5.8654, 6.345, 7.1743, 8.13673,
      9100.4564, 12, 231, 3453, 1231, 453, 13, 11;
  std::vector<double> testData2{34.2763,     21.25021, 37.243,  54.2343, 5.0243,
                                6546.218856, 7.774276, 75358.2, 9.2};
  Kgrid<double, 4, 4> grid(name, WORLD_HEIGHT, WORLD_WIDTH, Y_CENTRE, X_CENTRE);

  auto mat = grid.matrix();
  while (true) {
    grid.setWin(world);   // must set the window!
    grid.write(testData); // must write the data!
    grid.setPrecision(8);
    grid.borderStyle();
    grid.gridLines(false);
    grid.label(grid.name());
    grid.setTitle("Matrix Editor Program");
    grid.showTitle(true);
    grid.align(0, 0);

    grid.display();
    grid.fill(' ', true);

    auto selectedElement = grid.selectedRaw();

    mvwprintw(grid_window, Y_CENTRE, X_CENTRE,
              std::to_string(selectedElement).c_str());
    wgetch(grid_window);

    wclear(grid_window);
  } // while true
  delwin(world);
  endwin();
  return 0;
}
//} // namespace DisplayKernel
//} // namespace BlackOS
