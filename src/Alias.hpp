#ifndef COMMDS_ALIAS_HEADER
#define COMMDS_ALIAS_HEADER

#include "Macros.hpp"

/********************************************************************************
 *                                   Aliases
 ********************************************************************************/

namespace Comms
{
  using Char = char;
  using ULong = unsigned long;
  using Int = int;
  using UInt = unsigned int;
  using Double = double;

  using ScopeLock = std::unique_lock<std::mutex>;

  template <typename T>
  using Vec = std::vector<T>;

  template <typename T>
  using U_ptr = std::unique_ptr<T>;

  using String = std::vector<Char>;
  using StdString = std::string;

  using Socket_Num = int;

  using Buffer = String;
}

#endif
