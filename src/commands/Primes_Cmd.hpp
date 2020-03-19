#ifndef COMMS_PRIMES_CMD_HEADER
#define COMMS_PRIMES_CMD_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  /**
   * TEST: Ask local server for primes
   * ex: primes <num>
   */
  struct Primes_Cmd : Command_Route<Primes_Cmd>
  {
    auto run() -> void
    {
      ULong num = args.empty() ? 100 : fct::min(
          ULLONG_MAX, std::stoull( toStdStr( args.at( 0 ) ) ) );

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
    }
  };
} // namespace Comms

#endif
