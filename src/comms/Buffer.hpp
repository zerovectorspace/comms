#ifndef COMMS_BUFFER_HEADER
#define COMMS_BUFFER_HEADER

#include "Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Buffer { Buffer() {

  }};

  template <> struct Buffer<New_Line> { Buffer() {
    _glob.bufs.push_back( String{} );
    _glob.buf = &_glob.bufs.back();

    _glob.curs.pos.x = _glob.pad_x;
    _glob.curs.pos.y = _glob.pad_y;

    _glob.redraw = true;
  }};

  template <> struct Buffer<Backspace> { Buffer() {
    if ( _glob.buf->size() > 0 )
      _glob.buf->pop_back();

    _glob.redraw = true;

    // Get cursor position
    _glob.curs.pos.x = _glob.pad_x;
    for ( auto const& c : *_glob.buf )
    {
      Character ch = _glob.chrs[ c ];
      _glob.curs.pos.x += (ch.adv >> 6);
    }
  }};

  template <> struct Buffer<Char> { Buffer( Char ch ) {
    _glob.buf->push_back( ch );
  }};
        
}

#endif
