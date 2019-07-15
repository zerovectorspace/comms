#[[
    This file is part of Comms.

    Comms is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Comms is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Comms.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2018 Zachary Young
    ]]

cmake_minimum_required( VERSION 3.10.3 )
project( Comms )

SET( CMAKE_CXX_STANDARD 17 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

set( GCC_FLAGS "-fmax-errors=1 -Wall -Wextra -Werror" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ${GCC_FLAGS}" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g ${GCC_FLAGS}" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 ${GCC_FLAGS}" )
set( CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_RELEASE} -Os ${GCC_FLAGS}" )

add_definitions( -DDEBUG_EXTRA )

set(
    LIBRARY_FILES

    src/Alias.hpp
    src/Data.hpp
    src/Engine.hpp
    src/Exec.hpp
    src/Global.hpp
    src/Helper.hpp
    src/Include.hpp
    src/Macros.hpp
    src/Taglist.hpp

    src/components/Buffer.hpp
    src/components/Event.hpp
    src/components/Font.hpp
    src/components/Include.hpp
    src/components/Prog_Win.hpp
    src/components/Render.hpp

    src/commands/Command.hpp
    src/commands/Include.hpp
    src/commands/Program.hpp
    src/commands/Window.hpp
)
set (
    TEST_FILES
)
set (
    MAIN_FILES
    src/Main.cpp
)

set( OpenGL_GL_PREFERENCE GLVND )

find_package( OpenGL REQUIRED )
find_package( SDL2 REQUIRED )
find_package( GLEW REQUIRED )

find_package( Freetype REQUIRED )

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/fct

    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}

    ${FREETYPE_INCLUDE_DIRS}
)

add_subdirectory( ${CMAKE_SOURCE_DIR}/fct )

# set( CATCH_INCLUDE_DIR /usr/include/catch )
# add_library( Catch INTERFACE )
# target_include_directories( Catch INTERFACE ${CATCH_INCLUDE_DIR} )

add_library( Comms STATIC ${LIBRARY_FILES} )
set_target_properties( Comms PROPERTIES LINKER_LANGUAGE CXX )

set( CMAKE_EXE_LINKER_FLAGS  ${CMAKE_EXE_LINKER_FLAGS} )

add_executable( main ${MAIN_FILES} )
target_link_libraries(
    main
    Comms
    m
    SDL2
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/glsl $<TARGET_FILE_DIR:main>/glsl)

