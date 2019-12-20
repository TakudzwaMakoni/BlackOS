#blackosdipslay

cmake_minimum_required( VERSION 3.14 FATAL_ERROR)

project (BlackOSDisplay LANGUAGES CXX)


find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_MENU
    src/DisplayMenu.cpp
    src/Directives.cpp
    src/Kfield.cpp
    src/Kmenu.cpp)

set(SRC_GRID
    src/DisplayGrid.cpp
    src/Directives.cpp
    src/Kfield.cpp
    src/Kmenu.cpp)

set(SRC_CANVAS
    src/DisplayCanvas.cpp
    src/Kcanvas.cpp)

set(INC_FILES
    inc/Directives.h
    inc/Kfield.h
    inc/Kgrid.h
    inc/Kmenu.h
    inc/Kwindow.h)

set(TEST_FILES
    unit_tests/catch.hpp
    unit_tests/KmenuTest.cpp
    )

add_executable(Kmenu
      ${SRC_MENU}
      ${INC_FILES})
add_executable(Kgrid
    ${SRC_GRID}
    ${INC_FILES})
add_executable(Kcanvas
      ${SRC_CANVAS}
      ${INC_FILES})
add_executable(KmenuTests
    ${TEST_FILES})

set(CMAKE_CXX_FLAGS "-lncurses")

target_link_libraries (Kmenu Eigen3::Eigen)
target_link_libraries (Kmenu ${CURSES_LIBRARIES})

target_link_libraries (Kgrid Eigen3::Eigen)
target_link_libraries (Kgrid ${CURSES_LIBRARIES})

target_link_libraries(Kcanvas Eigen3::Eigen)
target_link_libraries(Kcanvas ${CURSES_LIBRARIES})

target_link_libraries (KmenuTests Eigen3::Eigen)
target_link_libraries (KmenuTests ${CURSES_LIBRARIES})
