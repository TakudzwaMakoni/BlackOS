//
// Created by Takudzwa Makoni on 2019-07-13.
//

// test
#include "../header/BlackOSScripts.h"

namespace BlackOSScripts{

    void writeToFile(){
        std::string buf, fullPath;

        fullPath = "testexample.txt";
        buf = "this is a test!";
        std::ofstream file;

        file.open(fullPath);
        file << buf;
        file.close();
    }

}

