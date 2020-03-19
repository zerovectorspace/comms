#ifndef COMMS_PROTO_ROUTE_HEADER
#define COMMS_PROTO_ROUTE_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  template <typename Child>
  class Proto_Route : public Route
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
      auto get( Peer* p ) -> Route::Error
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->get();
        return ret;
      }

      auto put( Peer* p ) -> Route::Error
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->put();
        return ret;
      }

      auto pst( Peer* p ) -> Route::Error
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->pst();
        return ret;
      }

      auto upd( Peer* p ) -> Route::Error
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->upd();
        return ret;
      }

      auto del( Peer* p ) -> Route::Error
      {
        peer = p;
        auto ret = static_cast<Child*>(this)->del();
        return ret;
      }

    private:
    /******************** Default API ********************/
      auto get() -> Route::Error
      {
        return Route::Error::NotFound;
      }

      auto put() -> Route::Error
      {
        return Route::Error::NotFound;
      }

      auto pst() -> Route::Error
      {
        return Route::Error::NotFound;
      }

      auto upd() -> Route::Error
      {
        return Route::Error::NotFound;
      }

      auto del() -> Route::Error
      {
        return Route::Error::NotFound;
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
