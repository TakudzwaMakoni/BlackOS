/**
 * Natives
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

#include "../inc/Natives.h"

namespace BlackOS {
namespace Trinkets {

namespace {
void usageNavigateDir() {
  std::cout << "Usage:\n"
            << "ndir [options] <path>\n"
            << "options:\n'-a' : show hidden\n";
}

void usageListChildren() {
  std::cout << "Usage:\n"
            << "ls [options] <path>\n"
            << "options:\n'-a' : show hidden\n";
}

static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
  tcgetattr(0, &old);         /* grab old terminal i/o settings */
  current = old;              /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
    current.c_lflag |= ECHO; /* set echo mode */
  } else {
    current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) { tcsetattr(0, TCSANOW, &old); }

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
} // namespace

int navigateDir(int argc, char **argv) {

  /*
  ====================================================================
  SYSTEM ARGUMENTS
  ====================================================================
  */

  bool withHidden;
  size_t cursor_pos_y;
  size_t cursor_pos_x;
  std::string initPath;

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

  if (argc == 1) {
    withHidden = 0;
    initPath = std::filesystem::current_path();
  } else if (argc == 2) {
    // argv1 is either hide or path
    std::string argv1 = argv[1];

    if (argv1[0] == '-') {
      // is an option

      // is a recognised option
      if (argv1 == "-a") {
        withHidden = 1;
        initPath = std::filesystem::current_path();
      } else {
        usageNavigateDir();
        return 2;
      }
    } else {
      // is a path
      initPath = argv1;
    }
  } else if (argc == 3) {
    // total options under current implementation is 1 (-a), then additional
    // argument must be a path
    std::string argv1 = argv[1];
    if (argv1 != "-a") {
      usageNavigateDir();
      return 2;
    }
    withHidden = argv[1];
    initPath = argv[2];
  } else {
    usageNavigateDir();
    return 2;
  }

  // remove trailing '/' if any
  if (initPath.back() == '/') {
    initPath.pop_back();
  }

  std::filesystem::path parentPath(initPath);
  std::string chosenPath;
  size_t fieldIdx;
  size_t fieldSz;
  std::vector<std::string> fields;
  std::vector<std::filesystem::path> children;

  std::string title;
  size_t menuWidth;
  size_t menuHeight;
  size_t pagination;

  // break conditions for menu display.
  // 'a' key to navigate up a directory.
  // 'd' key to navigate into highlighted directory.
  // 'h' key to toggle hidden folders view.
  // 'ENTER' key to change directory, or open file with EDITOR environment
  // variable.
  // 'q' or 'ESC' key to exit navigation menu.
  std::vector<int> breakConditions = {(int)'a', (int)'d',     (int)'q',
                                      (int)'h', 10 /*ENTER*/, 27 /*ESC*/};

  std::string hiddenAttribute = "showing hidden paths: ";

  // create menu object
  BlackOS::DisplayKernel::Menu NavigationMenu(ROWS - cursor_pos_y, COLS,
                                              cursor_pos_y, cursor_pos_x);

  BlackOS::DisplayKernel::Window AttributeWindow(1, COLS, ROWS - 1, 0);

  // create path navigator object;
  PathController pathController;

  NavigationMenu.setWin(1);
  AttributeWindow.setWin(1);
  while (1) {

    pathController.showHidden(withHidden);

    try {
      pathController.loadParent(parentPath);
    } catch (std::filesystem::filesystem_error &e) {
      NavigationMenu.clear();
      NavigationMenu.insert(e.what(), 0, 0);
      NavigationMenu.refresh();
      NavigationMenu.pause();
      if (parentPath == initPath) {
        // user initialised parent directory without access
        NavigationMenu.setWin(0);
        AttributeWindow.setWin(0);
        return -1; // leave here TODO: exit codes
      } else {
        // user navigated into directory without permissions
        // return to parent directory.
        parentPath = parentPath.parent_path();
      }
      continue;
    }

    fields = pathController.generateFields();
    title = pathController.generateTitle();
    children = pathController.children();
    fieldSz = fields.size();

    // include 1 additional space.
    menuWidth = title.length() + 1;
    menuHeight = ROWS - cursor_pos_y;
    pagination = menuHeight - 3;

    NavigationMenu.resize(menuHeight, menuWidth);
    NavigationMenu.reposition(cursor_pos_y /*maintain cursor y position*/,
                              cursor_pos_x /*left of screen*/);

    if (fieldSz == 0) {
      std::string message;
      if (withHidden) {
        message = "no entries to show.";
      } else {
        message = "no entries to show (excl. hidden paths).";
      }
      NavigationMenu.clear(); // clear previous output
      NavigationMenu.insert(message.c_str(), cursor_pos_y, 0);
      NavigationMenu.refresh(); // present message to screen
      wgetch(stdscr);
      // go back up a level and skip iteration.
      parentPath = parentPath.parent_path(); // go up a dir
      NavigationMenu.clear(); // clear message for next iteration.
      continue;
    }

    // load all menu attributes first
    NavigationMenu.loadTitle(title,
                             BlackOS::DisplayKernel::TextStyle::underline);
    NavigationMenu.loadFields(fields);
    NavigationMenu.loadFieldAlignment(-1, 1); // top left of window
    NavigationMenu.paginate(pagination, pagination <= fieldSz);

    NavigationMenu.hideBorder();
    NavigationMenu.showTitle();

    std::string showingHidden = withHidden ? "t" : "f";
    std::string hiddenInfo = hiddenAttribute + showingHidden;
    size_t attributePosition = menuHeight - 1;
    std::vector<size_t> ignoreBlocks = {attributePosition, 0, attributePosition,
                                        hiddenInfo.length()};
    NavigationMenu.insert(hiddenInfo, attributePosition, 0);
    NavigationMenu.refresh();

    NavigationMenu.display(breakConditions, ignoreBlocks);

    // retrieve last key entered by user on exit display call
    int lastKey = NavigationMenu.lastKeyPressed();

    if (lastKey == (int)'q' || lastKey == 27 /*ESC*/) {
      // user exited program
      NavigationMenu.clear();
      NavigationMenu.setWin(0);
      AttributeWindow.setWin(0);

      return 0; // leave here
    } else if (lastKey == (int)'a' /*out of directory*/) {
      // user navigated up a directory
      parentPath = parentPath.parent_path();
      NavigationMenu.clear();
    } else if (lastKey == (int)'d' /*into dir*/) {

      // user navigated into a child directory
      // get index pertaining to field chosen by user
      fieldIdx = NavigationMenu.selectedFieldIndex();

      // assign new path to chosen path in children,
      // mapped by fieldIdx.
      auto newPath = children[fieldIdx];

      // check the path is a directory
      // set parent to this path if true
      if (pathType(newPath) == "directory") {
        parentPath = newPath;
      }
      // clear window for next iteration
      NavigationMenu.clear();
    } else if (lastKey == (int)'h') {
      // toggle hide files
      if (withHidden) {
        withHidden = 0;
      } else {
        withHidden = 1;
      }
      NavigationMenu.clear();
    } else {
      // last key pressed is enter
      fieldIdx = NavigationMenu.selectedFieldIndex();
      chosenPath = children[fieldIdx];
      NavigationMenu.clear();
      NavigationMenu.setWin(0);
      AttributeWindow.setWin(0);

      break; // break from while loop
    }
  }
  changeDir(chosenPath.c_str());
  return 0;
}

