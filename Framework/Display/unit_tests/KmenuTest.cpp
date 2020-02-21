#define CATCH_CONFIG_RUNNER

#include "../inc/Kmenu.h"
#include "../inc/Directives.h"
#include "../inc/Kfield.h"
#include "../testHelpers/inc/MenuGenerator.h"
#include "Eigen/Dense"
#include "ncurses.h"
#include <catch2/catch.hpp>
#include <chrono>
#include <cstdio>
#include <thread>

using namespace BlackOS::Display;

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
  auto glambda = [](WINDOW *world) {
    int winSzX, winSzY;

    auto const &menu = TestHelpers::testMenu();
    menu->setWin(world);
    getmaxyx(world, winSzY, winSzX);

    return winSzY == WORLD_HEIGHT && winSzX == WORLD_WIDTH;
  };

  initscr(); // initialise curses data
  cbreak();  // allow screen to echo
  WINDOW *world = newwin(0, 0, 0, 0);

  bool resize_success = glambda(world);

  delwin(world);
  endwin(); // exit curses mode

  REQUIRE(resize_success);
}

TEST_CASE("test window is repositioned to initialised menu position after call "
          "setWin",
          "[window]") {
  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = [](WINDOW *world) {
    int winPosX, winPosY;

    auto const &menu = TestHelpers::testMenu();

    menu->setWin(world);
    getbegyx(world, winPosY, winPosX);

    return winPosY == Y_CENTRE && winPosX == X_CENTRE;
  };

  initscr(); // initialise curses data
  cbreak();  // allow screen to echo
  WINDOW *world = newwin(0, 0, 0, 0);

  bool reposition_success = glambda(world);

  delwin(world);
  endwin(); // exit curses mode

  REQUIRE(reposition_success);
}

TEST_CASE("test window is unset on call setWin with empty parameters",
          "[window]") {
  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = [](WINDOW *world) {
    auto const &menu = TestHelpers::testMenu();
    menu->setWin(world);
    menu->setWin();
    return !menu->windowSet();
  };

  initscr(); // initialise curses data
  cbreak();  // allow screen to echo
  WINDOW *world = newwin(0, 0, 0, 0);

  bool window_is_null = glambda(world);

  delwin(world);
  endwin(); // exit curses mode

  REQUIRE(window_is_null);
}

TEST_CASE("test menu is filled excluding title bar", "[window]") {
  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = [](WINDOW *world) {
    auto const &menu = TestHelpers::testMenu();
    menu->addTitle("test_menu_title");
    menu->setWin(world);
    menu->fill('w', true);

    bool filled = true;
    for (int i = 2; i < WORLD_HEIGHT - 1 /*window coord correction*/; ++i) {
      for (int j = 1; j < WORLD_WIDTH - 1 /*window coord correction*/; ++j) {
        auto character = menu->getCharFromWin(i, j, false);
        int ch = 'w';
        if (!(character == ch))
          filled = false;
      }
    }
    bool titleBar_preserved = true;
    for (int j = 1; j < WORLD_WIDTH - 1 /*window coord correction*/; ++j) {
      if (menu->getCharFromWin(1, j, false) == 'w')
        titleBar_preserved = false;
    }
    return filled && titleBar_preserved;
  };

  initscr(); // initialise curses data
  cbreak();  // prevent input buffer
  WINDOW *world = newwin(0, 0, 0, 0);

  bool fill_successful = glambda(world);

  delwin(world);
  endwin(); // exit curses mode

  REQUIRE(fill_successful);
}

TEST_CASE("test menu is filled including title bar", "[window]") {
  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = [](WINDOW *world) {
    auto const &menu = TestHelpers::testMenu();
    menu->addTitle("test_menu_title");
    menu->setWin(world);
    menu->fill('w', false);

    bool filled = true;
    for (int i = 1; i < WORLD_HEIGHT - 1 /*window coord correction*/; ++i) {
      for (int j = 1; j < WORLD_WIDTH - 1 /*window coord correction*/; ++j) {
        auto character = menu->getCharFromWin(i, j, false);
        int ch = 'w';
        if (!(character == ch))
          filled = false;
      }
    }
    return filled;
  };

  initscr(); // initialise curses data
  cbreak();  // prevent input buffer
  WINDOW *world = newwin(0, 0, 0, 0);

  bool fill_successful = glambda(world);

  delwin(world);
  endwin(); // exit curses mode

  REQUIRE(fill_successful);
}

