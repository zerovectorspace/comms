#ifndef COMMS_ENGINE_HEADER
#define COMMS_ENGINE_HEADER

#include "comms/Command.hpp"

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

  // Event Polling :: handles input
  template <> struct Engine<Poll_Events> { Engine() {
    SDL_Event ev;
    while ( _glob.is_running && SDL_WaitEvent( &ev ) )
    {
      switch( ev.type )
      {
        case SDL_QUIT:
        {
          _glob.is_running = false;
          break;
        }
      }
    }
  }};

  // Main Loop :: high level
  template <> struct Engine<Main_Loop> { Engine() {
    while ( _glob.is_running )
    {
      Exec<Engine<Poll_Events>>{}();
    }
  }};

  // Engine :: init
  template <> struct Engine<Init> { Engine() {
    Exec<
      Prog_Win<Init>,
      Command<Init>,
      Engine<Main_Loop>
    >{}();

    SDL_Quit();
  }};
}

#endif
