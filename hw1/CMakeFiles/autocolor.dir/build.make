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
CMAKE_SOURCE_DIR = /Users/david/Documents/cse455/hw1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/david/Documents/cse455/hw1

# Include any dependencies generated for this target.
include CMakeFiles/autocolor.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/autocolor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/autocolor.dir/flags.make

CMakeFiles/autocolor.dir/autocolor.cpp.o: CMakeFiles/autocolor.dir/flags.make
CMakeFiles/autocolor.dir/autocolor.cpp.o: autocolor.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/david/Documents/cse455/hw1/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/autocolor.dir/autocolor.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/autocolor.dir/autocolor.cpp.o -c /Users/david/Documents/cse455/hw1/autocolor.cpp

CMakeFiles/autocolor.dir/autocolor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/autocolor.dir/autocolor.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/david/Documents/cse455/hw1/autocolor.cpp > CMakeFiles/autocolor.dir/autocolor.cpp.i

CMakeFiles/autocolor.dir/autocolor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/autocolor.dir/autocolor.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/david/Documents/cse455/hw1/autocolor.cpp -o CMakeFiles/autocolor.dir/autocolor.cpp.s

CMakeFiles/autocolor.dir/autocolor.cpp.o.requires:
.PHONY : CMakeFiles/autocolor.dir/autocolor.cpp.o.requires

CMakeFiles/autocolor.dir/autocolor.cpp.o.provides: CMakeFiles/autocolor.dir/autocolor.cpp.o.requires
	$(MAKE) -f CMakeFiles/autocolor.dir/build.make CMakeFiles/autocolor.dir/autocolor.cpp.o.provides.build
.PHONY : CMakeFiles/autocolor.dir/autocolor.cpp.o.provides

CMakeFiles/autocolor.dir/autocolor.cpp.o.provides.build: CMakeFiles/autocolor.dir/autocolor.cpp.o

CMakeFiles/autocolor.dir/utils.cpp.o: CMakeFiles/autocolor.dir/flags.make
CMakeFiles/autocolor.dir/utils.cpp.o: utils.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/david/Documents/cse455/hw1/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/autocolor.dir/utils.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/autocolor.dir/utils.cpp.o -c /Users/david/Documents/cse455/hw1/utils.cpp

CMakeFiles/autocolor.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/autocolor.dir/utils.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/david/Documents/cse455/hw1/utils.cpp > CMakeFiles/autocolor.dir/utils.cpp.i

CMakeFiles/autocolor.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/autocolor.dir/utils.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/david/Documents/cse455/hw1/utils.cpp -o CMakeFiles/autocolor.dir/utils.cpp.s

CMakeFiles/autocolor.dir/utils.cpp.o.requires:
.PHONY : CMakeFiles/autocolor.dir/utils.cpp.o.requires

CMakeFiles/autocolor.dir/utils.cpp.o.provides: CMakeFiles/autocolor.dir/utils.cpp.o.requires
	$(MAKE) -f CMakeFiles/autocolor.dir/build.make CMakeFiles/autocolor.dir/utils.cpp.o.provides.build
.PHONY : CMakeFiles/autocolor.dir/utils.cpp.o.provides

CMakeFiles/autocolor.dir/utils.cpp.o.provides.build: CMakeFiles/autocolor.dir/utils.cpp.o

# Object files for target autocolor
autocolor_OBJECTS = \
"CMakeFiles/autocolor.dir/autocolor.cpp.o" \
"CMakeFiles/autocolor.dir/utils.cpp.o"

# External object files for target autocolor
autocolor_EXTERNAL_OBJECTS =

autocolor: CMakeFiles/autocolor.dir/autocolor.cpp.o
autocolor: CMakeFiles/autocolor.dir/utils.cpp.o
autocolor: CMakeFiles/autocolor.dir/build.make
autocolor: CMakeFiles/autocolor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable autocolor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/autocolor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/autocolor.dir/build: autocolor
.PHONY : CMakeFiles/autocolor.dir/build

CMakeFiles/autocolor.dir/requires: CMakeFiles/autocolor.dir/autocolor.cpp.o.requires
CMakeFiles/autocolor.dir/requires: CMakeFiles/autocolor.dir/utils.cpp.o.requires
.PHONY : CMakeFiles/autocolor.dir/requires

CMakeFiles/autocolor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/autocolor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/autocolor.dir/clean

CMakeFiles/autocolor.dir/depend:
	cd /Users/david/Documents/cse455/hw1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/david/Documents/cse455/hw1 /Users/david/Documents/cse455/hw1 /Users/david/Documents/cse455/hw1 /Users/david/Documents/cse455/hw1 /Users/david/Documents/cse455/hw1/CMakeFiles/autocolor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/autocolor.dir/depend

