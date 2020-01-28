#ifndef COMMS_DATA_HEADER
#define COMMS_DATA_HEADER

#include "src/Helper.hpp"

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
    Bool is_garbage = false;
    struct pollfd* poll_socket = nullptr;
  };

  enum class Buffer_Status
  {
    EMPTY, INPUT, OUTPUT
  };

  /*
   * Peer :: Handle to Connection for comms
   */
  struct Peer
  {
    // Used to protect mutable state
    U_ptr<std::mutex> mut{};

    // Holds Connection Information for Peer
    Connection conn{};

    // Buffer state for recv and send
    Buffer_Status buffer_stat = Buffer_Status::EMPTY;

    //Buffer for sending and recieving
    String buffer{ "EMPTY"_s };
  };

  /*
   * User :: A meat space user
   */
  struct User
  {
    String pw{};
  };

  /*
   * Character :: Holds texture of character
   */
  struct Character {
    UInt     texture_id;
    IVec2 size;
    IVec2 bearing;
    Long       adv;
  };

  /*
   * Cursor :: Position text is written
   */
  struct Cursor {
    UVec2 pos = { 5, 585 };
  };

  /**
   * Input Modes
   */
  enum class MODE 
  {
    Command_Input,
    Command_Exec,
    KB_Shortcut,
    Text_Input
  };

  /*
   * VWindow :: Virtual Window
   * 
   * Covers the entire window, holds Panes
   */
  struct VWindow
  {
    bool redraw = false;

    /***
     * Cursor
     *
     * Cursor will be in the pane in the future
     */
    Cursor curs{};

    /***
     * Buffer :: Holds all text
     */
    Vec<String> bufs = { String{} };
    String* buf = &bufs.back();

    /***
     * Input mode
     */
    MODE mode = MODE::Text_Input;
  };
} // namespace Comms

#endif
