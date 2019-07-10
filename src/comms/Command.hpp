#ifndef COMMS_COMMAND_COMMAND_HEADER
#define COMMS_COMMAND_COMMAND_HEADER

#include "Window.hpp"
#include "Program.hpp"

namespace Comms
{
  template <typename... Ts> struct Command { Command() {

  }};

  // Exec_Command :: Grabs lambda from command map, executes lambda w/ args
  template <> struct Command<Run> { Command( String& command, Vec<String>& args ) {
    if ( auto p = lcl_comms.find( toStdStr( command ) ) ; p != lcl_comms.end() )
      p->second( std::move( args ) );
  }};

  template <> struct Command<Init> { Command() {
    lcl_comms.emplace( "help", []( [[maybe_unused]] Vec<String>&& args ){

    });

    lcl_comms.emplace( "test", []( [[maybe_unused]] Vec<String>&& args ){

    });
  }};

}

#endif
