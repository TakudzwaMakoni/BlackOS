cmake_minimum_required(VERSION 3.14)
project(BlackOS)

set(CMAKE_CXX_STANDARD 14)
set(ENV{TERM} "iterm2")
set(CMAKE_CXX_FLAGS "-lncurses")
add_executable(BlackOS source/main.cpp source/BlackOSScripts.cpp header/BlackOSScripts.h header/BlackOSObjects.h source/BlackOSObjects.cpp)


find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries (BlackOS Eigen3::Eigen)