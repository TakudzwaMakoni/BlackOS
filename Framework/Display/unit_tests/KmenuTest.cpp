#define CATCH_CONFIG_RUNNER

#include "../inc/Kmenu.h"
#include "../inc/Directives.h"
#include "../inc/Kfield.h"
#include "../testHelpers/inc/MenuGenerator.h"
#include "Eigen/Dense"
#include "ncurses.h"
#include <catch2/catch.hpp>

#define WORLD_WIDTH (COLS - 2)
#define WORLD_HEIGHT (LINES - 2)
#define Y_CENTRE (LINES - WORLD_HEIGHT) / 2
#define X_CENTRE (COLS - WORLD_WIDTH) / 2

using namespace BlackOS::Display;

TEST_CASE("test window is resized to initialised menu size after call setWin",
          "[window]") {

  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = [](int menuSzY, int menuSzX) {
    int winSzX, winSzY;

    WINDOW *world = newwin(0, 0, 0, 0);
    auto const &menu = TestHelpers::testMenuInitialisedWithSizeAndPos(
        menuSzY, menuSzX, 20, 19);
    menu->setWin(world);
    getmaxyx(world, winSzY, winSzX);

    return winSzX == menuSzX && winSzY == menuSzY;
  };

  initscr(); // initialise curses data
  cbreak();  // allow screen to echo

  bool resize_success = glambda(24, 12);

  endwin(); // exit curses mode

  REQUIRE(resize_success);
}

TEST_CASE("test window is repositioned to initialised menu position after call "
          "setWin",
          "[window]") {

  // generic lambda forces destructor of class Object to be called while still
  // in curses mode.
  auto glambda = [](int menuPosY, int menuPosX) {
    int winPosX, winPosY;

    WINDOW *world = newwin(0, 0, 0, 0);
    auto const &menu = TestHelpers::testMenuInitialisedWithSizeAndPos(
        24, 12, menuPosY, menuPosX);

    menu->setWin(world);
    getbegyx(world, winPosY, winPosX);

    return winPosX == menuPosX && winPosY == menuPosY;
  };

  initscr(); // initialise curses data
  cbreak();  // allow screen to echo

  bool reposition_success = glambda(20, 19);

  endwin(); // exit curses mode

  REQUIRE(reposition_success);
}

int main(int argc, char const *argv[]) {

  int result = Catch::Session().run(argc, argv);

  return result;
}
