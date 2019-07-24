
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



int main(int argc, const char * argv[]) {

    initscr();
    cbreak();

    // create main menu

    std::vector<int> framex(50,50);



    std::string fname1 = "* HOME *";
    std::string fmessage1 = "you selected 'home'.";


    std::string fname2 = "* SETTINGS *";
    std::string fmessage2 = "you selected 'settings'.";

    std::string fname3 = "* PROFILES *";
    std::string fmessage3 = "you selected 'profiles'.";

    std::string fname4 = "* QUIT *";
    std::string fmessage4 = "you selected 'quit'.";

    BlackOSObjects::Kfield field1(fname1, fmessage1, BlackOSScripts::writeToFile);
    BlackOSObjects::Kfield field2(fname2, fmessage2, BlackOSScripts::writeToFile);
    BlackOSObjects::Kfield field3(fname3, fmessage3, BlackOSScripts::writeToFile);
    BlackOSObjects::Kfield field4(fname4, fmessage4, BlackOSScripts::writeToFile);

    std::vector<BlackOSObjects::Kfield> mainFields = {field1,field2,field3,field4};
    std::string mainMenuName = "BlackOS version 1.0";

    BlackOSObjects::Kmenu BOS_MAIN_MENU(mainMenuName , mainFields, framex);
    std::unique_ptr<BlackOSObjects::Kmenu> holder = std::make_unique<BlackOSObjects::Kmenu>(BOS_MAIN_MENU);
    // create main window
    std::vector<int> winsize = {WORLD_HEIGHT, WORLD_WIDTH, Y_CENTRE, X_CENTRE};

    int xOffset = 0;
    int yOffset = 0;

    std::string name = "testdata";
    Eigen::Matrix3d testData;
    testData << 1,2,3,4,5,6,7,8,9;

   BlackOSObjects::Kgrid<double,3,3> grid(name,testData);

    Eigen::Matrix3d matr2 = grid.matrix();
   // k_grid.read(&matr2);

    BlackOSObjects::Kwindow k_window(winsize, holder, xOffset,yOffset );
    k_window.display();

    endwin();
    return 0;
}
