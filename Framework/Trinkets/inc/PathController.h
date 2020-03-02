#ifndef PATH_CONTROLLER_H
#define PATH_CONTROLLER_H

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

#include <filesystem>
#include <string>
#include <vector>

namespace BlackOS {
namespace Trinkets {
struct PathController {
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
