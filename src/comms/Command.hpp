#ifndef COMMS_COMMAND_COMMAND_HEADER
#define COMMS_COMMAND_COMMAND_HEADER

#include "Prog_Win.hpp"
#include "Window.hpp"
#include "Program.hpp"

namespace Comms
{
  template <typename... Ts> struct Command { Command() {

  }};

  // Exec_Command :: Grabs lambda from command map, executes lambda w/ args
  template <> struct Command<Init> { Command() {

  }};

}

#endif
