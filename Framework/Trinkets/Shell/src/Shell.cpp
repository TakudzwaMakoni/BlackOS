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

#include "../Shell.h"

namespace BlackOS {
namespace Trinkets {

/// generates a shared pointer to DisplayKernel Screen instance.
Window_sptr generateSharedWindow() {
  auto win = std::make_shared<DisplayKernel::Window>(0, 0, 0, 0);
  return win;
}

/// generates a shared pointer to DisplayKernel Window instance.
Window_uptr generateUniqueWindow() {
  auto window = std::make_unique<DisplayKernel::Window>(0, 0, 0, 0);
  return window;
}

std::vector<char *> nullTerminatedArgV(std::vector<std::string> const &v) {
  std::vector<char *> result;
  for (auto const &s : v) {
    result.push_back(const_cast<char *>(s.c_str()));
  }
  result.push_back(nullptr);
  return result;
}

/// executes the visual bell in screen
int Shell::bell() {

  start_color();
  _DISPLAY->bgfg(COLOR_BLACK, _BACKGROUND);
  _DISPLAY->refresh();
  usleep(50000);
  _DISPLAY->bgfg(COLOR_WHITE, _BACKGROUND);
  _DISPLAY->refresh();
  usleep(50000);
  _DISPLAY->bgfg(COLOR_RED, _BACKGROUND);
  _DISPLAY->refresh();
  usleep(50000);
  _DISPLAY->bgfg(COLOR_GREEN, _BACKGROUND);
  _DISPLAY->refresh();
  usleep(50000);
  _DISPLAY->bgfg(COLOR_YELLOW, _BACKGROUND);
  _DISPLAY->refresh();
  usleep(50000);
  _DISPLAY->bgfg(COLOR_BLUE, _BACKGROUND);
  _DISPLAY->refresh();
  usleep(50000);
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _DISPLAY->refresh();

  return 0;
}

int Shell::configTheme() {
  if (_ARGC != 3) {
    _DISPLAY->print("not enough arguments!\n");
    return 1;
  }
  std::string theme = _ARGV[2];

  command_map::const_iterator x;
  x = _THEME_MAP.find(theme);

  if (x != _THEME_MAP.end()) {
    (this->*(x->second))(); // call using pointer
    return 0;
  } else {
    std::string message = "theme " + theme + " is unrecognised.";
    _DISPLAY->print(message.c_str());
    _DISPLAY->newLine();
    _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
    return 1;
  }
  return 0;
}

int Shell::configThemeInvader() {
  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_GREEN;
  _BACKGROUND = COLOR_BLACK;
  _CURSOR_COLOUR = "green";

  _STD_FG = standardColours::GREEN;
  _STD_BG = standardColours::BLACK;

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  start_color();
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _USING_COLOR_FLAG = 1;
  _DISPLAY->refresh();

  return 0;
}

int Shell::configThemeIre() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_RED;
  _BACKGROUND = COLOR_BLACK;
  _CURSOR_COLOUR = "red";

  _STD_FG = standardColours::RED;
  _STD_BG = standardColours::BLACK;

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  start_color();
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _USING_COLOR_FLAG = 1;
  _DISPLAY->refresh();

  return 0;
}

int Shell::configThemeClassic() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_WHITE;
  _BACKGROUND = COLOR_BLACK;
  _CURSOR_COLOUR = "white";

  _STD_FG = standardColours::WHITE;
  _STD_BG = standardColours::BLACK;

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  start_color();
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _USING_COLOR_FLAG = 1;
  _DISPLAY->refresh();

  return 0;
}

int Shell::configThemeThinkPad() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_WHITE;
  _BACKGROUND = COLOR_BLACK;
  _CURSOR_COLOUR = "red";

  _STD_FG = standardColours::WHITE;
  _STD_BG = standardColours::BLACK;

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  start_color();
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _USING_COLOR_FLAG = 1;
  _DISPLAY->refresh();

  return 0;
}

