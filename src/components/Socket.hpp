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
      fct::print( "[ERROR] Socket<Listen,Unix> ", ' ' );
      fct::print( fct::Str( std::strerror(error) ) );
    }

    fct::print( "Listening on local unix socket", ' ' );
    fct::print( new_socket );

    _g.socket_unix = new_socket;

    auto& p_fd = _g.poll_fds.emplace_back(
        pollfd{ new_socket, POLLIN | POLLHUP | POLLERR, 0 } );

    auto [pr, _] =
      _g.peer_map.emplace( new_socket,
           Peer{
             nullptr,
             Connection{ Connection::Type::LOCAL, {}, false, &p_fd },
             Peer::Buffer_Status::EMPTY,
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
    {
      fct::print( "[ERROR] Socket<Connect,Unix> Can't create socket" );
      exit(1);
    }

    static uint connection_tries = 0;
    try
    {
      if ( connection_tries > 0 ) { sleep_for(5000); }

      if ( connect( new_socket, (struct sockaddr*) &remote, sizeof(remote) ) == -1 )
      {
        throw errno;
      }

      connection_tries = 0;
    }
    catch ( int error )
    {
      fct::print( "[ERROR] Socket<Connect,Unix> " );
      connection_tries += 1;

      return;
    }

    fct::print( "Connected to local unix socket", ' ' );
    fct::print( new_socket );

    _g.socket_unix = new_socket;

    auto& p_fd = _g.poll_fds.emplace_back(
        pollfd{ new_socket, POLLIN | POLLHUP | POLLERR, 0 } );

    auto [pr, _] =
      _g.peer_map.emplace( new_socket,
           Peer{
             nullptr,
             Connection{ Connection::Type::LOCAL, {}, false, &p_fd },
             Peer::Buffer_Status::EMPTY,
             {},
             nullptr,
             0,
             _g.vwin } );

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
      fct::print( "[ERROR] Socket<Accept,Unix> ", ' ' );
      fct::print( fct::Str( std::strerror(error) ) );
      return;
    }

    auto& p_fd = _g.poll_fds.emplace_back(
        pollfd{ new_socket, POLLIN | POLLHUP | POLLERR, 0 } );

    auto [pr, _] =
      _g.peer_map.emplace( new_socket,
           Peer{
             nullptr,
             Connection{ Connection::Type::LOCAL, {}, false, &p_fd },
             Peer::Buffer_Status::EMPTY,
             {} } );

    /**
     * Must set U_ptr after initialization
     *   Mutex has deleted copy assignment
     */
    pr->second.mut.reset( new std::mutex() );

    fct::print( "Accepted new connection on", ' ' );
    fct::print( _g.socket_unix );

    fct::print( "New Local client", ' ');
    fct::print( new_socket );
  }};

  template <> struct Socket<Snd,Unix> { Socket( void* b, UInt s ) {
    fct::print( "Socket<Snd,Unix>: ", ' ' );
    fct::print( _g.socket_unix );
    send( _g.socket_unix, b, s, 0 );
  }};

  template <> struct Socket<Snd,TCP> {
    Socket( Socket_Num n, void* b, UInt s ) {
      send( n, b, s, 0 );
    }

    Socket( Peer& p ) {
      if ( p.buffer_stat != Peer::Buffer_Status::OUTPUT ) { return; }

      send( p.conn.poll_fd->fd, p.buffer.data(), p.buffer.size(), 0 );
    }
  };

  template <> struct Socket<Rcv,TCP> { Socket( Peer& p ) {
    Vec<Char> buffer;
    // Int total = 0;
    Int num_bytes = 0;
    auto socket = p.conn.poll_fd->fd;

    fcntl( socket, F_SETFL, O_NONBLOCK );

    auto BUFFER_SIZE = 4096;

    buffer.resize( BUFFER_SIZE, '\0' );
    p.buffer.clear();
    do
    {
      if (buffer.front() != '\0')
        buffer.resize( BUFFER_SIZE, '\0' );

      num_bytes = (int) recv( socket, buffer.data(), BUFFER_SIZE, 0 );

      p.buffer.reserve( p.buffer.size() );

      /*
       * Search to one before the end
       * if not found, return end
       * else return one after the
       * first null term byte found
       */
      // auto second_null_byte = std::find( buffer.begin(), buffer.end() - 1, '\0' );

      if ( num_bytes > 0 )
      {
        p.buffer.insert( p.buffer.end(), buffer.begin(), buffer.begin() + num_bytes );
      }

      /*
       * We need this in case we fill up the buffer
       * If there is still data in the last position
       * of buffer, then we still need a '\0' at the
       * end
       */
      // if ( second_null_byte == buffer.end() - 1 )
          // p.buffer.push_back( '\0' );

    } while ( (recv( socket, buffer.data(), BUFFER_SIZE, MSG_PEEK) > 0) && num_bytes > 0 );

    p.buffer_stat = Peer::Buffer_Status::INPUT;
  }};

  template <> struct Socket<Poll> { Socket() {
    int ret = poll( _g.poll_fds.data(), _g.poll_fds.size(), 500 );

    if ( ret < 0 )
    {
      _g.is_running = false;
      return;
    }
  }};

  template <> struct Socket<Dispatch> { Socket( Peer& p ) {
    auto& socket = *(p.conn.poll_fd);

    if ( socket.revents & POLLIN )
    {
      if ( _g.app == App::SERVER && socket.fd == _g.socket_unix )
      {
        // Accept local connection
        Socket<Accept,Unix>{};
      }
      else if ( socket.fd == _g.socket_udp )
      {
        // Receive UDP packet
        fct::print( "Received UDP packet on socket", ' ' );
        fct::print( socket.fd );
      }
      else
      {
        // Receive TCP packet
        fct::print( "Received TCP Packet on socket", ' ' );
        fct::print( socket.fd );
        Socket<Rcv,TCP>{ p };
      }
    }

    if ( socket.revents & POLLOUT )
    {
      // Nothing
      fct::print( "Received POLLOUT on socket" );
      fct::print( socket.fd );
    }

    if ( socket.revents & POLLHUP )
    {
      // Received a hang up
      p.conn.is_garbage = true;
      fct::print( "Received POLLHUP on socket", ' ' );
      fct::print( socket.fd );
    }

    if ( socket.revents & POLLERR )
    {
      // Received an error
      fct::print( "Received POLLERR on socket", ' ' );
      fct::print( socket.fd );
    }
  }};

  template <> struct Socket<Close> { Socket() {
    for ( auto const& pr : _g.peer_map )
    {
      auto& socket = pr.first;
      shutdown( socket, SHUT_RDWR );
      close( socket );
    }
  }};

  template <> struct Socket<Garbage> { Socket() {
    auto sockets = Vec<Socket_Num>{}; 
    Bool ref_changed = false;

    for ( auto& [ s, p ] : _g.peer_map )
    {
      if ( ! p.conn.is_garbage )
        continue;

      ref_changed = true;

      /*
       * Keep track of garbage
       *   to remove from Peer_Map
       */
      sockets.push_back( s );
      fct::print( "Removing socket", ' ' );
      fct::print( s );

      std::erase_if( _g.poll_fds, [&p]( pollfd& p_fd ){
        return p.conn.poll_fd->fd == p_fd.fd;
      });

      // Set clients unix socket to closed
      _g.socket_unix = _g.socket_unix == s ? -1 : _g.socket_unix;
    }

    /*
     * Close sockets
     */
    for ( auto const& s : sockets )
    {
      shutdown( s, SHUT_RDWR );
      close( s );
      _g.peer_map.erase( s );
    }

    if ( ! ref_changed )
      return;

    /*
     * Copy pollfds to Peer_Map due to
     *   reference invalidation from mutation
     */
    for ( auto& p_fd : _g.poll_fds )
      _g.peer_map.at( p_fd.fd ).conn.poll_fd = &p_fd;
  }};

  template <> struct Socket<Init> { Socket() {
    _g.poll_fds.reserve( 1024 );
  }};

} // namespace Comms

#endif
