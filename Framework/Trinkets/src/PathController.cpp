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

#include "../inc/PathController.h"
#include "../inc/NavigationHelpers.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fmt/format.h>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

namespace BlackOS {
namespace Trinkets {

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
