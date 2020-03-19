#ifndef COMMS_COMMAND_COMMAND_HEADER
#define COMMS_COMMAND_COMMAND_HEADER

#include "src/protocol/Include.hpp"

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
    auto ws = fct::tail( fct::words( buf ) );

    Buffer<Clear>{};

    if ( ws.size() == 0 )
      return;

    auto cmd_name = ws.at( 0 );
    auto args = fct::tail( ws );

    if ( auto pr = _g.lcl_comms.find( fct::toStdStr( cmd_name ) ) ; pr != _g.lcl_comms.end() )
    {
      auto cmd = pr->second;

      if ( cmd.async )
      {
        std::thread( [ cmd_name, args, cmd ]()
        {
          cmd.make()->run( std::move( cmd_name ), std::move( args ) );
        } ).detach();
      }
      else
      {
        cmd.make()->run( std::move( cmd_name ), std::move( args ) );
      }
    }
    else
    {
      auto brand = "*** Command not found: "_s;
      cmd_name.insert( cmd_name.begin(), brand.begin(), brand.end() );

      Print<Client>{ std::move( cmd_name ) };
    }

    _g.vwin->mode = MODE::Text_Input;
  }};

  /**
   * Factory to create the command
   */
  template <typename EndPoint_T>
  U_ptr<Command_Base> command()
  {
    return std::make_unique<EndPoint_T>();
  }

  // Command<Init> :: Define command maps
  template <> struct Command<Init> { Command() {
    _g.lcl_comms = Command_Map{
      { "quit", Cmd{ false, command<Quit_Cmd> } },
      { "help", Cmd{ false, command<Help_Cmd> } },
      { "win", Cmd{ false, command<Win_Cmd> } },
      { "clear", Cmd{ false, command<Clear_Cmd> } },
      { "view", Cmd{ false, command<View_Cmd> } },
      { "async", Cmd{ true, command<Async_Cmd> } },
      { "primes", Cmd{ false, command<Primes_Cmd> } },
      { "shutdown", Cmd{ false, command<Shutdown_Cmd> } },
      { "bad", Cmd{ false, command<Bad_Cmd> } }
    };
  }};

  #ifdef Command_List
    #undef Command_List
  #endif
} // namespace Comms

#endif
