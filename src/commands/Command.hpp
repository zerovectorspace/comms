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
      auto& cmd = pr->second;

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
        std::thread( [ cmd_name, args, &cmd ]()
        {
          cmd.cmd->run( std::move( cmd_name ), std::move( args ) );
        } ).detach();
      }
      else
      {
        cmd.cmd->run( std::move( cmd_name ), std::move( args ) );
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

  #define add_command( name, async, command ) \
            _g.lcl_comms.emplace( name, \
              Cmd{ async, U_ptr<Command_Base>(new command()) } );

  /**
   * Command<Init> :: Define command maps
   */
  template <> struct Command<Init> { Command() {
    add_command( "quit"     , false , Quit_Cmd );
    add_command( "help"     , false , Help_Cmd );
    add_command( "win"      , false , Win_Cmd );
    add_command( "clear"    , false , Clear_Cmd );
    add_command( "view"     , false , View_Cmd );
    add_command( "async"    , true  , Async_Cmd );
    add_command( "primes"   , true  , Primes_Cmd );
    add_command( "shutdown" , false , Shutdown_Cmd );
    add_command( "bad"      , false , Bad_Cmd );
  }};

  #ifdef add_command
    #undef add_command
  #endif
} // namespace Comms

#endif
