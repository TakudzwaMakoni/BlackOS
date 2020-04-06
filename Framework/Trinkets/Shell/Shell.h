#ifndef TRINKETS_TR_SHELL_H
#define TRINKETS_TR_SHELL_H

/**
 * Tr Shell
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

#include "../helpers/PathController.h"
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
#include <signal.h>
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

class Shell;

static char const *CLEAR_SCREEN_ANSI = "\e[0;0H\e[2J";

using Window_sptr = std::shared_ptr<DisplayKernel::Window>;
using Window_uptr = std::unique_ptr<DisplayKernel::Window>;

Window_sptr generateSharedWindow();
Window_uptr generateUniqueWindow();
std::vector<char *> nullTerminatedArgV(std::vector<std::string> const &v);

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

class Shell {

public:
  /// command type
  typedef int (Shell::*command)(void);
  /// command map
  typedef std::pair<std::string, command> pair;
  /// command map
  typedef std::map<std::string, command> command_map;

  Shell();

  /// execute native commands
  int execute();
  ///
  void logCursorPosition();
  ///
  size_t cursorY();
  ///
  size_t cursorX();
  /// load the shell
  void loadShell();
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
  ///
  void printToScreen(std::string const &, bool newlineAtBeginning = true);

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
  ///
  int listView();

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
  ///
  int cpos();
  ///
  int MOVE();

  ~Shell();

private:
  std::vector<std::string> splitString(std::string, std::string const &);
  ///
  std::vector<std::string> splitString(std::string, char const);
  ///
  void newLine(bool newlineAtBeginning = true);

  // constants
  int const _MAX_ARGS = 1024;
  int const _MAX_MEMORY_HISTORY = 50;

  // display object variables
  Window_sptr _DISPLAY;
  int _CURSOR_Y;
  int _CURSOR_X;
  size_t _PROMPT_LEN;
  size_t _TERM_SIZE_Y;
  size_t _TERM_SIZE_X;
  size_t _DISPLAY_SIZE_Y;
  size_t _DISPLAY_SIZE_X;
  struct termios _OLDT;
  struct termios _NEWT;
  bool _COLOUR_SUPPORT;
  std::vector<size_t> _IGNORE_BLOCKS;

  // environment variables
  std::string _PATH;
  std::string _TERM;
  std::string _SHELL;
  std::string _HOME;

  // shell config variables
  int _CURSOR = 2;
  int _BACKGROUND = COLOR_BLACK;
  int _FOREGROUND = COLOR_WHITE;
  int _STD_BG = standardColours::BLACK;
  int _STD_FG = standardColours::WHITE;
  std::string _CURSOR_COLOUR = "red";
  bool _SHOW_BORDER = 0;

  // default system colours / styles

  int _STYLE_ERROR;
  int _STYLE_INFO;
  int _STYLE_IMPORTANT = A_STANDOUT;

  // system variables
  std::filesystem::path _CONFIG_FILE;
  std::filesystem::path _SHELL_ENV_FILE;
  std::filesystem::path _SHORTCUTS_FILE;
  std::filesystem::path _HISTORY_FILE;
  std::string _LAST_COMMAND;
  std::string _TIME_OF_LAST_COMMAND;
  std::string _RESULT_OF_LAST_COMMAND;
  std::vector<std::string> _ARGV;
  std::deque<std::string> _COMMAND_HISTORY;
  int _ARGC;

  // screen attributes
  bool _LIST_VIEW_ENABLED = 0;
  bool _TTY_FLAG_FALLBACK = 0;
  bool _USING_COLOR_FLAG = 0;
  std::filesystem::path _CURRENT_DIR; // or should we only get this on call?

  // subwindows
  // TODO: option to get compiler settings to enable attributes?
  Window_uptr _LSVIEW; // update with cd command
  size_t _LSVIEW_SIZE_Y;
  size_t _LSVIEW_SIZE_X;
  size_t _LSVIEW_POS_Y = 0;
  size_t _LSVIEW_POS_X;

  // user input variables
  struct sigaction _SIGNAL_INT_HANDLER;
  int _DELETE = -1; // delete custom keymap

  /// internal methods
  void displayListView(std::filesystem::path &);

  command_map _COMMAND_MAP{
      pair("bell", &Shell::bell),
      pair("cd", &Shell::changeDir),
      pair("clear", &Shell::clearScreen),
      pair("configure", &Shell::configure),
      pair("config", &Shell::configure),
      pair("ls", &Shell::listChildren),
      pair("lsconfig", &Shell::listConfigVariables),
      pair("nd", &Shell::navigateDir),
      pair("ndir", &Shell::navigateDir),
      pair("rainbow", &Shell::rainbow),
      pair("sc", &Shell::shortcut),
      pair("shortcut", &Shell::shortcut),
      pair("memory", &Shell::printMemoryHistory),
      pair("lsview", &Shell::listView),
      pair("cpos", &Shell::cpos),
      pair("MOVE", &Shell::MOVE),
  };

  command_map _SHELL_CONFIG_MAP{
      pair("BG", &Shell::configBackgroundColour),
      pair("CURSOR", &Shell::configCursor),
      pair("CURSCOL", &Shell::configCursorColour),
      pair("DELETE", &Shell::configDeleteKey),
      pair("FG", &Shell::configForegroundColour),
      pair("THEME", &Shell::configTheme),
  };

  command_map _THEME_MAP{
      pair("invader", &Shell::configThemeInvader),
      pair("ire", &Shell::configThemeIre),
      pair("neptune", &Shell::configThemeNeptune),
      pair("classic", &Shell::configThemeClassic),
      pair("anticlassic", &Shell::configThemeAntiClassic),
      pair("thinkpad", &Shell::configThemeThinkPad),
      pair("system", &Shell::configThemeSystem),
      pair("ugly", &Shell::configThemeUgly),
  };
};

} // namespace Trinkets
} // namespace BlackOS

#endif
