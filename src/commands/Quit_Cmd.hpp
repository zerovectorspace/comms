#ifndef COMMS_QUIT_HEADER
#define COMMS_QUIT_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  /**
   * Exit the local client
   * ex: quit
   */
  struct Quit_Cmd : Command_Route<Quit_Cmd>
  {
    auto run() -> void
    {
      _g.is_running = false;
    }
  };
} // namespace Comms

#endif
