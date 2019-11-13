cmake_minimum_required(VERSION 3.14)
project(BlackOS)

set(CMAKE_CXX_STANDARD 11 )
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(Framework/BlackOSDisplay/inc)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(DisplayMenu
        Framework/BlackOSDisplay/inc/Directives.h
        Framework/BlackOSDisplay/inc/Kfield.h
        Framework/BlackOSDisplay/inc/Kgrid.h
        Framework/BlackOSDisplay/inc/Kmenu.h
        Framework/BlackOSDisplay/inc/Kwindow.h
        Framework/BlackOSDisplay/src/Directives.cpp
        Framework/BlackOSDisplay/src/DisplayMenu.cpp
        Framework/BlackOSDisplay/src/Kfield.cpp
        Framework/BlackOSDisplay/src/Kmenu.cpp)

add_executable(DisplayGrid
         Framework/BlackOSDisplay/inc/Directives.h
         Framework/BlackOSDisplay/inc/Kfield.h
         Framework/BlackOSDisplay/inc/Kgrid.h
         Framework/BlackOSDisplay/inc/Kmenu.h
         Framework/BlackOSDisplay/inc/Kwindow.h
         Framework/BlackOSDisplay/src/Directives.cpp
         Framework/BlackOSDisplay/src/DisplayGrid.cpp
         Framework/BlackOSDisplay/src/Kfield.cpp
         Framework/BlackOSDisplay/src/Kmenu.cpp)

set (CMAKE_CXX_FLAGS "-lncurses")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

target_link_libraries(DisplayMenu ${CURSES_LIBRARIES})
target_link_libraries(DisplayMenu Eigen3::Eigen)

target_link_libraries(DisplayGrid ${CURSES_LIBRARIES})
target_link_libraries(DisplayGrid Eigen3::Eigen)
