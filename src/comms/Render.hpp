#ifndef COMMS_RENDER_HEADER
#define COMMS_RENDER_HEADER

#include "Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Render { Render() {

  }};

  template <> struct Render<Background> { Render() {
    glClearColor( 0.157f, 0.157f, 0.157f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  }};

  template <> struct Render<Swap> { Render() {
    SDL_GL_SwapWindow( _glob.win );
  }};

  template <> struct Render<Char> { Render( Char const c, glm::uvec2& pos, glm::vec3 const& color = _glob.font_color ) {
    Character ch = _glob.chrs[ c ];

    GLfloat xpos = pos.x + ch.bearing.x;
    GLfloat ypos = pos.y - (ch.size.y - ch.bearing.y);

    GLfloat w = ch.size.x;
    GLfloat h = ch.size.y;

    // Draw square
    GLfloat vertices[6][4] = {
      { xpos,     ypos + h,   0.0, 0.0 },            
      { xpos,     ypos,       0.0, 1.0 },
      { xpos + w, ypos,       1.0, 1.0 },

      { xpos,     ypos + h,   0.0, 0.0 },
      { xpos + w, ypos,       1.0, 1.0 },
      { xpos + w, ypos + h,   1.0, 0.0 }           
    };

    glUseProgram( _glob.gl_prog );

    // color -> gl( text_color )
    glUniform3f( glGetUniformLocation( _glob.gl_prog, "text_color" ), color.x, color.y, color.z );
    glBindVertexArray( _glob.VAO );

    // Render glyph texture over quad
    glBindTexture( GL_TEXTURE_2D, ch.texture_id );

    // Update content of _glob.VBO memory
    glBindBuffer( GL_ARRAY_BUFFER, _glob.VBO );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices );

    // Render quad
    glDrawArrays( GL_TRIANGLES, 0, 6 );

    pos.x += (ch.adv >> 6);
  }};

  template <> struct Render<Buffers,Dec> { Render() {
  }};

  template <> struct Render<Buffers,Asc> { Render() {
    if ( ! _glob.redraw )
      return;

    Render<Background>{};

    glm::uvec2 pos = { _glob.pad_x, _glob.pad_y };

    for ( Int i = _glob.bufs.size() - 1 ; i >= 0 ; i-- )
    {
      // Set cursor back to the left
      pos.x = _glob.pad_x;

      for ( auto const& ch : _glob.bufs[ i ] )
      {
        Render<Char>{ ch, pos };
      }

      // Go to next line
      pos.y += _glob.line_height;

      if ( (Int) pos.y > _glob.win_h )
        break;
    }

    _glob.redraw = false;
  }};
}

#endif
