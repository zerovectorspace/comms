#include "src/Engine.hpp"

/*
 * Include Heirarchy
 * -----------------
 *
 * Include
 *  ^- Macros
 *     ^- Alias
 *        ^- Helper
 *           ^- | Data
 *                ^- Global
 *                   ^- Exec
 *                      ^- Taglist
 *                         ^-  | Buffer
 *                             | Event
 *                             | Font
 *                             | Prog_Win
 *                             | Print
 *                             | Render
 *                             | Socket
 *                               ^- components/Include
 *                                  ^- | Primes Res/Req
 *                                     | Protocol
 *                                       ^- protocol/Include
 *                                          ^- | Program
 *                                             | Window
 *                                             | Command
 *                                               ^- commands/Include
 *                                                  ^- Engine
 *                                                     ^- main
 *
 *
 */

auto help( char const* name )
{
  fct::print( "Usage:", ' ' );
  fct::print( name, ' ' );
  fct::print( "[OPTION...]\n\n"
              "  -s Start the socket server\n"
              "  -c Start the user client");
}

void handle_args( int argc, char* argv[] )
{
  using namespace Comms;

  struct Cmd
  {
    const char* flag;
    bool kill = false;
    void(*fn)(void);
  };

  auto cmds = Vec<Cmd>{
    { "-s", true, [](){ Engine<Server,Init>{}; } },
    { "-c", true, [](){ Engine<Client,Init>{}; } }
  };

  for ( auto const& cmd : cmds )
  {
    for ( int i = 0 ; i < argc ; i++ )
    {
      if (    strlen( argv[i] ) == strlen( cmd.flag )
           && strncmp( argv[i], cmd.flag, strlen( cmd.flag ) ) == 0 )
      {
        (*cmd.fn)();

        if ( cmd.kill )
          goto end_exec;
      }
    }
  }

  /** No Engine was started */
  help( ( argv[0] + 2 ) );

  end_exec: ; // end execution early
}

int main( int argc, char* argv[] )
{
  handle_args( argc, argv );
}

