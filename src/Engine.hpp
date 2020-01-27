#ifndef COMMS_ENGINE_HEADER
#define COMMS_ENGINE_HEADER

#include "src/commands/Include.hpp"

/********************************************************************************
 *                                   Engine
 ********************************************************************************/

namespace Comms
{
  template <typename... Ts> struct Engine { Engine() {

  }};

  // Parse_Args :: splits input into a command and args
  template <> struct Engine<Parse_Args> { Engine( String& command, Vec<String>& args ) {
    auto w       = words( command );
         args    = tail( w );
         command = head( w );
  }};

  // Main Loop :: high level
  template <> struct Engine<Client,Loop> { Engine() {
    std::thread{ [](){
      while ( _g.is_running )
      {
        Socket<Poll>{};

        for ( auto& [socket, peer] : _g.peer_map )
        {
          Exec<
            Socket<Dispatch>
          >{}( peer );
        }
      }
    }}.detach();

    while ( _g.is_running )
    {
      Exec<
        Event<Poll>,
        Command<Client,Dispatch>,
        Render<Buffers,Asc>,
        Render<Swap>
      >{}();
    }
  }};

  // Engine :: init
  template <> struct Engine<Client,Init> { Engine() {
    Exec<
      Socket<Connect,Unix>,
      Prog_Win<Init>,
      Font<Init>,
      Command<Init>,
      VWin<Init>,
      Render<Background>,
      Engine<Client,Loop>,
      Prog_Win<Kill>
    >{}();
  }};

  template <> struct Engine<Server,Loop> { Engine() {
    while ( _g.is_running )
    {
      Socket<Poll>{};

      for ( auto& [socket, peer] : _g.peer_map )
      {
        Exec<
          Socket<Dispatch>
          // Protocol<Decode>
          // Crypto<Decode>
          // Command<Rcv,Dispatch>
          // Command<Snd,Dispatch>
        >{}( peer );
      }
    }
  }};

  // Engine<Server,Init> :: Initialize the servers
  template <> struct Engine<Server,Init> { Engine() {
    Exec<
      Socket<Listen,Unix>,
      Engine<Server,Loop>
    >{}();
  }};

} // namespace Comms

#endif
