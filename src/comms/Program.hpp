#ifndef COMMS_COMMANDS_PROGRAM_HEADER
#define COMMS_COMMANDS_PROGRAM_HEADER

#include "Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Program { Program() {
    View.appendln( "error", "Program::default" );
  }};

  template <> struct Program<Commands> { Program() {
    StdString cs = "help         View list of commands\n"
      "win <title>  Open window named <title>\n"
      "view <opts...> \n";

    View.fresh_print( cs.data() );
  }};
}

#endif
