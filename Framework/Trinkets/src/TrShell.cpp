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

#include "../inc/TrShell.h"

namespace BlackOS {
namespace Trinkets {

Screen_sptr generateScreen() {
  auto screen = std::make_shared<DisplayKernel::Screen>();
  return screen;
}

Window_sptr generateWindow() {
  auto const termSz = DisplayKernel::TERMINAL_SIZE();
  size_t const ROWS = termSz[0];
  size_t const COLS = termSz[1];
  auto window = std::make_shared<DisplayKernel::Window>(ROWS, COLS, 0, 0);
  return window;
}

void vectorToNullArray(std::vector<std::string> const &v, char **a) {
  int i = 0;
  while (i < v.size()) {
    std::string str = v[i];
    char *cstr = new char[str.size() + 1];
    strcpy(cstr, str.c_str());
    a[i] = cstr;
    i++;
  }
  a[i] = nullptr;
}

ScreenShell::ScreenShell(Screen_sptr &display) {

  // std::string const PATHENV std::string const TERMENV std::string const
  // SHELLENV
  //    std::string const HOMEENV

  // required environment variables
  char *path = getenv("PATH");
  char *term = getenv("TERM");
  char *shell = getenv("SHELL");
  char *home = getenv("HOME");

  if (path == nullptr) {
    std::cout << "no PATH environment variable is set." << std::endl;
    exit(ExitStatus::ERROR);
  }
  if (term == nullptr) {
    std::cout << "no TERM environment variable is set." << std::endl;
    exit(ExitStatus::ERROR);
  }
  if (shell == nullptr) {
    std::cout << "no SHELL environment variable is set." << std::endl;
    exit(ExitStatus::ERROR);
  }
  if (home == nullptr) {
    std::cout << "no HOME environment variable is set." << std::endl;
    exit(ExitStatus::ERROR);
  }

  _PATH = path;
  _TERM = term;
  _SHELL = shell;
  _HOME = home;

  _CONFIG_FILE = _HOME + "/.tr/config.txt";
  _SHELL_ENV_FILE = _HOME + "/.tr/environment.txt";
  _SHORTCUTS_FILE = _HOME + "/.tr/shortcuts.txt";

  _display = display;
}

void clearDisplay() {}

void ScreenShell::initShell() {
  _display->setWin(1);

  noecho();
  scrollok(stdscr, TRUE);
  curs_set(_CURSOR);

  auto const termSz = DisplayKernel::TERMINAL_SIZE();
  size_t const ROWS = termSz[0];

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
  auto banner = splashScreen(v);

  std::string pushDown((ROWS - 8 /*banner height*/) / 2, '\n');
  printw(pushDown.c_str());
  for (std::string const &line : banner) {
    printw(line.c_str());
    printw("\n");
  }
  printw("\n");
  changeDir();

  // check colour support
  if (has_colors() == FALSE) {
    printf("warning: this terminal does not support colour\n");
    printw("\n");
  }

  // initialise shell with config settings and shell variables described in
  // config and environment files.
  initShellVariables();
  initEnvironmentVariables();

  // recommended environment variables
  char *editor = getenv("EDITOR");
  if (editor == nullptr) {
    printw("warning: no EDITOR environment variable is set.");
    printw("\n");
  }
}

int ScreenShell::initShellVariables() {
  std::ifstream infile(_CONFIG_FILE);
  std::string a, b;
  while (infile >> a >> b) {
    configureShell(a, b);
  }
  return 0;
}

int ScreenShell::initEnvironmentVariables() {
  std::ifstream infile(_SHELL_ENV_FILE);
  std::string a, b;
  while (infile >> a >> b) {
    setenv(a.c_str(), b.c_str(), 1);
  }
  return 0;
}

void ScreenShell::displayPrompt() {
  if (_TTY_FLAG_FALLBACK) {
    system("stty -echo -icanon");
    _TTY_FLAG_FALLBACK = 0;
    logCursorPosition();
    move(_cursorY, _cursorX);
  }

  curs_set(_CURSOR);
  char buf[MAX_ARGS];
  getcwd(buf, sizeof buf);
  noecho();

  std::string prompt = buf;
  prompt = "Tr " + prompt + "> ";
  _promptLen = prompt.length();
  printw(prompt.c_str());
  logCursorPosition();
}

size_t ScreenShell::cursorY() const { return _cursorY; }

size_t ScreenShell::cursorX() const { return _cursorY; }

void ScreenShell::logCursorPosition() { getsyx(_cursorY, _cursorX); }

void ScreenShell::configureShell(std::vector<std::string> const &argv) {
  if (argv.size() != 3) {
    printw("Usage:\n"
           "config/configure <arg1> <arg2>\n"
           "options:\n'CURSOR' [int: 0 - 2]\n'DELETE' [int 0-127]\n");
    return;
  }
  std::string const argv1 = argv[1];
  std::string const argv2 = argv[2];

  configureShell(argv1, argv2);
}

void ScreenShell::configureShell(std::string const &argv1,
                                 std::string const &argv2) {

  /**
   * CONFIG CURSOR
   *
   */
  if (argv1 == "CURSOR") {
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
      return;
    }
    if (value < 0 || value > 2) {

      printw(errorMessage.c_str());
      printw("\n");
      logCursorPosition();
      return;
    }

    _CURSOR = value;
    curs_set(_CURSOR); // TODO: extra arg to save preference to config file?

  }
  /**
   * CONFIG DELETE
   *
   */
  else if (argv1 == "DELETE") {
    // linux TERM environment variable does not seem to read regular backspace.
    // allow the user to specify which key is delete.
    std::string errorMessage =
        "could not assign delete key to this value: " + argv2 +
        "\nexpected a single character, or number in range " +
        "0-127, or 'unset' to unset.";

    if (argv2 == "unset") {
      _DELETE = -1;
      return;
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
        return;
      }

      if (value == 10 /*Enter*/ || value == 35 /*Space*/) {
        // may need to specify more reserved values.
        printw("this key is reserved.");
        printw("\n");
        return;
      }
      _DELETE = value;
    } catch (...) {
      // parse character

      if (argv2.length() != 1) {
        printw(errorMessage.c_str());
        printw("\n");
        return;
      }

      char c = argv2[0];
      if (std::isprint(static_cast<unsigned char>(c))) {
        _DELETE = (int)c;
      } else {
        printw("2nd argument is not a printable character.");
        printw("\n");
      }
    }
  }
  /**
   * CONFIG CURSOR_COLOUR
   *
   */
  else if (argv1 == "CURSOR_COLOUR") {
    if (!(argv2 == "red" || argv2 == "black" || argv2 == "white" ||
          argv2 == "blue" || argv2 == "green" || argv2 == "yellow")) {
      std::string message = "2nd argument " + argv2 + " is not a valid colour.";
      printw(message.c_str());
      printw("\n");
      return;
    }

    // TODO: check if Background is the same colour and warn invisibility

    printf("\e]12;%s\a", argv2.c_str());
    fflush(stdout);
  } else if (argv1 == "BACKGROUND" || argv1 == "BG") {

    if (!(argv2 == "red" || argv2 == "black" || argv2 == "white" ||
          argv2 == "blue" || argv2 == "green" || argv2 == "yellow")) {
      std::string message = "2nd argument " + argv2 + " is not a valid colour.";
      printw(message.c_str());
      printw("\n");
      return;
    }

    if (argv2 == "black")
      _BACKGROUND = COLOR_BLACK;
    if (argv2 == "white")
      _BACKGROUND = COLOR_WHITE;
    if (argv2 == "red")
      _BACKGROUND = COLOR_RED;
    if (argv2 == "blue")
      _BACKGROUND = COLOR_BLUE;
    if (argv2 == "yellow")
      _BACKGROUND = COLOR_YELLOW;
    if (argv2 == "green")
      _BACKGROUND = COLOR_GREEN;

    start_color();
    // TODO: check if Foreground is the same colour and warn invisibility/deny
    init_pair(1 /*1 reserved for BG/FG pair*/, _FOREGROUND, _BACKGROUND);
    bkgd(COLOR_PAIR(1));
    refresh();

    _USING_COLOR_FLAG = 1;
  } else if (argv1 == "FOREGROUND" || argv1 == "FG") {

    if (!(argv2 == "red" || argv2 == "black" || argv2 == "white" ||
          argv2 == "blue" || argv2 == "green" || argv2 == "yellow")) {
      std::string message = "2nd argument " + argv2 + " is not a valid colour.";
      printw(message.c_str());
      printw("\n");
      return;
    }

    if (argv2 == "black")
      _FOREGROUND = COLOR_BLACK;
    if (argv2 == "white")
      _FOREGROUND = COLOR_WHITE;
    if (argv2 == "red")
      _FOREGROUND = COLOR_RED;
    if (argv2 == "blue")
      _FOREGROUND = COLOR_BLUE;
    if (argv2 == "yellow")
      _FOREGROUND = COLOR_YELLOW;
    if (argv2 == "green")
      _FOREGROUND = COLOR_GREEN;

    start_color();
    // TODO: check if Foreground is the same colour and warn invisibility/deny
    init_pair(1 /*1 reserved for BG/FG pair*/, _FOREGROUND, _BACKGROUND);
    bkgd(COLOR_PAIR(1));
    refresh();

    _USING_COLOR_FLAG = 1;
  } else {
    std::string message = "Shell variable " + argv1 + " is unrecognised.";
    printw(message.c_str());
    printw("\n");
    logCursorPosition();
  }
}

