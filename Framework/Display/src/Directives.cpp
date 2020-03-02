/**
 *  Copyright 2019-07-13 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "../inc/Directives.h"
#include <ncurses.h>

namespace BlackOS {
namespace Display {
namespace Directives {
void doNothing() {}
void exitProgram() {
  endwin();
  exit(0);
}
void writeToFile() {
  std::string buf, fullPath;

  fullPath = "testexample.txt";
  buf = "this is a test!";
  std::ofstream file;
  file.open(fullPath);
  file << buf;
  file.close();
}
void animate(int aniCode) {}
} // namespace Directives
} // namespace Display
} // namespace BlackOS
