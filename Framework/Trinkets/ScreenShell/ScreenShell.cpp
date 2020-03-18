/**
 * Tr(inkets) ScreenShell
 *
 * Copyright (C) 2020 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "ScreenShell.h"

using namespace BlackOS::Trinkets;

// thanks to Sarah Vessels for Pipe/Redirect:
// https://www.3till7.net/2008/11/29/c-shell-with-forks-and-pipes/index.html
int main() {

  // use stdscreen
  auto display = generateScreen();

  ScreenShell shell(display);
  shell.initShell();

  while (1) {
    shell.displayPrompt();
    shell.readArgs();
    shell.runCommand();
    shell.resetArgs();
  }

  return RESULT::SUCCESS;
}