void ScreenShell::setShellEnv(std::vector<std::string> const &argv) {
  if (argv.size() != 3) {
    printw("Usage:\n"
           "set <ENV> <value>\n");
    return;
  }
  setenv(argv[1].c_str(), argv[2].c_str(), 1);
}

int ScreenShell::readArgs(std::vector<std::string> &argv) {

  keypad(stdscr, TRUE);

  std::string line;
  char ch;

  do {
    ch = getch();
    if (ch == '\n') {
      break;
    } else if ((_DELETE > 0 && (int)ch == _DELETE) || (int)ch == 8 ||
               (int)ch == KEY_BACKSPACE || (int)ch == KEY_DC ||
               (int)ch == 127) {
      logCursorPosition();
      if (_cursorX > _promptLen) {
        printw("\b");
        delch();
        refresh();
      }
      if (!line.empty()) {
        line.pop_back();
      }
    } else if ((int)ch == 27 || (int)ch == KEY_UP || (int)ch == 3) {
      printw("\n");
      refresh();
      return UserInput::UP;
    } else if ((int)ch == KEY_LEFT || (int)ch == 4) {
      logCursorPosition();
      if (_cursorX > _promptLen) {
        printw("\b");
        refresh();
      }
    } else if ((int)ch == KEY_RIGHT || (int)ch == 5) {
      auto const termSz = DisplayKernel::TERMINAL_SIZE();
      size_t const COLS = termSz[1];
      logCursorPosition();
      if (_cursorX < COLS) {
        move(_cursorY, _cursorX + 1);
        refresh();
      }
    } else if ((int)ch == KEY_DOWN || (int)ch == 2) {
      printw("\n");
      refresh();
      return UserInput::DOWN;
    } else {
      addch(ch);
      logCursorPosition();
      refresh();
      line += ch;
    }

  } while (1);

  // tokenize the string
  std::stringstream ss(line);
  std::string item;

  while (std::getline(ss, item, ' '))
    argv.push_back(item);
  printw("\n");

  return 0;
}

