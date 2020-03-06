##################
#  LIB_TRINKETS  #
##################

find_package(fmt REQUIRED) # Natives

        #######################
        #  TRINKETS  LIBRARY  #
        #######################

        add_library(Trinkets
            STATIC
            ${TRINKETS_PATH}/inc/NavigationHelpers.h
            ${TRINKETS_PATH}/src/NavigationHelpers.cpp
            ${TRINKETS_PATH}/inc/PathController.h
            ${TRINKETS_PATH}/src/PathController.cpp
            ${TRINKETS_PATH}/inc/Shell.h
            ${TRINKETS_PATH}/src/Shell.cpp
            ${TRINKETS_PATH}/inc/TrShell.h
            ${TRINKETS_PATH}/src/TrShell.cpp
            ${TRINKETS_PATH}/inc/Natives.h
            ${TRINKETS_PATH}/src/Natives.cpp
            ${TRINKETS_PATH}/inc/SplashScreen.h
            ${TRINKETS_PATH}/src/SplashScreen.cpp
            )
        target_link_directories(Trinkets PRIVATE
            ${DISPLAY_KERNEL_LINK_DIR}
            )
        target_include_directories(Trinkets
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            )
        target_link_libraries(Trinkets DisplayKernel)
        target_link_libraries(Trinkets fmt::fmt)
