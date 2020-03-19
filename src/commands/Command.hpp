#ifndef COMMS_COMMAND_COMMAND_HEADER
#define COMMS_COMMAND_COMMAND_HEADER

#include "src/protocol/Include.hpp"

namespace Comms
{
  template <typename... Ts> struct Command { Command() {

  }};

  /**
   * Execute command in relevant Command_Map
   */
  template <> struct Command<Client,Dispatch> { Command() {
    if ( _g.vwin->mode != MODE::Command_Exec )
      return;

    /**
     * The words on the command line
     *   excluding the 'cmd>' prompt
     */
    auto ws = fct::tail( fct::words( *(_g.vwin->buf) ) );

    /**
     * Clear the line for the next input
     * This must be done after we've grabbed the line
     *   from the buffer
     */
    Buffer<Clear>{};

    /**
     * No command was entered
     */
    if ( ws.size() == 0 )
      return;

    /**
     * Command name
     * The first word on the line
     */
    auto cmd_name = ws.at( 0 );

    /**
     * Arguments to the command
     * The rest of the words on the line
     */
    auto args = fct::tail( ws );

    if ( auto pr = _g.lcl_comms.find( fct::toStdStr( cmd_name ) ) ; pr != _g.lcl_comms.end() )
    {
      /**
       * Cmd object
       * See Data.hpp
       */
      auto cmd = pr->second;

      /**
       * Create the U_Ptr<Command_Route> object
       * Run the command in a thread or in serial
       */
      if ( cmd.async )
      {
        /**
         * Run the thread with copies of the
         *   command name, arguments, and Cmd object
         *   i.e. These could be destroyed before the thread is run
         */
        std::thread( [ cmd_name, args, cmd ]()
        {
          cmd.make()->run( std::move( cmd_name ), std::move( args ) );
        } ).detach();
      }
      else
      {
        cmd.make()->run( std::move( cmd_name ), std::move( args ) );
      }
    }
    else
    {
      /**
       * The command entered doesn't exist
       * Prompt the user on the GUI
       */
      auto brand = "*** Command not found: "_s;
      cmd_name.insert( cmd_name.begin(), brand.begin(), brand.end() );

      Print<Client>{ std::move( cmd_name ) };
    }

    /**
     * Command has been entered
     * Return to text input mode
     */
    _g.vwin->mode = MODE::Text_Input;
  }};

  /**
   * Factory to create the command
   */
  template <typename EndPoint_T>
  U_ptr<Command_Base> command()
  {
    return std::make_unique<EndPoint_T>();
  }

  /**
   * Command<Init> :: Define command maps
   */
  template <> struct Command<Init> { Command() {
    _g.lcl_comms = Command_Map{
      { "quit"     , Cmd{ false, command<Quit_Cmd> } },
      { "help"     , Cmd{ false, command<Help_Cmd> } },
      { "win"      , Cmd{ false, command<Win_Cmd> } },
      { "clear"    , Cmd{ false, command<Clear_Cmd> } },
      { "view"     , Cmd{ false, command<View_Cmd> } },
      { "async"    , Cmd{ true , command<Async_Cmd> } },
      { "primes"   , Cmd{ false, command<Primes_Cmd> } },
      { "shutdown" , Cmd{ false, command<Shutdown_Cmd> } },
      { "bad"      , Cmd{ false, command<Bad_Cmd> } }
    };
  }};
} // namespace Comms

#endif
