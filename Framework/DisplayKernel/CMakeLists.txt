#dipslay

project (DisplayKernel LANGUAGES CXX)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")

set (CMAKE_CXX_FLAGS "-lncurses")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Kmenu
    prototypes/DisplayMenu.cpp
    src/DisplayHelpers.cpp
    src/Kmenu.cpp)

#add_executable(Kgrid
#    prototypes/DisplayGrid.cpp)

#add_executable(Kcanvas
#    prototypes/DisplayCanvas.cpp
#    src/Kcanvas.cpp)

add_executable(KmenuTests
    unit_tests/catch.hpp
    unit_tests/KmenuTest.cpp
    testHelpers/src/MenuGenerator.cpp
    src/DisplayHelpers.cpp
    src/Kmenu.cpp)



#target_link_libraries (Kgrid Eigen3::Eigen)
#target_link_libraries (Kgrid ${CURSES_LIBRARIES})

#target_link_libraries(Kcanvas Eigen3::Eigen)
#target_link_libraries(Kcanvas ${CURSES_LIBRARIES})

#target_link_libraries (KmenuTests Eigen3::Eigen)
#target_link_libraries (KmenuTests ${CURSES_LIBRARIES})
