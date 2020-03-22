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

#include <cctype>
#include <cerrno>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <deque>
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
#include <thread>
#include <unistd.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {

class ScreenShell;

static char const *CLEAR_SCREEN_ANSI = "\e[0;0H\e[2J";

/// command type
typedef int (ScreenShell::*command)(void);
/// command map
typedef std::pair<std::string, command> pair;
/// command map
typedef std::map<std::string, command> command_map;

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

enum UserInput { UP = -2, DOWN };
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
std::vector<char *> nullTerminatedArgV(std::vector<std::string> const &v);

class ScreenShell {

public:
  ScreenShell(Screen_sptr &display);

  /// execute native commands
  int execute();
  ///
  void logCursorPosition();
  ///
  size_t cursorY();
  ///
  size_t cursorX();
  /// initialise the shell
  void initShell();
  /// Reads input from the user into the given array and returns the number of
  /// arguments taken in.
  int readArgs();
  /// Given the number of arguments and an array of arguments, this will execute
  /// those arguments.  The first argument in the array should be a command.
  void runCommand();
  ///
  int initShellVariables();
  ///
  void resetArgs();
  ///
  void appendTofile(std::filesystem::path const &, std::string const &);
  ///
  void logResult();
  ///
  int rainbow();
  ///
  int printMemoryHistory();
  ///
  std::vector<std::string> argv() const;
  ///
  int initEnvironmentVariables();
  ///
  void displayPrompt();
  ///
  int configure(std::string const &, std::string const &);
  ///
  int openWithTextEditor(std::string const &);
  ///
  void splashScreen(std::vector<std::string> const &argv);

  /// Native commands

  ///
  int bell();
  ///
  int clearScreen();
  ///
  int listConfigVariables();
  ///
  int configure();
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

  /// configurations

  ///
  int configCursor();
  ///
  int configCursorColour();
  ///
  int configDeleteKey();
  ///
  int configBackgroundColour();
  ///
  int configForegroundColour();
  ///
  int configTheme();
  ///
  int configThemeInvader();
  ///
  int configThemeIre();
  ///
  int configThemeNeptune();
  ///
  int configThemeClassic();
  ///
  int configThemeThinkPad();
  ///
  int configThemeAntiClassic();
  ///
  int configThemeUgly();
  ///
  int configThemeSystem();

  ~ScreenShell();

private:
  // constants
  int const _MAX_ARGS = 1024;
  int const _MAX_MEMORY_HISTORY = 50;

  // display variables
  Screen_sptr _DISPLAY;
  size_t _CURSOR_Y = 0;
  size_t _CURSOR_X = 0;
  size_t _PROMPT_LEN;
  size_t _TERM_SIZE_Y;
  size_t _TERM_SIZE_X;
  struct termios _OLDT;
  struct termios _NEWT;

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
  std::filesystem::path _CONFIG_FILE;
  std::filesystem::path _SHELL_ENV_FILE;
  std::filesystem::path _SHORTCUTS_FILE;
  std::filesystem::path _HISTORY_FILE;
  bool _TTY_FLAG_FALLBACK = 0;
  bool _USING_COLOR_FLAG = 0;
  bool _COLOUR_SUPPORT;
  std::string _LAST_COMMAND;
  std::string _TIME_OF_LAST_COMMAND;
  std::string _RESULT_OF_LAST_COMMAND;
  std::vector<std::string> _ARGV;
  std::deque<std::string> _COMMAND_HISTORY;
  int _ARGC;
  command_map _COMMAND_MAP{
      pair("bell", &ScreenShell::bell),
      pair("cd", &ScreenShell::changeDir),
      pair("clear", &ScreenShell::clearScreen),
      pair("configure", &ScreenShell::configure),
      pair("config", &ScreenShell::configure),
      pair("ls", &ScreenShell::listChildren),
      pair("lsconfig", &ScreenShell::listConfigVariables),
      pair("nd", &ScreenShell::navigateDir),
      pair("ndir", &ScreenShell::navigateDir),
      pair("rainbow", &ScreenShell::rainbow),
      pair("sc", &ScreenShell::shortcut),
      pair("shortcut", &ScreenShell::shortcut),
      pair("memory", &ScreenShell::printMemoryHistory),
  };

  command_map _SHELL_CONFIG_MAP{
      pair("BG", &ScreenShell::configBackgroundColour),
      pair("CURSOR", &ScreenShell::configCursor),
      pair("CURSCOL", &ScreenShell::configCursorColour),
      pair("DELETE", &ScreenShell::configDeleteKey),
      pair("FG", &ScreenShell::configForegroundColour),
      pair("THEME", &ScreenShell::configTheme)};

  command_map _THEME_MAP{
      pair("invader", &ScreenShell::configThemeInvader),
      pair("ire", &ScreenShell::configThemeIre),
      pair("neptune", &ScreenShell::configThemeNeptune),
      pair("classic", &ScreenShell::configThemeClassic),
      pair("anticlassic", &ScreenShell::configThemeAntiClassic),
      pair("thinkpad", &ScreenShell::configThemeThinkPad),
      pair("system", &ScreenShell::configThemeSystem),
      pair("ugly", &ScreenShell::configThemeUgly)};
};

} // namespace Trinkets
} // namespace BlackOS

#endif
