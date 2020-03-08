#ifndef TRINKETS_NATIVES_H
#define TRINKETS_NATIVES_H

/**
 * Natives
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

#include "../inc/PathController.h" // NavigateDir, ListChildren
#include "DisplayHelpers.h"        //(libDisplayKernel) NavigateDir
#include "Menu.h"                  //(libDisplayKernel) NavigateDir
#include "NavigationHelpers.h"     //(libDisplayKernel) NavigateDir
#include "Window.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {
enum userInput { up = -5, down, left, right, backspace };
int parseUserInput(char **argv);
int changeDir(char const *path = nullptr);
int listChildren(int, char **, std::vector<std::string> &v);
int navigateDir(int, char **, int y = -1, int x = -1);
} // namespace Trinkets
} // namespace BlackOS

#endif
