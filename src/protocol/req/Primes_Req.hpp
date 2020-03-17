#ifndef COMMS_PRIMES_REQUEST_HEADER
#define COMMS_PRIMES_REQUEST_HEADER

#include "src/components/Include.hpp"

namespace Comms
{
  struct Primes_Req : Proto_Route<Primes_Req>
  {
    auto get() -> ErrorStatus
    {
      using namespace Comms::Proto;

      auto msg = static_cast<const PrimesReq*>(peer->message->command());

      try
      {
        ULong num = msg->number();

        auto fs = Vec<Bool>( num, true );
        ULong n = fct::sqrt( num );

        for ( ULong i = 2 ; i < n ; i++ )
        {
          if ( fs[i] )
          {
            ULong k = 0;
            while (true)
            {
              ULong j = i * i + k * i;
              if ( j > num )
                break;

              fs[ j ] = false;
              k += 1;
            }
          }
        }

        Vec<ULong> out{};
        for ( ULong i = 2 ; i < fs.size() ; i++ )
        {
          if ( fs[i] )
          {
            out.push_back( i );
          }
        }

        FB message{1024};
        auto metadata = Protocol_Metadata( message );
        auto cmd = CreatePrimesRes( message, message.CreateVector( out ) );

        message.Finish(
            CreateMessage(
              message,
              metadata,
              Rest_POST,
              Proto_PrimesRes,
              cmd.Union()));

        auto b = message.GetBufferPointer();
        ULong s = message.GetSize();
        peer->buffer.reserve(s);

        peer->buffer = String{ b, (b+s) };
        peer->message_size = message.GetSize();

        has_response = true;
        sleep_for(5000);
      }
      catch(...)
      {
        fct::print( "ERROR: primes <n> st 0 <= n <= ULLONG_MAX"_s );
        return ErrorStatus::InvalidInput;
      }

      return ErrorStatus::Success;
    }
  };
} // namespace Comms

#endif
