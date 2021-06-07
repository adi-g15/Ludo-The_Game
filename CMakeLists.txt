cmake_minimum_required(VERSION 3.0.0)
project(Ludo-The_Game VERSION 2.3.1)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cpp")
add_executable(ludo main.cpp ${SOURCES})

target_include_directories(ludo PRIVATE includes)
target_include_directories(ludo PRIVATE .)

find_package(Threads REQUIRED)
target_link_libraries(ludo PRIVATE Threads::Threads)

# If you want, you can install the 'ludo' executable, just run 'cmake --install .' inside build directory
install(TARGETS ludo DESTINATION bin COMPONENT binaries)
