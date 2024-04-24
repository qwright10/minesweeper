cmake_minimum_required(VERSION 3.25)
project(minesweeper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(SOURCE_FILES main.cpp)

add_executable(minesweeper ${SOURCE_FILES})
find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(minesweeper sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