int Shell::configThemeAntiClassic() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_BLACK;
  _BACKGROUND = COLOR_WHITE;
  _CURSOR_COLOUR = "black";

  _STD_FG = standardColours::BLACK;
  _STD_BG = standardColours::WHITE;

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  start_color();
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _USING_COLOR_FLAG = 1;
  _DISPLAY->refresh();

  return 0;
}

int Shell::configThemeNeptune() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_WHITE;
  _BACKGROUND = COLOR_BLUE;
  _CURSOR_COLOUR = "white";

  _STD_FG = standardColours::WHITE;
  _STD_BG = standardColours::BLUE;

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  start_color();
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _USING_COLOR_FLAG = 1;
  _DISPLAY->refresh();

  return 0;
}

int Shell::configThemeSystem() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = -1;
  _BACKGROUND = -1;
  _CURSOR_COLOUR = "white";

  _STD_FG = standardColours::WHITE;
  _STD_BG = standardColours::BLACK;

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  bkgd(COLOR_PAIR(0));

  _USING_COLOR_FLAG = 0;

  _DISPLAY->refresh();

  return 0;
}

int Shell::configThemeUgly() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_BLACK;
  _BACKGROUND = COLOR_YELLOW;
  _CURSOR_COLOUR = "black";

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  start_color();
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _USING_COLOR_FLAG = 1;
  _DISPLAY->refresh();

  return 0;
}

/// alternate colours in screen
int Shell::rainbow() {
  auto start = std::chrono::system_clock::now();
  auto end = std::chrono::system_clock::now();
  start_color();
  while (
      (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() !=
       1)) {
    end = std::chrono::system_clock::now();
    _DISPLAY->bgfg(COLOR_BLACK, _BACKGROUND);
    _DISPLAY->refresh();
    usleep(50000);
    _DISPLAY->bgfg(COLOR_WHITE, _BACKGROUND);
    _DISPLAY->refresh();
    usleep(50000);
    _DISPLAY->bgfg(COLOR_RED, _BACKGROUND);
    _DISPLAY->refresh();
    usleep(50000);
    _DISPLAY->bgfg(COLOR_GREEN, _BACKGROUND);
    _DISPLAY->refresh();
    usleep(50000);
    _DISPLAY->bgfg(COLOR_YELLOW, _BACKGROUND);
    _DISPLAY->refresh();
    usleep(50000);
    _DISPLAY->bgfg(COLOR_BLUE, _BACKGROUND);
    _DISPLAY->refresh();
  }
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);

  return 0;
}

/// append data to file.
void Shell::appendTofile(std::filesystem::path const &path,
                         std::string const &data) {
  std::ofstream outfile;
  outfile.open(path, std::ios_base::app);
  outfile << data + '\n';
  outfile.close();
}

/// load the shell to screen
void Shell::loadShell() {
  _DISPLAY->setWin(BlackOS::DisplayKernel::WIN_SET_CODE::INIT_PARENT);
  // load default colour scheme
  noraw();
  cbreak();
  noecho();

  if (_COLOUR_SUPPORT) {
    // 2 reserved for error
    init_pair(2, COLOR_RED, COLOR_WHITE);
    // 3 reserved for info
    init_pair(3, COLOR_BLUE, COLOR_BLACK);

    _STYLE_ERROR = COLOR_PAIR(2);
    _STYLE_INFO = COLOR_PAIR(3);
    _STYLE_IMPORTANT = A_STANDOUT;
  }

  _DISPLAY->resize(_DISPLAY_SIZE_Y, _DISPLAY_SIZE_X);
  _DISPLAY->reposition(0, 0);

  // TODO: not make this default and add as config command
  //_SHOW_BORDER = 1;
  _DISPLAY->hideBorder();

  noecho();
  // noraw();
  cbreak();
  curs_set(_CURSOR);

  std::string title = "Tr(inkets) (C) Shell";
  std::string version = "1.0";
  std::string repo = "Operation Dark Horse";
  std::string license = "GPL v3.0";
  std::string year = "2020";
  std::string language = "C++";
  std::string author = "Takudzwa Makoni";
  std::string git = "https://github.com/TakudzwaMakoni";

  std::vector<std::string> v{title, version,  repo,   license,
                             year,  language, author, git};
  splashScreen(v);

  // check colour support
  _COLOUR_SUPPORT = has_colors();
  if (_COLOUR_SUPPORT == FALSE) {
    _DISPLAY->print("warning: this terminal does not support colour\n");
    _DISPLAY->newLine();
  }

  // initialise shell with config settings and shell variables described in
  // config and environment files.
  initShellVariables();
  resetArgs();

  initEnvironmentVariables();
  resetArgs();

  // recommended environment variables
  char *editor = getenv("EDITOR");
  if (editor == nullptr) {
    _DISPLAY->print("warning: no EDITOR environment variable is set.");
    _DISPLAY->newLine();
  }

  changeDir();
  _DISPLAY->refresh();
  bell();
}

