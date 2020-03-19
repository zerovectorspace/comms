#ifndef COMMS_SHUTDOWN_CMD_HEADER
#define COMMS_SHUTDOWN_CMD_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  /**
   * Shutdown the local server
   * ex: shutdown
   */
  struct Shutdown_Cmd : Command_Route<Shutdown_Cmd>
  {
    auto run() -> void
    {
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
    }
  };
} // namespace Comms

#endif
