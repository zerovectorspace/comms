#ifndef COMMS_PROGRAM_WINDOW_HEADER
#define COMMS_PROGRAM_WINDOW_HEADER

#include "Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Prog_Win { Prog_Win() {

  }};

  static auto resize(int)
  {

  }

  // Prog_Win ::
  template <> struct Prog_Win<Init> { Prog_Win() {
    signal( SIGWINCH, resize );

    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
      fct::print( "ERROR: SDL_Init" );
      return;
    }

    _glob.win = SDL_CreateWindow( "comms"
                                , 0, 0
                                , 640, 480
                                , SDL_WINDOW_OPENGL
                                | SDL_WINDOW_BORDERLESS
                                | SDL_WINDOW_INPUT_GRABBED
                                | SDL_WINDOW_RESIZABLE 
                                | SDL_WINDOW_MAXIMIZED );

    if ( ! _glob.win )
    {
      fct::print( "ERROR: SDL_CreateWindow" );
      return;
    }

    _glob.ren = SDL_CreateRenderer( _glob.win
                                  , -1
                                  , SDL_RENDERER_ACCELERATED
                                  | SDL_RENDERER_PRESENTVSYNC );

    if ( ! _glob.ren )
    {
      fct::print( "ERROR: SDL_CreateRenderer" );
      SDL_DestroyWindow( _glob.win );
      SDL_Quit();
      return;
    }

    glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( _glob.win );

  }};
}

#endif
