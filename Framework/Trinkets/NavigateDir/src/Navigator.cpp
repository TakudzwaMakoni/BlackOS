#include "../inc/Navigator.h"
#include "../inc/NavigationHelpers.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>

namespace BlackOS {
namespace Trinkets {

Navigator::Navigator(std::string const &path, bool const showHiddenFiles)
    : _parentPath(path), _showHiddenFiles(showHiddenFiles) {
  _loadChildren();
}

size_t Navigator::childrenSize() const { return _children.size(); }

std::list<std::string> Navigator::children() const { return _children; }

void Navigator::_loadChildren() {
  for (const auto &entry : std::filesystem::directory_iterator(_parentPath)) {
    std::string entryString = entry.path();
    if (_showHiddenFiles) {
      _children.push_back(entryString);
    } else if (!_showHiddenFiles) {
      if (stripSubstring(entryString, _parentPath + "/")[0] != '.')
        _children.push_back(entryString);
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

  _children.sort(_nocase);
}

} // namespace Trinkets
} // namespace BlackOS
