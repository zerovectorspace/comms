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
      Buffer<Lines>{ fct::toStr( cmd.insert( 0, "\n*** Command not found: " ) ) };
    }

    _g.vwin->mode = MODE::Text_Input;
  }};

  // Command<Init> :: Define command maps
  template <> struct Command<Init> { Command() {
    _g.lcl_comms.emplace( "help", Cmd{ false, []( Command_List cmds ){ Program<Commands>{}; } } );

    _g.lcl_comms.emplace( "win", Cmd{ false, []( Command_List cmds ) {
      if ( cmds.empty() )
      return;

      auto vwin_name = fct::toStdStr( cmds.at( 0 ) );

      VWin<New>{ vwin_name };
    }});

    _g.lcl_comms.emplace( "view", Cmd{ false,[]( Command_List cmds ) {
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
    }});

    _g.lcl_comms.emplace( "async", Cmd{ true, []( Command_List cmds ){
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(2s);

      for ( auto const& cmd : cmds ) 
      Print<Client>{ fct::show( cmd ) };
    }});

    _g.lcl_comms.emplace( "primes", Cmd{ true, []( Command_List cmds ){
      ULong num = 0;

      try
      {
        num = cmds.empty() ? 100 : fct::min( ULLONG_MAX, std::stoull( toStdStr( cmds.at( 0 ) ) ) );

        auto fs = Vec<Bool>( num, true );
        ULong n = fct::sqrt( num );

        for ( ULong i = 2 ; i < n ; i++ )
        {
          if ( fs[i] )
          {
            ULong k = 0;
            while (true)
            {
              ULong j = i * i + k * i;
              if ( j > num )
                break;

              fs[ j ] = false;
              k += 1;
            }
          }
        }

        String out{};
        for ( ULong i = 2, j = 0 ; i < fs.size() ; i++ )
        {
          if ( fs[i] )
          {
            auto i_str = fct::show( i );
            out.reserve( out.size() + i_str.size() + 2 );
            out.insert( out.end(), i_str.begin(), i_str.end() );
            out.push_back( ' ' );

            if ( j == 10 )
            {
              out.push_back( '\n' );
              j = 0;
            }

            j++;
          }
        }
        Print<Client>{ std::move( out ) };
      }
      catch(...)
      {
        Print<Client>{ "ERROR: primes <n> st 0 <= n <= ULLONG_MAX"_s };
        return;
      }
    }});

    _g.lcl_comms.emplace( "quit", Cmd{ true, []( Command_List cmds ){ _g.is_running = false; } } );
  }};

  #ifdef Command_List
    #undef Command_List
  #endif

} // namespace Comms

#endif
