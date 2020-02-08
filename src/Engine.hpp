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

  template <> struct Engine<Client,Loop> { Engine() {
    std::thread{ [](){
      while ( _g.is_running )
      {
        if ( _g.socket_unix == -1 )
          Socket<Connect,Unix>{};

        Socket<Poll>{};

        for ( auto& [socket, peer] : _g.peer_map )
        {
          Exec<
            Socket<Dispatch>
          >{}( peer );
        }

        Socket<Garbage>{};
      }
    }}.detach();

    while ( _g.is_running )
    {
      Exec<
        Event<Poll,Win>,
        Command<Client,Dispatch>,
        Render<Buffers,Asc>,
        Render<Swap>
      >{}();
    }
  }};

  template <> struct Engine<Client,Init> { Engine() {
    Exec<
      Socket<Init>,
      Prog_Win<Init>,
      Font<Init>,
      Command<Init>,
      VWin<Init>,
      Render<Background>,
      Engine<Client,Loop>,
      Socket<Close>,
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
          Socket<Dispatch>,
          Print<Peer,Buffers>
         // Protocol<Decode>
          // Crypto<Decode>
          // Command<Rcv,Dispatch>
          // Command<Snd,Dispatch>
        >{}( peer );

        peer.buffer_stat = Buffer_Status::EMPTY;
      }

      Socket<Garbage>{};
    }
  }};

  template <> struct Engine<Server,Init> { Engine() {
    Exec<
      Socket<Init>,
      Socket<Listen,Unix>,
      Engine<Server,Loop>,
      Socket<Close>
    >{}();
  }};

} // namespace Comms

#endif
