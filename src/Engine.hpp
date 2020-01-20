#ifndef COMMS_ENGINE_HEADER
#define COMMS_ENGINE_HEADER

#include "src/commands/Include.hpp"

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

  // Main Loop :: high level
  template <> struct Engine<Main_Loop> { Engine() {
    Render<Background>{};

    VWin<Init>{};

    while ( _g.is_running )
    {
      Exec<
        Event<Poll>,
        Command<Run>,
        Render<Buffers,Asc>,
        Render<Swap>
      >{}();
    }

    SDL_Quit();
  }};

  // Engine :: init
  template <> struct Engine<Init> { Engine() {
    Exec<
      Prog_Win<Init>,
      Font<Init>,
      Command<Init>,
      Engine<Main_Loop>
    >{}();
  }};
} // namespace Comms

#endif
