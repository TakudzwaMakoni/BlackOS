#include "../inc/MenuGenerator.h"
#include "../../inc/Directives.h"
#include "../../inc/Kfield.h"
#include "../../inc/Kmenu.h"
#include <memory>
#include <vector>

namespace BlackOS {
namespace DisplayKernel {
namespace TestHelpers {

Kmenu_sptr testMenu() {
  std::string const menuName = "test_menu";
  auto menu = std::make_shared<Kmenu>(menuName, WORLD_HEIGHT, WORLD_WIDTH,
                                      Y_CENTRE, X_CENTRE);
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

  menu->loadFields(test_fields);
  menu->fieldStyle("!TEST!");
  menu->alignFields(0, 0);
  menu->borderStyle('!', '!', '!', '!', '!', '!', '!', '!');
  menu->loadTitle("TEST_MENU with" + std::to_string(pagination) +
                  "fields per page.");
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
} // namespace DisplayKernel
} // namespace BlackOS
