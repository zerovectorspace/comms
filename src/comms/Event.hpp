#ifndef COMMS_EVENT_HEADER
#define COMMS_EVENT_HEADER

#include "Taglist.hpp"
#include "Prog_Win.hpp"
#include "Render.hpp"

namespace Comms
{
  template <typename... Ts> struct Event { Event() {

  }};

  template <> struct Event<Win> { Event( SDL_Event const& ev ) {
    switch ( ev.window.event )
    {
      case SDL_WINDOWEVENT_ENTER:
      case SDL_WINDOWEVENT_FOCUS_GAINED:
      case SDL_WINDOWEVENT_TAKE_FOCUS:
      case SDL_WINDOWEVENT_RESIZED:
        Exec<Prog_Win<Dimensions>,
             Prog_Win<Viewport>,
             Prog_Win<Projection>>{}();
        _glob.redraw = true;

        break;
      case SDL_WINDOWEVENT_RESTORED:
        _glob.redraw = true;

        break;
      case SDL_WINDOWEVENT_FOCUS_LOST:
        SDL_Event f;
        f.type = SDL_WINDOWEVENT;
        f.window.event = SDL_WINDOWEVENT_FOCUS_GAINED;
        SDL_WaitEvent(&f);

        break;
    }
  }};

  template <> struct Event<Keydown> { Event( SDL_Event const& ev ) {
    switch ( ev.key.keysym.sym )
    {
      case SDLK_LALT:
      case SDLK_RALT:
      case SDLK_CAPSLOCK:
      case SDLK_LSHIFT:
      case SDLK_RSHIFT:
      case SDLK_LCTRL:
      case SDLK_RCTRL:
      case SDLK_LGUI:
      case SDLK_RGUI:
        break;
      case SDLK_BACKSPACE:
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

        break;
      default:
        auto ch_name = StdString{ SDL_GetKeyName( ev.key.keysym.sym ) };
        Char ch = '\0';

        if ( ch_name == StdString{ "Space" } )
          ch = ' ';
        else if ( ch_name == StdString{ "Return" } )
        {
          _glob.bufs.push_back( String{} );
          _glob.buf = &_glob.bufs.back();

          _glob.curs.pos.x = _glob.pad_x;
          _glob.curs.pos.y = _glob.pad_y;

          _glob.redraw = true;

          break;
        }
        else
          ch = ch_name.at( 0 );

        auto mods = SDL_GetModState();
        if ( (mods & KMOD_LSHIFT) || (mods & KMOD_RSHIFT) )
        {
          switch ( ev.key.keysym.sym )
          {
            case SDLK_0:
              ch = ')';
              break;
            case SDLK_1:
              ch = '!';
              break;
            case SDLK_2:
              ch = '@';
              break;
            case SDLK_3:
              ch = '#';
              break;
            case SDLK_4:
              ch = '$';
              break;
            case SDLK_5:
              ch = '%';
              break;
            case SDLK_6:
              ch = '^';
              break;
            case SDLK_7:
              ch = '&';
              break;
            case SDLK_8:
              ch = '*';
              break;
            case SDLK_9:
              ch = '(';
              break;
            case SDLK_EQUALS:
              ch = '+';
              break;
            case SDLK_MINUS:
              ch = '_';
              break;
            case SDLK_QUOTE:
              ch = '"';
              break;
            case SDLK_PERIOD:
              ch = '>';
              break;
            case SDLK_COMMA:
              ch = '<';
              break;
            case SDLK_SEMICOLON:
              ch = ':';
              break;
            case SDLK_SLASH:
              ch = '?';
              break;
          }
        }
        else
          ch = (mods & KMOD_CAPS) ? ch : fct::toLower( ch );

        _glob.buf->push_back( ch );
        Render<Char>{ ch, _glob.curs.pos };

        break;
    }
  }};
}

#endif
