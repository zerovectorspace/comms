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

  // toStdStr :: String -> StdString
  auto toStdStr( String const& xs ) -> StdString
  {
    return StdString{ begin( xs ), end( xs ) };
  }

  // toStr :: StdString -> String
  auto toStr( StdString const& x ) -> String
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

  std::stringstream time_now()
  {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream time_str;
    time_str << std::put_time( &tm, "%F %T" ) << " ";

    return time_str;
  }
} // namespace Comms

#endif
