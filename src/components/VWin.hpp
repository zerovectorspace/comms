#ifndef COMMS_VWin_HEADER
#define COMMS_VWin_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct VWin { VWin() {

  }};

  template <> struct VWin<New> { VWin( StdStr const& name ) {
    if ( _g.vwins.count( name ) )
    {
      _g.vwin = &_g.vwins.at( name );
      return;
    }

    auto [vwin, _] = _g.vwins.emplace( name, VWindow{} );
    _g.vwin = &vwin->second;
    _g.vwin->mut = std::make_unique<std::mutex>();

    _g.vwin->curs.pos = { _g.pad_x, _g.pad_y };
    _g.vwin->redraw = true;
  }};

  template <> struct VWin<Init> { VWin() {
    auto [vwin, _] = _g.vwins.emplace( "1", VWindow{} );
    _g.vwin = &vwin->second;
    _g.vwin->mut = std::make_unique<std::mutex>();

    _g.vwin->curs.pos = { _g.pad_x, _g.pad_y };
  }};
} // namespace Comms

#endif
