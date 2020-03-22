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

#include "../ScreenShell.h"

namespace BlackOS {
namespace Trinkets {

/// generates a shared pointer to DisplayKernel Screen instance.
Screen_sptr generateScreen() {
  auto screen = std::make_shared<DisplayKernel::Screen>();
  return screen;
}

/// generates a shared pointer to DisplayKernel Window instance.
Window_sptr generateWindow() {
  auto const termSz = DisplayKernel::TERMINAL_SIZE();
  size_t const ROWS = termSz[0];
  size_t const COLS = termSz[1];
  auto window = std::make_shared<DisplayKernel::Window>(ROWS, COLS, 0, 0);
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
int ScreenShell::bell() {

  start_color();

  init_pair(1, COLOR_BLACK, _BACKGROUND);
  bkgd(COLOR_PAIR(1));
  refresh();
  usleep(50000);
  init_pair(1, COLOR_WHITE, _BACKGROUND);
  bkgd(COLOR_PAIR(1));
  refresh();
  usleep(50000);
  init_pair(1, COLOR_RED, _BACKGROUND);
  bkgd(COLOR_PAIR(1));
  refresh();
  usleep(50000);
  init_pair(1, COLOR_GREEN, _BACKGROUND);
  bkgd(COLOR_PAIR(1));
  refresh();
  usleep(50000);
  init_pair(1, COLOR_YELLOW, _BACKGROUND);
  bkgd(COLOR_PAIR(1));
  refresh();
  usleep(50000);
  init_pair(1, COLOR_BLUE, _BACKGROUND);
  bkgd(COLOR_PAIR(1));
  refresh();

  init_pair(1, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));
  refresh();

  return 0;
}

int ScreenShell::configTheme() {
  if (_ARGC != 3) {
    printw("not enough arguments!\n");
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
    printw(message.c_str());
    printw("\n");
    logCursorPosition();
    return 1;
  }
  return 0;
}

int ScreenShell::configThemeInvader() {
  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_GREEN;
  _BACKGROUND = COLOR_BLACK;
  _CURSOR_COLOUR = "green";

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  init_pair(1, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));

  _USING_COLOR_FLAG = 1;

  refresh();

  return 0;
}

int ScreenShell::configThemeIre() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_RED;
  _BACKGROUND = COLOR_BLACK;
  _CURSOR_COLOUR = "red";

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  init_pair(1, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));

  _USING_COLOR_FLAG = 1;

  refresh();

  return 0;
}

int ScreenShell::configThemeClassic() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_WHITE;
  _BACKGROUND = COLOR_BLACK;
  _CURSOR_COLOUR = "white";

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  init_pair(1, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));

  _USING_COLOR_FLAG = 1;

  refresh();

  return 0;
}

int ScreenShell::configThemeThinkPad() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_WHITE;
  _BACKGROUND = COLOR_BLACK;
  _CURSOR_COLOUR = "red";

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  init_pair(1, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));

  _USING_COLOR_FLAG = 1;

  refresh();

  return 0;
}

int ScreenShell::configThemeAntiClassic() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_BLACK;
  _BACKGROUND = COLOR_WHITE;
  _CURSOR_COLOUR = "black";

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  init_pair(1, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));

  _USING_COLOR_FLAG = 1;

  refresh();

  return 0;
}

int ScreenShell::configThemeNeptune() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_WHITE;
  _BACKGROUND = COLOR_BLUE;
  _CURSOR_COLOUR = "white";

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  init_pair(1, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));

  _USING_COLOR_FLAG = 1;

  refresh();

  return 0;
}

int ScreenShell::configThemeSystem() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = -1;
  _BACKGROUND = -1;
  _CURSOR_COLOUR = "white";

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  bkgd(COLOR_PAIR(0));

  _USING_COLOR_FLAG = 0;

  refresh();

  return 0;
}

int ScreenShell::configThemeUgly() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  _FOREGROUND = COLOR_BLACK;
  _BACKGROUND = COLOR_YELLOW;
  _CURSOR_COLOUR = "black";

  printf("\e]12;%s\a", _CURSOR_COLOUR.c_str());
  fflush(stdout);

  init_pair(1, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));

  _USING_COLOR_FLAG = 1;

  refresh();

  return 0;
}

