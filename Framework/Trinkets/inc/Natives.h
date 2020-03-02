<<<<<<< HEAD
/**
 * Copyright 2020 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
 *
 * This Program is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This Program is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This Program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */
=======
// Natives (c), Takudzwa 2020
>>>>>>> 6f9c07145595a5dd68afc1194c198d1d573b2e50

#ifndef NATIVES_H
#define NATIVES_H

#include "../inc/PathController.h" // NavigateDir, ListChildren
#include "DisplayHelpers.h"        //(libDisplayKernel) NavigateDir
#include "Kmenu.h"                 //(libDisplayKernel) NavigateDir
#include "NavigationHelpers.h"     //(libDisplayKernel) NavigateDir

<<<<<<< HEAD
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
=======
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstring>
>>>>>>> 6f9c07145595a5dd68afc1194c198d1d573b2e50
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

<<<<<<< HEAD
namespace BlackOS {
namespace Trinkets {
int NavigateDir(int, char **);
int ListChildren(int, char **);
} // namespace Trinkets
} // namespace BlackOS
=======
namespace BlackOS{
namespace Trinkets{
int NavigateDir(int , char **);
int ListChildren(int, char **);
}
}
>>>>>>> 6f9c07145595a5dd68afc1194c198d1d573b2e50

#endif
