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

std::string produceBanner(std::string const &str, int winwidth) {
  int widthA = (winwidth + str.length()) / 2;
  std::stringstream ss;
  ss << std::setw(widthA) << std::right << str;
  return ss.str();
}

} // namespace

int shortcut(std::string const &file, int y, int x,
             std::vector<int> const &colours) {
  size_t cursor_pos_y;
  size_t cursor_pos_x;

  // uncomment to allow externally specified window position
  // cursor_pos_y = strtol(argv[2], nullptr, 10);
  // cursor_pos_x = strtol(argv[3], nullptr, 10);
  // initPath = argv[4];

  auto const termSz = BlackOS::DisplayKernel::TERMINAL_SIZE();
  size_t const ROWS = termSz[0];
  size_t const COLS = termSz[1];

  if (y < 0 || y > ROWS - 10)
    cursor_pos_y = ROWS / 2;
  else
    cursor_pos_y = y;

  if (x < 0)
    cursor_pos_x = 0;
  else
    cursor_pos_x = x;
  // uncomment if using externally specified window position

  std::string chosenPath;
  size_t fieldIdx;
  size_t fieldSz;

  std::string title;
  size_t menuWidth;
  size_t menuHeight;
  size_t pagination;

  std::string hiddenAttribute = "showing hidden paths: ";

  // create menu object
  BlackOS::DisplayKernel::Menu ShortcutMenu(ROWS - cursor_pos_y, COLS,
                                            cursor_pos_y, cursor_pos_x);

  // load shortcuts
  int maxNameLen = 0;
  int maxDirLen = 0;

  std::ifstream infile(file);
  std::string shortcutName, directory;
  std::vector<std::string> shortcutNames, directories, fields;

  while (infile >> shortcutName >> directory) {
    shortcutNames.push_back(shortcutName);
    directories.push_back(directory);

    if (maxNameLen < shortcutName.length())
      maxNameLen = shortcutName.length();
    if (maxDirLen < directory.length())
      maxDirLen = directory.length();
  }

  if (maxNameLen > 8 /*length of 'shortcut'*/)
    maxNameLen += 3;
  else
    maxNameLen = 11;

  if (maxDirLen < 6 /*length of 'entity'*/)
    maxDirLen = 6;

  std::string formatString = "{0:<" + std::to_string(maxNameLen) + "}{1:<" +
                             std::to_string(maxDirLen) + "}";
  title = fmt::format(formatString, "shortcut", "path");
  for (int i = 0; i < shortcutNames.size(); i++) {
    std::string field =
        fmt::format(formatString, shortcutNames[i], directories[i]);
    fields.push_back(field);
  }

  fieldSz = fields.size();
  ShortcutMenu.setWin(1);

  if (!colours.empty()) {
    ShortcutMenu.bgfg(colours[0], colours[1]);
  }

  if (fieldSz == 0) {
    std::string message = "no entries to show.";
    ShortcutMenu.clear(); // clear previous output
    ShortcutMenu.insert(message.c_str(), 0, 0);
    ShortcutMenu.refresh(); // present message to screen
    ShortcutMenu.pause();
    ShortcutMenu.setWin(0);
    return 0;
  }

  menuWidth = maxDirLen + maxNameLen + 1;
  menuHeight = ROWS - cursor_pos_y - 2;
  pagination = menuHeight - 1;

  ShortcutMenu.loadTitle(title, BlackOS::DisplayKernel::TextStyle::underline);
  ShortcutMenu.loadFields(fields);
  ShortcutMenu.loadFieldAlignment(-1, 1);
  ShortcutMenu.paginate(pagination, pagination <= fieldSz);

  ShortcutMenu.hideBorder();
  ShortcutMenu.showTitle();

  ShortcutMenu.resize(menuHeight, menuWidth);
  ShortcutMenu.reposition(cursor_pos_y /*maintain cursor y position*/,
                          cursor_pos_x /*left of screen*/);

  std::vector<int> breakConditions = {(int)'d', (int)'q', 10 /*ENTER*/,
                                      27 /*ESC*/};
  ShortcutMenu.display(breakConditions);

  int lastKey = ShortcutMenu.lastKeyPressed();
  size_t selected = ShortcutMenu.selectedFieldIndex();

  if (lastKey == (int)'q' || lastKey == 27 /*ESC*/) {
    // user exited program
    ShortcutMenu.clear();
    ShortcutMenu.refresh();
    ShortcutMenu.setWin(0);
    return 0; // leave here
  } else if (lastKey == (int)'d' /*begin navigateDir into dir*/) {
    // user navigated up a directory
    ShortcutMenu.clear(); // clear previous output
    ShortcutMenu.refresh();
    ShortcutMenu.setWin(0);
    std::string chosenDir = directories[selected];
    std::vector<std::string> argvsForND={"nd",chosenDir};
    navigateDir(argvsForND, cursor_pos_y, cursor_pos_x,colours);
    return 0;
  } else {
    // enter was pressed
    ShortcutMenu.clear(); // clear previous output
    ShortcutMenu.refresh();
    ShortcutMenu.setWin(0);
    changeDir({"cd",directories[selected]});
    return 0;
  }
}

