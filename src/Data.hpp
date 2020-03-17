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
    enum class Type
    {
      LOCAL, REMOTE
    };

    Type type = Type::REMOTE;
    /* ip:port */
    Vec<String> hostname{};
    Bool is_garbage = false;
    struct pollfd* poll_fd = nullptr;
  };

  /*
   * Peer :: Handle to Connection for comms
   */
  struct Peer
  {
    enum class Buffer_Status
    {
      EMPTY, INPUT, OUTPUT
    };

    // Used to protect mutable state
    U_ptr<std::mutex> mut{};

    // Holds Connection Information for Peer
    Connection conn{};

    // Buffer state for recv and send
    Buffer_Status buffer_stat = Buffer_Status::EMPTY;

    //Buffer for sending and recieving
    String buffer{ "EMPTY"_s };

    // The Protocol message
    Proto::Message const* message = nullptr;

    // The Protocol message size
    ULong message_size = 0;
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

  /**
   * Command Wrapper
   */
  struct Cmd
  {
    Bool async = false;
    void(*f)( Vec<String>&& );
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

    /*
     * Buffer mutex
     */
    U_ptr<std::mutex> mut = nullptr;

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

  /*
   * ErrorStatus
   * Http Request Error Status
   */
  enum class ErrorStatus
  {
    Success,
    NotFound,
    UnauthAccess,
    InvalidVerb,
    InvalidInput
  };

  auto show( ErrorStatus e ) -> String
  {
    switch (e)
    {
      case ErrorStatus::Success:
        return "ErrorStatus::Success"_s;
        break;
      case ErrorStatus::NotFound:
        return "ErrorStatus::NotFound"_s;
        break;
      case ErrorStatus::UnauthAccess:
        return "ErrorStatus::UnauthAccess"_s;
        break;
      case ErrorStatus::InvalidVerb:
        return "ErrorStatus::InvalidVerb"_s;
        break;
      case ErrorStatus::InvalidInput:
        return "ErrorStatus::InvalidInput"_s;
        break;
    }

    return "ErrorStatus::Unknown"_s;
  }

  /*
   * Route_Base
   * Interface for Routes
   */
  struct Route_Base
  {
    /******************** Internal API ********************/
      virtual auto get( Peer* p ) -> ErrorStatus = 0;

      virtual auto put( Peer* p ) -> ErrorStatus = 0;

      virtual auto pst( Peer* p ) -> ErrorStatus = 0;

      virtual auto upd( Peer* p ) -> ErrorStatus = 0;

      virtual auto del( Peer* p ) -> ErrorStatus = 0;

      virtual ~Route_Base() {}
  };
} // namespace Comms

#endif
