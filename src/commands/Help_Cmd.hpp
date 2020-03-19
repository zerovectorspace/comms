#ifndef COMMS_HELP_CMD_HEADER
#define COMMS_HELP_CMD_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  /**
   * Show help
   * List all commands available to GUI client
   * ex: help
   */
  struct Help_Cmd : Command_Route<Help_Cmd>
  {
    auto run() -> void
    {
      Program<Commands>{};
    }
  };
} // namespace Comms

#endif
