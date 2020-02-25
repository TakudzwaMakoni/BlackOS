#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "Directives.h"

#include <filesystem>
#include <list>
#include <string>
#include <vector>

namespace BlackOS {
namespace Trinkets {
struct Navigator {
public:
  Navigator(std::filesystem::path const &path, bool const showHiddenFiles = 0);

  size_t childrenSize() const;
  std::vector<std::filesystem::path> children() const;
  std::vector<std::string> generateFields();
  std::string generateTitle() const;
  std::filesystem::path parentPathObj() const;

private:
  std::vector<std::filesystem::path> _children;
  std::filesystem::path _parentPath;
  bool _showHiddenFiles;
  size_t _max = 0;

  void _loadChildren();
  void _loadMax();
};
} // namespace Trinkets
} // namespace BlackOS
#endif
