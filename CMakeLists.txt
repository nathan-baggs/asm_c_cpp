cmake_minimum_required(VERSION 3.18)

project(
  asm_c_cpp
  VERSION "0.1.0"
  DESCRIPTION "A game made in three languages"
  LANGUAGES ASM C CXX)

include(FetchContent)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL2_DISABLE_SDL2MAIN OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  sdl
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.24.2)
FetchContent_MakeAvailable(sdl)

add_subdirectory(asm)
add_subdirectory(c)
add_subdirectory(cpp)