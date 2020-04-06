/**
 * DisplayScreen
 *
 * Copyright (C) 2019, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "Screen.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using namespace BlackOS::DisplayKernel;

int main(int argc, const char *argv[]) {

  Screen screen;

  auto termSz = TERMINAL_SIZE();
  size_t termSzY = termSz[0];
  size_t termSzX = termSz[1];

  screen.setWin(WIN_SET_CODE::INIT_PARENT);
  screen.loadTitle("test screen", A_REVERSE);

  screen.hideBorder();
  screen.showTitle();

  screen.fill('x', 1);
  refresh();

  for (int i = 0; i < termSzY; i++) {
    for (int j = 0; j < termSzX - 1; j++) {
      screen.erase(i, j, i, j + 1);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      refresh();
    }
  }

  screen.pause();
  screen.setWin(WIN_SET_CODE::KILL_PARENT);

  return 0;
}
