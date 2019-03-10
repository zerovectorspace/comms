#ifndef COMMS_COMMANDS_EXEC_HEADER
#define COMMS_COMMANDS_EXEC_HEADER

#include "../Global.hpp"

namespace Comms
{
  /*
   * Exec :: A factory class for Functors.
   *      :: Mimics composition of functions
   */
  template <typename... Ts> struct Exec {

    // Used to hold command in container
    Exec() {}

    template <typename... Args>
    Exec( Args&&... args )
    {
      (*this)( args... );
    }

    template <typename... Args>
    auto operator () ( Args&&... args )
    {
      ( Ts{ args... } , ... );
    }

    auto operator () ( Peer* p )
    {
      // Protect peer from modification in other threads
      ScopeLock lk{ p->mut };

      // Call each Functor in sequence
      ( Ts{}( p ), ... );
    }

    auto operator () ()
    {
      ( Ts{} , ... );
    }
  };

  // Try_Exec :: wraps an anonymous lamba with try/catch blocks
  struct Try_Exec {
    template <typename F> 
    Try_Exec( F f ) {
      try {
        f();
      } catch ( ... ) {
        auto time_str = time_now();
        time_str << "[Try_Exec] [ERROR]";

        View.appendln( "error", time_str.str().c_str() );
        View.refresh();
      }
    }
  
    template <typename F, typename G> 
    Try_Exec( F f, G g ) {
      try {
        f();
      } catch ( ... ) {
        g();
      }
    }
  };
}

#endif
