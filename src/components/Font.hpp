#ifndef COMMS_FONT_HEADER
#define COMMS_FONT_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Font { Font() {

  }};

  template <> struct Font<Load> { Font() {
    if ( FT_New_Face( _glob.ft_lib, _glob.ft_file.c_str(), 0, &_glob.ft_face ) )
    {
      fct::print( "ERROR: FT_New_Face" );
      return;
    }
      
    FT_Set_Pixel_Sizes( _glob.ft_face, 0, _glob.font_size );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); // Disable byte-alignment restriction
      
    for ( GLubyte i = 0; i < 128; i++ )
    {
      // Load character glyph 
      if ( FT_Load_Char( _glob.ft_face, i, FT_LOAD_RENDER ) )
      {
        fct::print( "ERROR: FT_Load_Char" );
        continue;
      }

      // Generate texture
      UInt texture;
      glGenTextures( 1, &texture );
      glBindTexture( GL_TEXTURE_2D, texture );
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        _glob.ft_face->glyph->bitmap.width,
        _glob.ft_face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        _glob.ft_face->glyph->bitmap.buffer
      );

      // Set texture options
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

      // Now store character for later use
      Character chr = {
        texture, 
        glm::ivec2( _glob.ft_face->glyph->bitmap.width, _glob.ft_face->glyph->bitmap.rows ),
        glm::ivec2( _glob.ft_face->glyph->bitmap_left, _glob.ft_face->glyph->bitmap_top ),
        _glob.ft_face->glyph->advance.x
      };

      _glob.chrs.insert( std::pair<Char, Character>( i, chr ) );
    }
    glBindTexture( GL_TEXTURE_2D, 0 );

    FT_Done_Face( _glob.ft_face );
  }};

  // Font :: init
  template <> struct Font<Init> { Font() {
    if ( FT_Init_FreeType( &_glob.ft_lib ) )
    {
      fct::print( "ERROR: FT_Init_FreeType" );
      return;
    }

    Font<Load>{};
  }};
}

#endif
