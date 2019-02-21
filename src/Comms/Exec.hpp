#ifndef COMMS_COMMANDS_EXEC_HEADER
#define COMMS_COMMANDS_EXEC_HEADER

#include "../Global.hpp"

namespace Comms
{
  /*
   * Exec :: A factory class for Functors.
   *      :: Mimics composition of functions
   */
  template <typename... Ts> struct Exec { Exec() {}
    template <typename... Args>
    Exec( Args... args )
    {
      (*this)( args... );
    }

    template <typename... Args>
    auto operator () ( Args&&... args )
    {
      std::cout << "Exec::operator()( Args )\n";
      ( Ts{ args... } , ...);
    }

    auto operator () ( Peer* p )
    {
      std::cout << "Exec::operator()( Peer )\n";
      // Protect peer from modification in other threads
      ScopeLock lk{ p->mut };

      // Call each Functor in sequence
      ( Ts{}( p ), ... );
    }

    auto operator () ()
    {
      std::cout << "Exec::operator()()\n";
      ( Ts{} , ... );
    }
  };

  // Try_Exec :: wraps an anonymous lamba with try/catch blocks
  template <typename F> struct Try_Exec { Try_Exec( F f ) {
    try {
      f();
    } catch ( ... ) {
      auto time_str = time_now();
      time_str << "[Error] Not enough arguments";

      View.print( "error", time_str.str().c_str() );
    }
  }};
}

#endif
