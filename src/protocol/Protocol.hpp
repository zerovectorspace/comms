#ifndef COMMS_PROTOCOL_HEADER
#define COMMS_PROTOCOL_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  auto show( Comms::Proto::Proto n ) -> String
  {
    using namespace Comms::Proto;
    String ret{};
    switch (n)
    {
      case Proto_ErrorRes:
        return "ErrorRes"_s;
      case Proto_ShutdownReq:
        return "ShutdownReq"_s;
      case Proto_PrimesReq:
        return "PrimesReq"_s;
      case Proto_PrimesRes:
        return "PrimesRes"_s;
      default:
        return ""_s;
    }
  }

  template <typename... Ts> struct Protocol { Protocol() {

  }};

  template <> struct Protocol<Decode> { Protocol( Peer& p ) {
    if ( p.buffer_stat == Buffer_Status::EMPTY ) { return; }

    p.message = Proto::GetMessage( p.buffer.data() );
  }};

  template <> struct Protocol<Error> { Protocol( Peer& p, ErrorStatus err ) {
      FB message{1024};
      auto metadata = Protocol_Metadata( message );
      String err_str = show( p.message->command_type() );
      String err_brand = "[ERROR] "_s;
      err_str.insert(err_str.begin(), err_brand.begin(), err_brand.end());
      String e = show(err);
      err_str.push_back(' ');
      err_str.insert(err_str.end(), e.begin(), e.end());
      auto cmd = Proto::CreateErrorRes( message, message.CreateString( err_str.data() ) );

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
  }};

  template <> struct Protocol<Rcv,Dispatch> { Protocol( Peer& p ) {
    if ( p.buffer_stat == Buffer_Status::EMPTY ) { return; }

    using namespace Comms::Proto;

    Proto_Map* pm = (p.conn.type == Connection::Type::LOCAL)
                    ? &_g.proto_local : &_g.proto_remote;

    ErrorStatus err = ErrorStatus::UnauthAccess;
    if ( auto pr = pm->find( p.message->command_type() ); pr != pm->end() )
    {
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
    }

    if ( err != ErrorStatus::Success ) { Protocol<Error>{ p, err }; }
  }};

  template <typename EndPoint_T>
  U_ptr<Route_Base> route()
  {
    return std::make_unique<EndPoint_T>();
  }

  template <> struct Protocol<Init> { Protocol() {
    using namespace Comms::Proto;

    _g.proto_local = Proto_Map{
      { Proto_ErrorRes    , route<Error_Res> },
      { Proto_ShutdownReq , route<Shutdown_Req> },
      { Proto_PrimesReq   , route<Primes_Req> },
      { Proto_PrimesRes   , route<Primes_Res> }
    };

    _g.proto_remote = Proto_Map{};

  }};
} // namespace Comms

#endif
