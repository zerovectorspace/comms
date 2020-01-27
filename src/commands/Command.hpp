#ifndef COMMS_COMMAND_COMMAND_HEADER
#define COMMS_COMMAND_COMMAND_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  #define Command_List [[maybe_unused]] std::vector<std::vector<char>>&&

  template <typename... Ts> struct Command { Command() {

  }};

  // Command<Run> :: Execute command in relevant Command_Map
  template <> struct Command<Client,Dispatch> { Command() {
    if ( _g.vwin->mode != MODE::Command_Exec )
      return;

    auto buf  = *(_g.vwin->buf);
    auto cmds = fct::tail( fct::words( buf ) );

    Buffer<Clear>{};

    if ( cmds.size() == 0 )
      return;

    auto cmd = fct::toStdStr( cmds.at( 0 ) );

    if ( _g.lcl_comms.count( cmd ) )
    {
      _g.lcl_comms.at( cmd )( std::move( fct::tail( cmds ) ) );
    }
    else
    {
      Buffer<Lines>{ fct::toStr( cmd.insert( 0, "\n*** Command not found: " ) ) };
      _g.lcl_comms.at( "help" )( Vec<String>{} );
    }

    _g.vwin->mode = MODE::Text_Input;
  }};

  // Command<Init> :: Define command maps
  template <> struct Command<Init> { Command() {
    _g.lcl_comms.emplace( "help", []( Command_List cmds ){ Program<Commands>{}; } );

    _g.lcl_comms.emplace( "win", []( Command_List cmds )
    {
      if ( cmds.empty() )
        return;

      auto vwin_name = fct::toStdStr( cmds.at( 0 ) );

      VWin<New>{ vwin_name };
    });

    _g.lcl_comms.emplace( "quit", []( Command_List cmds ){ _g.is_running = false; } );
  }};

  #ifdef Command_List
    #undef Command_List
  #endif

} // namespace Comms

#endif
