/**
 * SplashScreen
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
