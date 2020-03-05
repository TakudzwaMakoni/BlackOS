/**
 * MenuGenerator
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

#include "../inc/MenuGenerator.h"
#include "DisplayHelpers.h"
#include "Menu.h"

#include <memory>
#include <vector>

namespace BlackOS {
namespace DisplayKernel {
namespace TestHelpers {

Menu_sptr testMenu() {
  auto const termSz = TERMINAL_SIZE();
  size_t ROWS = termSz[0];
  size_t COLS = termSz[1];

  auto menu = std::make_shared<Menu>(ROWS, COLS, 0, 0);
  return menu;
}

Menu_sptr testMenuWithEightPaginatedFields(int pagination) {
  auto const termSz = TERMINAL_SIZE();
  size_t ROWS = termSz[0];
  size_t COLS = termSz[1];

  auto menu = std::make_shared<Menu>(ROWS, COLS, 0, 0);

  std::string const fname0 = "FIELD 1";
  std::string const fname1 = "FIELD 2";
  std::string const fname2 = "FIELD 3";
  std::string const fname3 = "FIELD 4";
  std::string const fname4 = "FIELD 5";
  std::string const fname5 = "FIELD 6";
  std::string const fname6 = "FIELD 7";
  std::string const fname7 = "QUIT";

  std::vector<std::string> test_fields = {fname0, fname1, fname2, fname3,
                                          fname4, fname5, fname6, fname7};

  menu->loadFields(test_fields);
  menu->fieldStyle("!TEST!");
  menu->loadFieldAlignment(0, 0);
  menu->borderStyle('!', '!', '!', '!', '!', '!', '!', '!');
  menu->loadTitle("TEST_MENU with" + std::to_string(pagination) +
                  "fields per page.");
  menu->paginate(pagination);

  return menu;
}

Menu_sptr testMenuInitialisedWithSizeAndPos(int const sizeY, int const sizeX,
                                            int const posY, int const posX) {

  auto menu = std::make_shared<Menu>(sizeY, sizeX, posY, posX);
  return menu;
}
} // namespace TestHelpers
} // namespace DisplayKernel
} // namespace BlackOS
