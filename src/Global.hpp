#ifndef COMMS_GLOBAL_HEADER
#define COMMS_GLOBAL_HEADER

#include "src/Data.hpp"

namespace Comms
{
  using Command_Map      = std::unordered_map<StdStr, void(*)( Vec<String>&& )>;
  using Peer_Map         = std::unordered_map<Socket_Num, Peer>;
  using Char_Texture_Map = std::unordered_map<Char, Character>;
  using VWindow_Map      = std::unordered_map<StdStr, VWindow>;

  struct Global
  {
    /***
     * Command maps
     */
    Command_Map lcl_comms{};
    Command_Map win_comms{};
    Command_Map net_comms{};

    /***
     * Peer_Map :: Handle to `Peer`s
     */
    Peer_Map peer_map{};

    /***
     * Unix Socket
     */
    Socket_Num socket_unix = -1;
    String socket_unix_filename = "comms.socket"_s;
    Socket_Num socket_udp{};

    /***
     * OpenGL
     */
    Int win_w = 800;
    Int win_h = 600;
    SDL_Window*   win = nullptr;
    SDL_Renderer* ren = nullptr;
    UInt gl_prog{};
    UInt text_VAO{};
    UInt text_VBO{};

    bool is_running = true;
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

    /***
     * Buffer :: Holds all text
     */
    Vec<String> bufs = { String{} };
    String* buf = &bufs.back();

    /***
     * Command prompt
     */
    String cmd_prompt = "cmd> "_s;

    /***
     * Input mode
     */
    MODE mode = MODE::Text_Input;

    /***
     * Cursor
     */
    Cursor curs{};

    /***
     * VWins :: Holds all VWindows
     */
    VWindow_Map vwins{};
    VWindow* vwin = nullptr;
  };

  /***
   * Global data object
   */
  Global _g{};
} // namespace Comms

#endif
