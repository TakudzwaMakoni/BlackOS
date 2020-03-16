/**
 * PathController
 *
 * Copyright (C) 2020, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "PathController.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {

std::string PathController::stripSubstring(std::string str,
                                           std::string const &substring) {
  size_t pos = std::string::npos;
  // Search for the substring in string in a loop untill nothing is found
  while ((pos = str.find(substring)) != std::string::npos) {
    // If found then erase it from string
    str.erase(pos, substring.length());
  }
  return str;
}

std::string PathController::timestampToDateTime(time_t const rawtime) {
  struct tm *dt;
  char buffer[80];
  dt = localtime(&rawtime);
  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M  %Z", dt);
  return std::string(buffer);
}

std::string PathController::pathLastModifiedDate(std::string const &path) {
  struct stat modResult;
  if (stat(path.c_str(), &modResult) == 0) {
    return timestampToDateTime(modResult.st_mtime);
  } else {
    return "unknown";
  }
}

std::string PathController::pathPermissions(std::string const &path) {
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

std::string PathController::pathType(std::filesystem::path const &path) {
  namespace fs = std::filesystem;
  if (fs::is_directory(path)) {
    return "directory";
  } else if (fs::is_regular_file(path)) {
    return "file";
  } else {
    return "unknown";
  }
}

std::filesystem::path PathController::parentPathObj() const {
  return _parentPath;
}

size_t PathController::childrenSize() const { return _children.size(); }

std::vector<std::filesystem::path> PathController::children() const {
  return _children;
}

void PathController::showHidden(bool const showHiddenFiles) {
  _showHiddenFiles = showHiddenFiles;
}

void PathController::loadParent(std::filesystem::path const &path) {
  _parentPath = path;
  // use list for case insensitive sort
  std::list<std::filesystem::path> childrenList;

  for (const auto &entry : std::filesystem::directory_iterator(_parentPath)) {
    std::string entryString = entry.path().filename(); // name of child.
    if (_showHiddenFiles) {
      childrenList.push_back(entry.path());
    } else if (!_showHiddenFiles) {
      if (entryString[0] != '.')
        childrenList.push_back(entry.path());
    }
  }

  auto _nocase = [](const std::string &first, const std::string &second) {
    unsigned int i = 0;
    while ((i < first.length()) && (i < second.length())) {
      if (tolower(first[i]) < tolower(second[i]))
        return true;
      else if (tolower(first[i]) > tolower(second[i]))
        return false;
      ++i;
    }
    return (first.length() < second.length());
  };

  childrenList.sort(_nocase);
  _children = {std::make_move_iterator(std::begin(childrenList)),
               std::make_move_iterator(std::end(childrenList))};

  // get max length of loaded children.
  for (std::string const &entry : _children) {
    std::string entryName = stripSubstring(entry, _parentPath.string() + "/");
    size_t nameLen = entryName.length();
    if (nameLen > _max) {
      _max = nameLen;
    }
  }
}

std::vector<std::string> PathController::generateFields() {
  // generate fields
  std::vector<std::string> fields;
  for (std::string const &entry : _children) {
    std::string entityPad = std::to_string(_max + 3);
    std::string formatString = "{0:<" + entityPad + "}{1:<12}{2:<12}{3:<21}";
    std::string fieldName = fmt::format(
        formatString, stripSubstring(entry, _parentPath.string() + "/"),
        pathType(entry), pathPermissions(entry), pathLastModifiedDate(entry));
    fields.push_back(fieldName);
  }
  return fields;
}

std::string PathController::generateTitle() const {
  std::string entityPad = std::to_string(_max + 3);
  std::string formatString = "{0:<" + entityPad + "}{1:<12}{2:<12}{3:<21}";

  std::string title =
      fmt::format(formatString, "entity", "type", "access", "modified");
  return title;
}

} // namespace Trinkets
} // namespace BlackOS
