#include "../inc/Kfield.h"
#include "../inc/Kgrid.h"
#include "../inc/Kmenu.h"
#include "../inc/Kwindow.h"

#include "../inc/Directives.h"

#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

#define WORLD_WIDTH (COLS - 2)
#define WORLD_HEIGHT (LINES - 2)
#define Y_CENTRE (LINES - WORLD_HEIGHT) / 2
#define X_CENTRE (COLS - WORLD_WIDTH) / 2

int main(int argc, const char *argv[]) {

  initscr();
  cbreak();
  cursor(0);
  while (true) {

    // create main menu

    std::string fname1 = "* HOME *";
    std::string fmessage1 = "you selected 'home'.";

    std::string fname2 = "* SETTINGS *";
    std::string fmessage2 = "you selected 'settings'.";

    std::string fname3 = "* PROFILES *";
    std::string fmessage3 = "you selected 'profiles'.";

    std::string fname4 = "* QUIT *";
    std::string fmessage4 = "you selected 'quit'.";

    BlackOSDisplay::Kfield field1(fname1, fmessage1, Directives::writeToFile);
    BlackOSDisplay::Kfield field2(fname2, fmessage2, Directives::writeToFile);
    BlackOSDisplay::Kfield field3(fname3, fmessage3, Directives::writeToFile);
    BlackOSDisplay::Kfield field4(fname4, fmessage4, Directives::writeToFile);

    std::vector<BlackOSDisplay::Kfield> mainFields = {field1, field2, field3,
                                                      field4};
    std::string mainMenuName = "BlackOS version 1.0 ";

    BlackOSDisplay::Kmenu main_menu(1, mainMenuName, WORLD_HEIGHT, WORLD_WIDTH,
                                    Y_CENTRE, X_CENTRE);
    //  create main window

    std::string name = "Rotation Matrix A";
    Eigen::Matrix<double, 3, 3> testData;
    testData << 11.089, 2.436, 3.5268, 4.721346, 5.8654, 6.345, 7.1743, 8.13673,
        9100.4564;
    std::vector<double> testData2{34.2,   21.25, 37.2,    54.2, 5.2,
                                  6546.2, 7.2,   75358.2, 9.2};
    BlackOSDisplay::Kgrid<double, 3, 3> grid(name, testData2, WORLD_HEIGHT,
                                             WORLD_WIDTH, Y_CENTRE, X_CENTRE);
    grid.setPrecision(2);
    grid.borderStyle();
    grid.gridLines(false);
    grid.label(grid.name());
    grid.setTitle("Matrix Editor Program");
    grid.showTitle(true);
    grid.align(0, 0);
    auto mat = grid.matrix();
    printw(std::to_string(mat.coeff(0, 0)).c_str());

    grid.display();
    auto selectedElement = grid.selectedRaw();
    WINDOW *grid_window = grid.window();
    wclear(grid_window);
    mvwprintw(grid_window, grid.centreY(), grid.centreX(),
              std::to_string(selectedElement).c_str());
    wgetch(grid_window);

    main_menu.setFieldAlign(0, 0);
    main_menu.setBorderStyle(0, 0, 0, 0, '*', '*', '*', '*');
    main_menu.label("BlackOS No. 2");
    main_menu.setFields(mainFields);
    main_menu.display();
    auto selectedField = main_menu.getSelectedField();
    if (selectedField.name() == fname4)
      break;
    WINDOW *main_win = main_menu.window();
    std::string msg = selectedField.message();
    int msgLen = msg.length();
    wclear(main_win);
    mvwprintw(main_win, main_menu.centreY(), main_menu.centreX() - msgLen,
              selectedField.message().c_str());
    wgetch(main_win);

  } // while true
  endwin();
  return 0;
}
