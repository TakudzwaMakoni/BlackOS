// ListChildren (c), By Takudzwa Makoni 2020

#include "DisplayHelpers.h"
#include "PathController.h"

#include <iostream>

int main(int argc, char const *argv[]) {

  /*
  ====================================================================
  SYSTEM ARGUMENTS
  ====================================================================
  */

  bool withHidden;
  size_t cursor_pos_y;
  size_t cursor_pos_x;
  std::string initPath;

  withHidden = strtol(argv[1], nullptr, 10);

  // uncomment to allow externally specified window position
  // cursor_pos_y = strtol(argv[2], nullptr, 10);
  // cursor_pos_x = strtol(argv[3], nullptr, 10);
  // initPath = argv[4];

  auto const termSz = BlackOS::DisplayKernel::TERMINAL_SIZE();
  size_t const ROWS = termSz[0];
  size_t const COLS = termSz[1];

  // uncomment if using externally specified window position
  cursor_pos_y = ROWS / 2;
  cursor_pos_x = 0;
  initPath = argv[2];

  // remove trailing '/' if any
  if (initPath.back() == '/') {
    initPath.pop_back();
  }

  /*
  ====================================================================
  PATH CONTROLLER VARIABLES
  ====================================================================
  */

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
    exit(2);
  }

  title = pathController.generateTitle();
  fields = pathController.generateFields();
  std::cout << "\n\033[4m" << title << "\033[0m"
            << "\n";
  for (std::string const &field : fields) {
    std::cout << field << "\n";
  }
  std::cout << std::endl;
  return 0;
}
