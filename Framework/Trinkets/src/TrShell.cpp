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

namespace {
struct cout_redirect {
  cout_redirect(std::streambuf *new_buffer)
      : old(std::cout.rdbuf(new_buffer)) {}

  ~cout_redirect() { std::cout.rdbuf(old); }

private:
  std::streambuf *old;
};
} // namespace

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

ScreenShell::ScreenShell(Screen_sptr &display) { _display = display; }

void clearDisplay() {}

void ScreenShell::initShell() {
  _display->setWin(1);
  keypad(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  curs_set(2);
}

void ScreenShell::displayPrompt(size_t y) {
  curs_set(2);
  char buf[MAX_ARGS];
  getcwd(buf, sizeof buf);
  std::string prompt = buf;
  prompt = "Tr " + prompt + "> ";
  _promptLen = prompt.length();
  printw(prompt.c_str()); // TODO: make wrapper?
  logCursorPosition();
}

size_t ScreenShell::cursorY() const { return _cursorY; }

size_t ScreenShell::cursorX() const { return _cursorY; }

void ScreenShell::logCursorPosition() { getyx(stdscr, _cursorY, _cursorX); }

int ScreenShell::readArgs(char **argv) {

  std::string line;
  char *cstr;
  int argc = 0;
  int count = 0;
  char ch;

  do {
    ch = getch();

    int intch = ch;
    if (ch == '\n') {
      break;
    } else if (intch == 27 || intch == KEY_UP) {
      return userInput::up;
    } else if (intch == KEY_BACKSPACE || intch == KEY_DC || intch == 127) {
      if (!line.empty()) {

        move(_cursorY, _promptLen + count - 1);
        delch();
        logCursorPosition();
        refresh();
        count--;
        line.pop_back();
      }
    } else if ((int)ch == KEY_LEFT) {
      // do something
      return userInput::left;
    } else if ((int)ch == KEY_RIGHT) {
      // do something
      return userInput::right;
    } else {
      addch(ch);
      logCursorPosition();
      refresh();
      count++;
      line += ch;
    }
  } while (ch != '\n');
  move(_cursorY + 1, 0);

  // tokenize the string
  std::stringstream ss(line);
  std::string item;

  while (std::getline(ss, item, ' ')) {
    cstr = new char[item.size() + 1];
    strcpy(cstr, item.c_str());
    argv[argc] = cstr;
    argc++;
  }
  // Have to have the last argument be NULL so that execvp works.
  argv[argc] = NULL;

  return argc;
}

int ScreenShell::execute(int argc, char **argv) {

  char cmd[100];
  std::string command = argv[0];
  if (command == "cd") {
    changeDir(argv[1]);
  } else if (command == "ls") {
    std::vector<std::string> children;
    listChildren(argc, argv, children);
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
  } else if (command == "ndir" || command == "nd") {
    navigateDir(argc, argv, _cursorY + 1, 0);
    logCursorPosition();
  } else if (command == "set") {
    setenv(argv[1], argv[2], 1);
    logCursorPosition();
  } else if (command == "clear") {
    clear();
    move(0, 0);
    logCursorPosition();
  } else {
    return 1;
  }
  return 0;
}

PipeRedirect ScreenShell::parseCommand(int argc, char **argv, char **cmd1,
                                       char **cmd2) {
  // Assume no pipe or redirect will be found.
  PipeRedirect result = PipeRedirect::NEITHER;

  // Will hold the index of argv where the pipe or redirect is found.
  int split = -1;

  // Go through the array of arguments...
  for (int i = 0; i < argc; i++) {
    // Pipe found!
    if (strcmp(argv[i], "|") == 0) {
      result = PipeRedirect::PIPE;
      split = i;

      // Redirect found!
    } else if (strcmp(argv[i], ">>") == 0) {
      result = PipeRedirect::REDIRECT;
      split = i;
    }
  }

  // If either a pipe or a redirect was found...
  if (result != PipeRedirect::NEITHER) {
    // Go through the array of arguments up to the point where the
    // pipe/redirect was found and store each of those arguments in cmd1.
    for (int i = 0; i < split; i++)
      cmd1[i] = argv[i];

    // Go through the array of arguments from the point where the pipe/redirect
    // was found through the end of the array of arguments and store each
    // argument in cmd2.
    int count = 0;
    for (int i = split + 1; i < argc; i++) {
      cmd2[count] = argv[i];
      count++;
    }

    // Terminate cmd1 and cmd2 with NULL, so that execvp likes them.
    cmd1[split] = NULL;
    cmd2[count] = NULL;
  }

  // Return an enum showing whether a pipe, redirect, or neither was found.
  return result;
}

// Given the number of arguments (argc) and an array of arguments (argv),
// this will fork a new process and run those arguments.
// Thanks to http://tldp.org/LDP/lpg/node11.html for their tutorial on pipes
// in C, which allowed me to handle user input with ampersands.
void ScreenShell::runCmd(int argc, char **argv) {
  pid_t pid;
  const char *amp;
  amp = "&";
  bool found_amp = false;

  // If we find an ampersand as the last argument, set a flag.
  if (strcmp(argv[argc - 1], amp) == 0)
    found_amp = true;

  // TODO: no ampersand support for parent processes?
  if (/*run builtin comands with parent*/ execute(argc, argv) != 0) {

    // Fork our process
    pid = fork();

    // error
    if (pid < 0)
      perror("Error (pid < 0)");

    // child process
    else if (pid == 0) {
      // If the last argument is an ampersand, that's a special flag that we
      // don't want to pass on as one of the arguments.  Catch it and remove
      // it here.
      if (found_amp) {
        argv[argc - 1] = NULL;
        argc--;
      }

      char cmd[100];
      strcpy(cmd, "/usr/bin/");
      strcat(cmd, argv[0]);
      int result = execve(cmd, argv, environ);

      // perror("execve error");
      if (result != 0) {
        exit(3); // duplicate child process is created.
      }
      // parent process
    } else if (!found_amp)
      waitpid(pid, NULL, 0); // only wait if no ampersand

    logCursorPosition();
  }
}

void ScreenShell::redirectCmd(char **cmd, char **file) {
  int fds[2]; // file descriptors
  int count;  // used for reading from stdout
  int fd;     // single file descriptor
  char c;     // used for writing and reading a character at a time
  pid_t pid;  // will hold process ID; used with fork()

  pipe(fds);

  // child process #1
  if (fork() == 0) {
    // Thanks to http://linux.die.net/man/2/open for showing which headers
    // need to be included to use this function and its flags.
    fd = open(file[0], O_RDWR | O_CREAT, 0666);

    // open() returns a -1 if an error occurred
    if (fd < 0) {
      printf("Error: %s\n", strerror(errno));
      return;
    }

    dup2(fds[0], 0);

    // Don't need stdout end of pipe.
    close(fds[1]);

    // Read from stdout...
    while ((count = read(0, &c, 1)) > 0)
      write(fd, &c, 1); // Write to file.

    // Okay, so this is a bit contrived, but when I didn't have any kind of exec
    // function call here, I got my SarahShell prompt repeated over and over
    // again on the Multilab machines, I think because of this crazy child
    // process or something.  When I put this execlp here with the useless call
    // to echo, however, that looping stops and you can actually enter things
    // at the prompt again, hurray!
    execlp("echo", "echo", NULL);

    // child process #2
  } else if ((pid = fork()) == 0) {
    dup2(fds[1], 1);

    // Don't need stdin end of pipe.
    close(fds[0]);

    // Output contents of the given file to stdout.
    execvp(cmd[0], cmd);
    perror("execvp failed");

    // parent process
  } else {
    waitpid(pid, NULL, 0);
    close(fds[0]);
    close(fds[1]);
  }
}

// This pipes the output of cmd1 into cmd2.
void ScreenShell::pipeCmd(char **cmd1, char **cmd2) {
  int fds[2]; // file descriptors
  pipe(fds);
  pid_t pid;

  // child process #1
  if (fork() == 0) {
    // Reassign stdin to fds[0] end of pipe.
    dup2(fds[0], 0);

    // Not going to write in this child process, so we can close this end
    // of the pipe.
    close(fds[1]);

    // Execute the second command.
    execvp(cmd2[0], cmd2);
    perror("execvp failed");

    // child process #2
  } else if ((pid = fork()) == 0) {
    // Reassign stdout to fds[1] end of pipe.
    dup2(fds[1], 1);

    // Not going to read in this child process, so we can close this end
    // of the pipe.
    close(fds[0]);

    // Execute the first command.
    execvp(cmd1[0], cmd1);
    perror("execvp failed");
    // parent process
  } else
    waitpid(pid, NULL, 0);
}

void ScreenShell::quit(std::string) { std::cout << "goodbye!\n"; }

ScreenShell::~ScreenShell() { std::cout << "exited Tr.\n"; }

} // namespace Trinkets
} // namespace BlackOS
