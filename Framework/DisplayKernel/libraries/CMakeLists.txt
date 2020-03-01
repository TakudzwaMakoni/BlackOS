#########################
#  LIB_DISPLAY_KERNERL  #
#########################

find_package(Curses REQUIRED)

add_library(DisplayKernel
    STATIC
    ${DISPLAY_KERNEL_PATH}/inc/DisplayHelpers.h
    ${DISPLAY_KERNEL_PATH}/src/DisplayHelpers.cpp
    #${DISPLAY_KERNEL_PATH}/inc/Kcanvas.h
    #${DISPLAY_KERNEL_PATH}/src/Kcanvas.cpp
    #${DISPLAY_KERNEL_PATH}/inc/Kgrid.h
    #${DISPLAY_KERNEL_PATH}/inc/Kgrid.tpp
    ${DISPLAY_KERNEL_PATH}/inc/Kmenu.h
    ${DISPLAY_KERNEL_PATH}/src/Kmenu.cpp
    ${DISPLAY_KERNEL_PATH}/inc/Kwindow.h
    )

# DisplayKernel Library depends on Ncurses
target_include_directories(DisplayKernel  PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(DisplayKernel ${CURSES_LIBRARIES})
