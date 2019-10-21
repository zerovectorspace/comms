#ifndef COMMS_BUFFER_HEADER
#define COMMS_BUFFER_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Buffer { Buffer() {

  }};

  // Get current cursor position
  template <> struct Buffer<Cursor,Pos> { Buffer() {
    _g.vwin->curs.pos.x = _g.pad_x;
    for ( auto const& c : *_g.vwin->buf )
    {
      Character ch = _g.chrs[ c ];
      _g.vwin->curs.pos.x += (ch.adv >> 6);
    }
  }};

  template <> struct Buffer<New_Line> { Buffer() {
    _g.vwin->bufs.push_back( String{} );
    _g.vwin->buf = &_g.vwin->bufs.back();

    _g.vwin->curs.pos.x = _g.pad_x;
    _g.vwin->curs.pos.y = _g.pad_y;

    _g.vwin->redraw = true;
  }};

  template <> struct Buffer<Backspace> { Buffer() {
    UInt min_ch = (_g.vwin->mode == MODE::Command) ? _g.cmd_prompt.size() : 0;

    if ( _g.vwin->buf->size() == min_ch )
      return;

    _g.vwin->buf->pop_back();

    Buffer<Cursor,Pos>{};

    _g.vwin->redraw = true;
  }};

  template <> struct Buffer<Char> { Buffer( Char ch ) {
    _g.vwin->buf->push_back( ch );
  }};

  template <> struct Buffer<String> { Buffer( String const& ch ) {
    _g.vwin->buf->insert( _g.vwin->buf->begin(), ch.begin(), ch.end() );

    Buffer<Cursor,Pos>{};
  }};
}

#endif
