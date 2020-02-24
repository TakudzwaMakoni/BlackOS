#ifndef NAVIGATION_HELPERS_H
#define NAVIGATION_HELPERS_H

#include "Directives.h"
#include "Kfield.h"
#include "Kmenu.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <time.h>

namespace BlackOS {
namespace Trinkets {

std::string stripSubstring(std::string str, std::string const &substring);
std::string timestampToDateTime(time_t const rawtime);
std::string pathPermissions(std::string const &path);
std::string pathLastModifiedDate(std::string const &path);
std::vector<BlackOS::DisplayKernel::Kfield>
loadFields(std::list<std::string> const &children,
           std::string const &parentPath);
std::string pathType(std::filesystem::path const &path);

// copyright 2008 Sarah Vessels
void run_cmd(int argc, char **argv);
} // namespace Trinkets
} // namespace BlackOS

#endif
