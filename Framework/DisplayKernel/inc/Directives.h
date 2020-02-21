//
// Created by Takudzwa Makoni on 2019-07-13.
//

#ifndef BLACKOS_DIRECTIVES_H
#define BLACKOS_DIRECTIVES_H

#include <fstream>
#include <ncurses.h>
#include <string>

namespace BlackOS {
namespace DisplayKernel {
typedef void (*directive)();
namespace Directives {
void doNothing();
void writeToFile();
void animate();
void exitProgram();
} // namespace Directives
} // namespace DisplayKernel
} // namespace BlackOS

#endif // BLACKOS_DIRECTIVES_H
