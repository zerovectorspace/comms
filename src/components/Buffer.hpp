#ifndef COMMS_BUFFER_HEADER
#define COMMS_BUFFER_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Buffer { Buffer() {

  }};

  // Get current cursor position
  template <> struct Buffer<Cursor,Pos> { Buffer() {
    _g.curs.pos.x = _g.pad_x;
    for ( auto const& c : *_g.buf )
    {
      Character ch = _g.chrs[ c ];
      _g.curs.pos.x += (ch.adv >> 6);
    }
  }};

  template <> struct Buffer<New_Line> { Buffer() {
    _g.bufs.push_back( String{} );
    _g.buf = &_g.bufs.back();

    _g.curs.pos.x = _g.pad_x;
    _g.curs.pos.y = _g.pad_y;

    _g.redraw = true;
  }};

  template <> struct Buffer<Backspace> { Buffer() {
    UInt min_ch = (_g.mode == MODE::Command) ? _g.cmd_prompt.size() : 0;

    if ( _g.buf->size() == min_ch )
      return;

    _g.buf->pop_back();

    Buffer<Cursor,Pos>{};

    _g.redraw = true;
  }};

  template <> struct Buffer<Char> { Buffer( Char ch ) {
    _g.buf->push_back( ch );
  }};

  template <> struct Buffer<String> { Buffer( String const& ch ) {
    _g.buf->insert( _g.buf->begin(), ch.begin(), ch.end() );

    Buffer<Cursor,Pos>{};
  }};
}

#endif