int listChildren(int argc, char **argv) {

  /*
  ====================================================================
  SYSTEM ARGUMENTS
  ====================================================================
  */

  bool withHidden;
  size_t cursor_pos_y;
  size_t cursor_pos_x;
  std::string initPath;

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

  if (argc == 1) {
    withHidden = 0;
    initPath = std::filesystem::current_path();
  } else if (argc == 2) {
    // argv1 is either hide or path
    std::string argv1 = argv[1];

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
  } else if (argc == 3) {
    // total options under current implementation is 1 (-a), then additional
    // argument must be a path
    std::string argv1 = argv[1];
    if (argv1 != "-a") {
      usageListChildren();
      return 2;
    }
    withHidden = argv[1];
    initPath = argv[2];
  } else {
    usageListChildren();
    return 2;
  }

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
    return -1;
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

int changeDir(char const *path) {
  if (path == nullptr) {
    char const *homeDir = getenv("HOME");
    chdir(homeDir);
  } else {
    if (chdir(path) != 0) {
      char errStr[256] = "cd: ";
      perror(strcat(errStr, path));
      return 1;
    }
  }
  return 0;
}

int parseUserInput(char **argv) {
  std::string line;
  char *cstr;
  char ch;
  int argc = 0;
  do {
    ch = getch_(1);
    std::cout << ch;
    if (ch == 27) {
      return userInput::up;
    }
    line += ch;
  } while (ch != '\n');

  // tokenize the string
  std::stringstream ss(line);
  std::string item;

  while (std::getline(ss, item, ' ')) {
    cstr = new char[item.size() + 1];
    strcpy(cstr, item.c_str());
    argv[argc] = cstr;
    argc++;
  }
  return 0;
}

} // namespace Trinkets
} // namespace BlackOS
