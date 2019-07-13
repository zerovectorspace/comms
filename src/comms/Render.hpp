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

  template <> struct Render<Char> { Render( GLchar const c, glm::uvec2& pos, glm::vec3 const& color = _glob.font_color ) {
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

    pos.x += 1 + (ch.adv >> 6);
  }};

  template <> struct Render<Buffers> { Render() {
    if ( ! _glob.redraw )
      return;

    Render<Background>{};

    // Set cursor back to the top
    _glob.curs.pos = { 5, 585 };

    for ( uint i = 0 ; i < _glob.bufs.size() ; i++ )
    {
      // Set cursor back to the left
      _glob.curs.pos.x = 5;
      for ( auto const& ch : _glob.bufs[ i ] )
      {
        Render<Char>{ ch, _glob.curs.pos };
      }

      // Go to next line
      if ( i < _glob.bufs.size() - 1 )
        _glob.curs.pos.y -= 15;
    }

    _glob.redraw = false;
  }};
}

#endif
