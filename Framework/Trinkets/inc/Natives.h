// Natives (c), Takudzwa 2020

#ifndef NATIVES_H
#define NATIVES_H

#include "../inc/PathController.h" // NavigateDir, ListChildren
#include "DisplayHelpers.h"        //(libDisplayKernel) NavigateDir
#include "Kmenu.h"                 //(libDisplayKernel) NavigateDir
#include "NavigationHelpers.h"     //(libDisplayKernel) NavigateDir

#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

namespace BlackOS{
namespace Trinkets{
int NavigateDir(int , char **);
int ListChildren(int, char **);
}
}

#endif
