#ifndef COMMS_PROGRAM_WINDOW_HEADER
#define COMMS_PROGRAM_WINDOW_HEADER

#include "Taglist.hpp"

namespace Comms
{
  template <typename... Ts> struct Prog_Win { Prog_Win() {

  }};

  static auto resize(int)
  {

  }

  template <> struct Prog_Win<GLSL_Compile> { Prog_Win() {
    auto shad_str = fct::readFile( "glsl/text_vert.glsl" );
    auto shad_cstr = shad_str.c_str();

    GLuint v_shad = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( v_shad, 1, &shad_cstr, NULL );
    glCompileShader( v_shad );

    shad_str = fct::readFile( "glsl/text_frag.glsl" );
    shad_cstr = shad_str.c_str();

    GLuint f_shad = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( f_shad, 1, &shad_cstr, NULL );
    glCompileShader( f_shad );

    _glob.gl_prog = glCreateProgram();
    glAttachShader( _glob.gl_prog, v_shad );
    glAttachShader( _glob.gl_prog, f_shad );
    glLinkProgram( _glob.gl_prog );

    glDeleteShader( v_shad );
    glDeleteShader( f_shad );
  }};

  // Prog_Win ::
  template <> struct Prog_Win<Init> { Prog_Win() {
    signal( SIGWINCH, resize );

    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
      fct::print( "ERROR: SDL_Init" );
      return;
    }

    _glob.win = SDL_CreateWindow( "comms"
                                , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
                                , 800, 600
                                , SDL_WINDOW_OPENGL
                                | SDL_WINDOW_SHOWN
                                | SDL_WINDOW_BORDERLESS
                                | SDL_WINDOW_INPUT_GRABBED
                                | SDL_WINDOW_RESIZABLE 
                                | SDL_WINDOW_MAXIMIZED );

    if ( ! _glob.win )
    {
      fct::print( "ERROR: SDL_CreateWindow" );
      return;
    }

    _glob.ren = SDL_CreateRenderer( _glob.win
                                  , -1
                                  , SDL_RENDERER_ACCELERATED
                                  | SDL_RENDERER_PRESENTVSYNC );

    if ( ! _glob.ren )
    {
      fct::print( "ERROR: SDL_CreateRenderer" );
      SDL_DestroyWindow( _glob.win );
      SDL_Quit();
      return;
    }

    glewExperimental = GL_TRUE;
    glewInit();

    // Set OpenGL options
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glViewport( 0, 0, 800, 600 );

    Prog_Win<GLSL_Compile>{};

    glUseProgram( _glob.gl_prog );
    glm::mat4 projection = glm::ortho( 0.f, 800.f, 0.f, 600.f );
    glUniformMatrix4fv( glGetUniformLocation( _glob.gl_prog, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );

    // Configure VAO/VBO for texture quads
    glGenVertexArrays( 1, &_glob.VAO );
    glGenBuffers( 1, &_glob.VBO );

    glBindVertexArray( _glob.VAO );
      glBindBuffer( GL_ARRAY_BUFFER, _glob.VBO );
      glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0 );
  }};
}

#endif
