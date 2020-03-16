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
    auto cmds = fct::tail( fct::words( buf ) );

    Buffer<Clear>{};

    if ( cmds.size() == 0 )
      return;

    auto cmd = fct::toStdStr( cmds.at( 0 ) );

    if ( _g.lcl_comms.count( cmd ) )
    {
      auto& c = _g.lcl_comms.at( cmd );
      if ( c.async )
      {
        std::thread( [&c,cmds](){ c.f( std::move( fct::tail( cmds ) ) ); } ).detach();
      }
      else
      {
        c.f( std::move( fct::tail( cmds ) ) );
      }
    }
    else
    {
      Print<Client>{ fct::toStr( cmd.insert( 0, "*** Command not found: " ) ) };
    }

    _g.vwin->mode = MODE::Text_Input;
  }};

  // Command<Init> :: Define command maps
  template <> struct Command<Init> { Command() {
    _g.lcl_comms = Command_Map{
      /**
       * Show help
       * List all commands available to GUI client
       * ex: help
       */
      { "help", Cmd{ false, []( Command_List cmds ){ Program<Commands>{}; } } },

      /**
       * Move to a different window
       * ex: win <name>
       *
       * Creates a new window if it doesn't exist
       */
      { "win", Cmd{ false, []( Command_List cmds ) {
        if ( cmds.empty() )
          return;

        auto vwin_name = fct::toStdStr( cmds.at( 0 ) );

        VWin<New>{ vwin_name };
      }}},

      /**
       * View information on various session components
       * ex: view win
       */
      { "view", Cmd{ false,[]( Command_List cmds ) {
        if ( cmds.empty() )
          return;

        for ( auto const& cmd : cmds )
        {
          if ( cmd == "win"_s )
          {
            String out = "-------------- windows ---------------\n"_s;

            for ( auto const& [name, vwin] : _g.vwins )
            {
              out.insert( out.end(), name.begin(), name.end() );
              out.push_back( '\n' );
            }

            String tmp = "--------------------------------------\n\n"_s;
            out.insert( out.end(), tmp.begin(), tmp.end() );

            Buffer<Lines>{ out };
            Buffer<New_Line>{};
          }
        }
      }}},

      /**
       * TEST: Print user input after 2 seconds
       * ex: async this is printed back
       */
      { "async", Cmd{ true, []( Command_List cmds ){
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);

        Print<Client>{ fct::show( fct::unlines( cmds ) ) };
      }}},

      /**
       * TEST: Ask local server for primes
       * ex: primes <num>
       */
      { "primes", Cmd{ true, []( Command_List cmds ){
        ULong num = cmds.empty() ? 100 : fct::min(
            ULLONG_MAX, std::stoull( toStdStr( cmds.at( 0 ) ) ) );

        FB message{1024};
        auto metadata = Protocol_Metadata( message );
        auto cmd = Proto::CreatePrimesReq( message, num );

        message.Finish(
            Proto::CreateMessage(
              message,
              metadata,
              Proto::Rest_GET,
              Proto::Proto_PrimesReq,
              cmd.Union()));

        Socket<Snd,Unix>{ message.GetBufferPointer(), message.GetSize() };
      }}},

      /**
       * Shutdown the local server
       * ex: shutdown
       */
      { "shutdown", Cmd{ true, []( Command_List cmds ){
        Print<Client>{ fct::show("Shutting down local server") };

        FB message{1024};

        auto metadata = Protocol_Metadata( message );
        auto cmd = Proto::CreateShutdownReq( message );

        message.Finish(
            Proto::CreateMessage(
              message,
              metadata,
              Proto::Rest_PUT,
              Proto::Proto_ShutdownReq,
              cmd.Union()));

        Socket<Snd,Unix>{ message.GetBufferPointer(), message.GetSize() };
      }}},

      /**
       * TEST: Send request to local server
       *       for nonexistent endpoint
       * ex: bad
       */
      { "bad", Cmd{ true, []( Command_List cmds ){
        FB message{1024};

        auto metadata = Protocol_Metadata( message );
        auto cmd = Proto::CreateShutdownReq( message );

        message.Finish(
            Proto::CreateMessage(
              message,
              metadata,
              Proto::Rest_GET,
              Proto::Proto_ShutdownReq,
              cmd.Union()));

        Socket<Snd,Unix>{ message.GetBufferPointer(), message.GetSize() };
      }}},

      /**
       * Exit the local client
       * ex: quit
       */
      { "quit", Cmd{ true, []( Command_List cmds ){ _g.is_running = false; } } },
    };
  }};

  #ifdef Command_List
    #undef Command_List
  #endif

} // namespace Comms

#endif
