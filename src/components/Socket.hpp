#ifndef COMMS_SOCKET_HEADER
#define COMMS_SOCKET_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Socket { Socket() {

  }};

  template <> struct Socket<Listen,TCP> { Socket() {

  }};

  template <> struct Socket<Connect,TCP> { Socket() {

  }};

  template <> struct Socket<Listen,Unix> { Socket() {
    struct sockaddr_un local;
    memset( &local, 0, sizeof( local ) );

    local.sun_family = AF_UNIX;
    strncpy( local.sun_path,
             _g.socket_unix_filename.data(),
             _g.socket_unix_filename.size() );

    int new_socket;
    if ( (new_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 )
      return;

    try
    {
      unlink( _g.socket_unix_filename.data() );

      if ( bind( new_socket, (struct sockaddr*) &local, sizeof( local ) ) == -1 )
          throw errno;
      if ( listen( new_socket, 10 ) == -1 )
          throw errno;
    }
    catch ( int error )
    {
      fct::print( fct::Str( std::strerror(error) ) );
    }

    fct::print( "Listening on local unix socket", ' ' );
    fct::print( new_socket );

    _g.socket_fds.push_back( pollfd{ new_socket, POLLIN | POLLHUP | POLLERR, 0 } );
    _g.socket_unix = new_socket;

    auto [pr, _] =
      _g.peer_map.emplace( new_socket,
           Peer{
             nullptr,
             Connection{ {}, &_g.socket_fds.back() },
             {} } );

    /**
     * Must set U_ptr after initialization
     *   Mutex has deleted copy assignment
     */
    pr->second.mut.reset( new std::mutex() );
  }};

  template <> struct Socket<Connect,Unix> { Socket() {
    struct sockaddr_un remote;
    memset( &remote, 0, sizeof( remote ) );

    remote.sun_family = AF_UNIX;
    strncpy( remote.sun_path,
             _g.socket_unix_filename.data(),
             _g.socket_unix_filename.size() );

    int new_socket;
    if ((new_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
      return;

    try
    {
      if ( connect( new_socket, (struct sockaddr*) &remote, sizeof(remote) ) == -1 )
        throw errno;
    }
    catch ( int error )
    {
      fct::print( fct::Str( std::strerror(error) ) );
      return;
    }

    fct::print( "Connected to local unix socket", ' ' );
    fct::print( _g.socket_unix_filename );

    _g.socket_fds.push_back( pollfd{ new_socket, POLLIN | POLLHUP | POLLERR, 0 } );
    _g.socket_unix = new_socket;

    auto [pr, _] =
      _g.peer_map.emplace( new_socket,
           Peer{
             nullptr,
             Connection{ {}, &_g.socket_fds.back() },
             {} } );

    /**
     * Must set U_ptr after initialization
     *   Mutex has deleted copy assignment
     */
    pr->second.mut.reset( new std::mutex() );

    // send( new_socket, "hello", 6, 0 );
  }};

  template <> struct Socket<Accept,Unix> { Socket() {
    int new_socket;
    struct sockaddr_un remote;

    try
    {
        socklen_t s = sizeof(remote);
        if ((new_socket = accept(_g.socket_unix, (struct sockaddr*) &remote, &s)) == -1)
            throw errno;
    }
    catch (int error)
    {
      fct::print( fct::Str( std::strerror(error) ) );
      return;
    }

    fct::print( "Accepted to connection on ", ' ' );
    fct::print( _g.socket_unix_filename );

    _g.socket_fds.push_back( pollfd{ new_socket, POLLIN | POLLHUP | POLLERR, 0 } );

    auto [pr, _] =
      _g.peer_map.emplace( new_socket,
           Peer{
             nullptr,
             Connection{ {}, &_g.socket_fds.back() },
             {} } );

    /**
     * Must set U_ptr after initialization
     *   Mutex has deleted copy assignment
     */
    pr->second.mut.reset( new std::mutex() );

    fct::print( "New Local client", ' ');
    fct::print( new_socket );
  }};

  template <> struct Socket<Poll> { Socket() {
    int ret = poll( _g.socket_fds.data(), _g.socket_fds.size(), 500 );

    if ( ret < 0 )
    {
      // _g.is_running = false;
      return;
    }
  }};

  template <> struct Socket<Dispatch> { Socket( Peer& p ) {
    auto& socket = *p.conn.poll_socket;

    if ( socket.revents & POLLIN )
    {
      if ( socket.fd == _g.socket_unix )
      {
        // Accept local connection
        Socket<Accept,Unix>{};
      }
      else if ( socket.fd == _g.socket_udp )
      {
        // Receive UDP packet
        // fct::print( "Received UDP packet" );
      }
      else
      {
        // Receive TCP packet
        // fct::print( "Received TCP packet" );
      }
    }

    if ( socket.revents & POLLOUT )
    {
      // Nothing
      // fct::print( "Received POLLOUT" );
    }

    if ( socket.revents & POLLHUP )
    {
      // Received a hang up
      // fct::print( "Received POLLHUP" );
    }

    if ( socket.revents & POLLERR )
    {
      // Received an error
      // fct::print( "Received POLLERR" );
    }
  }};

  template <> struct Socket<Poll,Dispatch> { Socket() {
    Socket<Poll>{};

    for ( auto& [socket, peer] : _g.peer_map )
    {
      Socket<Dispatch>{ peer };
    }
  }};


} // namespace Comms

#endif