/// load shell configuration variables from config.txt file
int Shell::initShellVariables() {
  std::ifstream infile(_CONFIG_FILE);
  std::string a, b;
  while (infile >> a >> b) {
    if (a == "!" || a.empty())
      continue;
    configure(a, b);
  }
  return 0;
}

/// open text file with EDITOR environment variable
int Shell::openWithTextEditor(std::string const &path) {
  char *editor = getenv("EDITOR");
  if (editor == NULL)
    return -1;
  std::string command = editor;
  command += " " + path;
  system(command.c_str());
  return 0;
}

/// load environment variables from environment.txt file
int Shell::initEnvironmentVariables() {
  std::ifstream infile(_SHELL_ENV_FILE);
  std::string a, b;
  while (infile >> a >> b) {
    if (a == "!" || a.empty())
      continue;
    setenv(a.c_str(), b.c_str(), 1);
  }
  return 0;
}

/// log last command and result to memory cache
void Shell::logResult() {

  // log last command to history file
  appendTofile(_HISTORY_FILE, _LAST_COMMAND);

  if (_COMMAND_HISTORY.size() == _MAX_MEMORY_HISTORY - 1)
    _COMMAND_HISTORY.pop_front();

  // add to virtual history buffer;
  if (!_LAST_COMMAND.empty() && _LAST_COMMAND != "memory")
    _COMMAND_HISTORY.push_front(_LAST_COMMAND);
}

/// print the prompt to the screen
void Shell::displayPrompt() {
  std::string currentDir = _CURRENT_DIR;
  std::string prompt = "Tr " + currentDir + "> ";
  _PROMPT_LEN = prompt.length();
  _DISPLAY->print(prompt.c_str());
  _DISPLAY->refresh();
}

/// return cursor y position
size_t Shell::cursorY() {
  _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
  return _CURSOR_Y;
}

/// return cursor x position
size_t Shell::cursorX() {
  _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
  return _CURSOR_X;
}

/// update log entry for cursor position in Screen
void Shell::logCursorPosition() {
  _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
}

/// apply configurations to Shell
int Shell::configure() {

  std::string argv1 = _ARGV[1];

  command_map::const_iterator x;
  x = _SHELL_CONFIG_MAP.find(argv1);

  if (x != _SHELL_CONFIG_MAP.end()) {
    (this->*(x->second))(); // call using pointer
    return 0;
  } else {
    std::string message = "Shell variable " + argv1 + " is unrecognised.";
    _DISPLAY->print(message.c_str());
    _DISPLAY->newLine();
    _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
    return 1;
  }
  return 0;
}

std::vector<std::string> Shell::splitString(std::string str,
                                            std::string const &delimiter) {
  size_t pos = 0;
  std::string token;
  std::vector<std::string> words;
  while ((pos = str.find(delimiter)) != std::string::npos) {
    token = str.substr(0, pos);
    words.push_back(token);
    str = str.substr(pos + delimiter.length());
  }
  words.push_back(str);
  return words;
}

std::vector<std::string> Shell::splitString(std::string str,
                                            char const delimiter) {
  std::string delimStr;
  delimStr += delimiter;
  auto const words = splitString(str, delimStr);
  return words;
}

