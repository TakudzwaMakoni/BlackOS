#include "../header/BlackOSScripts.h"
#include "../header/Kfield.h"
#include "../header/Kwindow.h"

#include <string>
#include <vector>
#include <ncurses.h>

#define WORLD_WIDTH (COLS - 10)
#define WORLD_HEIGHT (LINES - 10)
#define Y_CENTRE (LINES - WORLD_HEIGHT) / 2
#define X_CENTRE (COLS - WORLD_WIDTH) / 2

void script(){};

int main(int argc, const char * argv[]) {

    initscr();
    cbreak();

    // create main menu

    std::vector<int> framex(50,50);

    directive testScript = script;


    std::string fname1 = "* HOME *";
    std::string fname2 = "* SETTINGS *";
    std::string fname3 = "* PROFILES *";
    std::string fname4 = "* QUIT *";

    Kfield field1(fname1, writeToFile);
    Kfield field2(fname2, writeToFile);
    Kfield field3(fname3, testScript);
    Kfield field4(fname4, testScript);

    fieldVector mainFields = {field1,field2,field3,field4};
    std::string mainMenuName = "BlackOS version 1.0";

    Kmenu BOS_MAIN_MENU(mainMenuName , mainFields, framex);

    // create main window
    std::vector<int> winsize = {WORLD_HEIGHT, WORLD_WIDTH, Y_CENTRE, X_CENTRE};

    Kwindow window(winsize, &BOS_MAIN_MENU, 'c');

    endwin();

    return 0;
}