##################
#  LIB_TRINKETS  #
##################

add_library(Trinkets
    STATIC
    ${TRINKETS_PATH}/inc/NavigationHelpers.h
    ${TRINKETS_PATH}/src/NavigationHelpers.cpp
    ${TRINKETS_PATH}/inc/PathController.h
    ${TRINKETS_PATH}/src/PathController.cpp
    ${TRINKETS_PATH}/inc/Shell.h
    ${TRINKETS_PATH}/src/Shell.cpp
    )
