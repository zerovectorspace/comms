#include "View.hpp"

#include <algorithm>
#include <cstring>
#include <sstream>

View::~View()
{
    endwin();
}

void View::get_win_dims()
{
    getmaxyx(stdscr, h, w);
}

void View::init()
{
    initscr();
    get_win_dims();

    start_color();
    use_default_colors();
}

void View::new_win(const StdString& name, const int& w, const int& h, const int& x, const int& y, bool is_writable)
{
    auto winbuf = std::make_unique<WinBuf>();
    winbuf->window = newwin(h, w, y, x);
    winbuf->w = w;
    winbuf->h = h;
    winbuf->is_writable = is_writable;

    window_map.emplace(name, std::move(winbuf));
}

void View::del_win(const StdString& win)
{
    delwin(window_map.at(win)->window);
    window_map.erase(win);
}

void View::refresh(const StdString& win)
{
    auto w = window_map.at(win).get();

    wclear(w->window);

    auto i = w->buffer.begin();
    for ( ; i != w->buffer.end() ; i++ )
    {
      wprintw(w->window, i->data());
      wprintw(w->window, "\n");
    }

    wrefresh(w->window);
}

void View::refresh()
{
  View::refresh( ".cl" );
  View::refresh( current_win );
}

void View::print(const StdString& win, const char* buf)
{
    auto w = window_map.at(win).get();
    appendln(win, buf);

    wclear(w->window);

    auto i = w->buffer.begin();
    for ( ; i != w->buffer.end() ; i++ )
    {
      wprintw(w->window, i->data());
      wprintw(w->window, "\n");
    }

    wrefresh(w->window);
}

void View::fresh_print(const StdString& win, const char* buf)
{
    auto w = window_map.at(win).get();
    w->buffer.clear();
    wclear(w->window);

    for ( int j = 0 ; j < w->h ; j++ )
      appendln(win, "");

    appendln(win, buf);

    auto i = w->buffer.begin();
    for ( ; i != w->buffer.end() ; i++ )
      wprintw(w->window, i->data());

    wrefresh(w->window);
}

void View::fresh_print(const char* buf)
{
    fresh_print( current_win, buf );
}

// void View::flush(const StdString& win)
// {
    // auto w = window_map.at(win).get();

    // print(win, w->buffer.data());
// }

// void View::fresh_flush(const StdString& win)
// {
    // auto w = window_map.at(win).get();

    // fresh_print(win, w->buffer.data());
// }

void View::append(const StdString& win, const char* buf)
{
    auto w = window_map.at(win).get();

    w->buffer.push_back( Vec<char>{ buf, buf + std::strlen(buf) + 1 } );

    if ( w->buffer.size() > (uint) w->h )
      w->buffer.pop_front();
}

void View::appendln(const StdString& win, const char* buf)
{
    auto w = window_map.at(win).get();

    w->buffer.push_back( Vec<char>{ buf, buf + std::strlen(buf) + 1 } );

    if ( w->buffer.size() > (uint) w->h )
      w->buffer.pop_front();
}

void View::clear_buffer(const StdString& win)
{
    auto w = window_map.at(win).get();

    w->buffer.clear();
}

void View::clear_win(const StdString& win)
{
    auto w = window_map.at(win).get();

    wclear(w->window);
    wprintw(w->window, "");
    wrefresh(w->window);
}

void View::move(const StdString& win, const int& x, const int& y)
{
    auto w = window_map.at(win).get();

    wmove(w->window, y, x);
}

char View::get_char(const StdString& win)
{
    auto w = window_map.at(win).get();
    char ch = wgetch(w->window);

    return ch;
}

Vec<char> View::get_line(const StdString& win, bool with_zero)
{
    auto w = window_map.at(win).get();
    Vec<char> input(w->w, '\n');

    wgetnstr(w->window, input.data(), w->w);

    /*
     * Get only entered characters, include trailing null char
     */
    return Vec<char>{input.begin(), std::find(input.begin(), input.end(), '\0') + with_zero};
}

Vec<char> View::get_multi_line(const StdString& win)
{
    auto w = window_map.at(win).get();
    Vec<char> input(4096, '\0');

    wgetnstr(w->window, input.data(), 4096);

    /*
     * Get only entered characters, include trailing null char
     */
    //return Vec<char>{input.begin(), std::find(input.begin(), input.end(), '\0') + 1};
    return input;
}

WinBuf* View::get_window(const StdString& win)
{
    if ( auto p = window_map.find( win ) ; p != window_map.end() )
      return p->second.get();

    return nullptr;
}

int View::width()
{
    return w;
}

int View::height()
{
    return h;
}

bool View::is_writable( StdString const& win )
{
  return window_map.at( win )->is_writable;

}

bool View::is_writable()
{
  return window_map.at( current_win )->is_writable;

}

Vec<StdString> View::win_list()
{
  Vec<StdString> output{};

  for ( auto const& p : window_map )
  {
    output.push_back( p.first );
  }

  return output;
}
