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
    return menu->window() == nullptr;
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
    menu->setTitle("test_menu_title");
    menu->setWin(world);
    menu->fill('w', true);

    bool filled = true;
    for (int i = 2; i <= 8; ++i) {
      for (int j = 1; j <= 8; ++j) {
        auto character = menu->getChrfromW(i, j, false);
        int ch = (int)'w';
        if (!(character == ch))
          filled = false;
      }
    }
    bool titleBar_preserved = true;
    for (int j = 1; j <= 8; ++j) {
      if (menu->getChrfromW(1, j, false) == 'w')
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
    menu->setTitle("test_menu_title");
    menu->setWin(world);
    menu->fill('w', false);

    bool filled = true;
    for (int i = 1; i <= 8; ++i) {
      for (int j = 1; j <= 8; ++j) {
        auto character = menu->getChrfromW(i, j, false);
        int ch = (int)'w';
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
/*
TEST_CASE("test kErase", "[member_functions]") {
  auto glambda = [](WINDOW *world) {
    auto const &menu = TestHelpers::testMenu();
    menu->setTitle("test_menu_title");
    menu->setWin(world);
  };
  initscr();
  cbreak();
  WINDOW *world = newwin(0, 0, 0, 0);
  bool exit_success = glambda(world);
  delwin(world);
  endwin();
  REQUIRE(exit_success);
}
*/
int main(int argc, char const *argv[]) {

  int result = Catch::Session().run(argc, argv);

  return result;
}