int ScreenShell::execute(std::vector<std::string> const &argv) {

  std::string command = argv[0];

  /**
   * NATIVE COMMAND CD
   *
   */
  if (command == "cd") {
    changeDir(argv[0]);
  } else if (command == "ls") {
    std::vector<std::string> children;
    listChildren(argv, children);
    printw("\n"); // TODO: wrapper for printw?
    std::string title = children.front();
    attron(A_UNDERLINE);
    printw(title.c_str());
    attroff(A_UNDERLINE);
    printw("\n");
    for (std::vector<std::string>::iterator i = children.begin() + 1;
         i != children.end(); i++) {
      printw(i->c_str());
      printw("\n");
    }
    printw("\n");
    logCursorPosition();
  }
  /**
   * NATIVE COMMAND ND/NDIR
   *
   */
  else if (command == "ndir" || command == "nd") {
    int y, x;
    getsyx(y, x);
    if (_USING_COLOR_FLAG) {
      navigateDir(argv, y + 2, 0, {_FOREGROUND, _BACKGROUND});
    } else {
      navigateDir(argv, y + 2, 0);
    }

  }
  /**
   * NATIVE COMMAND SET
   *
   */
  else if (command == "set") {
    setShellEnv(argv);
    logCursorPosition();
  }
  /**
   * NATIVE COMMAND CONFIG/CONFIGURE
   *
   */
  else if (command == "config" || command == "configure") {
    configureShell(argv);
    logCursorPosition();
  }
  /**
   * NATIVE COMMAND CLEAR
   *
   */
  else if (command == "clear") {
    clear();
    move(0, 0);
    logCursorPosition();
  }
  /**
   * NATIVE COMMAND SHORTCUT
   *
   */
  else if (command == "shortcut" || command == "sc") {
    int y, x;
    getsyx(y, x);
    if (_USING_COLOR_FLAG) {
      shortcut(_SHORTCUTS_FILE, y + 2, 0, {_FOREGROUND, _BACKGROUND});
    } else {
      shortcut(_SHORTCUTS_FILE, y + 2, 0);
    }
  }
  /**
   * FALLBACK TO SHELL
   *
   */
  else {
    system("stty sane");
    _TTY_FLAG_FALLBACK = 1;
    std::cout << "\n";
    return 1;
  }
  return 0;
}

// Given the number of arguments (argc) and an array of arguments (argv),
// this will fork a new process and run those arguments.
// Thanks to http://tldp.org/LDP/lpg/node11.html for their tutorial on pipes
// in C, which allowed me to handle user input with ampersands.
void ScreenShell::runCommand(std::vector<std::string> const &argv) {
  int result;

  pid_t pid;

  // TODO: no ampersand support for parent processes?
  if (/*run builtin comands with parent*/ execute(argv) != 0) {
    // Fork our process
    pid = fork();
    char **argvA;
    vectorToNullArray(argv, argvA);

    // error
    if (pid < 0)
      perror("Error (pid < 0)");

    // child process
    else if (pid == 0) {

      char cmd[100];
      strcpy(cmd, "/usr/bin/");
      strcat(cmd, argvA[0]);
      int result = execvp(cmd, argvA);
      perror("Fallback Shell");

      if (result != 0) {
        exit(3); // duplicate child process is created.
      }
      // parent process
    }
    waitpid(pid, NULL, 0); // only wait if no ampersand
  }
}

ScreenShell::~ScreenShell() {
  _display->setWin(0);
  std::cout << "\nexited Tr.\n";
}

} // namespace Trinkets
} // namespace BlackOS
