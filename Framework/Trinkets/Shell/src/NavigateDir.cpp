/**
 * Tr(inkets) Shell NavigateDir
 *
 * Copyright (C) 2020 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "../../helpers/PathController.h"
#include "../Shell.h"
#include "Menu.h"
#include "Window.h"

namespace DK = BlackOS::DisplayKernel;

namespace BlackOS {
namespace Trinkets {

int Shell::navigateDir() {
  curs_set(0);
  _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);

  bool withHidden;
  std::string initPath;

  int y, x;
  if (_CURSOR_Y < 0 || _CURSOR_Y > _DISPLAY_SIZE_Y - 10)
    y = _DISPLAY_SIZE_Y / 2;
  else
    y = _CURSOR_Y + 1;

  // uncomment if using externally specified window position

  if (_ARGC == 1) {
    withHidden = 0;
    initPath = std::filesystem::current_path();
  } else if (_ARGC == 2) {
    // argv1 is either hide or path
    std::string argv1 = _ARGV[1];

    if (argv1[0] == '-') {
      // is an option

      // is a recognised option
      if (argv1 == "-a") {
        withHidden = 1;
        initPath = std::filesystem::current_path();
      } else {
        _DISPLAY->print("Usage:\n"
                        "ndir [options] <path>\n"
                        "options:\n'-a' : show hidden\n");
        _DISPLAY->moveCursor(_CURSOR_Y, 0);
        curs_set(_CURSOR);
        _DISPLAY->refresh();
        return 2;
      }
    } else {
      // is a path
      initPath = argv1;
    }
  } else if (_ARGC == 3) {
    // total options under current implementation is 1 (-a), then additional
    // argument must be a path
    std::string argv1 = _ARGV[1];
    if (argv1 != "-a") {
      _DISPLAY->print("Usage:\n"
                      "ndir [options] <path>\n"
                      "options:\n'-a' : show hidden\n");
      curs_set(_CURSOR);
      _DISPLAY->refresh();
      return 2;
    }
    withHidden = 1;
    initPath = _ARGV[2];
  } else {
    _DISPLAY->print("Usage:\n"
                    "ndir [options] <path>\n"
                    "options:\n'-a' : show hidden\n");
    curs_set(_CURSOR);
    _DISPLAY->refresh();
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
  // 's' to enter shortcus
  std::vector<int> breakConditions = {(int)'a',   (int)'d', (int)'q',
                                      (int)'h',   (int)'e', 10 /*ENTER*/,
                                      27 /*ESC*/, (int)'s'};

  std::string hiddenAttribute = "showing hidden paths: ";

  // create menu object
  BlackOS::DisplayKernel::Menu NavigationMenu(_DISPLAY_SIZE_Y - y,
                                              _DISPLAY_SIZE_X, y, 0);

  // create new window object
  BlackOS::DisplayKernel::Window CurrentDirWindow(1, _DISPLAY_SIZE_X, y, 0);

  // create path navigator object;
  PathController pathController;

  NavigationMenu.setWin(DK::WIN_SET_CODE::INIT_CHILD);
  CurrentDirWindow.setWin(DK::WIN_SET_CODE::INIT_CHILD);

  if (_USING_COLOR_FLAG) {
    NavigationMenu.bgfg(_FOREGROUND, _BACKGROUND);
    CurrentDirWindow.bgfg(_FOREGROUND, _BACKGROUND);
  }

  while (1) {

    pathController.showHidden(withHidden);

    try {
      pathController.loadParent(parentPath);
    } catch (std::filesystem::filesystem_error &e) {
      NavigationMenu.eraseWin();
      NavigationMenu.insert(e.what(), 0, 0);
      NavigationMenu.refresh();
      NavigationMenu.pause();
      if (parentPath == initPath) {
        // user initialised parent directory without access
        NavigationMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
        CurrentDirWindow.setWin(
            BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
        _DISPLAY->moveCursor(_CURSOR_Y, 0);
        curs_set(_CURSOR);
        _DISPLAY->refresh();
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
    menuHeight = _DISPLAY_SIZE_X - y - 2;
    pagination = menuHeight - 3;

    NavigationMenu.resize(menuHeight, menuWidth);
    NavigationMenu.reposition(y + 2 /*maintain cursor _CURSOR_Y position*/,
                              0 /*left of screen*/);

    if (fieldSz == 0) {
      std::string message;
      if (withHidden) {
        message = "no entries to show.";
      } else {
        message = "no entries to show (excl. hidden paths).";
      }
      NavigationMenu.eraseWin(); // clear previous output
      NavigationMenu.insert(message.c_str(), 0, 0);
      NavigationMenu.refresh(); // present message to screen
      NavigationMenu.pause();
      // go back up a level and skip iteration.
      parentPath = parentPath.parent_path(); // go up a dir
      NavigationMenu.eraseWin(); // clear message for next iteration.
      continue;
    }

    // load all menu attributes first
    NavigationMenu.loadTitle(title, A_BOLD);
    NavigationMenu.initFields(fields);
    NavigationMenu.loadFieldAlignment(-1, 1);
    NavigationMenu.paginate(pagination, pagination <= fieldSz);

    NavigationMenu.hideBorder();
    NavigationMenu.showTitle();

    std::string showingHidden = withHidden ? "t" : "f";
    std::string hiddenInfo = hiddenAttribute + showingHidden;
    size_t attributePosition = menuHeight - 1;

    std::string currentDir = parentPath;
    std::string currentDirMessage = "looking in: ";
    currentDir = " " + currentDir;

    CurrentDirWindow.resize(1,
                            currentDirMessage.length() + currentDir.length());
    CurrentDirWindow.reposition(y, 0);
    CurrentDirWindow.insert(currentDirMessage, 0, 0, A_UNDERLINE);
    CurrentDirWindow.print(currentDir);
    CurrentDirWindow.refresh();

    std::vector<size_t> ignoreBlocks = {attributePosition, 0, attributePosition,
                                        hiddenInfo.length()};
    NavigationMenu.insert(hiddenInfo, attributePosition, 0);
    NavigationMenu.refresh();

    // NavigationMenu.display(breakConditions, ignoreBlocks);

    int selection;
    int currentPage;
    NavigationMenu.resetHighlighted();
    NavigationMenu.setKeypad(true);

    // initial LSVIEW window
    if (_LIST_VIEW_ENABLED) {
      auto listViewPath = children[NavigationMenu.highlighted()];
      if (pathController.pathType(listViewPath) == "directory") {
        try {
          displayListView(listViewPath);
        } catch (std::filesystem::filesystem_error &e) {
          _LSVIEW->eraseWin();
          _LSVIEW->print(e.what());
          _LSVIEW->refresh();
        }
      } else {
        _LSVIEW->eraseWin();
        _LSVIEW->print("not a directory!");
        _LSVIEW->refresh();
      }
    }

    while (true) {

      NavigationMenu.loadFields();
      selection = NavigationMenu.getCharFromUser(); // calls refresh implicitly
      currentPage = NavigationMenu.page();
      switch (selection) {
      case KEY_LEFT:
        if (currentPage != 0) {      // page()
          NavigationMenu.backPage(); // backPage()
          NavigationMenu.eraseExcept(ignoreBlocks);
        }
        break;
      case KEY_RIGHT:
        if (currentPage !=
            NavigationMenu.numPages() - 1) { // page() numPartitions()
          NavigationMenu.forwardPage();
          NavigationMenu.eraseExcept(ignoreBlocks);
        }
        break;
      case KEY_UP:
        if (NavigationMenu.highlighted() != 0) {
          NavigationMenu.moveHighlightUp();
          if (_LIST_VIEW_ENABLED) {
            auto listViewPath = children[NavigationMenu.highlighted()];
            if (pathController.pathType(listViewPath) == "directory") {
              try {
                displayListView(listViewPath);
              } catch (std::filesystem::filesystem_error &e) {
                _LSVIEW->eraseWin();
                _LSVIEW->print(e.what());
                _LSVIEW->refresh();
              }
            } else {
              _LSVIEW->eraseWin();
              _LSVIEW->print("not a directory!");
              _LSVIEW->refresh();
            }
          }
        }
        break;
      case KEY_DOWN:
        if (NavigationMenu.highlighted() !=
            NavigationMenu.numFieldsThisPage() - 1) { // numFieldsThisPage()
          NavigationMenu.moveHighlightDown();         // moveHighlightUp()
          if (_LIST_VIEW_ENABLED) {
            auto listViewPath = children[NavigationMenu.highlighted()];
            if (pathController.pathType(listViewPath) == "directory") {
              try {
                displayListView(listViewPath);
              } catch (std::filesystem::filesystem_error &e) {
                _LSVIEW->eraseWin();
                _LSVIEW->print(e.what());
                _LSVIEW->refresh();
              }
            } else {
              _LSVIEW->eraseWin();
              _LSVIEW->print("not a directory!");
              _LSVIEW->refresh();
            }
          }
        }
        break;
      default:
        break;
      }

      bool exitStatus = 0;
      for (const int i : breakConditions) {
        if (selection == i) {
          exitStatus = 1;
        }
      }
      if (exitStatus) {
        break;
      }
    }

    if (selection == (int)'q' || selection == 27 /*ESC*/) {
      // user exited program
      NavigationMenu.eraseWin();
      CurrentDirWindow.eraseWin();
      NavigationMenu.refresh();
      CurrentDirWindow.refresh();
      NavigationMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
      CurrentDirWindow.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
      _DISPLAY->moveCursor(_CURSOR_Y, 0);
      curs_set(_CURSOR);
      _DISPLAY->refresh();

      if (_LIST_VIEW_ENABLED) {
        try {
          displayListView(parentPath);
        } catch (std::filesystem::filesystem_error &e) {
          _LSVIEW->eraseWin();
          _LSVIEW->print(e.what());
          _LSVIEW->refresh();
        }
      }

      return 0; // leave here
    } else if (selection == (int)'a' /*out of directory*/) {
      // user navigated up a directory
      parentPath = parentPath.parent_path();
      NavigationMenu.eraseWin();
      CurrentDirWindow.eraseWin();
    } else if (selection == (int)'d' /*into dir*/) {

      // user navigated into a child directory
      // get index pertaining to field chosen by user
      fieldIdx = NavigationMenu.selectedFieldIndex();

      // assign new path to chosen path in children,
      // mapped by fieldIdx.
      auto newPath = children[fieldIdx];

      // check the path is a directory
      // set parent to this path if true
      if (pathController.pathType(newPath) == "directory") {
        parentPath = newPath;
      }
      // clear window for next iteration
      NavigationMenu.eraseWin();
      CurrentDirWindow.eraseWin();
    } else if (selection == (int)'h') {
      // toggle hide files
      if (withHidden) {
        withHidden = 0;
      } else {
        withHidden = 1;
      }
      NavigationMenu.eraseWin();
    } else if (selection == (int)'e') {

      // exit at parent directory

      if (_LIST_VIEW_ENABLED) {
        try {
          displayListView(parentPath);
        } catch (std::filesystem::filesystem_error &e) {
          _LSVIEW->eraseWin();
          _LSVIEW->print(e.what());
          _LSVIEW->refresh();
        }
      }

      std::string parentDir = parentPath;
      _ARGV = {"cd", parentDir};
      _ARGC = 2;
      changeDir();
      NavigationMenu.eraseWin();
      CurrentDirWindow.eraseWin();
      NavigationMenu.refresh();
      CurrentDirWindow.refresh();
      NavigationMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
      CurrentDirWindow.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);

      curs_set(_CURSOR);
      break;
    } else if (selection == (int)'s') {

      NavigationMenu.eraseWin();
      CurrentDirWindow.eraseWin();
      NavigationMenu.refresh();
      CurrentDirWindow.refresh();
      NavigationMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
      CurrentDirWindow.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
      _ARGV = {"sc"};
      _ARGC = 1;
      shortcut();
      _DISPLAY->moveCursor(_CURSOR_Y, 0);
      _DISPLAY->refresh();
      return 0;
    } else {

      // last key pressed is enter
      fieldIdx = NavigationMenu.selectedFieldIndex();
      chosenPath = children[fieldIdx];
      auto chosenPathType = pathController.pathType(chosenPath);
      if (chosenPathType == "directory") {
        _ARGV = {"cd", chosenPath};
        _ARGC = 2;
        changeDir();
        NavigationMenu.eraseWin();
        CurrentDirWindow.eraseWin();
        NavigationMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
        CurrentDirWindow.setWin(
            BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
        break;
      } else if (chosenPathType == "file") {
        int result = openWithTextEditor(chosenPath);
        NavigationMenu.eraseWin();
        if (result == -1) {
          NavigationMenu.insert("no EDITOR environment variable is set.", 0, 0);
          NavigationMenu.refresh();
          NavigationMenu.pause();
        }
      }
    }
  }
  _DISPLAY->moveCursor(_CURSOR_Y, 0);
  curs_set(_CURSOR);
  _DISPLAY->refresh();
  return 0;
}
} // namespace Trinkets
} // namespace BlackOS
