/**
 *  Copyright 2019 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
 *
 * This Program is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This Program is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This Program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

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

// namespace BlackOS {
// namespace BlackOSDisplay {

using namespace BlackOS::Display;

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
      canvas.kErase(i, j, i, j + 1);
      usleep(9000);
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
