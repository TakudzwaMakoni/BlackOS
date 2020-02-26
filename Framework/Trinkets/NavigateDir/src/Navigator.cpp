#include "../inc/Navigator.h"
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

std::filesystem::path Navigator::parentPathObj() const { return _parentPath; }

size_t Navigator::childrenSize() const { return _children.size(); }

std::vector<std::filesystem::path> Navigator::children() const {
  return _children;
}

void Navigator::showHidden(bool const showHiddenFiles) {
  _showHiddenFiles = showHiddenFiles;
}

void Navigator::loadParent(std::filesystem::path const &path) {
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

std::vector<std::string> Navigator::generateFields() {
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

std::string Navigator::generateTitle() const {
  std::string entityPad = std::to_string(_max + 3);
  std::string formatString = "{0:<" + entityPad + "}{1:<12}{2:<12}{3:<21}";

  std::string title =
      fmt::format(formatString, "entity", "type", "access", "modified");
  return title;
}

} // namespace Trinkets
} // namespace BlackOS
