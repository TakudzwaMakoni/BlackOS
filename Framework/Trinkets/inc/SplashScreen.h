#ifndef TRINKETS_SPLASH_SCREEN_H
#define TRINKETS_SPLASH_SCREEN_H

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {

void pf(std::string const &str, int winwidth);
std::vector<std::string> splashScreen(std::vector<std::string> const &argv);
} // namespace Trinkets
} // namespace BlackOS

#endif
