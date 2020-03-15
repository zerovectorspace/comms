#ifndef COMMS_PROTOCOL_HEADER
#define COMMS_PROTOCOL_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  template <typename... Ts> struct Protocol { Protocol() {

  }};

  template <> struct Protocol<Decode> { Protocol( Peer& p ) {
    if ( p.buffer_stat == Buffer_Status::EMPTY ) { return; }

    p.message = Proto::GetMessage( p.buffer.data() );
  }};

  template <> struct Protocol<Rcv,Dispatch> { Protocol( Peer& p ) {
    if ( p.buffer_stat == Buffer_Status::EMPTY ) { return; }

    using namespace Comms::Proto;

    auto pr = _g.proto_comms.find( p.message->command_type() );
    if ( pr == _g.proto_comms.end() ) {
      fct::print( "That command doesn't exist" );
      return; }

    ErrorStatus err = ErrorStatus::UnauthAccess;
    auto route = pr->second();
    switch ( p.message->request() )
    {
      case Rest_GET:
        err = route->get( &p );
        break;
      case Rest_PUT:
        err = route->put( &p );
        break;
      case Rest_POST:
        err = route->pst( &p );
        break;
      case Rest_UPDATE:
        err = route->upd( &p );
        break;
      case Rest_DELETE:
        err = route->del( &p );
        break;
    }

    if ( err != ErrorStatus::Success )
    {
      FB message{1024};
      auto metadata = Protocol_Metadata( message );
      auto cmd = Proto::CreateErrorRes( message, message.CreateString( show(err).data() ) );

      message.Finish(
          Proto::CreateMessage(
            message,
            metadata,
            Proto::Rest_POST,
            Proto::Proto_ErrorRes,
            cmd.Union()));

      auto b = message.GetBufferPointer();
      ULong s = message.GetSize();
      p.buffer.reserve(s);

      p.buffer = String{ b, (b+s) };
      p.message_size = message.GetSize();
      p.buffer_stat = Buffer_Status::OUTPUT;
    }
  }};

  template <typename EndPoint_T>
  U_ptr<Route_Base> route()
  {
    return std::make_unique<EndPoint_T>();
  }

  template <> struct Protocol<Init> { Protocol() {
    using namespace Comms::Proto;
    _g.proto_comms = Proto_Map{
      { Proto_ErrorRes, route<Error_Res> },
      { Proto_ShutdownReq, route<Shutdown_Req> },
      { Proto_PrimesReq, route<Primes_Req> },
      { Proto_PrimesRes, route<Primes_Res> }
    };
  }};
} // namespace Comms

#endif
