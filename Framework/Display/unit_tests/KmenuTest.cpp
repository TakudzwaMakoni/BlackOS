// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#define WORLD_WIDTH (COLS - 2)
#define WORLD_HEIGHT (LINES - 2)
#define Y_CENTRE (LINES - WORLD_HEIGHT) / 2
#define X_CENTRE (COLS - WORLD_WIDTH) / 2

#include "../inc/Kmenu.h"
#include "../inc/Directives.h"
#include "../inc/Kfield.h"
#include "../testHelpers/inc/MenuGenerator.h"
#include "Eigen/Dense"
#include "ncurses.h"
#include <catch2/catch.hpp>

using namespace BlackOS::Display;

TEST_CASE("test window is resized to initialised menu size after call setWin",
          "[window]") {

  initscr(); // initialise curses data
  cbreak();  // allow screen to echo
  WINDOW *world = newwin(0, 0, 0, 0);

  auto const &menu =
      TestHelpers::testMenuInitialisedWithSizeAndPos(24, 12, 20, 19);
  menu->setWin(world);

  int winSzX, winSzY;
  getmaxyx(world, winSzY, winSzX);
  std::vector<int> const winSz{winSzY, winSzX};

  delwin(world);
  endwin(); // exit curses mode

  REQUIRE(winSzY == 24);
  REQUIRE(winSzX == 12);
}

TEST_CASE("test window is repositioned to initialised menu position after call "
          "setWin",
          "[window]") {
  initscr(); // initialise curses data
  cbreak();  // allow screen to echo
  WINDOW *world = newwin(0, 0, 0, 0);

  auto const &menu =
      TestHelpers::testMenuInitialisedWithSizeAndPos(24, 12, 20, 19);
  menu->setWin(world);

  int winPosX, winPosY;
  getbegyx(world, winPosY, winPosX);

  delwin(world);
  endwin(); // exit curses mode

  REQUIRE(winPosY == 20);
  REQUIRE(winPosX == 19);
}
