#blackosdipslay

cmake_minimum_required( VERSION 3.14 FATAL_ERROR)

project (BlackOSDisplay LANGUAGES CXX)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Kmenu
    src/DisplayMenu.cpp
    src/Directives.cpp
    src/Kfield.cpp
    src/Kmenu.cpp)

add_executable(Kgrid
    src/DisplayGrid.cpp)

add_executable(Kcanvas
      src/DisplayCanvas.cpp
      src/Kcanvas.cpp)

add_executable(KmenuTests
    unit_tests/catch.hpp
    unit_tests/KmenuTest.cpp
    testHelpers/src/MenuGenerator.cpp
    src/Directives.cpp
    src/Kfield.cpp
    src/Kmenu.cpp)

set(CMAKE_CXX_FLAGS "-lncurses")

target_link_libraries (Kmenu Eigen3::Eigen)
target_link_libraries (Kmenu ${CURSES_LIBRARIES})

target_link_libraries (Kgrid Eigen3::Eigen)
target_link_libraries (Kgrid ${CURSES_LIBRARIES})

target_link_libraries(Kcanvas Eigen3::Eigen)
target_link_libraries(Kcanvas ${CURSES_LIBRARIES})

target_link_libraries (KmenuTests Eigen3::Eigen)
target_link_libraries (KmenuTests ${CURSES_LIBRARIES})
