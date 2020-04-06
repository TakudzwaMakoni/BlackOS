/**
 * MenuTests
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

// Using catch2 headers

#define CATCH_CONFIG_RUNNER

#include "Menu.h"
#include "../testHelpers/inc/MenuGenerator.h"
#include "DisplayHelpers.h"
#include <catch2/catch.hpp>
#include <chrono>
#include <cstdio>
#include <thread>

using namespace BlackOS::DisplayKernel;

/*
TEST_CASE("some test", "[some_tag]") {
  auto glambda = [](WINDOW *world) {};
  initscr();
  cbreak();
  WINDOW *world = newwin(0, 0, 0, 0);
  bool exit_success = glambda(world);
  delwin(world);
  endwin();
  REQUIRE(exit_success);
}
*/

TEST_CASE("test window is resized to initialised menu size after call setWin",
          "[window]") {
  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = [&]() {
    auto const termSz = TERMINAL_SIZE();
    size_t const ROWS = termSz[0];
    size_t const COLS = termSz[1];
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(ROWS, COLS, 0, 0);

    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    size_t y = menu->winSzY();
    size_t x = menu->winSzX();
    menu->setWin(WIN_SET_CODE::KILL_PARENT);
    return y == ROWS && x == COLS;
  };

  bool resize_success = glambda();
  REQUIRE(resize_success);
}

TEST_CASE("test window is repositioned to initialised menu position after call "
          "setWin",
          "[window]") {
  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = []() {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(0, 0, 10, 20);
    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    size_t y = menu->winPosY();
    size_t x = menu->winPosX();
    menu->setWin(WIN_SET_CODE::KILL_PARENT);

    return y == 10 && x == 20;
  };

  bool reposition_success = glambda();
  REQUIRE(reposition_success);
}

TEST_CASE("test window is unset on call setWin with empty parameters",
          "[window]") {
  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = []() {
    auto const &menu = TestHelpers::testMenu();
    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    menu->setWin(WIN_SET_CODE::KILL_PARENT);
    return !menu->windowSet();
  };

  bool window_is_null = glambda();
  REQUIRE(window_is_null);
}

TEST_CASE("test menu is filled excluding title bar", "[window]") {
  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = []() {
    auto const termSz = TERMINAL_SIZE();
    size_t const ROWS = termSz[0];
    size_t const COLS = termSz[1];

    auto const &menu = TestHelpers::testMenu();
    menu->loadTitle("test_menu_title");
    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    menu->fill('w', true);

    bool filled = true;
    for (int i = 2; i < ROWS - 1 /*window coord correction*/; ++i) {
      for (int j = 1; j < COLS - 1 /*window coord correction*/; ++j) {
        auto character = menu->getCharFromWin(i, j, false);
        int ch = 'w';
        if (!(character == ch))
          filled = false;
      }
    }
    bool titleBar_preserved = true;
    for (int j = 1; j < COLS - 1 /*window coord correction*/; ++j) {
      if (menu->getCharFromWin(1, j, false) == 'w')
        titleBar_preserved = false;
    }
    menu->setWin(WIN_SET_CODE::KILL_PARENT);
    return filled && titleBar_preserved;
  };

  bool fill_successful = glambda();
  REQUIRE(fill_successful);
}

TEST_CASE("test menu is filled including title bar", "[window]") {
  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = []() {
    auto const termSz = TERMINAL_SIZE();
    size_t const ROWS = termSz[0];
    size_t const COLS = termSz[1];

    auto const &menu = TestHelpers::testMenu();
    menu->loadTitle("test_menu_title");
    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    menu->fill('w', false);

    bool filled = true;
    for (int i = 1; i < ROWS - 1 /*window coord correction*/; ++i) {
      for (int j = 1; j < COLS - 1 /*window coord correction*/; ++j) {
        auto character = menu->getCharFromWin(i, j, false);
        int ch = 'w';
        if (!(character == ch))
          filled = false;
      }
    }
    menu->setWin(WIN_SET_CODE::KILL_PARENT);
    return filled;
  };

  bool fill_successful = glambda();
  REQUIRE(fill_successful);
}

TEST_CASE("test erase call erases horizontal block", "[member_functions]") {

  // horizontal block
  auto glambda = []() {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 0, 0);
    menu->loadTitle("test_menu_title");
    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    menu->fill('w', false);
    // erase from y = 0  and x = (3,5)
    menu->erase(1, 3, 1, 5);

    bool isBlank1 = menu->getCharFromWin(1, 3) == ' ';
    bool isBlank2 = menu->getCharFromWin(1, 4) == ' ';
    bool isBlank3 = menu->getCharFromWin(1, 5) == ' ';

    menu->setWin(WIN_SET_CODE::KILL_PARENT);
    return isBlank1 && isBlank2 && isBlank3;
  };

  bool exit_success = glambda();
  REQUIRE(exit_success);
}

