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
    GLuint     texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    Long       adv;
  };

  /*
   * Cursor :: Position text is written
   */
  struct Cursor {
    glm::uvec2 pos = { 5, 585 };
  };

  /**
   * Input Modes
   */
  enum class MODE 
  {
    Command,
    KB_Shortcut,
    Text_Input
  };

  /*
   * WPane :: Window pane 
   */
  struct WPane
  {
    bool redraw = false;

    glm::uvec2 pos = { 0, 0 };
    glm::uvec2 dim = { 0, 0 };

    /***
     * Cursor
     */
    Cursor curs{};

    /***
     * Font
     */
    glm::vec4 bg_color = { 0.157f, 0.157f, 0.157f, 1.0f };
    glm::vec3 font_color = { 0.921, 0.859, 0.698 };
    UInt font_size = 16;
    UInt pad_x = 5;
    UInt pad_y = font_size / 4;
    UInt line_height = font_size + pad_y / 2;

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
}

#endif
