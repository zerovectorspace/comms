#ifndef COMMS_COMMANDS_WINDOW_HEADER
#define COMMS_COMMANDS_WINDOW_HEADER

#include "Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Window { Window() {
    auto win = "error";

    if ( ! View.get_window( win ) )
      View.new_win( win, View.width(), View.height() - 1, 0, 0, false );

    std::stringstream out;
    out = time_now();
    out << "[Window::default] ";

    /**
     * Append -> Refresh
     * Updates when viewing this window
     */
    View.appendln( win, out.str().c_str() );
    View.refresh();
  }};

  template <> struct Window<Open> { Window( StdString const& win, bool is_writable = true ) {
    if ( View.get_window( win ) )
      View.refresh( win );
    else
    {
      View.new_win( win, View.width(), View.height() - 1, 0, 0, is_writable );
      View.fresh_print( win, "" );
    }

    View.current_win = win ;
  }};

  template <> struct Window<Del> { Window( StdString const& win ) {
    View.del_win( win );

    View.refresh( "main" );
    View.current_win = "main" ;
  }};

  template <> struct Window<List> { Window() {
    auto win = ".win_list";
    View.current_win = win;

    if ( ! View.get_window( win ) )
      View.new_win( win, View.width(), View.height() - 1, 0, 0, false );

    StdString winlist{};

    int i = 1;
    for ( auto const& w : View.win_list() )
    {
      if ( w.at( 0 ) == '.' )
        continue;

      winlist += std::to_string( i ) + ". " + w + "\n";
      i++;
    }

    View.fresh_print( win, winlist.data() );
  }};
}

#endif