void Shell::printToScreen(std::string const &str, bool newlineAtBeginning) {
  _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
  auto const &words = splitString(str, "\n");
  for (auto const &word : words) {
    _DISPLAY->print(word.c_str());
    _DISPLAY->newLine(newlineAtBeginning);
  }
}

int Shell::configure(std::string const &argv1, std::string const &argv2) {

  // borrow _ARGV
  resetArgs();
  _ARGV.push_back("configure");
  _ARGV.push_back(argv1);
  _ARGV.push_back(argv2);
  _ARGC = 3;

  command_map::const_iterator x;
  x = _SHELL_CONFIG_MAP.find(argv1);

  if (x != _SHELL_CONFIG_MAP.end()) {
    (this->*(x->second))(); // call using pointer
    return 0;
  } else {
    std::string message = "Shell variable " + argv1 + " is unrecognised.";
    _DISPLAY->print(message.c_str());
    _DISPLAY->newLine();
    _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
    return 1;
  }

  resetArgs();
  return 0;
}

int Shell::configCursor() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  std::string argv1 = _ARGV[1];
  std::string argv2 = _ARGV[2];

  int value;
  std::string errorMessage =
      "could not assign cursor to this value: " + argv2 +
      "\n2nd argument must be in range: 0 <= [arg2] <= 2";
  try {
    value = std::stoi(argv2);
  } catch (...) {

    _DISPLAY->print(errorMessage.c_str());
    _DISPLAY->newLine();
    _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
    return 1;
  }
  if (value < 0 || value > 2) {

    _DISPLAY->print(errorMessage.c_str());
    _DISPLAY->newLine();
    _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
    return 1;
  }

  _CURSOR = value;
  curs_set(_CURSOR); // TODO: extra arg to save preference to config file?
  return 0;
}

int Shell::configCursorColour() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  std::string argv1 = _ARGV[1];
  std::string argv2 = _ARGV[2];

  if (!(argv2 == "red" || argv2 == "black" || argv2 == "white" ||
        argv2 == "blue" || argv2 == "green" || argv2 == "yellow")) {

    std::string message = "2nd argument " + argv2 + " is not a valid colour.";

    _DISPLAY->print(message.c_str());
    _DISPLAY->newLine();
    return 1;
  }

  // TODO: check if Background is the same colour and warn invisibility

  printf("\e]12;%s\a", argv2.c_str());
  fflush(stdout);
  return 0;
}

int Shell::configDeleteKey() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  std::string argv1 = _ARGV[1];
  std::string argv2 = _ARGV[2];

  // linux TERM environment variable does not seem to read regular backspace.
  // allow the user to specify which key is delete.
  std::string errorMessage =
      "could not assign delete key to this value: " + argv2 +
      "\nexpected a single character, or number in range " +
      "0-127, or 'unset' to unset.";

  if (argv2 == "unset") {
    _DELETE = -1;
    return 1;
  }

  try {

    int value = std::stoi(argv2);

    if (value > 127 || value < 0) {
      std::string errorMessage =
          "could not assign delete key to this value: " +
          std::to_string(value) +
          "\n2nd argument must be in range: 0 < [arg2] < 127";
      _DISPLAY->print(errorMessage.c_str());
      _DISPLAY->newLine();
      return 1;
    }

    if (value == 10 /*Enter*/ || value == 35 /*Space*/) {
      // may need to specify more reserved values.
      _DISPLAY->print("this key is reserved.");
      _DISPLAY->newLine();
      return 1;
    }
    _DELETE = value;
  } catch (...) {
    // parse character

    if (argv2.length() != 1) {
      _DISPLAY->print(errorMessage.c_str());
      _DISPLAY->newLine();
      return 1;
    }

    char c = argv2[0];
    if (std::isprint(static_cast<unsigned char>(c))) {
      _DELETE = (int)c;
    } else {
      _DISPLAY->print("2nd argument is not a printable character.");
      _DISPLAY->newLine();
    }
  }
  return 0;
}