/// alternate colours in screen
int ScreenShell::rainbow() {
  auto start = std::chrono::system_clock::now();
  auto end = std::chrono::system_clock::now();
  while (
      (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() !=
       1)) {
    end = std::chrono::system_clock::now();
    init_pair(1, COLOR_BLACK, _BACKGROUND);
    bkgd(COLOR_PAIR(1));
    refresh();
    usleep(50000);
    init_pair(1, COLOR_WHITE, _BACKGROUND);
    bkgd(COLOR_PAIR(1));
    refresh();
    usleep(50000);
    init_pair(1, COLOR_RED, _BACKGROUND);
    bkgd(COLOR_PAIR(1));
    refresh();
    usleep(50000);
    init_pair(1, COLOR_GREEN, _BACKGROUND);
    bkgd(COLOR_PAIR(1));
    refresh();
    usleep(50000);
    init_pair(1, COLOR_YELLOW, _BACKGROUND);
    bkgd(COLOR_PAIR(1));
    refresh();
    usleep(50000);
    init_pair(1, COLOR_BLUE, _BACKGROUND);
    bkgd(COLOR_PAIR(1));
    refresh();
  }

  init_pair(1, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));

  return 0;
}

/// append data to file.
void ScreenShell::appendTofile(std::filesystem::path const &path,
                               std::string const &data) {
  std::ofstream outfile;
  outfile.open(path, std::ios_base::app);
  outfile << data + '\n';
  outfile.close();
}

/// load the shell to screen
void ScreenShell::initShell() {
  _DISPLAY->setWin(1);

  noecho();
  noraw();
  cbreak();
  scrollok(stdscr, TRUE);
  curs_set(_CURSOR);

  std::string title = "Tr(inkets) (C) ScreenShell";
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
  changeDir();

  // check colour support
  _COLOUR_SUPPORT = has_colors();
  if (_COLOUR_SUPPORT == FALSE) {
    printf("warning: this terminal does not support colour\n");
    printw("\n");
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
    printw("warning: no EDITOR environment variable is set.");
    printw("\n");
  }

  bell();
}

