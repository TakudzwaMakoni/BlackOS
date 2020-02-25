// navigate-dir 2020 by takudzwa Makoni (c)

#include "inc/NavigationHelpers.h"
#include "inc/Navigator.h"

#include "ncurses.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace BlackOS::Trinkets;

int main(int argc, char const *argv[]) {

  std::string trueVal = "True";
  bool showHidden = argv[1] == trueVal ? 1 : 0;
  long cursor_start = strtol(argv[2], NULL, 10);
  std::string initPath = argv[3];

  // remove trailing '/' if any
  if (initPath.back() == '/') {
    initPath.pop_back();
  }

  std::filesystem::path parentPath(initPath);

  size_t fieldIdx;
  std::string chosenPath;

  std::vector<int> breakConditions = {(int)'a', (int)'d',     (int)'q',
                                      (int)'h', 10 /*ENTER*/, 27 /*ESC*/};

  while (1) {

    Navigator testNav(parentPath, showHidden);
    auto fields = testNav.generateFields();

    std::string title = testNav.generateTitle();
    size_t fieldSz = fields.size();

    size_t width = title.length() + 1;
    size_t height = fieldSz + 2;

    std::string const menuID = "Navigator";
    size_t pagination = fieldSz;

    if (height > LINES) {
      cursor_start = 0;
      pagination = LINES - 2;
      height = LINES;
    }

    // enter curses mode
    initscr();
    cbreak();
    cursor(0);

    BlackOS::DisplayKernel::Kmenu NavigationMenu(menuID, height, width,
                                                 cursor_start, 0);

    if (fieldSz == 0) {

      std::string message;
      if (showHidden) {
        message = "no entries to show.";
      } else {
        message = "no entries to show (excl. hidden paths).";
      }
      mvwprintw(stdscr, cursor_start, 0, message.c_str());
      wgetch(stdscr);

      // go back up a level and skip iteration.
      parentPath = parentPath.parent_path();
      NavigationMenu.clear();
      cursor_start = 0;
      continue;
    }

    NavigationMenu.loadTitle(title,
                             BlackOS::DisplayKernel::TitleStyle::underline);
    NavigationMenu.loadFields(fields);
    NavigationMenu.loadFieldAlignment(-1, 1);
    NavigationMenu.paginate(pagination);

    NavigationMenu.setWin(1);
    NavigationMenu.hideBorder();
    NavigationMenu.showTitle();

    std::string showingHidden = showHidden ? "t" : "f";

    // TODO: duplicate inserting is a hack, better to implement KCanvas and have
    // separate window as attribute panel on bottom of screen
    NavigationMenu.insert("showing hidden paths: " + showingHidden, height - 1,
                          0);
    NavigationMenu.display(breakConditions);

    NavigationMenu.insert("showing hidden paths: " + showingHidden, height - 1,
                          0);
    int lastKey = NavigationMenu.lastKeyPressed();

    if (lastKey == (int)'q' /*exit*/) {
      NavigationMenu.exitWin();
      exit(1);
    } else if (lastKey == (int)'a' /*out of directory*/) {
      parentPath = parentPath.parent_path();
      NavigationMenu.clear();
      cursor_start = 0;
    } else if (lastKey == (int)'d' /*into dir*/) {
      fieldIdx = NavigationMenu.selectedFieldIndex();
      auto newPath = testNav.children()[fieldIdx];
      if (pathType(newPath) == "directory") {
        parentPath = newPath;
        cursor_start = 0;
      }
      NavigationMenu.clear();
    } else if (lastKey == (int)'h') {
      // toggle hide files
      if (showHidden) {
        showHidden = 0;
      } else {
        showHidden = 1;
      }
      NavigationMenu.clear();
    } else {
      // is enter
      fieldIdx = NavigationMenu.selectedFieldIndex();
      chosenPath = testNav.children()[fieldIdx];
      NavigationMenu.exitWin();
      break;
    }
  }

  std::string type = pathType(chosenPath);
  std::ofstream navigationDir("/tmp/navigationDirectory.txt");
  navigationDir << chosenPath;

  return 0;
}
