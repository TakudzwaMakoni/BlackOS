#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "Directives.h"
#include "Kfield.h"

#include <list>
#include <string>
#include <vector>

namespace BlackOS {
namespace Trinkets {
struct Navigator {
public:
  Navigator(std::string const &path, bool const showHiddenFiles = 0);

  size_t childrenSize() const;
  std::vector<std::string> children() const;
  std::vector<DisplayKernel::Kfield> generateFields();
  std::string generateTitle() const;

private:
  std::vector<std::string> _children;
  std::string _parentPath;
  bool _showHiddenFiles;
  size_t _max = 0;

  void _loadChildren();
  void _loadMax();
};
} // namespace Trinkets
} // namespace BlackOS
#endif
