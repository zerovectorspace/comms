#ifndef COMMS_HELPER_HEADER
#define COMMS_HELPER_HEADER

#include "src/Alias.hpp"

/********************************************************************************
 *                                   Helper
 ********************************************************************************/

namespace Comms
{
  // operator "" _s :: const Char* -> std::size_t -> String
  auto operator "" _s( const Char* x, std::size_t s ) -> String
  {
    return String{ x, x + s };
  }

  // toStdStr :: String -> StdStr
  auto toStdStr( String const& xs ) -> StdStr
  {
    return StdStr{ begin( xs ), end( xs ) };
  }

  // toStr :: StdStr -> String
  auto toStr( StdStr const& x ) -> String
  {
    return String{ begin( x ), end( x ) };
  }

  // head :: [T] -> T
  template <typename T, template <typename> typename Cont>
  auto head( Cont<T> const& xs ) -> T
  {
    return *( begin( xs ) );
  }

  // tail :: [T] -> [T]
  template <typename T, template <typename> typename Cont>
  auto tail( Cont<T> const& xs ) -> Cont<T>
  {
    return Cont<T>{ begin( xs ) + 1, end( xs ) };
  }

  // words :: String -> [String]
  auto words( String const& xs ) -> Vec<String>
  {
    Vec<String> out{};

    auto a = begin( xs );
    auto b = end( xs );
    auto it = a;

    for ( ; it != b ; advance( it, 1 ) )
    {
      if ( *it == ' ' )
      {
        auto tmp = String{ a , it };

        out.push_back( tmp );
        a = it + 1;
        while( *a == ' ' )
          a++;
      }
    }

    auto last = String{ a, b };
    if ( ! last.empty() )
      out.push_back( String{ a, b } );

    return out;
  }

  void sleep_for( int s )
  {
    std::this_thread::sleep_for( std::chrono::milliseconds( s ) );
  }

  std::stringstream time_now()
  {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream time_str;
    time_str << std::put_time( &tm, "%F %T" ) << " ";

    return time_str;
  }

  template <typename Scale>
  auto time_since_epoch()
  {
    using namespace std::chrono;
    return duration_cast<Scale>(system_clock::now().time_since_epoch()).count();
  }


} // namespace Comms

#endif
