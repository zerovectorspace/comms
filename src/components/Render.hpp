#ifndef COMMS_RENDER_HEADER
#define COMMS_RENDER_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Render { Render() {

  }};

  template <> struct Render<Win,Background> { Render() {
    glEnable( GL_SCISSOR_TEST );
    glScissor( 0, 0, _g.win_w, _g.win_h );

    glClearColor( 0.157f, 0.157f, 0.157f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glDisable( GL_SCISSOR_TEST );
  }};

  template <> struct Render<Swap> { Render() {
    SDL_GL_SwapWindow( _g.win );
  }};

  template <> struct Render<Char> { Render( Char const c, UVec2& pos, Vec3 const& color = _g.font_color ) {
    Character ch = _g.chrs[ c ];

    Float xpos = pos.x + ch.bearing.x;
    Float ypos = pos.y - (ch.size.y - ch.bearing.y);

    Float w = ch.size.x;
    Float h = ch.size.y;

    // Draw square
    Float vertices[6][4] = {
      { xpos,     ypos + h,   0.0, 0.0 },            
      { xpos,     ypos,       0.0, 1.0 },
      { xpos + w, ypos,       1.0, 1.0 },

      { xpos,     ypos + h,   0.0, 0.0 },
      { xpos + w, ypos,       1.0, 1.0 },
      { xpos + w, ypos + h,   1.0, 0.0 }           
    };

    glUseProgram( _g.gl_prog );

    // color -> gl( text_color )
    glUniform3f( glGetUniformLocation( _g.gl_prog, "text_color" ), color.x, color.y, color.z );
    glBindVertexArray( _g.text_VAO );

    // Render glyph texture over quad
    glBindTexture( GL_TEXTURE_2D, ch.texture_id );

    // Update content of _g.VBO memory
    glBindBuffer( GL_ARRAY_BUFFER, _g.text_VBO );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices );

    // Render quad
    glDrawArrays( GL_TRIANGLES, 0, 6 );

    pos.x += (ch.adv >> 6);
  }};

  template <> struct Render<Buffers,Dec> { Render() {
  }};

  template <> struct Render<Buffers,Asc> { Render() {
    if ( ! _g.vwin->redraw )
      return;

    Render<Win,Background>{};

    UVec2 pos = { _g.pad_x, _g.pad_y };

    for ( Int i = _g.vwin->bufs.size() - 1 ; i >= 0 ; i-- )
    {
      // Set cursor back to the left
      pos.x = _g.pad_x;

      for ( auto const& ch : _g.vwin->bufs[ i ] )
      {
        Render<Char>{ ch, pos };
      }

      // Go to next line
      pos.y += _g.line_height;

      if ( (Int) pos.y > _g.win_h )
        break;
    }

    _g.vwin->redraw = false;
  }};
} // namespace Comms

#endif
