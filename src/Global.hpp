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
    GLuint gl_prog{};
    GLuint VAO{};
    GLuint VBO{};
    glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));

    bool is_running = true;
    bool redraw = false;

    /***
     * FreeType Fonts
     */
    FT_Library ft_lib{};
    FT_Face ft_face{};
    Char const* ft_file = "/usr/share/fonts/TTF/DejaVuSansMono.ttf";
    UInt font_size = 14;
    glm::vec3 font_color = { 0.921, 0.859, 0.698 };

    Char_Texture_Map chrs;

    /***
     * Buffer :: Holds all text
     */
    Vec<String> bufs = { String{} };
    String* buf = &bufs.back();
;

    /***
     * Cursor
     */
    Cursor curs{};
  };

  /***
   * Global data object
   */
  Global _glob{};
}

#endif
