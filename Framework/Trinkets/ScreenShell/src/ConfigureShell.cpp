/**
 * Tr ScreenShell
 *
 * Copyright (C) 2020, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "../ScreenShell.h"

namespace BlackOS {
namespace Trinkets {

int ScreenShell::configureShell() {
  if (_ARGV.size() != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }
  std::string const argv1 = _ARGV[1];
  std::string const argv2 = _ARGV[2];

  configureShell(argv1, argv2);

  return 0;
}

} // namespace Trinkets
} // namespace BlackOS
