#include "../inc/MenuGenerator.h"
#include "../../inc/DisplayHelpers.h"
#include "../../inc/Kmenu.h"
#include <memory>
#include <vector>

namespace BlackOS {
namespace DisplayKernel {
namespace TestHelpers {

Kmenu_sptr testMenu() {
  auto const termSz = TERMINAL_SIZE();
  size_t ROWS = termSz[0];
  size_t COLS = termSz[1];

  std::string const menuName = "test_menu";
  auto menu = std::make_shared<Kmenu>(menuName, ROWS, COLS, 0, 0);
  return menu;
}

Kmenu_sptr testMenuWithEightPaginatedFields(int pagination) {
  auto const termSz = TERMINAL_SIZE();
  size_t ROWS = termSz[0];
  size_t COLS = termSz[1];

  std::string const menuName = "test_menu";
  auto menu = std::make_shared<Kmenu>(menuName, ROWS, COLS, 0, 0);

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

Kmenu_sptr testMenuInitialisedWithSizeAndPos(int const sizeY, int const sizeX,
                                             int const posY, int const posX) {
  std::string const menuName = "test_menu";
  auto menu = std::make_shared<Kmenu>(menuName, sizeY, sizeX, posY, posX);
  return menu;
}
} // namespace TestHelpers
} // namespace DisplayKernel
} // namespace BlackOS