TEST_CASE("test eraseExcept call excepts horizontal block",
          "[member_functions]") {

  // horizontal block
  auto glambda = []() {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 0, 0);
    menu->loadTitle("test_menu_title");
    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    menu->fill('w', false);
    // erase from y = 0  and x = (3,5)
    menu->eraseExcept(1, 3, 1, 5);

    bool notBlank1 = menu->getCharFromWin(1, 3) == 'w';
    bool notBlank2 = menu->getCharFromWin(1, 4) == 'w';
    bool notBlank3 = menu->getCharFromWin(1, 5) == 'w';

    // take samples page: 1 of 3 * of window character expected to be blank
    bool isBlank1 = menu->getCharFromWin(1, 6) == ' ';
    bool isBlank2 = menu->getCharFromWin(1, 7) == ' ';
    bool isBlank3 = menu->getCharFromWin(1, 8) == ' ';

    menu->setWin(WIN_SET_CODE::KILL_PARENT);
    return notBlank1 && notBlank2 && notBlank3 && isBlank1 && isBlank2 &&
           isBlank3;
  };

  bool exit_success = glambda();
  REQUIRE(exit_success);
}

TEST_CASE("test erase call erases vertical block", "[member_functions]") {

  // vertical block
  auto glambda = []() {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 0, 0);
    menu->loadTitle("test_menu_title");
    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    menu->fill('w', false);

    // erase from y = (3,5) and x = 0
    menu->erase(3, 1, 5, 1);

    bool isBlank1 = menu->getCharFromWin(3, 1) == ' ';
    bool isBlank2 = menu->getCharFromWin(4, 1) == ' ';
    bool isBlank3 = menu->getCharFromWin(5, 1) == ' ';

    menu->setWin(WIN_SET_CODE::KILL_PARENT);
    return isBlank1 && isBlank2 && isBlank3;
  };

  bool exit_success = glambda();
  REQUIRE(exit_success);
}

TEST_CASE("test erase call erases square block", "[member_functions]") {

  // square block
  auto glambda = []() {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 1, 1);
    menu->loadTitle("test_menu_title");
    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    menu->fill('w', false);
    // erase from y = (0,2) and x = (0,2) i.e 3x3 block
    menu->erase(1, 1, 2, 2);

    bool isBlank1 = menu->getCharFromWin(1, 1) == ' ';
    bool isBlank2 = menu->getCharFromWin(1, 1) == ' ';
    bool isBlank3 = menu->getCharFromWin(1, 2) == ' ';
    bool isBlank4 = menu->getCharFromWin(1, 1) == ' ';
    bool isBlank5 = menu->getCharFromWin(1, 1) == ' ';
    bool isBlank6 = menu->getCharFromWin(1, 2) == ' ';
    bool isBlank7 = menu->getCharFromWin(2, 1) == ' ';
    bool isBlank8 = menu->getCharFromWin(2, 1) == ' ';
    bool isBlank9 = menu->getCharFromWin(2, 2) == ' ';

    menu->setWin(WIN_SET_CODE::KILL_PARENT);
    return isBlank1 && isBlank2 && isBlank3 && isBlank4 && isBlank5 &&
           isBlank6 && isBlank7 && isBlank8 && isBlank9;
  };

  bool exit_success = glambda();
  REQUIRE(exit_success);
}

TEST_CASE("test erase call erases multiple square blocks",
          "[member_functions]") {

  // square block
  auto glambda = []() {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 0, 0);
    menu->loadTitle("test_menu_title");
    menu->setWin(WIN_SET_CODE::INIT_PARENT);
    menu->fill('w', false);
    // erase from y = (0,2) and x = (0,5) i.e 3x6 block
    menu->erase({1, 1, 2, 2, 1, 3, 2, 5});

    bool isBlank1 = menu->getCharFromWin(1, 1) == ' ';
    bool isBlank2 = menu->getCharFromWin(1, 1) == ' ';
    bool isBlank3 = menu->getCharFromWin(1, 2) == ' ';
    bool isBlank4 = menu->getCharFromWin(1, 1) == ' ';
    bool isBlank5 = menu->getCharFromWin(1, 1) == ' ';
    bool isBlank6 = menu->getCharFromWin(1, 2) == ' ';
    bool isBlank7 = menu->getCharFromWin(2, 1) == ' ';
    bool isBlank8 = menu->getCharFromWin(2, 1) == ' ';
    bool isBlank9 = menu->getCharFromWin(2, 2) == ' ';

    bool isBlank10 = menu->getCharFromWin(1, 3) == ' ';
    bool isBlank11 = menu->getCharFromWin(1, 4) == ' ';
    bool isBlank12 = menu->getCharFromWin(1, 5) == ' ';
    bool isBlank13 = menu->getCharFromWin(1, 3) == ' ';
    bool isBlank14 = menu->getCharFromWin(1, 4) == ' ';
    bool isBlank15 = menu->getCharFromWin(1, 5) == ' ';
    bool isBlank16 = menu->getCharFromWin(2, 3) == ' ';
    bool isBlank17 = menu->getCharFromWin(2, 4) == ' ';
    bool isBlank18 = menu->getCharFromWin(2, 5) == ' ';

    menu->setWin(WIN_SET_CODE::KILL_PARENT);
    return isBlank1 && isBlank2 && isBlank3 && isBlank4 && isBlank5 &&
           isBlank6 && isBlank7 && isBlank8 && isBlank9 && isBlank10 &&
           isBlank11 && isBlank12 && isBlank13 && isBlank14 && isBlank15 &&
           isBlank16 && isBlank17 && isBlank18;
  };

  bool exit_success = glambda();
  REQUIRE(exit_success);
}

int main(int argc, char const *argv[]) {
  int result = Catch::Session().run(argc, argv);

  // for opening new terminal window.

  std::cout << "\n press enter to exit." << std::endl;
  while (1) {
    int c = std::getchar();
    if (c == 10 && c != EOF)
      break;
  }
  return result;
}
