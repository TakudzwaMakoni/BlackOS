project (Trinkets LANGUAGES CXX)

find_package (Boost 1.72 REQUIRED)

set(CMAKE_CXX_FLAGS "-lncurses")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")

add_library(libKmenu
    STATIC
    ${DISPLAY_KERNEL_PATH}/inc/Kmenu.h
    ${DISPLAY_KERNEL_PATH}/src/Kmenu.cpp
    ${DISPLAY_KERNEL_PATH}/inc/Kfield.h
    ${DISPLAY_KERNEL_PATH}/src/Kfield.cpp
    ${DISPLAY_KERNEL_PATH}/inc/Directives.h
    ${DISPLAY_KERNEL_PATH}/src/Directives.cpp
    )


add_executable(NavigateDir
    NavigateDir/NavigateDir.cpp
    NavigateDir/src/Navigator.cpp
    )

target_include_directories(NavigateDir
    PRIVATE
    ${DISPLAY_KERNEL_PATH}/inc
    )

target_link_libraries(NavigateDir ${Boost_LIBRARIES})
target_link_libraries(NavigateDir libKmenu)
