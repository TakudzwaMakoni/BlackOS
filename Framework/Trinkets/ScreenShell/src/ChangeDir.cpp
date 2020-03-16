/**
 * Tr(inkets) ScreenShell NavigateDir
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

#include "../ScreenShell.h"

namespace BlackOS {
namespace Trinkets {

int ScreenShell::changeDir() {
  if (_ARGC == 1) {
    char const *homeDir = getenv("HOME");
    chdir(homeDir);
  } else if (_ARGC == 2) {

    std::string const &path = _ARGV[1];
    if (chdir(path.c_str()) != 0) {
      char errStr[256] = "cd: ";
      perror(strcat(errStr, path.c_str()));
      return 1;
    }
  }
  return 0;
}
} // namespace Trinkets
} // namespace BlackOS
