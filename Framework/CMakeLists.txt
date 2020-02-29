#######################
#       Framework     #
#######################

project (Trinkets LANGUAGES CXX)

set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DISPLAY_KERNEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/DisplayKernel")
set(TRINKETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Trinkets")

# MAKE libraries available for all Framework Projects
set(DISPLAY_KERNEL_LINK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${DISPLAY_KERNEL_PATH}/libraries")
set(TRINKETS_LINK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${TRINKETS_PATH}/libraries")

add_subdirectory(DisplayKernel)
add_subdirectory(Trinkets)
