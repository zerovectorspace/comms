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

  static auto resize(int)
  {

  }

  // Engine ::
  template <> struct Engine<Init> { Engine() {
    signal(SIGWINCH, resize);

    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
      fct::print( "ERROR: SDL_Init" );
      return;
    }

    auto win = SDL_CreateWindow( "comms"
                               , 0, 0
                               , 640, 480
                               , SDL_WINDOW_OPENGL
                               | SDL_WINDOW_BORDERLESS
                               | SDL_WINDOW_INPUT_GRABBED
                               | SDL_WINDOW_RESIZABLE 
                               | SDL_WINDOW_MAXIMIZED );

    if ( ! win )
    {
      fct::print( "ERROR: SDL_CreateWindow" );
      return;
    }

    auto ren = SDL_CreateRenderer( win
                                 , -1
                                 , SDL_RENDERER_ACCELERATED
                                 | SDL_RENDERER_PRESENTVSYNC );

    if ( ! ren )
    {
      fct::print( "ERROR: SDL_CreateRenderer" );
      SDL_DestroyWindow( win );
      SDL_Quit();
      return;
    }

    glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( win );

    SDL_Delay( 3000 );

    SDL_Quit();
  }};
}

#endif
