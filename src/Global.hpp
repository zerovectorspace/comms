#ifndef COMMS_GLOBAL_HEADER
#define COMMS_GLOBAL_HEADER

#include "Data.hpp"
#include "View.hpp"

/********************************************************************************
 *                                   Global
 ********************************************************************************/

namespace Comms
{
  using Command_Map = std::unordered_map<StdString, void(*)( Vec<String>&& )>;
  using Peer_Map    = std::unordered_map<Socket_Num, U_ptr<Peer>>;

  /*
   * View :: View for all classes
   */
  View View{};

  /*
   * Command maps
   */
  Command_Map lcl_comms{};
  Command_Map win_comms{};
  Command_Map net_comms{};

  /*
   * Peer_Map :: Handle to `Peer`s
   */
  Peer_Map peer_map{};
}

#endif
