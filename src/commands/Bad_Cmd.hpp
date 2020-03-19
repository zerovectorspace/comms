#ifndef COMMS_BAD_CMD_HEADER
#define COMMS_BAD_CMD_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  /**
   * TEST: Send request to local server
   *       for nonexistent endpoint
   * ex: bad
   */
  struct Bad_Cmd : Command_Route<Bad_Cmd>
  {
    auto run() -> void
    {
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
    }
  };
} // namespace Comms

#endif
