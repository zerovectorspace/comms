#ifndef COMMS_DATA_HEADER
#define COMMS_DATA_HEADER

#include "Helper.hpp"

/********************************************************************************
 *                                    Data
 ********************************************************************************/

namespace Comms
{
  /*
   * Connection :: Network Connection Information
   */
  struct Connection
  {
    /* ip:port */
    Vec<String> hostname{};
    struct pollfd poll_socket;
  };

  /*
   * Peer :: Handle to Connection for comms
   */
  struct Peer
  {
    // Used to protect mutable state
    std::mutex mut{};

    // Holds Connection Information for Peer
    U_ptr<Connection> conn{};

    //Buffer for sending and recieving
    Buffer buffer{ "EMPTY"_s };
  };

  /*
   * User :: A meat space user
   */
  struct User
  {
    String pw{};
  };
}

#endif
