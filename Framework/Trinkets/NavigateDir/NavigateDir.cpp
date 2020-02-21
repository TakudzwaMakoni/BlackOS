// navigate-dir 2020 by takudzwa Makoni (c)
#include "Directives.h"
#include "Kfield.h"
#include "Kmenu.h"
#include "inc/Navigator.h"

#include <iostream>
#include <string>

using namespace BlackOS::Trinkets;

void eraseAllSubStr(std::string &mainStr, std::string const &toErase) {
  size_t pos = std::string::npos;

  // Search for the substring in string in a loop untill nothing is found
  while ((pos = mainStr.find(toErase)) != std::string::npos) {
    // If found then erase it from string
    mainStr.erase(pos, toErase.length());
  }
}

int main(int argc, char const *argv[]) {
  std::string test;
  bool showHidden = 0;
  BlackOS::DisplayKernel::Kmenu menu(test, 1, 1, 1, 1);
  std::string path = "/home/takudzwa";

  Navigator testNav(path);
  std::cout << testNav.pathPermissions(path) << std::endl;
  std::cout << testNav.pathLastModifiedDate(path) << std::endl;

  return 0;
}
