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

Navigator::Navigator(std::string const &path, bool const showHiddenFiles)
    : _parentPath(path), _showHiddenFiles(showHiddenFiles) {
  _loadChildren();
  _loadMax();
}

size_t Navigator::childrenSize() const { return _children.size(); }

std::vector<std::string> Navigator::children() const { return _children; }

// use list for case insensitive sort
std::list<std::string> childrenList;
void Navigator::_loadChildren() {
  for (const auto &entry : std::filesystem::directory_iterator(_parentPath)) {
    std::string entryString = entry.path();
    if (_showHiddenFiles) {
      childrenList.push_back(entryString);
    } else if (!_showHiddenFiles) {
      if (stripSubstring(entryString, _parentPath + "/")[0] != '.')
        childrenList.push_back(entryString);
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
}

std::vector<std::string> Navigator::generateFields() {
  // generate fields
  std::vector<std::string> fields;
  for (std::string const &entry : _children) {
    std::string entityPad = std::to_string(_max + 3);
    std::string formatString = "{0:<" + entityPad + "}{1:<12}{2:<12}{3:<21}";
    std::string fieldName = fmt::format(
        formatString, stripSubstring(entry, _parentPath + "/"), pathType(entry),
        pathPermissions(entry), pathLastModifiedDate(entry));
    fields.push_back(fieldName);
  }
  return fields;
}

void Navigator::_loadMax() {
  for (std::string const &entry : _children) {
    std::string name = stripSubstring(entry, _parentPath + "/");
    if (name.size() > _max) {
      _max = name.size();
    }
  }
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
