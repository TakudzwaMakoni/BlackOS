#########################
#  LIB_DISPLAY_KERNERL  #
#########################

find_package(Curses REQUIRED)

add_library(DisplayKernel
    STATIC
    ${DISPLAY_KERNEL_PATH}/inc/DisplayHelpers.h
    ${DISPLAY_KERNEL_PATH}/src/DisplayHelpers.cpp
    ${DISPLAY_KERNEL_PATH}/inc/Screen.h
    ${DISPLAY_KERNEL_PATH}/src/Screen.cpp
    ${DISPLAY_KERNEL_PATH}/inc/Window.h
    ${DISPLAY_KERNEL_PATH}/src/Window.cpp
    #${DISPLAY_KERNEL_PATH}/inc/Grid.h
    #${DISPLAY_KERNEL_PATH}/inc/Grid.tpp
    ${DISPLAY_KERNEL_PATH}/inc/Menu.h
    ${DISPLAY_KERNEL_PATH}/src/Menu.cpp
    ${DISPLAY_KERNEL_PATH}/inc/DisplayObject.h
    )

# DisplayKernel Library depends on Ncurses
target_include_directories(DisplayKernel  PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(DisplayKernel ${CURSES_LIBRARIES})