/// load shell configuration variables from config.txt file
int ScreenShell::initShellVariables() {
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
int ScreenShell::openWithTextEditor(std::string const &path) {
  char *editor = getenv("EDITOR");
  if (editor == NULL)
    return -1;
  std::string command = editor;
  command += " " + path;
  system(command.c_str());
  return 0;
}

/// load environment variables from environment.txt file
int ScreenShell::initEnvironmentVariables() {
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
void ScreenShell::logResult() {

  // log last command to history file
  appendTofile(_HISTORY_FILE, _LAST_COMMAND);

  if (_COMMAND_HISTORY.size() == _MAX_MEMORY_HISTORY - 1)
    _COMMAND_HISTORY.pop_front();

  // add to virtual history buffer;
  if (!_LAST_COMMAND.empty() && _LAST_COMMAND != "memory")
    _COMMAND_HISTORY.push_front(_LAST_COMMAND);
}

/// print the prompt to the screen
void ScreenShell::displayPrompt() {

  curs_set(_CURSOR);
  char buf[_MAX_ARGS];
  getcwd(buf, sizeof buf);
  noecho();

  std::string prompt = buf;
  prompt = "Tr " + prompt + "> ";
  _PROMPT_LEN = prompt.length();
  printw(prompt.c_str());
  logCursorPosition();
}

/// return cursor y position
size_t ScreenShell::cursorY() {
  logCursorPosition();
  return _CURSOR_Y;
}

/// return cursor x position
size_t ScreenShell::cursorX() {
  logCursorPosition();
  return _CURSOR_X;
}

/// update log entry for cursor position in Screen
void ScreenShell::logCursorPosition() { getsyx(_CURSOR_Y, _CURSOR_X); }

/// apply configurations to ScreenShell
int ScreenShell::configure() {

  std::string argv1 = _ARGV[1];

  command_map::const_iterator x;
  x = _SHELL_CONFIG_MAP.find(argv1);

  if (x != _SHELL_CONFIG_MAP.end()) {
    (this->*(x->second))(); // call using pointer
    return 0;
  } else {
    std::string message = "Shell variable " + argv1 + " is unrecognised.";
    printw(message.c_str());
    printw("\n");
    logCursorPosition();
    return 1;
  }
  return 0;
}

int ScreenShell::configure(std::string const &argv1, std::string const &argv2) {

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
    printw(message.c_str());
    printw("\n");
    logCursorPosition();
    return 1;
  }

  resetArgs();
  return 0;
}

int ScreenShell::configCursor() {

  if (_ARGC != 3) {
    printw("Usage:\n"
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

    printw(errorMessage.c_str());
    printw("\n");
    logCursorPosition();
    return 1;
  }
  if (value < 0 || value > 2) {

    printw(errorMessage.c_str());
    printw("\n");
    logCursorPosition();
    return 1;
  }

  _CURSOR = value;
  curs_set(_CURSOR); // TODO: extra arg to save preference to config file?
  return 0;
}

int ScreenShell::configCursorColour() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  std::string argv1 = _ARGV[1];
  std::string argv2 = _ARGV[2];

  if (!(argv2 == "red" || argv2 == "black" || argv2 == "white" ||
        argv2 == "blue" || argv2 == "green" || argv2 == "yellow")) {

    std::string message = "2nd argument " + argv2 + " is not a valid colour.";

    printw(message.c_str());
    printw("\n");
    return 1;
  }

  // TODO: check if Background is the same colour and warn invisibility

  printf("\e]12;%s\a", argv2.c_str());
  fflush(stdout);
  return 0;
}

int ScreenShell::configDeleteKey() {

  if (_ARGC != 3) {
    printw("Usage:\n"
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
      printw(errorMessage.c_str());
      printw("\n");
      return 1;
    }

    if (value == 10 /*Enter*/ || value == 35 /*Space*/) {
      // may need to specify more reserved values.
      printw("this key is reserved.");
      printw("\n");
      return 1;
    }
    _DELETE = value;
  } catch (...) {
    // parse character

    if (argv2.length() != 1) {
      printw(errorMessage.c_str());
      printw("\n");
      return 1;
    }

    char c = argv2[0];
    if (std::isprint(static_cast<unsigned char>(c))) {
      _DELETE = (int)c;
    } else {
      printw("2nd argument is not a printable character.");
      printw("\n");
    }
  }
  return 0;
}

int ScreenShell::configBackgroundColour() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }

  std::string argv1 = _ARGV[1];
  std::string argv2 = _ARGV[2];

  if (!(argv2 == "red" || argv2 == "black" || argv2 == "white" ||
        argv2 == "blue" || argv2 == "green" || argv2 == "yellow")) {
    std::string message = "2nd argument " + argv2 + " is not a valid colour.";
    printw(message.c_str());
    printw("\n");
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

  start_color();
  // TODO: check if Foreground is the same colour and warn invisibility/deny
  init_pair(1 /*1 reserved for BG/FG pair*/, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));
  refresh();

  // printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  _USING_COLOR_FLAG = 1;
  return 0;
}

