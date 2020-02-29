#########################
#  DISPLAY_KERNEL/EXEC  #
#########################

set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


        ######################
        #  KMENU EXECUTABLE  #
        ######################

        set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(Kmenu
            displayMenu/DisplayMenu.cpp
            )

        target_link_directories(Kmenu
            PRIVATE
            ${DISPLAY_KERNEL_LINK_DIR}/DisplayKernel
            )

        target_include_directories(Kmenu
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            )

        target_link_libraries(Kmenu DisplayKernel)


        ############################
        #  KMENU_TESTS EXECUTABLE  #
        ############################

        set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        add_executable(KmenuTests
            unitTests/KmenuTest.cpp
            testHelpers/src/MenuGenerator.cpp
            )

        target_link_directories(KmenuTests
            PRIVATE
            ${DISPLAY_KERNEL_LINK_DIR}/DisplayKernel
            )

        target_include_directories(KmenuTests
            PRIVATE
            ${DISPLAY_KERNEL_PATH}/inc
            )
        target_link_libraries(KmenuTests DisplayKernel)
