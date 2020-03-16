#ifndef COMMS_BUFFER_HEADER
#define COMMS_BUFFER_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  struct Buffer_Lock
  {
    Buffer_Lock()
    {
      _g.vwin->mut->lock();
    }

    virtual ~Buffer_Lock()
    {
      _g.vwin->mut->unlock();
    }
  };

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

  template <> struct Buffer<Clear> : Buffer_Lock { Buffer() {
    _g.vwin->buf->clear();
    _g.vwin->redraw = true;
    _g.vwin->curs.pos.x = _g.pad_x;
    _g.vwin->mode = MODE::Text_Input;
  }};

  template <> struct Buffer<New_Line> : Buffer_Lock { Buffer( Bool redraw = true ) {
    _g.vwin->bufs.push_back( String{} );
    _g.vwin->buf = &_g.vwin->bufs.back();

    _g.vwin->curs.pos.x = _g.pad_x;
    _g.vwin->curs.pos.y = _g.pad_y;

    _g.vwin->redraw = redraw;
  }};

  template <> struct Buffer<Backspace> : Buffer_Lock { Buffer() {
    UInt min_ch = (_g.vwin->mode == MODE::Command_Input) ? _g.cmd_prompt.size() : 0;

    if ( _g.vwin->buf->size() == min_ch )
      return;

    _g.vwin->buf->pop_back();

    Buffer<Cursor,Pos>{};

    _g.vwin->redraw = true;
  }};

  template <> struct Buffer<Char> : Buffer_Lock { Buffer( Char ch, Bool redraw = false ) {
    _g.vwin->buf->push_back( ch );
    _g.vwin->redraw = redraw;
  }};

  template <> struct Buffer<String> : Buffer_Lock { Buffer( String const& s, Bool redraw = false ) {
    _g.vwin->buf->insert( _g.vwin->buf->begin(), s.begin(), s.end() );

    Buffer<Cursor,Pos>{};

    _g.vwin->redraw = redraw;
  }};

  template <> struct Buffer<Lines> : Buffer_Lock { Buffer( String const& s, Bool redraw = false ) {
    Vec<String> lines = fct::lines( s );

    for ( String l : lines )
      _g.vwin->bufs.push_back( l );
    _g.vwin->bufs.push_back( String{} );

    _g.vwin->buf = &_g.vwin->bufs.back();

    Buffer<Cursor,Pos>{};

    _g.vwin->redraw = redraw;
  }};
} // namespace Comms

#endif
