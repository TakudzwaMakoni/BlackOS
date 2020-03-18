#ifndef TRINKETS_TR_SHELL_H
#define TRINKETS_TR_SHELL_H

/**
 * Tr ScreenShell
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

#include "Screen.h"
#include "Window.h"

//#include <../../External/inc/pstream.h>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {

class ScreenShell;

int const MAX_ARGS = 1024;
static char const *CLEAR_SCREEN_ANSI = "\e[0;0H\e[2J";

/// command type
typedef int (ScreenShell::*command)(void);
/// command map
typedef std::pair<std::string, command> pair;
/// command map
typedef std::map<std::string, command> cmap;

enum class PipeRedirect { PIPE, REDIRECT, NEITHER };
enum RESULT {
  LAUNCH_FAILURE, // program failed to launch
  ABORT,          // call to abort program
  EXIT,           // user call to end program
  SUCCESS = 0,    // return function succeeded
  FAILURE,        // return function failed
  BAD_USAGE,      // return bad user input
  NO_INPUT,       // no user input
  END_OF_PROCESS  // process has ended
};
enum class USER_INPUT { UP = -2, DOWN }; // TODO: LEFT RIGHT ESC ETC.
enum standardColours {
  BLACK = 30,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
  BBLACK = 90,
  BRED,
  BGREEN,
  BYELLOW,
  BBLUE,
  BMAGENTA,
  BCYAN,
  BWHITE
};

using Screen_sptr = std::shared_ptr<DisplayKernel::Screen>;
using Window_sptr = std::shared_ptr<DisplayKernel::Window>;

Screen_sptr generateScreen();
Window_sptr generateWindow();
std::vector<char *> nullTerminatedArgV(std::vector<std::string> const &);
void vectorToNullArray(std::vector<std::string> const &v, char **a);

class ScreenShell {

public:
  ScreenShell(Screen_sptr &display);

  /// execute native commands
  int execute();
  ///
  void logCursorPosition();
  ///
  size_t cursorY() const;
  ///
  size_t cursorX() const;
  /// initialise the shell
  void initShell();
  /// Reads input from the user into the given array and returns the number of
  /// arguments taken in.
  void readArgs();
  /// Given the number of arguments and an array of arguments, this will execute
  /// those arguments.  The first argument in the array should be a command.
  void runCommand();
  ///
  int initShellVariables();
  ///
  void resetArgs();
  ///
  std::vector<std::string> argv() const;
  ///
  int initEnvironmentVariables();
  ///
  void displayPrompt();
  ///
  void moveCursor(int y, int x);
  ///
  int configureShell(std::string const &, std::string const &);
  ///
  int openWithTextEditor(std::string const &);
  ///
  void splashScreen(std::vector<std::string> const &argv);
  ///
  int getCharacter();

  /// Native commands

  ///
  int bell();
  ///
  int exitShell();
  ///
  int clearScreen();
  ///
  int listConfigVariables();
  ///
  int configureShell();
  ///
  int setShellEnv();
  ///
  int changeDir();
  ///
  int listChildren();
  ///
  int navigateDir();
  ///
  int shortcut();

  ~ScreenShell();

private:
  // display variables
  Screen_sptr _display;
  std::string _displayType;
  int _promptLen;
  int _termSzY;
  int _termSzX;
  int _cursorY;
  int _cursorX;

  // environment variables
  std::string _PATH;
  std::string _TERM;
  std::string _SHELL;
  std::string _HOME;

  // shell config variables
  int _CURSOR = 2;
  int _DELETE = -1;
  int _BACKGROUND = COLOR_BLACK;
  int _FOREGROUND = COLOR_WHITE;
  int _STD_BG = standardColours::BLACK;
  int _STD_FG = standardColours::WHITE;
  std::string _CURSOR_COLOUR = "red";

  // system variables
  struct termios _OLDT;
  struct termios _NEWT;
  std::filesystem::path _CONFIG_FILE;
  std::filesystem::path _SHELL_ENV_FILE;
  std::filesystem::path _SHORTCUTS_FILE;
  bool _TTY_FLAG_FALLBACK = 0;
  bool _USING_COLOR_FLAG = 0;
  bool _COLOUR_SUPPORT;
  std::vector<std::string> _ARGV;
  int _ARGC;
  cmap _COMMAND_MAP{pair("bell", &ScreenShell::bell),
                    pair("cd", &ScreenShell::changeDir),
                    pair("clear", &ScreenShell::clearScreen),
                    pair("configure", &ScreenShell::configureShell),
                    pair("config", &ScreenShell::configureShell),
                    pair("exit", &ScreenShell::exitShell),
                    pair("quit", &ScreenShell::exitShell),
                    pair("ls", &ScreenShell::listChildren),
                    pair("lsconfig", &ScreenShell::listConfigVariables),
                    pair("nd", &ScreenShell::navigateDir),
                    pair("ndir", &ScreenShell::navigateDir),
                    pair("sc", &ScreenShell::shortcut),
                    pair("shortcut", &ScreenShell::shortcut)};
};

} // namespace Trinkets
} // namespace BlackOS

#endif
