#ifndef COMMS_ENGINE_HEADER
#define COMMS_ENGINE_HEADER

#include "comms/Command.hpp"

/********************************************************************************
 *                                   Engine
 ********************************************************************************/

namespace Comms
{
  void RenderText( std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color );

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

    while ( _glob.is_running && SDL_PollEvent( &ev ) )
    {
      switch( ev.type )
      {
        case SDL_QUIT:
          _glob.is_running = false;

          break;

        case SDL_WINDOWEVENT:
          Event<Win>{ ev };

          break;

        case SDL_KEYDOWN:
          Event<Keydown>{ ev };

          break;
      }
    }
  }};

  // Main Loop :: high level
  template <> struct Engine<Main_Loop> { Engine() {
    Render<Background>{};

    _glob.curs.pos = { _glob.pad_x, _glob.pad_y };

    while ( _glob.is_running )
    {
      Exec<Engine<Poll_Events>,
           Render<Buffers>,
           Render<Swap>>{}();
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
}

#endif