int Shell::configBackgroundColour() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  std::string argv1 = _ARGV[1];
  std::string argv2 = _ARGV[2];

  if (!(argv2 == "red" || argv2 == "black" || argv2 == "white" ||
        argv2 == "blue" || argv2 == "green" || argv2 == "yellow")) {
    std::string message = "2nd argument " + argv2 + " is not a valid colour.";
    _DISPLAY->print(message.c_str());
    _DISPLAY->newLine();
    return 1;
  }

  if (argv2 == "black") {
    _BACKGROUND = COLOR_BLACK;
    _STD_BG = standardColours::BLACK;
  }
  if (argv2 == "white") {
    _BACKGROUND = COLOR_WHITE;
    _STD_BG = standardColours::WHITE;
  }
  if (argv2 == "red") {
    _BACKGROUND = COLOR_RED;
    _STD_BG = standardColours::RED;
  }
  if (argv2 == "blue") {
    _BACKGROUND = COLOR_BLUE;
    _STD_BG = standardColours::BLUE;
  }
  if (argv2 == "yellow") {
    _BACKGROUND = COLOR_YELLOW;
    _STD_BG = standardColours::YELLOW;
  }
  if (argv2 == "green") {
    _BACKGROUND = COLOR_GREEN;
    _STD_BG = standardColours::GREEN;
  }

  // TODO: check if Foreground is the same colour and warn invisibility/deny
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _DISPLAY->refresh();

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  _USING_COLOR_FLAG = 1;
  return 0;
}

