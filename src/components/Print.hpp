#ifndef COMMS_PRINT_HEADER
#define COMMS_PRINT_HEADER

#include "src/Taglist.hpp"
#include "Buffer.hpp"

namespace Comms
{
  template <typename... Ts> struct Print { Print() {

  }};

  template <> struct Print<Client> { Print( String&& val ) {
      Buffer<Lines>{ val };
      Buffer<New_Line>{};
  }};

  template <> struct Print<Peer,Buffers> { Print( Peer& p ) {
    if ( p.buffer_stat == Buffer_Status::INPUT )
      fct::print( p.buffer );
  }};
} // namespace Comms

#endif
