#ifndef NAVIGATION_HELPERS_H
#define NAVIGATION_HELPERS_H

#include <filesystem>
#include <list>
#include <string>
#include <time.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {

std::string stripSubstring(std::string str, std::string const &substring);
std::string timestampToDateTime(time_t const rawtime);
std::string pathPermissions(std::string const &path);
std::string pathLastModifiedDate(std::string const &path);
std::vector<std::string> loadFields(std::list<std::string> const &children,
                                    std::string const &parentPath);
std::string pathType(std::filesystem::path const &path);
} // namespace Trinkets
} // namespace BlackOS

#endif