int ScreenShell::configForegroundColour() {

  if (_ARGC != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return 1;
  }
  std::string argv1 = _ARGV[1];
  std::string argv2 = _ARGV[2];

  if (!(argv2 == "red" || argv2 == "black" || argv2 == "white" ||
        argv2 == "blue" || argv2 == "green" || argv2 == "yellow")) {
    std::string message = "2nd argument " + argv2 + " is not a valid colour.";
    printw(message.c_str());
    printw("\n");
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
  init_pair(1 /*1 reserved for BG/FG pair*/, _FOREGROUND, _BACKGROUND);
  bkgd(COLOR_PAIR(1));
  refresh();

  printf("\e[%im\e[%im", _STD_FG, _STD_BG + 10);

  _USING_COLOR_FLAG = 1;
  return 0;
}

/// reset _ARGV and _ARGC
void ScreenShell::resetArgs() {
  _ARGV = {};
  _ARGC = 0;
}

/// read in user arguments
int ScreenShell::readArgs() {
  keypad(stdscr, TRUE);
  std::string line;
  std::string userLine;

  char ch;
  int memorySz = _COMMAND_HISTORY.size();
  int historyCounter = -1;

  do {
    ch = getch();
    if (ch == '\n') {
      break;
    } else if ((_DELETE > 0 && (int)ch == _DELETE) || (int)ch == 8 ||
               (int)ch == KEY_BACKSPACE || (int)ch == KEY_DC ||
               (int)ch == 127) {
      logCursorPosition();
      if (_CURSOR_X > _PROMPT_LEN) {
        printw("\b");
        delch();
        refresh();
      }
      if (!line.empty()) {
        line.pop_back();
      }
    } else if ((int)ch == 27 || (int)ch == KEY_UP || (int)ch == 3) {
      if (!_COMMAND_HISTORY.empty()) {
        if (historyCounter + 1 < memorySz) {
          historyCounter++;
          logCursorPosition();
          std::string fromMemory = _COMMAND_HISTORY[historyCounter];
          _DISPLAY->erase(_CURSOR_Y, _PROMPT_LEN, _CURSOR_Y, _TERM_SIZE_X - 1);
          _DISPLAY->insert(fromMemory, _CURSOR_Y, _PROMPT_LEN);

          line = fromMemory;

          refresh();
        }
      }
    } else if ((int)ch == KEY_LEFT || (int)ch == 4) {
      logCursorPosition();
      if (_CURSOR_X > _PROMPT_LEN) {
        printw("\b");
        refresh();
      }
    } else if ((int)ch == KEY_RIGHT || (int)ch == 5) {
      auto const termSz = DisplayKernel::TERMINAL_SIZE();
      size_t const COLS = termSz[1];
      logCursorPosition();
      if (_CURSOR_X < COLS) {
        move(_CURSOR_Y, _CURSOR_X + 1);
        refresh();
      }
    } else if ((int)ch == KEY_DOWN || (int)ch == 2) {
      if (!_COMMAND_HISTORY.empty()) {
        logCursorPosition();

        if (historyCounter >= 0) {
          historyCounter--;
          std::string fromMemory = _COMMAND_HISTORY[historyCounter];
          _DISPLAY->erase(_CURSOR_Y, _PROMPT_LEN, _CURSOR_Y, _TERM_SIZE_X - 1);
          _DISPLAY->insert(fromMemory, _CURSOR_Y, _PROMPT_LEN);

          line = fromMemory;

          refresh();
        } else if (historyCounter < 0) {
          historyCounter = -1;
          _DISPLAY->erase(_CURSOR_Y, _PROMPT_LEN, _CURSOR_Y, _TERM_SIZE_X - 1);
          _DISPLAY->insert(userLine, _CURSOR_Y, _PROMPT_LEN);

          line = userLine;

          refresh();
        }
      }
    } else {
      addch(ch);
      logCursorPosition();
      refresh();
      line += ch;
      userLine = line;
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
  printw("\n");

  return 0;
}

/// returns _ARGV
std::vector<std::string> ScreenShell::argv() const { return _ARGV; }

/// print to screen the virutal memory history
int ScreenShell::printMemoryHistory() {
  if (!_COMMAND_HISTORY.empty()) {
    for (std::deque<std::string>::const_iterator command =
             _COMMAND_HISTORY.end();
         command != _COMMAND_HISTORY.begin(); command--) {
      printw("%s\n", command->c_str());
    }
  }
  return 0;
}

/// attempt to execute natively
int ScreenShell::execute() {

  command_map::const_iterator x;
  x = _COMMAND_MAP.find(_ARGV[0]);

  if (x != _COMMAND_MAP.end()) {
    (this->*(x->second))(); // call using pointer
    return 0;
  } else {
    tcgetattr(STDIN_FILENO, &_OLDT); /*store old settings */
    //_NEWT = _OLDT;                   /* copy old settings to new settings */
    //_NEWT.c_oflag |= (ONLCR);
    //_NEWT.c_lflag &= ~(ICANON | ECHO); /* make one change to old
    //                 settings in new settings */
    // tcsetattr(STDIN_FILENO, TCSANOW,
    //          &_NEWT); /*apply the new settings immediatly */
    system("stty sane");
    return 1;
  }
}

void ScreenShell::runCommand() {
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
    }
  }
}

ScreenShell::ScreenShell(Screen_sptr &display) {

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

  _DISPLAY = display;
  _TERM_SIZE_Y = termSz[0];
  _TERM_SIZE_X = termSz[1];
}

ScreenShell::~ScreenShell() {
  _DISPLAY->setWin(0);
  std::cout << "\nexited Tr.\n";
}

} // namespace Trinkets
} // namespace BlackOS
