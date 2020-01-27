#ifndef COMMS_VWin_HEADER
#define COMMS_VWin_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct VWin { VWin() {

  }};

  template <> struct VWin<New> { VWin( StdStr const& k ) {
    if ( _g.vwins.count( k ) )
    {
      _g.vwin = &_g.vwins.at( k );
      return;
    }

    auto [vwin, _] = _g.vwins.emplace( k, VWindow{} );
    _g.vwin = &vwin->second;

    _g.vwin->curs.pos = { _g.pad_x, _g.pad_y };
    _g.vwin->redraw = true;
  }};

  template <> struct VWin<Init> { VWin() {
    auto [vwin, _] = _g.vwins.emplace( "1", VWindow{} );
    _g.vwin = &vwin->second;

    _g.vwin->curs.pos = { _g.pad_x, _g.pad_y };
  }};
} // namespace Comms

#endif
