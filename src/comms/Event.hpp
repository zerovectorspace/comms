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
        Buffer<Backspace>{};

        break;
      default:
        auto ch_name = StdString{ SDL_GetKeyName( ev.key.keysym.sym ) };
        Char ch = '\0';

        if ( ch_name == StdString{ "Space" } )
          ch = ' ';
        else if ( ch_name == StdString{ "Return" } )
        {
          Buffer<New_Line>{};

          _glob.redraw = true;

          break;
        }
        else
          ch = ch_name.at( 0 );

        if ( (Int) _glob.curs.pos.x > _glob.win_w )
        {
          Buffer<New_Line>{};

          _glob.redraw = true;
        }

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

        Buffer<Char>{ ch };
        Render<Char>{ ch, _glob.curs.pos };

        break;
    }
  }};
}

#endif
