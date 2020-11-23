cmake_minimum_required(VERSION 3.0.0)
project(Ludo-The_Game VERSION 2.3.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cpp")
add_executable(ludo main.cpp ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

find_package(Threads REQUIRED)
target_link_libraries(ludo PRIVATE Threads::Threads)

target_include_directories(ludo PRIVATE includes)
target_include_directories(ludo PRIVATE .)
