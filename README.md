# comms

A client to client end-to-end encrypted messaging application with and included OpenGL based GUI. Currently in development.

## Prerequisites

- C++20 compliant compiler
- OpenGL libraries
- GLEW
- SDL2
- CMake

## Building


``` bash
$ cd ~
$ git clone <repo>
$ cd <repo>
$ cmake .
$ make comms
```

## Running

`comms` is built with a server/client architecture to facilitate future GUI alternatives. `comms` comes with a GUI client built using OpenGL. OpenGL was chosen to allow images, video, and sound in the future.

``` bash
$ ./comms -s # Start up the local Server
$ ./comms -c # Start up the local GUI Client *OpenGL Required
```

## Features

[x] OpenGL text rendering
[x] Multiple Windows
[] Split Windows
[] Settings
[] Sockets
  - [x] Local Server/Client Unix Socket Communication
  - [] Remote Server/Server UDP Communication
[] Protocol
[] Encryption layer
[] STUN server
[] UDP hole punching