int Shell::configForegroundColour() {

  if (_ARGC != 3) {
    _DISPLAY->print("Usage:\n"
                    "config/configure <arg1> <arg2>\n"
                    "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }
  std::string argv1 = _ARGV[1];
  std::string argv2 = _ARGV[2];

  if (!(argv2 == "red" || argv2 == "black" || argv2 == "white" ||
        argv2 == "blue" || argv2 == "green" || argv2 == "yellow")) {
    std::string message = "2nd argument " + argv2 + " is not a valid colour.";
    _DISPLAY->print(message.c_str());
    _DISPLAY->newLine();
    return 1;
  }

  if (argv2 == "black") {
    _FOREGROUND = COLOR_BLACK;
    _STD_FG = standardColours::BLACK;
  }
  if (argv2 == "white") {
    _FOREGROUND = COLOR_WHITE;
    _STD_FG = standardColours::WHITE;
  }
  if (argv2 == "red") {
    _FOREGROUND = COLOR_RED;
    _STD_FG = standardColours::RED;
  }
  if (argv2 == "blue") {
    _FOREGROUND = COLOR_BLUE;
    _STD_FG = standardColours::BLUE;
  }
  if (argv2 == "yellow") {
    _FOREGROUND = COLOR_YELLOW;
    _STD_FG = standardColours::YELLOW;
  }
  if (argv2 == "green") {
    _FOREGROUND = COLOR_GREEN;
    _STD_FG = standardColours::GREEN;
  }

  start_color();
  // TODO: check if Foreground is the same colour and warn invisibility/deny
  _DISPLAY->bgfg(_FOREGROUND, _BACKGROUND);
  _DISPLAY->refresh();

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  _USING_COLOR_FLAG = 1;
  return 0;
}

/// reset _ARGV and _ARGC
void Shell::resetArgs() {
  _ARGV = {};
  _ARGC = 0;
}

/// read in user arguments
int Shell::readArgs() {
  noecho();
  std::string line;
  std::string userLine;

  char ch;
  int memorySz = _COMMAND_HISTORY.size();
  int historyCounter = 0;

  do {
    ch = _DISPLAY->getCharFromUser();
    if (ch == '\n') {
      if (line.empty())
        bell();
      else
        break;
    } else if ((_DELETE > 0 && (int)ch == _DELETE) || (int)ch == 8 ||
               (int)ch == KEY_BACKSPACE || (int)ch == KEY_DC ||
               (int)ch == 127) {
      _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
      if (_CURSOR_X > _PROMPT_LEN) {
        _DISPLAY->print("\b");
        _DISPLAY->deleteChar();
        _DISPLAY->refresh();
      }
      if (!line.empty()) {
        line.pop_back();
      }
    } else if ((int)ch == 27 || (int)ch == KEY_UP || (int)ch == 3) {
      if (!_COMMAND_HISTORY.empty()) {
        if (historyCounter + 1 < memorySz) {
          historyCounter++;
          _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
          std::string fromMemory = _COMMAND_HISTORY[historyCounter];
          _DISPLAY->erase(_CURSOR_Y, _PROMPT_LEN, _CURSOR_Y, _TERM_SIZE_X - 1);
          _DISPLAY->insert(fromMemory, _CURSOR_Y, _PROMPT_LEN);

          line = fromMemory;

          _DISPLAY->refresh();
        }
      }
    } else if ((int)ch == KEY_LEFT || (int)ch == 4) {
      _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
      if (_CURSOR_X > _PROMPT_LEN) {
        _DISPLAY->print("\b");
        _DISPLAY->refresh();
      }
    } else if ((int)ch == KEY_RIGHT || (int)ch == 5) {
      _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
      if (_CURSOR_X < _DISPLAY_SIZE_X) {
        move(_CURSOR_Y, _CURSOR_X + 1);
        _DISPLAY->refresh();
      }
    } else if ((int)ch == KEY_DOWN || (int)ch == 2) {
      if (!_COMMAND_HISTORY.empty()) {
        _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);

        if (historyCounter > 0) {
          historyCounter--;
          std::string fromMemory = _COMMAND_HISTORY[historyCounter];
          _DISPLAY->erase(_CURSOR_Y, _PROMPT_LEN, _CURSOR_Y, _TERM_SIZE_X - 1);
          _DISPLAY->insert(fromMemory, _CURSOR_Y, _PROMPT_LEN);

          line = fromMemory;

          _DISPLAY->refresh();
        } else if (historyCounter <= 0) {
          historyCounter = 0;
          _DISPLAY->erase(_CURSOR_Y, _PROMPT_LEN, _CURSOR_Y, _TERM_SIZE_X - 1);
          _DISPLAY->insert(userLine, _CURSOR_Y, _PROMPT_LEN);

          line = userLine;
          _DISPLAY->refresh();
        }
      }
    } else {
      _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
      if (_CURSOR_X < _DISPLAY_SIZE_X) {
        _DISPLAY->addChar(ch);
        _DISPLAY->refresh();
        line += ch;
        userLine = line;
      }
    }
  } while (1);

  // store last command entered.
  _LAST_COMMAND = line;

  // tokenize the string
  std::stringstream ss(line);
  std::string item;

  while (std::getline(ss, item, ' '))
    _ARGV.push_back(item);
  _ARGC = _ARGV.size();

  return 0;
}

void Shell::displayListView(std::filesystem::path const &dir) {

  _LSVIEW_SIZE_Y = _TERM_SIZE_Y;
  _LSVIEW_SIZE_X = _TERM_SIZE_X / 4;
  _LSVIEW_POS_X = _TERM_SIZE_X - _LSVIEW_SIZE_X;

  _DISPLAY_SIZE_X = _TERM_SIZE_X - _LSVIEW_SIZE_X;
  _DISPLAY->resize(_DISPLAY_SIZE_Y, _DISPLAY_SIZE_X);
  if (_SHOW_BORDER)
    _DISPLAY->borderStyle();

  _DISPLAY->refresh();

  if (!_LSVIEW->windowSet()) {
    _LSVIEW->setWin(BlackOS::DisplayKernel::WIN_SET_CODE::INIT_CHILD);
    _LSVIEW->resize(_LSVIEW_SIZE_Y, _LSVIEW_SIZE_X);
    _LSVIEW->reposition(_LSVIEW_POS_Y, _LSVIEW_POS_X);
  }
  _LSVIEW->bgfg(_FOREGROUND, _BACKGROUND);
  if (_SHOW_BORDER)
    _LSVIEW->borderStyle();

  PathController path;
  path.loadParent(dir);
  auto const children = path.children();
  int childrenSize = children.size();
  _LSVIEW->eraseWin();
  _LSVIEW->refresh();
  _LSVIEW->print("contents:", A_UNDERLINE);
  _LSVIEW->newLines(2);

  int height = _LSVIEW_SIZE_Y - 4;
  if (childrenSize > height) {
    int remaining = childrenSize - height;
    for (int i = 0; i < height; i++) {
      auto const &child = children[i];
      attr_t style;
      auto const type = path.pathType(child);
      if (type == "directory")
        style = A_BOLD;
      else if (type == "file")
        style = A_NORMAL;
      else
        style = A_DIM;
      _LSVIEW->print(child.filename(), style);
      _LSVIEW->newLine();
    }
    _LSVIEW->newLines(2);
    std::string message = "+ " + std::to_string(remaining) + " remaining...";
    _LSVIEW->print(message);
  } else {
    for (auto const &child : children) {
      attr_t style;
      auto const type = path.pathType(child);
      if (type == "directory")
        style = A_BOLD;
      else if (type == "file")
        style = A_NORMAL;
      else
        style = A_DIM;
      _LSVIEW->print(child.filename(), style);
      _LSVIEW->newLine();
    }
  }

  _LSVIEW->refresh();
}

/// returns _ARGV
std::vector<std::string> Shell::argv() const { return _ARGV; }

/// print to screen the virutal memory history
int Shell::printMemoryHistory() {
  if (!_COMMAND_HISTORY.empty()) {
    for (std::deque<std::string>::const_iterator command =
             _COMMAND_HISTORY.end();
         command != _COMMAND_HISTORY.begin(); command--) {
      _DISPLAY->print("%s\n", command->c_str());
    }
  }
  return 0;
}

int Shell::MOVE() {
  int y, x;
  y = std::stoi(_ARGV[1]);
  x = std::stoi(_ARGV[2]);
  _DISPLAY->moveCursor(y, x);
  return 0;
}

/// attempt to execute natively
int Shell::execute() {

  command_map::const_iterator x;
  x = _COMMAND_MAP.find(_ARGV[0]);

  if (x != _COMMAND_MAP.end()) {
    _DISPLAY->newLine();
    (this->*(x->second))(); // call using pointer
    return 0;
  } else {
    tcgetattr(STDIN_FILENO, &_OLDT); /*store old settings */
    _NEWT = _OLDT;                   /* copy old settings to new settings */
    _NEWT.c_oflag |= (ONLCR);
    _NEWT.c_lflag &= ~(ICANON | ECHO); /* make one change to old
                     settings in new settings */
    tcsetattr(STDIN_FILENO, TCSANOW,
              &_NEWT); /*apply the new settings immediatly */
    // system("stty sane");
    return 1;
  }
}

void Shell::runCommand() {
  int result;
  pid_t pid;
  if (execute() != 0) {

    // Fork our process
    pid = fork();

    std::vector<char *> argv = nullTerminatedArgV(_ARGV);
    char **command = argv.data();

    // error
    if (pid < 0)
      perror("Error (pid < 0)");

    // child process
    else if (pid == 0) {

      std::cout << "\n";
      char cmd[100];
      strcpy(cmd, "/usr/bin/");
      strcat(cmd, command[0]);
      int result = execvp(cmd, command);
      perror("Fallback Shell");

      exit(RESULT::END_OF_PROCESS); // duplicate child process is created.

    } else if (pid > 0) {
      // parent
      waitpid(pid, NULL, 0);
      tcsetattr(STDIN_FILENO, TCSANOW, &_OLDT); /*reapply the old settings */
      _DISPLAY->newLine();
    }
  }
} // namespace Trinkets

int Shell::cpos() {
  _DISPLAY->cursorPosition(_CURSOR_Y, _CURSOR_X);
  std::string yx = std::to_string(_CURSOR_Y) + " " + std::to_string(_CURSOR_X);
  _DISPLAY->print(yx);
  _DISPLAY->newLine();
  return 0;
}

int Shell::listView() {
  // switch on/off ListView on display
  _LIST_VIEW_ENABLED = !_LIST_VIEW_ENABLED;

  if (!_LIST_VIEW_ENABLED) {
    if (_LSVIEW->windowSet()) {
      _LSVIEW->eraseWin();
      _LSVIEW->refresh();
      _LSVIEW->setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_CHILD);
    }
    _IGNORE_BLOCKS = {};

    _DISPLAY_SIZE_X = _TERM_SIZE_X + _LSVIEW_SIZE_X;
    if (_SHOW_BORDER) {
      _DISPLAY->borderStyle(' ');
      _DISPLAY->resize(_DISPLAY_SIZE_Y, _DISPLAY_SIZE_X);
      _DISPLAY->borderStyle();
    } else {
      _DISPLAY->resize(_DISPLAY_SIZE_Y, _DISPLAY_SIZE_X);
    }
    _DISPLAY->print("lsview subwindow disabled");
    _DISPLAY->newLine();
  } else {
    displayListView(_CURRENT_DIR);
    _DISPLAY->print("lsview subwindow enabled");
    _DISPLAY->newLine();
  }
  _DISPLAY->refresh();
  return 0;
}

