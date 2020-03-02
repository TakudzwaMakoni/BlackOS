/**
 *  Copyright 2019 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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
#include "../../inc/Directives.h"
#include "../../inc/Kfield.h"
#include "../../inc/Kmenu.h"
#include <memory>
#include <vector>

#define WORLD_WIDTH (COLS - 2)
#define WORLD_HEIGHT (LINES - 2)
#define Y_CENTRE (LINES - WORLD_HEIGHT) / 2
#define X_CENTRE (COLS - WORLD_WIDTH) / 2

namespace BlackOS {
namespace Display {
namespace TestHelpers {

Kmenu_sptr testMenu() {
  std::string const menuName = "test_menu";
  auto menu = std::make_shared<Kmenu>(menuName, WORLD_HEIGHT, WORLD_WIDTH,
                                      Y_CENTRE, X_CENTRE);
  menu->setTitle("test_menu_title");
  return menu;
}

Kmenu_sptr testMenuWithEightPaginatedFields(int pagination) {
  std::string const menuName = "test_menu";
  auto menu = std::make_shared<Kmenu>(menuName, WORLD_HEIGHT, WORLD_WIDTH,
                                      Y_CENTRE, X_CENTRE);
  std::string const fname0 = "FIELD 1";
  std::string const fmessage0 = "you selected 'FIELD 1'.";
  std::string const fname1 = "FIELD 2";
  std::string const fmessage1 = "you selected 'FIELD 2'.";
  std::string const fname2 = "FIELD 3";
  std::string const fmessage2 = "you selected 'FIELD 3'.";
  std::string const fname3 = "FIELD 4";
  std::string const fmessage3 = "you selected 'FIELD 4'.";
  std::string const fname4 = "FIELD 5";
  std::string const fmessage4 = "you selected 'FIELD 5'.";
  std::string const fname5 = "FIELD 6";
  std::string const fmessage5 = "you selected 'FIELD 6'.";
  std::string const fname6 = "FIELD 7";
  std::string const fmessage6 = "you selected 'FIELD 7'.";
  std::string const fname7 = "QUIT";

  Kfield field0(fname0, Directives::doNothing, fmessage0);
  Kfield field1(fname1, Directives::doNothing, fmessage1);
  Kfield field2(fname2, Directives::doNothing, fmessage2);
  Kfield field3(fname3, Directives::doNothing, fmessage3);
  Kfield field4(fname4, Directives::doNothing, fmessage4);
  Kfield field5(fname5, Directives::doNothing, fmessage5);
  Kfield field6(fname6, Directives::doNothing, fmessage6);
  Kfield field7(fname7, Directives::exitProgram);

  std::vector<Kfield> test_fields = {field0, field1, field2, field3,
                                     field4, field5, field6, field7};

  menu->setFields(test_fields);
  menu->setFieldStyle("!TEST!");
  menu->setFieldAlign(0, 0);
  menu->borderStyle('!', '!', '!', '!', '!', '!', '!', '!');
  menu->addFieldPadding();
  menu->setTitle("TEST_MENU with" + std::to_string(pagination) +
                 "fields per page.");
  menu->showTitle();
  menu->paginate(pagination);

  return menu;
}

Kmenu_sptr testMenuInitialisedWithSizeAndPos(int const sizeY, int const sizeX,
                                             int const posY, int const posX) {
  std::string const menuName = "test_menu";
  auto menu = std::make_shared<Kmenu>(menuName, sizeY, sizeX, posY, posX);
  return menu;
}
} // namespace TestHelpers
} // namespace Display
} // namespace BlackOS