TEST_CASE("test erase call erases horizontal block", "[member_functions]") {

  // horizontal block
  auto glambda = [](WINDOW *world) {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 0, 0);
    menu->addTitle("test_menu_title");
    menu->setWin(world);
    menu->fill('w', false);
    // erase from y = 0  and x = (3,5)
    menu->erase(1, 3, 1, 5);

    bool isBlank1 = menu->getCharFromWin(1, 3) == ' ';
    bool isBlank2 = menu->getCharFromWin(1, 4) == ' ';
    bool isBlank3 = menu->getCharFromWin(1, 5) == ' ';

    return isBlank1 && isBlank2 && isBlank3;
  };

  initscr();
  cbreak();
  WINDOW *world = newwin(0, 0, 0, 0);
  bool exit_success = glambda(world);
  delwin(world);
  endwin();
  REQUIRE(exit_success);
}

TEST_CASE("test eraseExcept call excepts horizontal block",
          "[member_functions]") {

  // horizontal block
  auto glambda = [](WINDOW *world) {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 0, 0);
    menu->addTitle("test_menu_title");
    menu->setWin(world);
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

    return notBlank1 && notBlank2 && notBlank3 && isBlank1 && isBlank2 &&
           isBlank3;
  };

  initscr();
  cbreak();
  WINDOW *world = newwin(0, 0, 0, 0);
  bool exit_success = glambda(world);
  delwin(world);
  endwin();
  REQUIRE(exit_success);
}

TEST_CASE("test erase call erases vertical block", "[member_functions]") {

  // vertical block
  auto glambda = [](WINDOW *world) {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 0, 0);
    menu->addTitle("test_menu_title");
    menu->setWin(world);
    menu->fill('w', false);

    // erase from y = (3,5) and x = 0
    menu->erase(3, 1, 5, 1);

    bool isBlank1 = menu->getCharFromWin(3, 1) == ' ';
    bool isBlank2 = menu->getCharFromWin(4, 1) == ' ';
    bool isBlank3 = menu->getCharFromWin(5, 1) == ' ';

    return isBlank1 && isBlank2 && isBlank3;
  };

  initscr();
  cbreak();
  WINDOW *world = newwin(0, 0, 0, 0);
  bool exit_success = glambda(world);
  delwin(world);
  endwin();
  REQUIRE(exit_success);
}

TEST_CASE("test erase call erases square block", "[member_functions]") {

  // square block
  auto glambda = [](WINDOW *world) {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 1, 1);
    menu->addTitle("test_menu_title");
    menu->setWin(world);
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

    return isBlank1 && isBlank2 && isBlank3 && isBlank4 && isBlank5 &&
           isBlank6 && isBlank7 && isBlank8 && isBlank9;
  };

  initscr();
  cbreak();
  WINDOW *world = newwin(0, 0, 0, 0);
  bool exit_success = glambda(world);
  delwin(world);
  endwin();
  REQUIRE(exit_success);
}

TEST_CASE("test erase call erases multiple square blocks",
          "[member_functions]") {

  // square block
  auto glambda = [](WINDOW *world) {
    auto const &menu =
        TestHelpers::testMenuInitialisedWithSizeAndPos(40, 40, 0, 0);
    menu->addTitle("test_menu_title");
    menu->setWin(world);
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

    return isBlank1 && isBlank2 && isBlank3 && isBlank4 && isBlank5 &&
           isBlank6 && isBlank7 && isBlank8 && isBlank9 && isBlank10 &&
           isBlank11 && isBlank12 && isBlank13 && isBlank14 && isBlank15 &&
           isBlank16 && isBlank17 && isBlank18;
  };

  initscr();
  cbreak();
  WINDOW *world = newwin(0, 0, 0, 0);
  bool exit_success = glambda(world);
  delwin(world);
  endwin();
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
