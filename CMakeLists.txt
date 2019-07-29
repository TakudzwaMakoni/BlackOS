cmake_minimum_required(VERSION 3.14)
project(BlackOS)

set( CMAKE_CXX_STANDARD 11 )
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Framework/BlackOSDisplay/inc)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(BlackOS
        Framework/BlackOSDisplay/inc/Directives.h
        Framework/BlackOSDisplay/inc/Kfield.h
        Framework/BlackOSDisplay/inc/Kgrid.h
        Framework/BlackOSDisplay/inc/Kmenu.h
        Framework/BlackOSDisplay/inc/Kwindow.h
        Framework/BlackOSDisplay/src/Directives.cpp
        Framework/BlackOSDisplay/src/Display.cpp
        Framework/BlackOSDisplay/src/Kfield.cpp
        Framework/BlackOSDisplay/src/Kmenu.cpp)

set(ENV{TERM} "iterm2")
set (CMAKE_CXX_FLAGS "-lncurses")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries (BlackOS Eigen3::Eigen)
target_link_libraries(BlackOS ${CURSES_LIBRARIES})