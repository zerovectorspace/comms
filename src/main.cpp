#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <poll.h>
#include <chrono>
#include <string>
#include <sstream>
#include <signal.h>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "View.hpp"

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


namespace Comms
{
/********************************************************************************
 *                                   Outline
 ********************************************************************************/
  /*
   *                                                       __.-> Exec     -> Command Functors
   *                                                     /` _.-> Try_Exec -> Command Functors
   * Engine -> Main_Loop -> Parse_Args -> Local Command Map`                               
   *       \             `-> Parse_Args -> Window Command Map -> Try_Exec -> Command Functors
   *        \                                                 `-> Exec    -> Command Functors
   *         \
   *          `Net_Loop -> Parse_Buffer -> Net Comm Map -> Exec     -> Command Functors
   *                                                   `-> Try_Exec -> Command Functors
   *                                                   
   */

/********************************************************************************
 *                                   Aliases
 ********************************************************************************/
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

/********************************************************************************
 *                                   Helper
 ********************************************************************************/
  // operator "" _s :: const Char* -> std::size_t -> String
  auto operator "" _s( const Char* x, std::size_t s ) -> String
  {
    return String{ x, x + s };
  }

  // toStdStr :: String -> StdString
  auto toStdStr( String const& xs ) -> StdString
  {
    return StdString{ begin( xs ), end( xs ) };
  }

  // toStr :: StdString -> String
  auto toStr( StdString const& x ) -> String
  {
    return String{ begin( x ), end( x ) };
  }

  // head :: [T] -> T
  template <typename T, template <typename> typename Cont>
    auto head( Cont<T> const& xs ) -> T
    {
      return *( begin( xs ) );
    }

  // tail :: [T] -> [T]
  template <typename T, template <typename> typename Cont>
    auto tail( Cont<T> const& xs ) -> Cont<T>
    {
      return Cont<T>{ begin( xs ) + 1, end( xs ) };
    }

  // words :: String -> [String]
  auto words( String const& xs ) -> Vec<String>
  {
    Vec<String> out{};

    auto a = begin( xs );
    auto b = end( xs );
    auto it = a;

    for ( ; it != b ; advance( it, 1 ) )
    {
      if ( *it == ' ' )
      {
        auto tmp = String{ a , it };

        out.push_back( tmp );
        a = it + 1;
        while( *a == ' ' )
          a++;
      }
    }

    auto last = String{ a, b };
    if ( ! last.empty() )
      out.push_back( String{ a, b } );

    return out;
  }

  std::stringstream time_now()
  {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream time_str;
    time_str << std::put_time( &tm, "%F %T" ) << " ";

    return time_str;
  }

/********************************************************************************
 *                                    Data
 ********************************************************************************/
  /*
   * Connection :: Network Connection Information
   */
  struct Connection
  {
    /* ip:port */
    Vec<String> hostname{};
    struct pollfd poll_socket;
  };

  /*
   * Peer :: Handle to Connection for comms
   */
  struct Peer
  {
    // Used to protect mutable state
    std::mutex mut{};

    // Holds Connection Information for Peer
    U_ptr<Connection> conn{};

    //Buffer for sending and recieving
    Buffer buffer{ "EMPTY"_s };
  };

  /*
   * User :: A meat space user
   */
  struct User
  {
    String pw{};
  };

/********************************************************************************
 *                                   Global
 ********************************************************************************/
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


/********************************************************************************
 *                                  Commands
 ********************************************************************************/
  /*
   * Exec :: A factory class for Functors.
   *      :: Mimics composition of functions
   */
  template <typename... Ts> struct Exec
  {
    Exec() {}

    template <typename... Args>
    Exec( Args... args )
    {
      (*this)( args... );
    }

    template <typename... Args>
    auto operator () ( Args&&... args )
    {
      std::cout << "Exec::operator()( Args )\n";
      ( Ts{ args... } , ...);
    }

    auto operator () ( Peer* p )
    {
      std::cout << "Exec::operator()( Peer )\n";
      // Protect peer from modification in other threads
      ScopeLock lk{ p->mut };

      // Call each Functor in sequence
      ( Ts{}( p ), ... );
    }

    auto operator () ()
    {
      std::cout << "Exec::operator()()\n";
      ( Ts{} , ... );
    }
  };

  // Try_Exec :: wraps an anonymous lamba with try/catch blocks
  template <typename F> struct Try_Exec { Try_Exec( F f ) {
    try {
      f();
    } catch ( ... ) {
      auto time_str = time_now();
      time_str << "[Error] Not enough arguments";

      View.print( "error", time_str.str().c_str() );
    }
  }};

  /********************* Tag List *********************/
    class New{};
    class Run{};
    class Commands{};
    class Open{};
    class List{};
    class Del{};
    class Get_PW{};
    class Unlock_Key{};
    class Init{};
    class Main_Loop{};
    class Parse_Args{};

  /********************* Program *********************/
  template <typename... Ts> struct Program { Program() {
    View.appendln( "error", "Program::default" );
  }};

  template <> struct Program<Commands> { Program() {
    StdString cs = "help         View list of commands\n"
      "win <title>  Open window named <title>\n"
      "view <opts...> \n";

    View.fresh_print( cs.data() );
  }};

  /********************* Window *********************/
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

