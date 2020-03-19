#ifndef COMMS_CLEAR_CMD_HEADER
#define COMMS_CLEAR_CMD_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  /**
   * Clear the screen
   * Delete everything in the VWindow buffer
   */
  struct Clear_Cmd : Command_Route<Clear_Cmd>
  {
    auto run() -> void
    {
      Buffer<Clear,All>{};
    }
  };
} // namespace Comms

#endif
