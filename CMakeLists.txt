cmake_minimum_required(VERSION 3.22.1)

project(KWSS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

add_executable(KWSS 
               src/main_test.cpp 
               lib/model/Point2i.cpp
               lib/view/Window.cpp)

target_link_libraries(KWSS ${CURSES_LIBRARY})
