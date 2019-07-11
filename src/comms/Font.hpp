#ifndef COMMS_FONT_HEADER
#define COMMS_FONT_HEADER

#include "Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Font { Font() {

  }};

  template <> struct Font<Load> { Font( Char const* file ) {
    FT_Face face;
    if ( FT_New_Face( _glob.ft_lib, file, 0, &face ) )
    {
      fct::print( "ERROR: FT_New_Face" );
      return;
    }
      
    FT_Set_Pixel_Sizes( face, 0, 48 );
  }};

  // Font :: init
  template <> struct Font<Init> { Font( Char const* file ) {
    if ( FT_Init_FreeType( &_glob.ft_lib ) )
    {
      fct::print( "ERROR: FT_Init_FreeType" );
      return;
    }

    Font<Load>{ file };
  }};
}

#endif