int navigateDir(std::vector<std::string> const &argv, int y, int x,
                std::vector<int> const &colours) {

  int argc = argv.size();

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

  if (y < 0 || y > ROWS - 10)
    cursor_pos_y = ROWS / 2;
  else
    cursor_pos_y = y;

  if (x < 0)
    cursor_pos_x = 0;
  else
    cursor_pos_x = x;
  // uncomment if using externally specified window position

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
    withHidden = 1;
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
  // 'e' key to exit here (in the current parent directory).
  // 'ENTER' key to change directory, or open file with EDITOR environment
  // variable.
  // 'q' or 'ESC' key to exit navigation menu.
  std::vector<int> breakConditions = {(int)'a', (int)'d', (int)'q',
                                      (int)'h', (int)'e', 10 /*ENTER*/,
                                      27 /*ESC*/};

  std::string hiddenAttribute = "showing hidden paths: ";

  // create menu object
  BlackOS::DisplayKernel::Menu NavigationMenu(ROWS - cursor_pos_y, COLS,
                                              cursor_pos_y, cursor_pos_x);

  // create new window object
  BlackOS::DisplayKernel::Window CurrentDirWindow(1, COLS, cursor_pos_y,
                                                  cursor_pos_x);

  if (!colours.empty()) {
    NavigationMenu.bgfg(colours[0], colours[1]);
    CurrentDirWindow.bgfg(colours[0], colours[1]);
  }
  // create path navigator object;
  PathController pathController;

  NavigationMenu.setWin(1);
  CurrentDirWindow.setWin(1);

  if (!colours.empty()) {
    NavigationMenu.bgfg(colours[0], colours[1]);
    CurrentDirWindow.bgfg(colours[0], colours[1]);
  }

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
        CurrentDirWindow.setWin(0);
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
    menuHeight = ROWS - cursor_pos_y - 2;
    pagination = menuHeight - 3;

    NavigationMenu.resize(menuHeight, menuWidth);
    NavigationMenu.reposition(cursor_pos_y + 2 /*maintain cursor y position*/,
                              cursor_pos_x /*left of screen*/);

    if (fieldSz == 0) {
      std::string message;
      if (withHidden) {
        message = "no entries to show.";
      } else {
        message = "no entries to show (excl. hidden paths).";
      }
      NavigationMenu.clear(); // clear previous output
      NavigationMenu.insert(message.c_str(), 0, 0);
      NavigationMenu.refresh(); // present message to screen
      NavigationMenu.pause();
      // go back up a level and skip iteration.
      parentPath = parentPath.parent_path(); // go up a dir
      NavigationMenu.clear(); // clear message for next iteration.
      continue;
    }

    // load all menu attributes first
    NavigationMenu.loadTitle(title,
                             BlackOS::DisplayKernel::TextStyle::underline);
    NavigationMenu.loadFields(fields);
    NavigationMenu.loadFieldAlignment(-1, 1);
    NavigationMenu.paginate(pagination, pagination <= fieldSz);

    NavigationMenu.hideBorder();
    NavigationMenu.showTitle();

    std::string showingHidden = withHidden ? "t" : "f";
    std::string hiddenInfo = hiddenAttribute + showingHidden;
    size_t attributePosition = menuHeight - 1;

    std::string currentDir = parentPath;
    std::string currentDirMessage = "looking in:";
    currentDir = " " + currentDir;

    CurrentDirWindow.resize(1,
                            currentDirMessage.length() + currentDir.length());
    CurrentDirWindow.insert(currentDirMessage, 0, 0,
                            BlackOS::DisplayKernel::TextStyle::underline);
    CurrentDirWindow.insert(currentDir, 0, currentDirMessage.length());
    CurrentDirWindow.refresh();

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
      CurrentDirWindow.clear();
      NavigationMenu.setWin(0);
      CurrentDirWindow.setWin(0);
      return 0; // leave here
    } else if (lastKey == (int)'a' /*out of directory*/) {
      // user navigated up a directory
      parentPath = parentPath.parent_path();
      NavigationMenu.clear();
      CurrentDirWindow.clear();
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
      CurrentDirWindow.clear();
    } else if (lastKey == (int)'h') {
      // toggle hide files
      if (withHidden) {
        withHidden = 0;
      } else {
        withHidden = 1;
      }
      NavigationMenu.clear();
    } else if (lastKey == (int)'e') {

      // exit at parent directory

      std::string parentDir = parentPath;
      changeDir({"cd",parentDir});
      NavigationMenu.clear();
      CurrentDirWindow.clear();
      NavigationMenu.setWin(0);
      CurrentDirWindow.setWin(0);
      break;
    } else {

      // last key pressed is enter
      fieldIdx = NavigationMenu.selectedFieldIndex();
      chosenPath = children[fieldIdx];
      auto chosenPathType = pathType(chosenPath);
      if (chosenPathType == "directory") {
        changeDir({"cd",chosenPath});
        NavigationMenu.clear();
        CurrentDirWindow.clear();
        NavigationMenu.setWin(0);
        CurrentDirWindow.setWin(0);
        break;
      } else if (chosenPathType == "file") {
        int result = openWithTextEditor(chosenPath);
        NavigationMenu.clear();
        if (result == -1) {
          NavigationMenu.insert("no EDITOR environment variable is set.", 0, 0);
          NavigationMenu.refresh();
          NavigationMenu.pause();
        }
      }
    }
  }

  return 0;
}

