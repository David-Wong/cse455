# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/david/Documents/cse455/hw4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/david/Documents/cse455/hw4

# Include any dependencies generated for this target.
include CMakeFiles/computeAccuracy.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/computeAccuracy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/computeAccuracy.dir/flags.make

CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o: CMakeFiles/computeAccuracy.dir/flags.make
CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o: computeAccuracy.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/david/Documents/cse455/hw4/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o -c /Users/david/Documents/cse455/hw4/computeAccuracy.cpp

CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/david/Documents/cse455/hw4/computeAccuracy.cpp > CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.i

CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/david/Documents/cse455/hw4/computeAccuracy.cpp -o CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.s

CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o.requires:
.PHONY : CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o.requires

CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o.provides: CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o.requires
	$(MAKE) -f CMakeFiles/computeAccuracy.dir/build.make CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o.provides.build
.PHONY : CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o.provides

CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o.provides.build: CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o

CMakeFiles/computeAccuracy.dir/utils.cpp.o: CMakeFiles/computeAccuracy.dir/flags.make
CMakeFiles/computeAccuracy.dir/utils.cpp.o: utils.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/david/Documents/cse455/hw4/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/computeAccuracy.dir/utils.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/computeAccuracy.dir/utils.cpp.o -c /Users/david/Documents/cse455/hw4/utils.cpp

CMakeFiles/computeAccuracy.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/computeAccuracy.dir/utils.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/david/Documents/cse455/hw4/utils.cpp > CMakeFiles/computeAccuracy.dir/utils.cpp.i

CMakeFiles/computeAccuracy.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/computeAccuracy.dir/utils.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/david/Documents/cse455/hw4/utils.cpp -o CMakeFiles/computeAccuracy.dir/utils.cpp.s

CMakeFiles/computeAccuracy.dir/utils.cpp.o.requires:
.PHONY : CMakeFiles/computeAccuracy.dir/utils.cpp.o.requires

CMakeFiles/computeAccuracy.dir/utils.cpp.o.provides: CMakeFiles/computeAccuracy.dir/utils.cpp.o.requires
	$(MAKE) -f CMakeFiles/computeAccuracy.dir/build.make CMakeFiles/computeAccuracy.dir/utils.cpp.o.provides.build
.PHONY : CMakeFiles/computeAccuracy.dir/utils.cpp.o.provides

CMakeFiles/computeAccuracy.dir/utils.cpp.o.provides.build: CMakeFiles/computeAccuracy.dir/utils.cpp.o

# Object files for target computeAccuracy
computeAccuracy_OBJECTS = \
"CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o" \
"CMakeFiles/computeAccuracy.dir/utils.cpp.o"

# External object files for target computeAccuracy
computeAccuracy_EXTERNAL_OBJECTS =

computeAccuracy: CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o
computeAccuracy: CMakeFiles/computeAccuracy.dir/utils.cpp.o
computeAccuracy: CMakeFiles/computeAccuracy.dir/build.make
computeAccuracy: CMakeFiles/computeAccuracy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable computeAccuracy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/computeAccuracy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/computeAccuracy.dir/build: computeAccuracy
.PHONY : CMakeFiles/computeAccuracy.dir/build

CMakeFiles/computeAccuracy.dir/requires: CMakeFiles/computeAccuracy.dir/computeAccuracy.cpp.o.requires
CMakeFiles/computeAccuracy.dir/requires: CMakeFiles/computeAccuracy.dir/utils.cpp.o.requires
.PHONY : CMakeFiles/computeAccuracy.dir/requires

CMakeFiles/computeAccuracy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/computeAccuracy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/computeAccuracy.dir/clean

CMakeFiles/computeAccuracy.dir/depend:
	cd /Users/david/Documents/cse455/hw4 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/david/Documents/cse455/hw4 /Users/david/Documents/cse455/hw4 /Users/david/Documents/cse455/hw4 /Users/david/Documents/cse455/hw4 /Users/david/Documents/cse455/hw4/CMakeFiles/computeAccuracy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/computeAccuracy.dir/depend
