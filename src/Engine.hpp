#ifndef COMMS_ENGINE_HEADER
#define COMMS_ENGINE_HEADER

#include "comms/Commands.hpp"

/********************************************************************************
 *                                   Engine
 ********************************************************************************/

namespace Comms
{
  template <typename... Ts> struct Engine { Engine() {
    View.appendln( "error", "Engine::default" );
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

    View.current_win = "main" ;
    View.fresh_print( "main", "" );

    while ( is_running )
    {
      View.fresh_print( ".cl", "> " );

      unsigned char ch = View.get_char( ".cl" );

      if ( ch == '\n' )
      {
        continue;
      }

      /* Exec command */
      else if ( ch == ':' )
      {
        View.fresh_print( ".cl", "Command > " );

        auto input = View.get_line( ".cl", false );

        if (  input == "\0"_s
           || input == "\n"_s
           || input.empty() )
          continue;

        if ( input == "exit"_s  ) 
          break;

        LOG_START()
        Exec<Engine<Parse_Args>,Command<Run>>{}( input, Vec<String>{} );
        LOG_STOP()
      }

      /* Detect arrow keys */
      else if ( ch == '\033' )
      {
        ch = View.get_char( ".cl" );
        ch = View.get_char( ".cl" );
        if ( ch == 'A' )
          View.print( "UP Pressed" );
        else if ( ch == 'B' )
          View.print( "DOWN Pressed" );
        else if ( ch == 'C' )
          View.print( "RIGHT Pressed" );
        else if ( ch == 'D' )
          View.print( "LEFT Pressed" );
      }

      /* Echo on current win */
      else
      {
        View.fresh_print( ".cl", "> " );
        ungetch( ch );

        auto input = View.get_line( ".cl", false );
        input.push_back( '\0' );

        if ( !  View.is_writable( View.current_win )
             || input == "\0"_s 
             || input == "\n"_s )
          continue;

        std::stringstream output;
        output << "> ";
        output << input.data();

        /* Remove trailing '\0' */
        input.pop_back();
        View.print( View.current_win, output.str().data() );
      }
    }
  }};

  static auto resize(int)
  {
    View.get_win_dims();
    View.refresh();
  }

  // Engine ::
  template <> struct Engine<Init> { Engine() {
      signal(SIGWINCH, resize);

      View.init();

      View.new_win( ".cl", View.width(), 1, 0, View.height() - 1, true );
      View.new_win( "error", View.width(), View.height() - 1, 0, 0, false );

      View.current_win = "main";
      View.new_win( View.current_win, View.width(), View.height() - 1, 0, 0, true );

      Exec<Command<Init>,Engine<Main_Loop>>{}();
    }
  };

  void init()
  {
    Engine<Init>{};
  }
}

#endif
