#include "../inc/NavigationHelpers.h"

#include <filesystem>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <vector>

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace BlackOS {
namespace Trinkets {
std::string stripSubstring(std::string str, std::string const &substring) {
  size_t pos = std::string::npos;
  // Search for the substring in string in a loop untill nothing is found
  while ((pos = str.find(substring)) != std::string::npos) {
    // If found then erase it from string
    str.erase(pos, substring.length());
  }
  return str;
}

std::string timestampToDateTime(time_t const rawtime) {
  struct tm *dt;
  char buffer[80];
  dt = localtime(&rawtime);
  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M  %Z", dt);
  return std::string(buffer);
}

std::string pathLastModifiedDate(std::string const &path) {
  struct stat result;
  if (stat(path.c_str(), &result) == 0) {
    return timestampToDateTime(result.st_mtime);
  } else {
    return "unknown";
  }
}

std::string pathPermissions(std::string const &path) {
  namespace fs = std::filesystem;
  fs::perms p = fs::status(path).permissions();

  std::string uR = (p & fs::perms::owner_read) != fs::perms::none ? "r" : "-";
  std::string uW = (p & fs::perms::owner_write) != fs::perms::none ? "w" : "-";
  std::string uX = (p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-";

  std::string gR = (p & fs::perms::group_read) != fs::perms::none ? "r" : "-";
  std::string gW = (p & fs::perms::group_write) != fs::perms::none ? "w" : "-";
  std::string gX = (p & fs::perms::group_exec) != fs::perms::none ? "x" : "-";

  std::string oR = (p & fs::perms::others_read) != fs::perms::none ? "r" : "-";
  std::string oW = (p & fs::perms::others_write) != fs::perms::none ? "w" : "-";
  std::string oX = (p & fs::perms::others_exec) != fs::perms::none ? "x" : "-";

  return uR + uW + uX + gR + gW + gX + oR + oW + oX;
}

std::string pathType(std::filesystem::path const &path) {
  namespace fs = std::filesystem;
  if (fs::is_directory(path)) {
    return "directory";
  } else if (fs::is_regular_file(path)) {
    return "file";
  } else {
    return "unknown";
  }
}

/*
    Copyright 2008 Sarah Vessels

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
// Given the number of arguments (argc) and an array of arguments (argv),
// this will fork a new process and run those arguments.
// Thanks to http://tldp.org/LDP/lpg/node11.html for their tutorial on pipes
// in C, which allowed me to handle user input with ampersands.
void run_cmd(int argc, char **argv) {
  pid_t pid;
  const char *amp;
  amp = "&";
  bool found_amp = false;

  // If we find an ampersand as the last argument, set a flag.
  if (strcmp(argv[argc - 1], amp) == 0)
    found_amp = true;

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

    execvp(argv[0], argv);
    perror("execvp error");

    // parent process
  } else if (!found_amp)
    waitpid(pid, NULL, 0); // only wait if no ampersand
}

} // namespace Trinkets
} // namespace BlackOS
