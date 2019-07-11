#ifndef COMMS_GLOBAL_HEADER
#define COMMS_GLOBAL_HEADER

#include "Data.hpp"

namespace Comms
{
  using Command_Map      = std::unordered_map<StdString, void(*)( Vec<String>&& )>;
  using Peer_Map         = std::unordered_map<Socket_Num, U_ptr<Peer>>;
  using Char_Texture_Map = std::unordered_map<GLchar, Character>;

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
     * OpenGL
     */
    SDL_Window*   win = nullptr;
    SDL_Renderer* ren = nullptr;

    bool is_running = true;

    /***
     * FreeType Fonts
     */
    FT_Library ft_lib{};
    FT_Face ft_face{};

    Char_Texture_Map chrs;
  };

  /***
   * Global data object
   */
  Global _glob{};
}

#endif
