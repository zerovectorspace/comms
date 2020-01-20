#ifndef COMMS_EVENT_HEADER
#define COMMS_EVENT_HEADER

#include "src/Taglist.hpp"
#include "src/components/Prog_Win.hpp"
#include "src/components/Render.hpp"

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
        _g.vwin->redraw = true;

        break;
      case SDL_WINDOWEVENT_RESTORED:
        _g.vwin->redraw = true;

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
      {
        if ( _g.vwin->mode == MODE::Text_Input )
        {
          Buffer<New_Line>{};
          break;
        }

        /*
         * Process Command
         */
        _g.vwin->mode = MODE::Command_Exec;

        break;
      }

      case SDLK_BACKSPACE:
        Buffer<Backspace>{};

        break;
    }
  }};

  template <> struct Event<Text> { Event( SDL_Event const& ev ) {
    Char ch = fct::Str( ev.text.text )[0];

    /*
     * Handle Keyboard shortcuts
     */
    
    // Start Command
    if ( _g.vwin->buf->empty() && ch == ':' )
    {
      Buffer<Backspace>{};
      Buffer<String>{ _g.cmd_prompt };

      _g.vwin->redraw = true;
      _g.vwin->mode = MODE::Command_Input;

      return;
    }

    Buffer<Char>{ ch };
    Render<Char>{ ch, _g.vwin->curs.pos };
  }};

  // Event Polling :: handles input
  template <> struct Event<Poll> { Event() {
    SDL_Event ev;

    while ( _g.is_running && SDL_PollEvent( &ev ) )
    {
      switch( ev.type )
      {
        case SDL_QUIT:
          _g.is_running = false;

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
} // namespace Comms

#endif
