// SPL(AS)H(C)R(ii)N (c), by Takudzwa Makoni 2020

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#define arraysize(ar) (sizeof(ar) / sizeof(ar[0]))

using namespace std;

void eol(int v = 1) { // print end of line to terminal
  for (int i = 0; i < v; i++) {
    cout << "\n";
  }
}
void pf(string str, int winwidth) { // print formatted

  int widthA =
      (winwidth + str.length()) / 2; // calc centre of terminal window position
  cout << setw(widthA) << right << str;
}

vector<int> winSz() {

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  vector<int> siz = {w.ws_row, w.ws_col};
  return siz;
}

void pushDown(int rows) {
  size_t correction = abs(rows - 8 /*average desirable value*/) / 2;
  for (int i = 0; i < correction; ++i) {
    eol();
  }
}

int main(int argc, char **argv) {

  string spc = " "; // space string for easier reading.
  string marginline = "* * * * * * * * * * * * * * * * * * * * * * * * * * * * "
                      "* * * *"; // border

  string title;
  string version;
  string repo;
  string company;
  string year;
  string language;
  string author;
  string git;

  if (argc > 1) {
    title = argv[1] + spc;
    version = argv[2] + spc;
    repo = argv[3] + spc;
    company = argv[4] + spc;
    year = argv[5] + spc;
    language = argv[6] + spc;
    author = argv[7] + spc;
    git = argv[8] + spc;
  }

  vector<int> sz = winSz(); // window
  int x = sz[1];
  int y = sz[0];

  // printf("\033[0;0H"); move cursor (TODO: dont do this here)
  // pushDown(y); // pushDown (TODO: dont need this here anymore)
  pf(marginline, x);
  eol(2);
  pf(title + "v" + version + ", " + repo, x);
  eol();
  pf(company + year + ", " + "written in " + language, x);
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
