/**
 * Shell
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

#include "../inc/TrShell.h"

namespace BlackOS {
namespace Trinkets {

Screen_sptr generateScreen() {
  auto screen = std::make_shared<DisplayKernel::Screen>();
  return screen;
}

Window_sptr generateWindow() {
  auto const termSz = DisplayKernel::TERMINAL_SIZE();
  size_t const ROWS = termSz[0];
  size_t const COLS = termSz[1];
  auto window = std::make_shared<DisplayKernel::Window>(ROWS, COLS, 0, 0);
  return window;
}

Shell::Shell(DisplayObject_sptr &display) {
  _displayType = display->winType();
  _display = display;
}

Shell::~Shell() {
  if (_display->windowSet())
    _display->setWin(0);
  std::cout << "exited Tr.\n";
}

/*
Shell::Shell(Screen_sptr &display) {
  _displayType = "Screen";
  display->setWin(1);
  display->insert("this a test", 0, 0);
  display->pause();
  display->setWin(0);
}

Shell::Shell(Window_sptr &display) {
  _displayType = "Window";
  display->setWin(1);
  display->insert("this a test", 0, 0);
  display->pause();
  display->setWin(0);
}
*/
void clearDisplay() {}

void Shell::initShell() {

  std::cout << CLEAR_SCREEN_ANSI;
  std::string title = "Tr(inkets) Shell (C)";
  std::string version = "1.0";
  std::string repo = "repo: Dark Horse (DH)";
  std::string license = "GPL v3.0 licensed";
  std::string year = "2020";
  std::string language = "C++";
  std::string author = "Takudzwa Makoni";
  std::string git = "https://github.com/TakudzwaMakoni";

  std::vector<std::string> v{title, version,  repo,   license,
                             year,  language, author, git};
  // splashScreen(v);
}

void Shell::quit(std::string) { std::cout << "goodbye!\n"; }

// Given the number of arguments and an array of arguments, this will
// execute those arguments.  The first argument in the array should be a
// command.
void runCommand(int, char **);

} // namespace Trinkets
} // namespace BlackOS
