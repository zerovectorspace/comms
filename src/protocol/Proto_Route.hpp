#ifndef COMMS_PROTO_ROUTE_HEADER
#define COMMS_PROTO_ROUTE_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  template <typename Child>
  class Proto_Route : public Route_Base
  {
    public:
    Peer* peer = nullptr;

    /******************** Public API ********************/
      auto get( Peer* p ) -> ErrorStatus
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->get();
        return ret;
      }

      auto put( Peer* p ) -> ErrorStatus
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->put();
        return ret;
      }

      auto pst( Peer* p ) -> ErrorStatus
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->pst();
        return ret;
      }

      auto upd( Peer* p ) -> ErrorStatus
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->upd();
        return ret;
      }

      auto del( Peer* p ) -> ErrorStatus
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->del();
        return ret;
      }

    private:
    /******************** Default API ********************/
      auto error_msg( char const* val ) -> ErrorStatus
      {
        std::cout << "  Endpoint doesn't exist: " << val << "\n";
        return ErrorStatus::NotFound;
      }

      auto get() -> ErrorStatus
      {
        error_msg( "get" );
        return ErrorStatus::NotFound;
      }

      auto put() -> ErrorStatus
      {
        error_msg( "put" );
        return ErrorStatus::NotFound;
      }

      auto pst() -> ErrorStatus
      {
        error_msg( "pst" );
        return ErrorStatus::NotFound;
      }

      auto upd() -> ErrorStatus
      {
        error_msg( "upd" );
        return ErrorStatus::NotFound;
      }

      auto del() -> ErrorStatus
      {
        error_msg( "del" );
        return ErrorStatus::NotFound;
      }
  };

  auto Protocol_Metadata( FB& fb ) -> FO<Comms::Proto::Metadata>
  {
    auto metadata = Proto::CreateMetadata( fb,
        Proto::CreateVersion( fb ),
        time_since_epoch<std::chrono::milliseconds>() );
    return metadata;
  }
} // namespace Comms

#endif
