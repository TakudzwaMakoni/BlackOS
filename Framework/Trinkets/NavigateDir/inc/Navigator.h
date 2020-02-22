#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <list>
#include <string>

namespace BlackOS {
namespace Trinkets {
struct Navigator {
public:
  Navigator(std::string const &path, bool const showHiddenFiles = 0);

  size_t childrenSize() const;
  std::list<std::string> children() const;

private:
  std::list<std::string> _children;
  std::string _parentPath;
  bool _showHiddenFiles;

  void _loadChildren();
};
} // namespace Trinkets
} // namespace BlackOS
#endif
