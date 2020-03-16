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

namespace {

std::string produceBanner(std::string const &str, int winwidth) {
  int widthA = (winwidth + str.length()) / 2;
  std::stringstream ss;
  ss << std::setw(widthA) << std::right << str;
  return ss.str();
}

} // namespace

void ScreenShell::splashScreen(std::vector<std::string> const &argv) {

  std::string spc = " "; // space string for easier reading.
  std::string marginline =
      "* * * * * * * * * * * * * * * * * * * * * * * * * * * * "
      "* * * *"; // border

  if (argv.size() != 8) {
    return;
  }

  std::string title;
  std::string version;
  std::string repo;
  std::string license;
  std::string year;
  std::string language;
  std::string author;
  std::string git;

  title = argv[0] + spc;
  version = argv[1] + spc;
  repo = argv[2] + spc;
  license = argv[3] + spc;
  year = argv[4] + spc;
  language = argv[5] + spc;
  author = argv[6] + spc;
  git = argv[7] + spc;

  std::string pushDown((_termSzY - 8 /*banner height*/) / 2, '\n');
  printw(pushDown.c_str());

  printw(produceBanner(marginline, _termSzX).c_str());
  printw("\n");
  printw(produceBanner(title + "v" + version + ", " + repo, _termSzX).c_str());
  printw("\n");
  printw(
      produceBanner(license + year + ", " + "written in " + language, _termSzX)
          .c_str());
  printw("\n");
  printw(produceBanner("by " + author, _termSzX).c_str());
  printw("\n");
  printw(produceBanner("git: " + git, _termSzX).c_str());
  printw("\n");
  printw(produceBanner(marginline, _termSzX).c_str());
  printw("\n");
}

} // namespace Trinkets
} // namespace BlackOS
