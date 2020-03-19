#ifndef COMMS_VIEW_CMD_HEADER
#define COMMS_VIEW_CMD_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  /**
   * View information on various session components
   * ex: view win
   */
  struct View_Cmd : Command_Route<View_Cmd>
  {
    auto run() -> void
    {
      if ( args.empty() )
        return;

      for ( auto const& a : args )
      {
        if ( a == "win"_s )
        {
          String out = "-------------- windows ---------------\n"_s;

          for ( auto const& [name, vwin] : _g.vwins )
          {
            out.insert( out.end(), name.begin(), name.end() );
            out.push_back( '\n' );
          }

          String tmp = "--------------------------------------"_s;
          out.insert( out.end(), tmp.begin(), tmp.end() );

          Print<Client>{ std::move( out ) };
        }
      }
    }
  };
} // namespace Comms

#endif
