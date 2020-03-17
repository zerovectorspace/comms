#ifndef COMMS_PROTO_ROUTE_HEADER
#define COMMS_PROTO_ROUTE_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  template <typename Child>
  class Proto_Route : public Route_Base
  {
    public:
    /**
     * Handle to local/remote peer for request/response
     */
    Peer* peer = nullptr;

    /**
     * Used to set Peer::Buffer_Status
     */
    Bool has_response = false;

    virtual ~Proto_Route()
    {
      if ( has_response )
      {
        peer->buffer_stat = Peer::Buffer_Status::OUTPUT;
      }
    }

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
      auto get() -> ErrorStatus
      {
        return ErrorStatus::NotFound;
      }

      auto put() -> ErrorStatus
      {
        return ErrorStatus::NotFound;
      }

      auto pst() -> ErrorStatus
      {
        return ErrorStatus::NotFound;
      }

      auto upd() -> ErrorStatus
      {
        return ErrorStatus::NotFound;
      }

      auto del() -> ErrorStatus
      {
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