  /********************* Commands *********************/
  template <typename... Ts> struct Command { Command() {
    View.appendln( "error", "Command::default" );
  }};

  template <> struct Command<Init> { Command() {
    lcl_comms.emplace( "help", []( [[maybe_unused]] Vec<String>&& args ){
      Exec<Window<Open>>{}( "help", false );
      Exec<Program<Commands>>{}();
    });

    lcl_comms.emplace( "win", []( Vec<String>&& args ){
      Try_Exec{ [&args](){ Exec<Window<Open>>{}( toStdStr( args.at( 0 ) ) ); } };
    });

    lcl_comms.emplace( "delwin", []( Vec<String>&& args ){
      Try_Exec{ [&args](){ Exec<Window<Del>>{}( toStdStr( args.at( 0 ) ) ); } };
    });

    lcl_comms.emplace( "view", []( Vec<String>&& args ){
      Try_Exec{ [&args](){
        if ( args.at( 0 ) == "win"_s )
          Exec<Window<List>>{}();
      }};
    });

    lcl_comms.emplace( "login", []( [[maybe_unused]] Vec<String>&& args ){
      /* Exec Default */
      Exec<Window<New>>{}();
    });

    lcl_comms.emplace( "test", []( [[maybe_unused]] Vec<String>&& args ){
      /* Exec Default */
      Exec<Window<New>>{}();
    });
  }};

  // Exec_Command :: Grabs lambda from command map, executes lambda w/ args
  template <> struct Command<Run> { Command( String& command, Vec<String>& args ) {
    if ( auto p = lcl_comms.find( toStdStr( command ) ) ; p != lcl_comms.end() )
      p->second( std::move( args ) );
  }};

/********************************************************************************
 *                                   Engine
 ********************************************************************************/
  template <typename... Ts> struct Engine { Engine() {
    View.appendln( "error", "Engine::default" );
  }};

  // Parse_Args :: splits input into a command and args
  template <> struct Engine<Parse_Args> { Engine( String& command, Vec<String>& args ) {
    auto w       = words( command );
         args    = tail( w );
         command = head( w );
  }};

  // Main_Loop :: handles input using a command line
  template <> struct Engine<Main_Loop> { Engine() {
    bool is_running = true;

    View.current_win = "main" ;
    View.fresh_print( "main", "" );

    while ( is_running )
    {
      View.fresh_print( ".cl", "> " );

      unsigned char ch = View.get_char( ".cl" );

      if ( ch == '\n' )
      {
        continue;
      }

      /* Exec command */
      else if ( ch == ':' )
      {
        View.fresh_print( ".cl", "Command > " );

        auto input = View.get_line( ".cl", false );

        if (  input == "\0"_s
           || input == "\n"_s
           || input.empty() )
          continue;

        if ( input == "exit"_s  ) 
          break;

        LOG_START()
        Exec<Engine<Parse_Args>,Command<Run>>{}( input, Vec<String>{} );
        LOG_STOP()
      }

      /* Detect arrow keys */
      else if ( ch == '\033' )
      {
        ch = View.get_char( ".cl" );
        ch = View.get_char( ".cl" );
        if ( ch == 'A' )
          View.print( "main", "UP Pressed" );
        else if ( ch == 'B' )
          View.print( "main", "DOWN Pressed" );
        else if ( ch == 'C' )
          View.print( "main", "RIGHT Pressed" );
        else if ( ch == 'D' )
          View.print( "main", "LEFT Pressed" );
      }

      /* Echo on current win */
      else
      {
        View.fresh_print( ".cl", "> " );
        ungetch( ch );

        auto input = View.get_line( ".cl", false );
        input.push_back( '\0' );

        if ( !  View.is_writable( View.current_win )
             || input == "\0"_s 
             || input == "\n"_s )
          continue;

        std::stringstream output;
        output << "> ";
        output << input.data();

        /* Remove trailing '\0' */
        input.pop_back();
        View.print( View.current_win, output.str().data() );
      }
    }
  }};

  // Engine ::
  template <> struct Engine<Init>
  {
    static auto resize(int)
    {
      View.get_win_dims();
      View.refresh();
    }

    Engine()
    {
      signal(SIGWINCH, resize);

      View.init();

      View.new_win( ".cl", View.width(), 1, 0, View.height() - 1, true );
      View.new_win( "error", View.width(), View.height() - 1, 0, 0, false );

      View.current_win = "main";
      View.new_win( View.current_win, View.width(), View.height() - 1, 0, 0, true );

      Exec<Command<Init>,Engine<Main_Loop>>{}();
    }
  };

  void init()
  {
    Engine<Init>{};
  }
} // namespace Comms


/********************************************************************************
*                                   Main
********************************************************************************/
  void test_colors()
  {
    initscr();

    start_color();
    use_default_colors();

    init_pair(1, -1, -1);

    for ( int i = 0 ; i < 1000 ; i++ )
    {
        init_pair( 2, -i, -1 );
        attron( COLOR_PAIR( 2 ) );
        printw("X");
    }

    attron( COLOR_PAIR( 1 ) );
    printw("\n");
    printw("And this in a green background!\n");
    refresh();

    getch();

    endwin();
  }

  int main()
  {
    Comms::init();
    // test_colors();
  }
