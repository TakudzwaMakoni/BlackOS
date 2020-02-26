#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <filesystem>
#include <string>
#include <vector>

namespace BlackOS {
namespace Trinkets {
struct Navigator {
public:
  size_t childrenSize() const;
  std::vector<std::filesystem::path> children() const;
  std::vector<std::string> generateFields();
  std::string generateTitle() const;
  std::filesystem::path parentPathObj() const;
  void loadChildren();
  void loadParent(std::filesystem::path const &path);
  void showHidden(bool const showHiddenFiles = 0);

private:
  std::vector<std::filesystem::path> _children;
  std::filesystem::path _parentPath;
  bool _showHiddenFiles;
  size_t _max = 0;
};
} // namespace Trinkets
} // namespace BlackOS
#endif
