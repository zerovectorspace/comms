#include "Engine.hpp"

/*
 * Include Heirarchy
 * -----------------
 *
 * Include
 *  ^- Macros
 *     ^- Alias
 *        ^- Helper
 *           ^- Data
 *              View
 *              ^- Global
 *                 ^- Exec
 *                    ^- Taglist
 *                       ^- | Window
 *                          | Program
 *                            ^- Command
 *                               ^- Commands
 *                                  ^- Engine
 *                                     ^- main
 *
 *
 */

/*
 * Program Flow
 * ------------
 *                                                                         __.-> Exec     -> Command Functors
 *                                                                       /`__.-> Try_Exec -> Command Functors
 * Engine<Init> -> Engine<Main_Loop> -> Engine<Parse_Args> -> Command<Run>`
 *       \                          `-> Engine<Parse_Args> -> Command<Run> -> Try_Exec -> Command Functors
 *        \                                                               `-> Exec     -> Command Functors
 *         \
 *          `Engine<Net_Loop> -> Engine<Parse_Buffer> -> Command<Run> -> Net Comm Map -> Exec    -> Command Functors
 *                                                                                  `-> Try_Exec -> Command Functors
 *                                                   
 */

int main()
{
  // using namespace Comms;
  // Engine<Init>{};

  if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
  {
    fct::print( "ERROR: SDL_Init" );
    return 1;
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
    return 1;
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
    return 1;
  }

  glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT );
  SDL_GL_SwapWindow( win );

  SDL_Delay( 3000 );

  SDL_Quit();
}
