###################
#  TRINKETS/EXEC  #
###################

find_package(fmt) # ListChildren, NavigateDir

        ###############################
        #  SCREEN_SHELL EXECUTABLE  #
        ###############################

        set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(Tr
            ScreenShell.cpp
            ../helpers/PathController.cpp
            src/ClearScreen.cpp
            src/ChangeDir.cpp
            src/ConfigureShell.cpp
            src/ListChildren.cpp
            src/ListConfigVariables.cpp
            src/NavigateDir.cpp
            src/SetShellEnv.cpp
            src/Shortcut.cpp
            src/SplashScreen.cpp
            src/ScreenShell.cpp
            )
        target_link_directories(Tr
            PRIVATE
            ${DISPLAY_KERNEL_LINK_DIR}
            )
        target_include_directories(Tr
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            )
        target_link_libraries(Tr DisplayKernel)
        target_link_libraries(Tr fmt::fmt)
