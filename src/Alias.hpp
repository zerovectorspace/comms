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

  using UVec2  = glm::uvec2;
  using IVec2  = glm::ivec2;
  using Vec3   = glm::vec3;
  using Vec4   = glm::vec4;
  using Mat4   = glm::mat4;

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
