#include "../inc/Navigator.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>
#include <sys/stat.h>
#include <time.h>

namespace BlackOS {
namespace Trinkets {

namespace {

bool _nocase(const std::string &first, const std::string &second) {
  unsigned int i = 0;
  while ((i < first.length()) && (i < second.length())) {
    if (tolower(first[i]) < tolower(second[i]))
      return true;
    else if (tolower(first[i]) > tolower(second[i]))
      return false;
    ++i;
  }
  return (first.length() < second.length());
}

std::string timestampToDateTime(time_t const rawtime) {
  struct tm *dt;
  char buffer[80];
  dt = localtime(&rawtime);
  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M %Z", dt);
  return std::string(buffer);
}

} // namespace

Navigator::Navigator(std::string const &path, bool const showHiddenFiles)
    : _parentPath(path), _showHiddenFiles(showHiddenFiles) {
  _loadChildren();
}

std::string Navigator::pathPermissions(std::string const &path) const {
  namespace fs = std::filesystem;
  fs::perms p = fs::status(path).permissions();

  std::string uR = (p & fs::perms::owner_read) != fs::perms::none ? "r" : "-";
  std::string uW = (p & fs::perms::owner_write) != fs::perms::none ? "w" : "-";
  std::string uX = (p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-";

  std::string gR = (p & fs::perms::group_read) != fs::perms::none ? "r" : "-";
  std::string gW = (p & fs::perms::group_write) != fs::perms::none ? "w" : "-";
  std::string gX = (p & fs::perms::group_exec) != fs::perms::none ? "x" : "-";

  std::string oR = (p & fs::perms::others_read) != fs::perms::none ? "r" : "-";
  std::string oW = (p & fs::perms::others_write) != fs::perms::none ? "w" : "-";
  std::string oX = (p & fs::perms::others_exec) != fs::perms::none ? "x" : "-";

  return uR + uW + uX + gR + gW + gX + oR + oW + oX;
}

size_t Navigator::childrenSize() const { return _children.size(); }

void Navigator::_stripSubstring(std::string &str,
                                std::string const &substring) {
  size_t pos = std::string::npos;
  // Search for the substring in string in a loop untill nothing is found
  while ((pos = str.find(substring)) != std::string::npos) {
    // If found then erase it from string
    str.erase(pos, substring.length());
  }
}
std::string Navigator::pathLastModifiedDate(std::string const &path) const {
  struct stat result;
  if (stat(path.c_str(), &result) == 0) {

    return timestampToDateTime(result.st_mtime);
  } else {
    return "unknown";
  }
}

void Navigator::_loadChildren() {
  for (const auto &entry : std::filesystem::directory_iterator(_parentPath)) {
    std::string entryString = entry.path();
    _stripSubstring(entryString, _parentPath + "/");
    if (_showHiddenFiles) {
      _children.push_back(entryString);
    } else if (!_showHiddenFiles) {
      if (entryString[0] != '.')
        _children.push_back(entryString);
    }
  }
  _children.sort(_nocase);
}

} // namespace Trinkets
} // namespace BlackOS
