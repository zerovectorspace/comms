#ifndef COMMS_COMMANDS_PROGRAM_HEADER
#define COMMS_COMMANDS_PROGRAM_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  template <typename... Ts> struct Program { Program() {

  }};

  template <> struct Program<Commands> { Program() {
    String cs = "--------------------------------------\n"
                "help         View list of commands\n"
                "win <title>  Open window named <title>\n"
                "view <opts...> \n"
                "quit         Quit the program\n"
                "--------------------------------------\n\n"_s;

    Buffer<Lines>{ cs };
  }};
} // namespace Comms

#endif
