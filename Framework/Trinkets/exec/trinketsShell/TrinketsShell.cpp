// Tr(inkets) shell (c), Takudzwa 2020
// Thanks to Sarah Vessels for pipe/redirect and Dr Tong Lai Yu.

#include "Shell.h"

// Takes user input until they quit the shell, and passes that input as
// arguments to be run.
int main() {

  char *argv[MAX_ARGS], *cmd1[MAX_ARGS], *cmd2[MAX_ARGS];
  PipeRedirect pipe_redirect;
  int argc;

  static int first_time = 1;
  if (first_time) {
    const char *CLEAR_SCREEN_ANSI = "\e[0;0H\e[2J";
    std::cout << CLEAR_SCREEN_ANSI;
  }

  std::cout << "Tr(inkets) Shell(c), Takudzwa Makoni 2020\n" << std::endl;

  // Keep returning the user to the prompt ad infinitum unless they enter
  // 'quit' or 'exit' (without quotes).

  while (true) {
    // Display a prompt.
    char buf[1024];
    std::cout << "Tr " << getcwd(buf, sizeof buf) << "> ";

    // Read in a command from the user.
    argc = read_args(argv);

    // Decipher the command we just read in and split it, if necessary, into
    // cmd1 and cmd2 arrays.  Set pipe_redirect to a PipeRedirect enum value to
    // indicate whether the given command pipes, redirects, or does neither.
    pipe_redirect = parse_command(argc, argv, cmd1, cmd2);

    // Determine how to handle the user's command(s).
    if (pipe_redirect == PipeRedirect::PIPE) // piping
      pipe_cmd(cmd1, cmd2);
    else if (pipe_redirect == PipeRedirect::REDIRECT) // redirecting
      redirect_cmd(cmd1, cmd2);
    else
      run_cmd(argc, argv); // neither

    // Reset the argv array for next time.
    for (int i = 0; i < argc; i++)
      argv[i] = NULL;
  }

  return 0;
}
