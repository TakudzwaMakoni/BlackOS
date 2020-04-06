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

#include "../Shell.h"
#include "Menu.h"
#include "Window.h"

#include <fmt/format.h>

namespace BlackOS {
namespace Trinkets {

int Shell::shortcut() {

  _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);

  int y, x;

  if (_CURSOR_Y < 0 || _CURSOR_Y > _TERM_SIZE_Y - 10)
    y = _TERM_SIZE_Y / 2;
  else
    y = _CURSOR_Y;

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
  BlackOS::DisplayKernel::Menu ShortcutMenu(_TERM_SIZE_Y - y, _TERM_SIZE_Y, y,
                                            0);

  // load shortcuts
  int maxNameLen = 0;
  int maxDirLen = 0;

  std::ifstream infile(_SHORTCUTS_FILE);
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
  ShortcutMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::INIT_CHILD);

  if (_USING_COLOR_FLAG) {
    ShortcutMenu.bgfg(_FOREGROUND, _BACKGROUND);
  }

  if (fieldSz == 0) {
    std::string message = "no entries to show.";
    ShortcutMenu.clear(); // clear previous output
    ShortcutMenu.insert(message.c_str(), 0, 0);
    ShortcutMenu.refresh(); // present message to screen
    ShortcutMenu.pause();
    ShortcutMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
    _DISPLAY->moveCursor(_CURSOR_Y, 0);
    _DISPLAY->refresh();
    return 0;
  }

  menuWidth = maxDirLen + maxNameLen + 1;
  menuHeight = _TERM_SIZE_Y - y - 2;
  pagination = menuHeight - 1;

  ShortcutMenu.loadTitle("title", A_UNDERLINE);
  ShortcutMenu.initFields(fields);
  ShortcutMenu.loadFieldAlignment(-1, 1);
  ShortcutMenu.paginate(pagination, pagination <= fieldSz);

  ShortcutMenu.hideBorder();
  ShortcutMenu.showTitle();

  ShortcutMenu.resize(menuHeight, menuWidth);
  ShortcutMenu.reposition(y /*maintain cursor y position*/,
                          0 /*left of screen*/);

  std::vector<int> breakConditions = {(int)'d', (int)'q', 10 /*ENTER*/,
                                      27 /*ESC*/};
  ShortcutMenu.display(breakConditions);

  int lastKey = ShortcutMenu.lastKeyPressed();
  size_t selected = ShortcutMenu.selectedFieldIndex();

  if (lastKey == (int)'q' || lastKey == 27 /*ESC*/) {
    // user exited program
    ShortcutMenu.clear();
    ShortcutMenu.refresh();
    ShortcutMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
    _DISPLAY->moveCursor(_CURSOR_Y, 0);
    _DISPLAY->refresh();
    return 0; // leave here
  } else if (lastKey == (int)'d' /*begin navigateDir into dir*/) {
    // user navigated up a directory
    ShortcutMenu.clear(); // clear previous output
    ShortcutMenu.refresh();
    ShortcutMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
    std::string chosenPath = directories[selected];
    _ARGV = {"nd", chosenPath};
    _ARGC = 2;
    navigateDir();
    _DISPLAY->moveCursor(_CURSOR_Y, 0);
    _DISPLAY->refresh();
    return 0;
  } else {
    // enter was pressed
    ShortcutMenu.clear(); // clear previous output
    ShortcutMenu.refresh();
    ShortcutMenu.setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
    std::string chosenPath = directories[selected];
    _ARGV = {"cd", chosenPath};
    _ARGC = 2;
    changeDir();
    _DISPLAY->moveCursor(_CURSOR_Y, 0);
    _DISPLAY->refresh();
    return 0;
  }
}
} // namespace Trinkets
} // namespace BlackOS
