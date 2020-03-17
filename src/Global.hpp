#ifndef COMMS_GLOBAL_HEADER
#define COMMS_GLOBAL_HEADER

#include "src/Data.hpp"

namespace Comms
{
  using Command_Map      = std::unordered_map<StdStr, Cmd>;
  using Proto_Map        = std::unordered_map<Proto::Proto, U_ptr<Route_Base> (*)(void)>;
  using Peer_Map         = std::unordered_map<Socket_Num, Peer>;
  using Char_Texture_Map = std::unordered_map<Char, Character>;
  using VWindow_Map      = std::unordered_map<StdStr, VWindow>;

  /**
   * Used to distiguish between Application type
   *
   * Local Server -> interacts with remote servers
   * Local Client -> the gui for user interaction
   */
  enum App {
    SERVER,
    CLIENT
  };

  /**
   * Global application state
   */
  struct Global
  {
    App app = App::SERVER;

    /**
     * Local commands to interact with
     *   the Local Client
     */
    Command_Map lcl_comms{};

    /**
     * Protocol to handle interactions
     *   between the Local Server/Client
     */
    Proto_Map proto_local{};

    /**
     * Protocol to handle interactions
     *   between the Remote Server/Server
     */
    Proto_Map proto_remote{};

    /**
     * Peer_Map :: Handle to `Peer`s
     */
    Peer_Map peer_map{};

    /**
     * Unix Socket
     */
    Vec<pollfd> poll_fds{};

    /**
     * Socket number for the
     *   Local Server/Client
     */
    Socket_Num socket_unix = -1;

    /**
     * Socket file name
     */
    String socket_unix_filename = "comms.socket"_s;

    /**
     * Socket number for the
     *   Server UDP listen socket
     */
    Socket_Num socket_udp{};

    /**
     * Local Client window dimensions
     */
    Int win_w = 800;
    Int win_h = 600;

    /**
     * SDL Window for the Local Client
     */
    SDL_Window*   win = nullptr;

    /**
     * SDL Renderer for the Local Client
     */
    SDL_Renderer* ren = nullptr;

    /**
     * Handle to the OpenGL program
     */
    UInt gl_prog{};

    /**
     * Handle to the OpenGL Vertex Array Object
     */
    UInt text_VAO{};

    /**
     * Handle to the OpenGL Vertex Buffer Object
     */
    UInt text_VBO{};

    /**
     * Semaphore to application running state
     */
    bool is_running = true;

    /**
     * Semaphore to redraw entire window
     */
    bool redraw = false;

    /***
     * FreeType Fonts
     */
    FT_Library ft_lib{};
    FT_Face ft_face{};
    StdStr ft_file = "/usr/share/fonts/TTF/DejaVuSansMono.ttf";
    UInt font_size = 16;
    UInt pad_x = 5;
    UInt pad_y = font_size / 2;
    UInt line_height = font_size + pad_y / 2;
    Vec3 font_color = { 0.921, 0.859, 0.698 };

    Char_Texture_Map chrs;

    /**
     * Command prompt
     */
    String cmd_prompt = "cmd> "_s;

    /**
     * Input mode
     */
    MODE mode = MODE::Text_Input;

    /**
     * Cursor
     */
    Cursor curs{};

    /**
     * Holds all VWindows
     */
    VWindow_Map vwins{};
    
    /**
     * Handle to currently viewed VWindow
     */
    VWindow* vwin = nullptr;
  };

  /**
   * Global data object
   */
  Global _g{};
} // namespace Comms

#endif
