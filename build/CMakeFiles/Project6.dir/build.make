# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /volume

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /volume/build

# Include any dependencies generated for this target.
include CMakeFiles/Project6.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Project6.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Project6.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Project6.dir/flags.make

CMakeFiles/Project6.dir/main.cpp.o: CMakeFiles/Project6.dir/flags.make
CMakeFiles/Project6.dir/main.cpp.o: ../main.cpp
CMakeFiles/Project6.dir/main.cpp.o: CMakeFiles/Project6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/volume/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Project6.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Project6.dir/main.cpp.o -MF CMakeFiles/Project6.dir/main.cpp.o.d -o CMakeFiles/Project6.dir/main.cpp.o -c /volume/main.cpp

CMakeFiles/Project6.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project6.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /volume/main.cpp > CMakeFiles/Project6.dir/main.cpp.i

CMakeFiles/Project6.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project6.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /volume/main.cpp -o CMakeFiles/Project6.dir/main.cpp.s

CMakeFiles/Project6.dir/DatabaseManipulation.cpp.o: CMakeFiles/Project6.dir/flags.make
CMakeFiles/Project6.dir/DatabaseManipulation.cpp.o: ../DatabaseManipulation.cpp
CMakeFiles/Project6.dir/DatabaseManipulation.cpp.o: CMakeFiles/Project6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/volume/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Project6.dir/DatabaseManipulation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Project6.dir/DatabaseManipulation.cpp.o -MF CMakeFiles/Project6.dir/DatabaseManipulation.cpp.o.d -o CMakeFiles/Project6.dir/DatabaseManipulation.cpp.o -c /volume/DatabaseManipulation.cpp

CMakeFiles/Project6.dir/DatabaseManipulation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project6.dir/DatabaseManipulation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /volume/DatabaseManipulation.cpp > CMakeFiles/Project6.dir/DatabaseManipulation.cpp.i

CMakeFiles/Project6.dir/DatabaseManipulation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project6.dir/DatabaseManipulation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /volume/DatabaseManipulation.cpp -o CMakeFiles/Project6.dir/DatabaseManipulation.cpp.s

# Object files for target Project6
Project6_OBJECTS = \
"CMakeFiles/Project6.dir/main.cpp.o" \
"CMakeFiles/Project6.dir/DatabaseManipulation.cpp.o"

# External object files for target Project6
Project6_EXTERNAL_OBJECTS =

Project6: CMakeFiles/Project6.dir/main.cpp.o
Project6: CMakeFiles/Project6.dir/DatabaseManipulation.cpp.o
Project6: CMakeFiles/Project6.dir/build.make
Project6: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.74.0
Project6: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.74.0
Project6: CMakeFiles/Project6.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/volume/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Project6"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Project6.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Project6.dir/build: Project6
.PHONY : CMakeFiles/Project6.dir/build

CMakeFiles/Project6.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Project6.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Project6.dir/clean

CMakeFiles/Project6.dir/depend:
	cd /volume/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /volume /volume /volume/build /volume/build /volume/build/CMakeFiles/Project6.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Project6.dir/depend

