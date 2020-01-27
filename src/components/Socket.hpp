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

    int new_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (new_socket == -1)
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

  template <> struct Socket<Rcv,TCP> { Socket( Peer& p ) {
    Vec<Char> buffer;
    // Int total = 0;
    Int num_bytes = 0;
    auto socket = p.conn.poll_socket->fd;

    fcntl( socket, F_SETFL, O_NONBLOCK );

    auto BUFFER_SIZE = 4096;

    buffer.resize( BUFFER_SIZE, '\0' );
    p.buffer.clear();
    do
    {
      if (buffer.front() != '\0')
        buffer.resize( BUFFER_SIZE, '\0' );

      num_bytes = (int) recv( socket, buffer.data(), BUFFER_SIZE, 0 );

      p.buffer.reserve( p.buffer.size() + num_bytes + 2 );

      /*
       * Search to one before the end
       * if not found, return end
       * else return one after the
       * first null term byte found
       */
      auto second_null_byte = std::find( buffer.begin(), buffer.end() - 1, '\0' ) + 1;

      p.buffer.insert( p.buffer.end(), buffer.begin(), second_null_byte );

      /*
       * We need this in case we fill up the buffer
       * If there is still data in the last position
       * of buffer, then we still need a '\0' at the
       * end
       */
      if ( second_null_byte == buffer.end() - 1 )
          p.buffer.push_back( '\0' );

      // total += num_bytes;
      // if (total > 16777216) break;

    } while ( (recv( socket, buffer.data(), BUFFER_SIZE, MSG_PEEK) > 0) && num_bytes > 0 );
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
        Socket<Rcv,TCP>{ p };
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

} // namespace Comms

#endif
