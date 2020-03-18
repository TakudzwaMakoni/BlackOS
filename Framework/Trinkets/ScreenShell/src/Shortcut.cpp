/**
 * Tr(inkets) ScreenShell NavigateDir
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

#include "../ScreenShell.h"
#include "Menu.h"
#include "Window.h"

#include <fmt/format.h>

namespace BlackOS {
namespace Trinkets {

int ScreenShell::shortcut() {

  int y, x;

  if (_cursorY < 0 || _cursorY > _termSzY - 10)
    y = _termSzY / 2;
  else
    y = _cursorY;

  clearScreen();

  std::string chosenPath;
  size_t fieldIdx;
  size_t fieldSz;

  std::string title;
  size_t menuWidth;
  size_t menuHeight;
  size_t pagination;

  std::string hiddenAttribute = "showing hidden paths: ";

  // create menu object
  BlackOS::DisplayKernel::Menu ShortcutMenu(_termSzY - y, _termSzX, y, 0);

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
  ShortcutMenu.setWin(1);

  if (_USING_COLOR_FLAG) {
    ShortcutMenu.bgfg(_FOREGROUND, _BACKGROUND);
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
  menuHeight = _termSzY - y - 2;
  pagination = menuHeight - 1;

  ShortcutMenu.loadTitle(title, BlackOS::DisplayKernel::TextStyle::underline);
  ShortcutMenu.loadFields(fields);
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
    ShortcutMenu.setWin(0);

    return 0; // leave here
  } else if (lastKey == (int)'d' /*begin navigateDir into dir*/) {
    // user navigated up a directory
    ShortcutMenu.clear(); // clear previous output
    ShortcutMenu.refresh();
    ShortcutMenu.setWin(0);
    std::string chosenPath = directories[selected];
    _ARGV = {"nd", chosenPath};
    _ARGC = 2;
    navigateDir();
    return 0;
  } else {
    // enter was pressed
    ShortcutMenu.clear(); // clear previous output
    ShortcutMenu.refresh();
    ShortcutMenu.setWin(0);
    std::string chosenPath = directories[selected];
    _ARGV = {"cd", chosenPath};
    _ARGC = 2;
    changeDir();
    return 0;
  }
}
} // namespace Trinkets
} // namespace BlackOS
