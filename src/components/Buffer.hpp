#ifndef COMMS_BUFFER_HEADER
#define COMMS_BUFFER_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  struct Buffer_Lock
  {
    std::mutex* m = nullptr;
    
    Buffer_Lock() : Buffer_Lock( _g.vwin )
    { }

    Buffer_Lock( VWindow* w ) : m( w->mut.get() )
    {
      m->lock();
    }

    virtual ~Buffer_Lock()
    {
      m->unlock();
    }
  };

  template <typename... Ts> struct Buffer { Buffer() {

  }};

  // Get current cursor position
  template <> struct Buffer<Cursor,Pos> {
    Buffer() : Buffer( _g.vwin )
    { }

    Buffer( VWindow* win )
    {
      win->curs.pos.x = _g.pad_x;
      for ( auto const& c : *win->buf )
      {
        Character ch = _g.chrs[ c ];
        win->curs.pos.x += (ch.adv >> 6);
      }
    }
  };

  template <> struct Buffer<Clear> : Buffer_Lock { Buffer() {
    _g.vwin->buf->clear();
    _g.vwin->redraw = true;
    _g.vwin->curs.pos.x = _g.pad_x;
    _g.vwin->mode = MODE::Text_Input;
  }};

  template <> struct Buffer<Clear,All> : Buffer_Lock { Buffer() {
    _g.vwin->bufs.clear();
    _g.vwin->bufs.push_back( String{} );
    _g.vwin->buf = &_g.vwin->bufs.back();

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
    _g.vwin->redraw = _g.vwin->redraw ? true : redraw;
  }};

  template <> struct Buffer<String> : Buffer_Lock { Buffer( String const& s, Bool redraw = false ) {
    _g.vwin->buf->insert( _g.vwin->buf->begin(), s.begin(), s.end() );

    Buffer<Cursor,Pos>{};

    _g.vwin->redraw = redraw;
  }};

  template <> struct Buffer<Lines> {
    Buffer( VWindow* win, String const& s, Bool redraw = false ) {
      Buffer_Lock{ win };

      Vec<String> lines = fct::lines( s );
      Vec<String>& bufs = win->bufs;
      String* buf = &bufs.back();
      
      /**
       * If there is something being typed in
       *   save it and place it at the end
       * If the line is empty
       *   discard it
       */
      String last_line{};
      if ( ! buf->empty() ) { last_line = *buf; }

      /**
       * Remove last line in buffer
       * Keeps line spacing uniform
       */
      bufs.pop_back();

      for ( String l : lines )
        bufs.push_back( l );
      bufs.push_back( last_line );

      win->buf = &bufs.back();

      Buffer<Cursor,Pos>{ win };

      _g.vwin->redraw = redraw;
    }

    Buffer( String const& s, Bool redraw = false )
      : Buffer( _g.vwin, s, redraw )
    { }
  };
} // namespace Comms

#endif
