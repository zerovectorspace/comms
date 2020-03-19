#ifndef COMMS_COMMAND_ROUTE_HEADER
#define COMMS_COMMAND_ROUTE_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  #define Arg_List std::vector<std::vector<char>>

  template <typename Child>
  class Command_Route : public Command_Base
  {
    public:
      /**
       * command name
       */
      String name;

      /**
       * List of arguments to command
       */
      Vec<String> args;

      /**
       * The window the command was executed
       */
      VWindow* win;

    /******************** Public API ********************/
      auto run( String c, Arg_List al = Vec<String>{} ) -> void
      {
        name = std::move(c);
        args = std::move(al);
        win  = _g.vwin;

        static_cast<Child*>(this)->run();
      }

    private:
    /******************** Default API ********************/
      auto run() -> void
      {
        fct::print( "Command doesn't exist", ' ' );
        fct::print( name );
      }
  };

  #ifdef Command_List
    #undef Command_List
  #endif
} // namespace Comms

#endif
