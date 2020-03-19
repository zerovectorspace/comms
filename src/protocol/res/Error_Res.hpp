#ifndef COMMS_ERROR_RESPONSE_HEADER
#define COMMS_ERROR_RESPONSE_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  struct Error_Res : Proto_Route<Error_Res>
  {
    auto pst() -> Route::Error
    {
      using namespace Comms::Proto;

      auto msg = static_cast<const ErrorRes*>(peer->message->command());
      Print<Client>{ toStr( msg->error()->str() ) };

      return Route::Error::Success;
    }
  };
} // namespace Comms

#endif
