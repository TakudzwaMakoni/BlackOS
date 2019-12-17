cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(BlackOS)

set(CMAKE_CXX_STANDARD 11 )
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CURSES_INCLUDE_DIR})

set (CMAKE_CXX_FLAGS "-lncurses")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_subdirectory(Framework)