int listChildren(std::vector<std::string> const &argv,
                 std::vector<std::string> &v) {

  int argc = argv.size();

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
    withHidden = 1;
    initPath = argv[2];
  } else {
    usageListChildren();
    return 2;
  }

  // remove trailing '/' if any
  if (initPath.back() == '/') {
    initPath.pop_back();
  }

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

  v.push_back(title);
  for (auto const field : fields) {
    v.push_back(field);
  }

  return 0;
}

int changeDir(std::vector<std::string> const &argv) {
  if (argv.size()==1) {
    char const *homeDir = getenv("HOME");
    chdir(homeDir);
  } else if (argv.size()==2) {
	
	std::string const &path = argv[1];
    if (chdir(path.c_str()) != 0) {
      char errStr[256] = "cd: ";
      perror(strcat(errStr, path.c_str()));
      return 1;
    }
  }
  return 0;
}

int openWithTextEditor(std::string const &path) {
  char *editor = getenv("EDITOR");
  if (editor == NULL)
    return -1;
  std::string command = editor;
  command += " " + path;
  system(command.c_str());
  return 0;
}

std::vector<std::string> splashScreen(std::vector<std::string> const &argv) {

  std::string spc = " "; // space string for easier reading.
  std::string marginline =
      "* * * * * * * * * * * * * * * * * * * * * * * * * * * * "
      "* * * *"; // border

  std::vector<std::string> v;

  if (argv.size() != 8) {
    return v;
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

  v.push_back(produceBanner(marginline, x));
  v.push_back(produceBanner(title + "v" + version + ", " + repo, x));
  v.push_back(
      produceBanner(license + year + ", " + "written in " + language, x));
  v.push_back(produceBanner("by " + author, x));
  v.push_back(produceBanner("git: " + git, x));
  v.push_back(produceBanner(marginline, x));

  return v;
}

} // namespace Trinkets
} // namespace BlackOS
