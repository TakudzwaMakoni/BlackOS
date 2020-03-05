#########################
#  DISPLAY_KERNEL/EXEC  #
#########################


        #######################
        #  SCREEN EXECUTABLE  #
        #######################

        set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(Screen
            displayScreen/DisplayScreen.cpp
            )

        target_link_directories(Screen
            PRIVATE
            ${DISPLAY_KERNEL_LINK_DIR}
            )

        target_include_directories(Screen
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            )

        target_link_libraries(Screen DisplayKernel)


        #####################
        #  MENU EXECUTABLE  #
        #####################

        set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(Menu
            displayMenu/DisplayMenu.cpp
            )
        target_link_directories(Menu
            PRIVATE
            ${DISPLAY_KERNEL_LINK_DIR}
            )

        target_include_directories(Menu
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            )
        target_link_libraries(Menu DisplayKernel)


        #######################
        #  WINDOW EXECUTABLE  #
        #######################

        set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(Window
            displayWindow/DisplayWindow.cpp
            )
        target_link_directories(Window
            PRIVATE
            ${DISPLAY_KERNEL_LINK_DIR}
            )

        target_include_directories(Window
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            )
        target_link_libraries(Window DisplayKernel)


        ###########################
        #  MENU_TESTS EXECUTABLE  #
        ###########################

        set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(MenuTests
            unitTests/MenuTest.cpp
            testHelpers/src/MenuGenerator.cpp
            )

        target_link_directories(MenuTests
            PRIVATE
            ${DISPLAY_KERNEL_LINK_DIR}
            )

        target_include_directories(MenuTests
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            )
        target_link_libraries(MenuTests DisplayKernel)
