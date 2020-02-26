//
// Created by Takudzwa Makoni on 2019-07-13.
//

// test
#include "../inc/DisplayHelpers.h"
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h>    // for STDOUT_FILENO
#include <vector>

namespace BlackOS {
namespace DisplayKernel {

std::vector<size_t> TERMINAL_SIZE() {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  return {size.ws_row, size.ws_col};
}
} // namespace DisplayKernel
} // namespace BlackOS
