# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/takudzwa/CLionProjects/BlackOS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/BlackOS.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/BlackOS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BlackOS.dir/flags.make

CMakeFiles/BlackOS.dir/source/main.cpp.o: CMakeFiles/BlackOS.dir/flags.make
CMakeFiles/BlackOS.dir/source/main.cpp.o: ../source/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BlackOS.dir/source/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BlackOS.dir/source/main.cpp.o -c /Users/takudzwa/CLionProjects/BlackOS/source/main.cpp

CMakeFiles/BlackOS.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BlackOS.dir/source/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/takudzwa/CLionProjects/BlackOS/source/main.cpp > CMakeFiles/BlackOS.dir/source/main.cpp.i

CMakeFiles/BlackOS.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BlackOS.dir/source/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/takudzwa/CLionProjects/BlackOS/source/main.cpp -o CMakeFiles/BlackOS.dir/source/main.cpp.s

CMakeFiles/BlackOS.dir/source/Kfield.cpp.o: CMakeFiles/BlackOS.dir/flags.make
CMakeFiles/BlackOS.dir/source/Kfield.cpp.o: ../source/Kfield.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/BlackOS.dir/source/Kfield.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BlackOS.dir/source/Kfield.cpp.o -c /Users/takudzwa/CLionProjects/BlackOS/source/Kfield.cpp

CMakeFiles/BlackOS.dir/source/Kfield.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BlackOS.dir/source/Kfield.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/takudzwa/CLionProjects/BlackOS/source/Kfield.cpp > CMakeFiles/BlackOS.dir/source/Kfield.cpp.i

CMakeFiles/BlackOS.dir/source/Kfield.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BlackOS.dir/source/Kfield.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/takudzwa/CLionProjects/BlackOS/source/Kfield.cpp -o CMakeFiles/BlackOS.dir/source/Kfield.cpp.s

CMakeFiles/BlackOS.dir/source/Kmenu.cpp.o: CMakeFiles/BlackOS.dir/flags.make
CMakeFiles/BlackOS.dir/source/Kmenu.cpp.o: ../source/Kmenu.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/BlackOS.dir/source/Kmenu.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BlackOS.dir/source/Kmenu.cpp.o -c /Users/takudzwa/CLionProjects/BlackOS/source/Kmenu.cpp

CMakeFiles/BlackOS.dir/source/Kmenu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BlackOS.dir/source/Kmenu.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/takudzwa/CLionProjects/BlackOS/source/Kmenu.cpp > CMakeFiles/BlackOS.dir/source/Kmenu.cpp.i

CMakeFiles/BlackOS.dir/source/Kmenu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BlackOS.dir/source/Kmenu.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/takudzwa/CLionProjects/BlackOS/source/Kmenu.cpp -o CMakeFiles/BlackOS.dir/source/Kmenu.cpp.s

CMakeFiles/BlackOS.dir/source/Kwindow.cpp.o: CMakeFiles/BlackOS.dir/flags.make
CMakeFiles/BlackOS.dir/source/Kwindow.cpp.o: ../source/Kwindow.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/BlackOS.dir/source/Kwindow.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BlackOS.dir/source/Kwindow.cpp.o -c /Users/takudzwa/CLionProjects/BlackOS/source/Kwindow.cpp

CMakeFiles/BlackOS.dir/source/Kwindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BlackOS.dir/source/Kwindow.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/takudzwa/CLionProjects/BlackOS/source/Kwindow.cpp > CMakeFiles/BlackOS.dir/source/Kwindow.cpp.i

CMakeFiles/BlackOS.dir/source/Kwindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BlackOS.dir/source/Kwindow.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/takudzwa/CLionProjects/BlackOS/source/Kwindow.cpp -o CMakeFiles/BlackOS.dir/source/Kwindow.cpp.s

CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.o: CMakeFiles/BlackOS.dir/flags.make
CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.o: ../source/BlackOSScripts.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.o -c /Users/takudzwa/CLionProjects/BlackOS/source/BlackOSScripts.cpp

CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/takudzwa/CLionProjects/BlackOS/source/BlackOSScripts.cpp > CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.i

CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/takudzwa/CLionProjects/BlackOS/source/BlackOSScripts.cpp -o CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.s

# Object files for target BlackOS
BlackOS_OBJECTS = \
"CMakeFiles/BlackOS.dir/source/main.cpp.o" \
"CMakeFiles/BlackOS.dir/source/Kfield.cpp.o" \
"CMakeFiles/BlackOS.dir/source/Kmenu.cpp.o" \
"CMakeFiles/BlackOS.dir/source/Kwindow.cpp.o" \
"CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.o"

# External object files for target BlackOS
BlackOS_EXTERNAL_OBJECTS =

BlackOS: CMakeFiles/BlackOS.dir/source/main.cpp.o
BlackOS: CMakeFiles/BlackOS.dir/source/Kfield.cpp.o
BlackOS: CMakeFiles/BlackOS.dir/source/Kmenu.cpp.o
BlackOS: CMakeFiles/BlackOS.dir/source/Kwindow.cpp.o
BlackOS: CMakeFiles/BlackOS.dir/source/BlackOSScripts.cpp.o
BlackOS: CMakeFiles/BlackOS.dir/build.make
BlackOS: CMakeFiles/BlackOS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable BlackOS"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BlackOS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BlackOS.dir/build: BlackOS

.PHONY : CMakeFiles/BlackOS.dir/build

CMakeFiles/BlackOS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BlackOS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BlackOS.dir/clean

CMakeFiles/BlackOS.dir/depend:
	cd /Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/takudzwa/CLionProjects/BlackOS /Users/takudzwa/CLionProjects/BlackOS /Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug /Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug /Users/takudzwa/CLionProjects/BlackOS/cmake-build-debug/CMakeFiles/BlackOS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BlackOS.dir/depend

