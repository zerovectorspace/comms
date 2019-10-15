#ifndef COMMDS_ALIAS_HEADER
#define COMMDS_ALIAS_HEADER

#include "src/Macros.hpp"

/********************************************************************************
 *                                   Aliases
 ********************************************************************************/

namespace Comms
{
  using Char   = char;
  using Int    = GLint;
  using UInt   = GLuint;
  using Long   = GLint64;
  using ULong  = GLuint64;
  using Float  = GLfloat;
  using Double = GLdouble;

  using ScopeLock = std::unique_lock<std::mutex>;

  template <typename T>
  using Vec = std::vector<T>;

  template <typename T>
  using U_ptr = std::unique_ptr<T>;

  using String = std::vector<Char>;
  using StdString = std::string;

  using Socket_Num = int;
}

#endif
