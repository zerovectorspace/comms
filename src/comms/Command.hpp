#ifndef COMMS_COMMAND_COMMAND_HEADER
#define COMMS_COMMAND_COMMAND_HEADER

#include "Window.hpp"
#include "Program.hpp"

namespace Comms
{
  template <typename... Ts> struct Command { Command() {
    View.appendln( "error", "Command::default" );
  }};

  // Exec_Command :: Grabs lambda from command map, executes lambda w/ args
  template <> struct Command<Run> { Command( String& command, Vec<String>& args ) {
    if ( auto p = lcl_comms.find( toStdStr( command ) ) ; p != lcl_comms.end() )
      p->second( std::move( args ) );
  }};

  template <> struct Command<Init> { Command() {
    lcl_comms.emplace( "help", []( [[maybe_unused]] Vec<String>&& args ){
      Exec<Window<Open>>{}( "help", false );
      Exec<Program<Commands>>{}();
    });

    lcl_comms.emplace( "win", []( Vec<String>&& args ){
      Try_Exec{ [&args](){ Exec<Window<Open>>{}( toStdStr( args.at( 0 ) ) ); } };
    });

    lcl_comms.emplace( "delwin", []( Vec<String>&& args ){
      Try_Exec{ [&args](){ Exec<Window<Del>>{}( toStdStr( args.at( 0 ) ) ); } };
    });

    lcl_comms.emplace( "view", []( Vec<String>&& args ){
      Try_Exec{ [&args](){
        if ( args.at( 0 ) == "win"_s )
          Exec<Window<List>>{}();
      }};
    });

    lcl_comms.emplace( "login", []( [[maybe_unused]] Vec<String>&& args ){
      /* Exec Default */
      Exec<Window<New>>{}();
    });

    lcl_comms.emplace( "test", []( [[maybe_unused]] Vec<String>&& args ){
      /* Exec Default */
      Exec<Window<New>>{}();
    });
  }};

}

#endif
