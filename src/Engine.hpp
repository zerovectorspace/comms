#ifndef COMMS_ENGINE_HEADER
#define COMMS_ENGINE_HEADER

#include "comms/Commands.hpp"

/********************************************************************************
 *                                   Engine
 ********************************************************************************/

namespace Comms
{
  template <typename... Ts> struct Engine { Engine() {

  }};

  // Parse_Args :: splits input into a command and args
  template <> struct Engine<Parse_Args> { Engine( String& command, Vec<String>& args ) {
    auto w       = words( command );
         args    = tail( w );
         command = head( w );
  }};

  // Main_Loop :: handles input using a command line
  template <> struct Engine<Main_Loop> { Engine() {
    bool is_running = true;

    while ( is_running )
    {

    }
  }};

  // Engine ::init
  template <> struct Engine<Init> { Engine() {
    Exec<Prog_Win<Init>>{}();
  }};
}

#endif
