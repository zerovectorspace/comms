#include "src/Engine.hpp"

/*
 * Include Heirarchy
 * -----------------
 *
 * Include
 *  ^- Macros
 *     ^- Alias
 *        ^- Helper
 *           ^- | Data
 *                ^- Global
 *                   ^- Exec
 *                      ^- Taglist
 *                       ^-  | Buffer
 *                           | Event
 *                           | Font
 *                           | Prog_Win
 *                           | Render
 *                             ^- components/Include
 *                                ^- | Command
 *                                   | Program
 *                                   | Window
 *                                     ^- commands/Include
 *                                        ^- Engine
 *                                           ^- main
 *
 *
 */

/*
 * Program Flow
 * ------------
 *                                                                         __.-> Exec     -> Command Functors
 *                                                                       /`__.-> Try_Exec -> Command Functors
 * Engine<Init> -> Engine<Main_Loop> -> Engine<Parse_Args> -> Command<Run>`
 *       \                          `-> Engine<Parse_Args> -> Command<Run> -> Try_Exec -> Command Functors
 *        \                                                               `-> Exec     -> Command Functors
 *         \
 *          `Engine<Net_Loop> -> Engine<Parse_Buffer> -> Command<Run> -> Net Comm Map -> Exec    -> Command Functors
 *                                                                                  `-> Try_Exec -> Command Functors
 *                                                   
 */

int main()
{
  using namespace Comms;
  Engine<Init>{};
}
