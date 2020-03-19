#ifndef COMMS_WIN_CMD_HEADER
#define COMMS_WIN_CMD_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  /**
   * Move to a different window
   * ex: win <name>
   *
   * Creates a new window if it doesn't exist
   */
  struct Win_Cmd : Command_Route<Win_Cmd>
  {
    auto run() -> void
    {
      if ( args.empty() )
        return;

      auto vwin_name = fct::toStdStr( fct::unwords( args ) );

      VWin<New>{ vwin_name };
    }
  };
} // namespace Comms

#endif
