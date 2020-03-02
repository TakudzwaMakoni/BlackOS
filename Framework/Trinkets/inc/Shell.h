/**
 * Copyright 2020 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#ifndef SHELL_H
#define SHELL_H

#include "../inc/Natives.h"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace BlackOS {
namespace Trinkets {

// Will be used to create an array to hold individual arguments passed by
// the user on the command line.
int const MAX_ARGS = 256;

enum class PipeRedirect { PIPE, REDIRECT, NEITHER };

std::string const PATHENV = "PATH=" + (std::string)getenv("PATH");
std::string const TERMENV = "TERM=" + (std::string)getenv("TERM");
std::string const SHELLENV = "SHELL=" + (std::string)getenv("SHELL");
std::string const EDITORENV = "EDITOR=vim";

// Splits a user's command into two commands, or a command and a file name.
PipeRedirect parse_command(int, char **, char **, char **);

// Pipes the first command's output into the second.
void pipe_cmd(char **, char **);

// Reads input from the user into the given array and returns the number of
// arguments taken in.
int read_args(char **);

// Redirects the output from the given command into the given file.
void redirect_cmd(char **, char **);

// Given the number of arguments and an array of arguments, this will execute
// those arguments.  The first argument in the array should be a command.
void run_cmd(int, char **);

// Given a string of user input, this will determine if the user wants to
// quit the shell.
bool want_to_quit(std::string);

//
int execute(int, char **);

} // namespace Trinkets
} // namespace BlackOS
#endif
