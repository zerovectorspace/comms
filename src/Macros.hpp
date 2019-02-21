#ifndef COMMS_MACRO_HEADER
#define COMMS_MACRO_HEADER

#include "Include.hpp"

namespace Comms
{
#define LOG_START() { auto begin = std::chrono::high_resolution_clock::now(); \
                      auto log = input;

#define LOG_STOP()  auto end = std::chrono::high_resolution_clock::now(); \
                    std::chrono::duration<Double, std::milli> diff = end - begin; \
                    if ( ! View.get_window( "log" ) ) \
                      View.new_win( "log", View.width(), View.height() - 1, 0, 0, false ); \
                    auto out = time_now(); \
                    log.push_back( '\0' ); \
                    out << "\"" << log.data() << "\" "; \
                    out << diff.count() << "ms ";\
                    View.appendln( "log", out.str().c_str() ); }

#define try_catch( code ) try { code } catch ( ...  ) { View.print( "error", "no!!!" ); }
}

#endif
