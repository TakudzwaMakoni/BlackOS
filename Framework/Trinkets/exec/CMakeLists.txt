###################
#  TRINKETS/EXEC  #
###################

find_package(fmt) # ListChildren, NavigateDir

        ##############################
        #  LIST_CHILDREN EXECUTABLE  #
        ##############################

        if (NOT fmt_FOUND)

        message("NOTE: you cannot build ListChildren: package 'fmt' was not found")

        else()

        set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(ListChildren
            listChildren/ListChildren.cpp
            )
        target_link_directories(ListChildren
            PRIVATE
            ${TRINKETS_LINK_DIR}
            ${DISPLAY_KERNEL_LINK_DIR}
            )
        target_include_directories(ListChildren
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            ${TRINKETS_PATH}/inc
            )
        target_link_libraries(ListChildren fmt::fmt)
        target_link_libraries(ListChildren DisplayKernel)
        target_link_libraries(ListChildren Trinkets)

        endif()


        #############################
        #  NAVIGATE_DIR EXECUTABLE  #
        #############################

        if(NOT fmt_FOUND)

        message("NOTE: you cannot build NavigateDir: package 'fmt' was not found")

        else()

        set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(NavigateDir
            navigateDir/NavigateDir.cpp
            )
        target_link_directories(NavigateDir
            PRIVATE
            ${TRINKETS_LINK_DIR}
            ${DISPLAY_KERNEL_LINK_DIR}
            )
        target_include_directories(NavigateDir
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            ${TRINKETS_PATH}/inc
            )
        target_link_libraries(NavigateDir fmt::fmt)
        target_link_libraries(NavigateDir DisplayKernel)
        target_link_libraries(NavigateDir Trinkets)

        endif()


        ###############################
        #  TRINKETS_SHELL EXECUTABLE  #
        ###############################

        set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(Tr
            trinketsShell/TrinketsShell.cpp
            )
        target_link_directories(Tr
            PRIVATE
            ${TRINKETS_LINK_DIR}
            )
        target_include_directories(Tr
            PRIVATE
            ${TRINKETS_PATH}/inc
            ${DISPLAY_KERNEL_PATH}/inc
            )
        target_link_libraries(Tr Trinkets)


        #############################
        #  SPLASHSCREEN EXECUTABLE  #
        #############################

        set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(Splash
            splashScreen/Splash.cpp
            )
        target_link_directories(Splash
            PRIVATE
            ${TRINKETS_LINK_DIR}
            )
        target_include_directories(Splash
            PRIVATE
            ${TRINKETS_PATH}/inc
            ${DISPLAY_KERNEL_PATH}/inc
            )
        target_link_libraries(Splash Trinkets)
