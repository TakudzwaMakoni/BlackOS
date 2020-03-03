
#include "../inc/SplashScreen.h"
#include "DisplayHelpers.h"

namespace BlackOS {
namespace Trinkets {

void eol(int v) { // print end of line to terminal
  for (int i = 0; i < v; i++) {
    std::cout << "\n";
  }
}
void pf(std::string const &str, int winwidth) { // print formatted

  int widthA =
      (winwidth + str.length()) / 2; // calc centre of terminal window position
  std::cout << std::setw(widthA) << std::right << str;
}

int splashScreen(std::vector<std::string> const &argv) {

  std::string spc = " "; // space string for easier reading.
  std::string marginline =
      "* * * * * * * * * * * * * * * * * * * * * * * * * * * * "
      "* * * *"; // border

  if (argv.size() != 8) {
    return 1;
  }
  std::string title;
  std::string version;
  std::string repo;
  std::string license;
  std::string year;
  std::string language;
  std::string author;
  std::string git;

  title = argv[0] + spc;
  version = argv[1] + spc;
  repo = argv[2] + spc;
  license = argv[3] + spc;
  year = argv[4] + spc;
  language = argv[5] + spc;
  author = argv[6] + spc;
  git = argv[7] + spc;

  std::vector<size_t> sz = DisplayKernel::TERMINAL_SIZE();
  int x = sz[1];
  int y = sz[0];

  eol((y-8)/2);
  pf(marginline, x);
  eol(2);
  pf(title + "v" + version + ", " + repo, x);
  eol();
  pf(license + year + ", " + "written in " + language, x);
  eol(2);
  pf("by " + author, x);
  eol();
  pf("git: " + git, x);
  eol(2);
  pf(marginline, x);
  eol(2); // padding

  // moves cursor back to top

  return 0;
}
} // namespace Trinkets
} // namespace BlackOS
