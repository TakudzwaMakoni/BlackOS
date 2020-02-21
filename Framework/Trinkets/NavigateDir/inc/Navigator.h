#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <list>
#include <string>

namespace BlackOS {
namespace Trinkets {
class Navigator {
public:
  Navigator(std::string const &path, bool const showHiddenFiles = 0);

  size_t childrenSize() const;
  std::string pathPermissions(std::string const &path) const;
  std::string pathLastModifiedDate(std::string const &path) const;

private:
  std::list<std::string> _children;
  std::string _parentPath;
  bool _showHiddenFiles;

  void _loadChildren();
  void _stripSubstring(std::string &str, std::string const &substring);
};
} // namespace Trinkets
} // namespace BlackOS
#endif
