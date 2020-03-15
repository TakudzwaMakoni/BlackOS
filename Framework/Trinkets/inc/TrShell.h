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

#include "../inc/Natives.h"
#include "../inc/SplashScreen.h"
#include "Screen.h"

#include <../../External/inc/pstream.h>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {

// thanks to Sarah Vessels for Pipe/Redirect:
// https://www.3till7.net/2008/11/29/c-shell-with-forks-and-pipes/index.html

int const MAX_ARGS = 1024;
static char const *CLEAR_SCREEN_ANSI = "\e[0;0H\e[2J";

enum class PipeRedirect { PIPE, REDIRECT, NEITHER };
enum ExitStatus { ERROR = -1, USER_EXIT };
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
void vectorToNullArray(std::vector<std::string> const &v, char **a);

class ScreenShell {

public:
  ScreenShell(Screen_sptr &display);

  // execute native commands
  int execute(std::vector<std::string> const &);

  void listChildrenWrapper(std::vector<std::string> const &);

  void logCursorPosition();

  size_t cursorY() const;

  size_t cursorX() const;

  // initialise the shell
  void initShell();

  // Given a string of user input, this will determine if the user wants to
  // quit the shell.
  void quit(std::string);

  // Given the number of arguments and an array of arguments, this will execute
  // those arguments.  The first argument in the array should be a command.
  void runCommand(std::vector<std::string> const &);

  // Reads input from the user into the given array and returns the number of
  // arguments taken in.
  int readArgs(std::vector<std::string> &);

  int initShellVariables();

  int initEnvironmentVariables();

  void displayPrompt();

  void configureShell(std::vector<std::string> const &);

  void configureShell(std::string const &, std::string const &);

  void setShellEnv(std::vector<std::string> const &);

  ~ScreenShell();

private:
  Screen_sptr _display;
  std::string _displayType;
  size_t _cursorY = 0;
  size_t _cursorX = 0;
  size_t _promptLen;

  // environment variables
  std::string _PATH;
  std::string _TERM;
  std::string _SHELL; // fallback shell
  std::string _HOME;

  // shell variables
  int _CURSOR = 2;
  std::string _CURSOR_COLOUR = "red";
  int _DELETE = -1;
  int _BACKGROUND = COLOR_BLACK;
  int _FOREGROUND = COLOR_WHITE;
  int _STD_BG = standardColours::BLACK;
  int _STD_FG = standardColours::WHITE;

  std::filesystem::path _CONFIG_FILE;
  std::filesystem::path _SHELL_ENV_FILE;
  std::filesystem::path _SHORTCUTS_FILE;

  bool _TTY_FLAG_FALLBACK = 0;
  bool _USING_COLOR_FLAG = 0;
};
} // namespace Trinkets
} // namespace BlackOS

#endif
