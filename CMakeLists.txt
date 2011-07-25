

cmake_minimum_required( VERSION 2.6 FATAL_ERROR )
project( nesem )

set(CMAKE_C_FLAGS_DEBUG "-O0 -Wall -g gdb -Wextra -DDEBUG $ENV{CFLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g gdb -Wextra -DDEBUG $ENV{CXXFLAGS}")

set(CMAKE_C_FLAGS_RELEASE "-ggdb -O3 -DNDEBUG $ENV{CFLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-ggdb -O3 -DNDEBUG $ENV{CXXFLAGS}")

set(CMAKE_CXX_FLAGS "-ggdb -Wall")

add_executable( nesem
  src/cpu.cpp
  src/gpu.cpp
  src/main.cpp
  src/mapper.cpp
  src/rom.cpp
  src/gpu.cpp
  src/window.cpp
)

target_link_libraries( nesem
  SDL
  boost_program_options
  boost_thread
)
