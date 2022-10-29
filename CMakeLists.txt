cmake_minimum_required(VERSION 3.18)

project(
  asm_c_cpp
  VERSION "0.1.0"
  DESCRIPTION "A game made in three languages"
  LANGUAGES ASM C CXX)

#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(asm)
add_subdirectory(c)
add_subdirectory(cpp)