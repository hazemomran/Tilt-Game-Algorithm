cmake_minimum_required(VERSION 3.22)
project(minesweeper)

set(CMAKE_CXX_STANDARD 14)

add_executable(minesweeper main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/Project 3/SFML/include/SFML)
target_link_libraries(minesweeper sfml-system sfml-window sfml-graphics sfml-audio)