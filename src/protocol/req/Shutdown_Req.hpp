#ifndef COMMS_SHUTDOWN_REQUEST_HEADER
#define COMMS_SHUTDOWN_REQUEST_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  struct Shutdown_Req : Proto_Route<Shutdown_Req>
  {
    auto put() -> Route::Error
    {
      fct::print( "Protocol Version: ", ' ' );
      fct::print(peer->message->metadata()->version()->major(), ' ');
      fct::print(peer->message->metadata()->version()->minor(), ' ');
      fct::print(peer->message->metadata()->version()->fix());

      fct::print( "Shutting Down" );

      _g.is_running = false;

      return Route::Error::Success;
    }
  };
} // namespace Comms

#endif
