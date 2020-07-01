cmake_minimum_required(VERSION 3.9.2)
project(Ludo-The_Game)

set(CMAKE_CXX_STANDARD 17)

include_directories(includes)

file(GLOB SOURCES "src/*.cpp")

add_executable(game ${SOURCES})