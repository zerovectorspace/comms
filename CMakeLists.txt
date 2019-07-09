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
    src/Global.hpp
    src/Helper.hpp
    src/Include.hpp
    src/Macros.hpp
    src/View.cpp
    src/View.hpp

    src/comms/Command.hpp
    src/comms/Commands.hpp
    src/comms/Exec.hpp
    src/comms/Program.hpp
    src/comms/Taglist.hpp
    src/comms/Window.hpp
)
set (
    TEST_FILES
)
set (
    MAIN_FILES
    src/Main.cpp
)

set( OpenGL_GL_PREFERENCE GLVND )

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/fct
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

add_subdirectory( ${CMAKE_SOURCE_DIR}/fct )

# set( CATCH_INCLUDE_DIR /usr/include/catch )
# add_library( Catch INTERFACE )
# target_include_directories( Catch INTERFACE ${CATCH_INCLUDE_DIR} )

add_library( Comms STATIC ${LIBRARY_FILES} )
set_target_properties( Comms PROPERTIES LINKER_LANGUAGE CXX )

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lncurses" )

add_executable( main ${MAIN_FILES} )
target_link_libraries(
    main
    Comms
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    SDL2
    m
)
