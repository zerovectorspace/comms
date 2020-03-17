#ifndef COMMS_PRIMES_RESPONSE_HEADER
#define COMMS_PRIMES_RESPONSE_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  struct Primes_Res : Proto_Route<Primes_Res>
  {
    auto pst() -> ErrorStatus
    {
      using namespace Comms::Proto;

      auto msg = static_cast<const PrimesRes*>(peer->message->command());
      auto xs = Vec<ULong>{msg->primes()->begin(), msg->primes()->end()};

      Print<Client>{ peer->win, fct::show(xs) };

      return ErrorStatus::Success;
    }
  };
} // namespace Comms

#endif
