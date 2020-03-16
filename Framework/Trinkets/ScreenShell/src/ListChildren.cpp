/**
 * Tr(inkets) ScreenShell NavigateDir
 *
 * Copyright (C) 2020 by Takudzwa Makoni
 * <https://github.com/TakudzwaMakoni>
 *
 * This Program is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This Program is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This Program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#include "../../helpers/PathController.h"
#include "../ScreenShell.h"

namespace BlackOS {
namespace Trinkets {

namespace {
void usageListChildren() {
  std::cout << "Usage:\n"
            << "ls [options] <path>\n"
            << "options:\n'-a' : show hidden\n";
}
} // namespace

int ScreenShell::listChildren() {

  bool withHidden;
  std::string initPath;

  if (_ARGC == 1) {
    withHidden = 0;
    initPath = std::filesystem::current_path();
  } else if (_ARGC == 2) {
    // argv1 is either hide or path
    std::string argv1 = _ARGV[1];

    if (argv1[0] == '-') {
      // is an option

      // is a recognised option
      if (argv1 == "-a") {
        withHidden = 1;
        initPath = std::filesystem::current_path();
      } else {
        usageListChildren();
        return 2;
      }
    } else {
      // is a path
      initPath = argv1;
    }
  } else if (_ARGC == 3) {
    // total options under current implementation is 1 (-a), then additional
    // argument must be a path
    std::string argv1 = _ARGV[1];
    if (argv1 != "-a") {
      usageListChildren();
      return 2;
    }
    withHidden = 1;
    initPath = _ARGV[2];
  } else {
    usageListChildren();
    return 2;
  }

  // remove trailing '/' if any
  if (initPath.back() == '/') {
    initPath.pop_back();
  }

  std::filesystem::path parentPath(initPath);
  std::string title;
  size_t fieldIdx;
  size_t fieldSz;
  std::vector<std::string> fields;
  std::vector<std::filesystem::path> children;

  // create path navigator object;
  BlackOS::Trinkets::PathController pathController;
  pathController.showHidden(withHidden);

  try {
    pathController.loadParent(parentPath);
  } catch (std::filesystem::filesystem_error &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  title = pathController.generateTitle();
  fields = pathController.generateFields();

  printw("\n");
  attron(A_UNDERLINE);
  printw(title.c_str());
  attroff(A_UNDERLINE);
  printw("\n");

  for (std::string const &field : fields) {
    printw(field.c_str());
    printw("\n");
  }
  printw("\n");
  refresh();

  return 0;
}
} // namespace Trinkets
} // namespace BlackOS
