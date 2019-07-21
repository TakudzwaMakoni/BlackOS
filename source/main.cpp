
#include "../header/BlackOSObjects.h"
#include "../header/BlackOSScripts.h"


#include <string>
#include <vector>
#include <ncurses.h>
#include <memory>

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

    BlackOSObjects::directive testScript = script;

    std::string fname1 = "* HOME *";
    std::string fname2 = "* SETTINGS *";
    std::string fname3 = "* PROFILES *";
    std::string fname4 = "* QUIT *";

    BlackOSObjects::Kfield field1(fname1, BlackOSScripts::writeToFile);
    BlackOSObjects::Kfield field2(fname2, BlackOSScripts::writeToFile);
    BlackOSObjects::Kfield field3(fname3, testScript);
    BlackOSObjects::Kfield field4(fname4, testScript);

    std::vector<BlackOSObjects::Kfield> mainFields = {field1,field2,field3,field4};
    std::string mainMenuName = "BlackOS version 1.0";

    BlackOSObjects::Kmenu BOS_MAIN_MENU(mainMenuName , mainFields, framex);
    std::unique_ptr<BlackOSObjects::Kmenu> holder = std::make_unique<BlackOSObjects::Kmenu>(BOS_MAIN_MENU);
    // create main window
    std::vector<int> winsize = {WORLD_HEIGHT, WORLD_WIDTH, Y_CENTRE, X_CENTRE};

    int xOffset = 0;
    int yOffset = 0;
    auto windowPair = std::make_pair(xOffset, yOffset);
    BlackOSObjects::Kwindow window(winsize, holder, windowPair );

    endwin();
    return 0;
}
