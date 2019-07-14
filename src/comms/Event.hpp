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
      case SDLK_RETURN:
        if ( ! _glob.is_command )
        {
          Buffer<New_Line>{};
          break;
        }

        // Handle Command here

        _glob.buf->clear();
        _glob.redraw = true;
        _glob.is_command = false;
        _glob.curs.pos.x = _glob.pad_x;

        break;

      case SDLK_BACKSPACE:
        Buffer<Backspace>{};

        break;

      case SDLK_SEMICOLON:
        // Colon is not the first character on line
        if ( (_glob.buf->size()) || ! (SDL_GetModState() & KMOD_SHIFT) )
          break;

        Buffer<Backspace>{};
        Buffer<Char>{ '>' };
        Render<Char>{ '>', _glob.curs.pos };
        Buffer<Char>{ ' ' };
        Render<Char>{ ' ', _glob.curs.pos };

        _glob.redraw = true;
        _glob.is_text = false;
        _glob.is_command = true;


        break;
    }
  }};

  template <> struct Event<Text> { Event( SDL_Event const& ev ) {
    if ( ! _glob.is_text )
    {
      _glob.is_text = true;
      return;
    }

    Char ch = fct::Str( ev.text.text )[0];

    Buffer<Char>{ ch };
    Render<Char>{ ch, _glob.curs.pos };
  }};

  // Event Polling :: handles input
  template <> struct Event<Poll> { Event() {
    SDL_Event ev;

    while ( _glob.is_running && SDL_PollEvent( &ev ) )
    {
      switch( ev.type )
      {
        case SDL_QUIT:
          _glob.is_running = false;

          break;

        case SDL_WINDOWEVENT:
          Event<Win>{ ev };

          break;

        case SDL_KEYDOWN:
          Event<Keydown>{ ev };

          break;

        case SDL_TEXTINPUT:
          Event<Text>{ ev };

          break;
      }
    }
  }};
}

#endif
