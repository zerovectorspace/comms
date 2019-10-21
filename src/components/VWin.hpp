#ifndef COMMS_VWin_HEADER
#define COMMS_VWin_HEADER

#include "src/Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct VWin { VWin() {

  }};

  template <> struct VWin<Init> { VWin() {
    _g.vwins.push_back( VWindow{} );
    _g.vwin = &_g.vwins.back();

    _g.vwin->curs.pos = { _g.pad_x, _g.pad_y };
  }};
}

#endif
