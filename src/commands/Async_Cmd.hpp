#ifndef COMMS_ASYNC_CMD_HEADER
#define COMMS_ASYNC_CMD_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  /**
   * TEST: Print user input after 2 seconds
   * ex: async this is printed back
   */
  struct Async_Cmd : Command_Route<Async_Cmd>
  {
    auto run() -> void
    {
      sleep_for(3000);

      fct::print( args );
      Print<Client>{ win, fct::show( fct::unwords( args ) ) };
    }
  };
} // namespace Comms

#endif
