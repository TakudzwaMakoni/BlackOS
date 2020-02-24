//
// Created by Takudzwa Makoni on 2019-07-13.
//

// test
#include "../inc/Directives.h"
#include <unistd.h>

namespace BlackOS {
namespace DisplayKernel {
namespace Directives {
void doNothing() {}
void exitProgram() {
  endwin();
  exit(0);
}
void writeToFile(std::string const &fullPath, std::string const &buffer) {

  std::ofstream file;
  file.open(fullPath);
  file << buffer;
  file.close();
}
void changeDirectory(std::string const &fullPath) { chdir(fullPath.c_str()); }
void animate(int aniCode) {}
} // namespace Directives
} // namespace DisplayKernel
} // namespace BlackOS
