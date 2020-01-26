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
    struct sockaddr_un sock;
    memset( &sock, 0, sizeof( sock ) );

    sock.sun_family = AF_UNIX;
    strncpy( sock.sun_path,
             _g.socket_unix_filename.data(),
             _g.socket_unix_filename.size() );

    if ( (_g.socket_unix = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 )
      return;

    try
    {
      unlink( _g.socket_unix_filename.data() );

      if ( bind( _g.socket_unix, (struct sockaddr*) &sock, sizeof( sock ) ) == -1 )
          throw errno;
      if ( listen( _g.socket_unix, 10 ) == -1 )
          throw errno;
    }
    catch (int error)
    {
      fct::print( fct::Str( std::strerror(error) ) );
    }

    auto [pr, _] =
      _g.peer_map.emplace( _g.socket_unix,
           Peer{
             nullptr,
             Connection{
               {},
               pollfd{
                 _g.socket_unix,
                 POLLIN | POLLHUP | POLLERR,
                 0 } },
             {} } );

    /**
     * Must set U_ptr after initialization
     *   Mutex has deleted copy assignment
     */
    pr->second.mut.reset( new std::mutex() );
  }};

  template <> struct Socket<Connect,Unix> { Socket() {

  }};
} // namespace Comms

#endif
