#include "../inc/Kfield.h"
#include "../inc/Kmenu.h"
#include "../inc/Kgrid.h"
#include "../inc/Kwindow.h"

#include "../inc/Directives.h"

#include <string>
#include <vector>
#include <ncurses.h>
#include <memory>
#include <ncurses.h>

#define WORLD_WIDTH (COLS - 1)
#define WORLD_HEIGHT (LINES - 1)
#define Y_CENTRE (LINES - WORLD_HEIGHT) / 2
#define X_CENTRE (COLS - WORLD_WIDTH) / 2


int main(int argc, const char * argv[]) {
    initscr();
    cbreak();
    cursor(0);
    while(true){
    
    
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
    
    //  std::vector<BlackOSDisplay::Kfield> mainFields = {field1,field2,field3,field4};
    //  std::string mainMenuName = "BlackOS version 1.0";
    
   //  BlackOSDisplay::Kmenu main_menu(1, mainMenuName, WORLD_HEIGHT, WORLD_WIDTH, Y_CENTRE, X_CENTRE);
   //  create main window
        
        
        std::string name = "testdata";
        Eigen::Matrix<double, 3, 3> testData;
        testData << 1.089,2.436,3.5268,4.721346,5.8654,6.345,7.1743,8.13673,9.4564;
        BlackOSDisplay::Kgrid<double, 3, 3> grid(name, testData, WORLD_HEIGHT, WORLD_WIDTH, Y_CENTRE, X_CENTRE);
        grid.setPrecision(5);
        grid.setBorderStyle();
        grid.setGrid(false);
        grid.setGridAlign(0, 0);
        grid.display();
        auto selectedElement = grid.getSelectedElement();
        WINDOW * grid_window = grid.window();
        wclear(grid_window);
        mvwprintw(grid_window, grid.centreY(), grid.centreX(), std::to_string(selectedElement).c_str());
        wgetch(grid_window);
    /*
    main_menu.setFieldAlign(0, 0);
        main_menu.setBorderStyle(0, 0, 0, 0, '*', '*', '*', '*');
    main_menu.setLabel("BlackOS NUmber 2");
    main_menu.setFields(mainFields);
    main_menu.display();
    auto selectedField = main_menu.getSelectedField();
    if (selectedField.name() == fname4)
        break;
    WINDOW * main_win = main_menu.window();
    std::string msg = selectedField.message();
        int msgLen = msg.length();
        wclear(main_win);
        mvwprintw(main_win, main_menu.centreY(), main_menu.centreX() - msgLen, selectedField.message().c_str());
        wgetch(main_win);
     */
    } // while true
    endwin();
    return 0;
}

