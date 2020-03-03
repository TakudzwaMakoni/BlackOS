#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {
void eol(int v = 1);
void pf(std::string const &str, int winwidth);
int splashScreen(std::vector<std::string> const &argv);
} // namespace Trinkets
} // namespace BlackOS

#endif
