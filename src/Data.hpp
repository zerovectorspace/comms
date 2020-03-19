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

  /**
   * Forward declare VWindow for Peer
   */
  struct VWindow;

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

    /**
     * The VWindow for the peer
     *
     * This allows us to recieve data
     *   on the correct screen
     */
     VWindow* win = nullptr;
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
   * Forward declar for Cmd
   */
  struct Command_Base;

  /**
   * Command Wrapper
   */
  struct Cmd
  {
    Bool async = false;
    U_ptr<Command_Base> cmd;
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
   * Command_Base
   * Interface for Commands
   */
  struct Command_Base
  {
    /******************** Internal API ********************/
      virtual auto run( String cmd, Vec<String> cmds ) -> void = 0;

      virtual ~Command_Base() {}
  };


  /*
   * Route_Base
   * Interface for Routes
   */
  struct Route
  {
    /*
     * Route::Error
     * Http Request Error Status
     */
    enum class Error
    {
      Success,
      NotFound,
      UnauthAccess,
      InvalidVerb,
      InvalidInput
    };

    /******************** Internal API ********************/
      virtual auto get( Peer* p ) -> Error = 0;

      virtual auto put( Peer* p ) -> Error = 0;

      virtual auto pst( Peer* p ) -> Error = 0;

      virtual auto upd( Peer* p ) -> Error = 0;

      virtual auto del( Peer* p ) -> Error = 0;

      virtual ~Route() {}
  };

  /**
   * Convert Route::Error to String
   *   for fct::print
   */
  auto show( Route::Error e ) -> String
  {
    switch (e)
    {
      case Route::Error::Success:
        return "Route::Error::Success"_s;
        break;
      case Route::Error::NotFound:
        return "Route::Error::NotFound"_s;
        break;
      case Route::Error::UnauthAccess:
        return "Route::Error::UnauthAccess"_s;
        break;
      case Route::Error::InvalidVerb:
        return "Route::Error::InvalidVerb"_s;
        break;
      case Route::Error::InvalidInput:
        return "Route::Error::InvalidInput"_s;
        break;
    }

    return "Route::Error::Unknown"_s;
  }



} // namespace Comms

#endif
