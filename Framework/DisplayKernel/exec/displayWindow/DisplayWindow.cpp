/**
 * DisplayWindow
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

#include "Window.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using namespace BlackOS::DisplayKernel;

int main(int argc, const char *argv[]) {

  Window window(1, 100, 0, 0);

  size_t winSzY = window.winSzY();
  size_t winSzX = window.winSzX();
  size_t winPosY = window.winSzY();
  size_t winPosX = window.winSzX();

  window.hideTitle();
  window.hideBorder();
  window.setWin(1);

  window.fill('x', 0);
  window.refresh();

  for (int i = 0; i < winSzY; i++) {
    for (int j = 0; j < winSzX - 1; j++) {
      window.erase(i, j, i, j + 1);
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      window.refresh();
    }
  }

  window.pause();
  window.setWin(0);

  return 0;
}
