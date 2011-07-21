

cmake_minimum_required( VERSION 2.6 FATAL_ERROR )
project( nesem )

set(CMAKE_C_FLAGS_DEBUG "-O0 -Wall -g -Wextra -DDEBUG $ENV{CFLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -Wextra -DDEBUG $ENV{CXXFLAGS}")

set(CMAKE_C_FLAGS_RELEASE "-g -O3 -DNDEBUG $ENV{CFLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -DNDEBUG $ENV{CXXFLAGS}")

add_executable( nesem
  src/bus.cpp
  src/cpu.cpp
  src/dma.cpp
  src/gpu.cpp
  src/main.cpp
  src/mapper.cpp
  src/memory.cpp
  src/rom.cpp
  src/window.cpp
)

target_link_libraries( nesem
  SDL
  boost_program_options
  boost_thread
)