Shell::Shell() {
  // required environment variables
  char *path = getenv("PATH");
  char *term = getenv("TERM");
  char *shell = getenv("SHELL");
  char *home = getenv("HOME");

  if (path == nullptr) {
    std::cout << "no PATH environment variable is set." << std::endl;
    exit(RESULT::LAUNCH_FAILURE);
  }
  if (term == nullptr) {
    std::cout << "no TERM environment variable is set." << std::endl;
    exit(RESULT::LAUNCH_FAILURE);
  }
  if (shell == nullptr) {
    std::cout << "no SHELL environment variable is set." << std::endl;
    exit(RESULT::LAUNCH_FAILURE);
  }
  if (home == nullptr) {
    std::cout << "no HOME environment variable is set." << std::endl;
    exit(RESULT::LAUNCH_FAILURE);
  }

  _PATH = path;
  _TERM = term;
  _SHELL = shell;
  _HOME = home;

  _CONFIG_FILE = _HOME + "/.tr/config.txt";
  _SHELL_ENV_FILE = _HOME + "/.tr/environment.txt";
  _SHORTCUTS_FILE = _HOME + "/.tr/shortcuts.txt";
  _HISTORY_FILE = _HOME + "/.tr/history.txt";

  // refresh history on startup
  std::ofstream outfile;
  outfile.open(_HISTORY_FILE, std::ios_base::trunc);
  outfile.close();

  auto const termSz = DisplayKernel::TERMINAL_SIZE();

  _DISPLAY = generateSharedWindow();
  _TERM_SIZE_Y = termSz[0];
  _TERM_SIZE_X = termSz[1];
  _DISPLAY_SIZE_Y = _TERM_SIZE_Y;
  _DISPLAY_SIZE_X = _TERM_SIZE_X;

  // TODO: compiler option to initialise subwindows
  _LSVIEW = generateUniqueWindow();

  // SIGINT may end process without exiting Shell
  std::string test = "test";
  auto signalHandler = [](int const i) {
    //_DISPLAY->print("\ncaptured signal: %d\n", i);
  };

  _SIGNAL_INT_HANDLER.sa_handler = signalHandler;
  sigemptyset(&_SIGNAL_INT_HANDLER.sa_mask);
  _SIGNAL_INT_HANDLER.sa_flags = 0;
  sigaction(SIGINT, &_SIGNAL_INT_HANDLER, NULL);
}

Shell::~Shell() {
  _DISPLAY->setWin(BlackOS::DisplayKernel::WIN_SET_CODE::KILL_PARENT);
  std::cout << "\nexited Tr.\n";
}
} // namespace Trinkets
} // namespace BlackOS
