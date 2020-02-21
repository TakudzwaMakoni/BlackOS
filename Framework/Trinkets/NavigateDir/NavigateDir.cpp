// navigate-dir 2020 by takudzwa Makoni (c)
#include "Directives.h"
#include "Kfield.h"
#include "Kmenu.h"

#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
  std::string test;
  BlackOS::DisplayKernel::Kmenu menu(test, 1, 1, 1, 1);
  std::string path = "/home/takudzwa";
  for (const auto &entry : std::filesystem::directory_iterator(path))
    std::cout << entry.path() << std::endl;
  return 0;
}
