# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /Users/g1ft/Desktop/homework/CMU_745/ass1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug

# Include any dependencies generated for this target.
include LocalOpt/CMakeFiles/LocalOptPass.dir/depend.make

# Include the progress variables for this target.
include LocalOpt/CMakeFiles/LocalOptPass.dir/progress.make

# Include the compile flags for this target's objects.
include LocalOpt/CMakeFiles/LocalOptPass.dir/flags.make

LocalOpt/CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.o: LocalOpt/CMakeFiles/LocalOptPass.dir/flags.make
LocalOpt/CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.o: ../LocalOpt/LocalOpt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object LocalOpt/CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.o"
	cd /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug/LocalOpt && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.o -c /Users/g1ft/Desktop/homework/CMU_745/ass1/LocalOpt/LocalOpt.cpp

LocalOpt/CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.i"
	cd /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug/LocalOpt && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/g1ft/Desktop/homework/CMU_745/ass1/LocalOpt/LocalOpt.cpp > CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.i

LocalOpt/CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.s"
	cd /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug/LocalOpt && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/g1ft/Desktop/homework/CMU_745/ass1/LocalOpt/LocalOpt.cpp -o CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.s

# Object files for target LocalOptPass
LocalOptPass_OBJECTS = \
"CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.o"

# External object files for target LocalOptPass
LocalOptPass_EXTERNAL_OBJECTS =

LocalOpt/libLocalOptPass.so: LocalOpt/CMakeFiles/LocalOptPass.dir/LocalOpt.cpp.o
LocalOpt/libLocalOptPass.so: LocalOpt/CMakeFiles/LocalOptPass.dir/build.make
LocalOpt/libLocalOptPass.so: LocalOpt/CMakeFiles/LocalOptPass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module libLocalOptPass.so"
	cd /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug/LocalOpt && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LocalOptPass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
LocalOpt/CMakeFiles/LocalOptPass.dir/build: LocalOpt/libLocalOptPass.so

.PHONY : LocalOpt/CMakeFiles/LocalOptPass.dir/build

LocalOpt/CMakeFiles/LocalOptPass.dir/clean:
	cd /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug/LocalOpt && $(CMAKE_COMMAND) -P CMakeFiles/LocalOptPass.dir/cmake_clean.cmake
.PHONY : LocalOpt/CMakeFiles/LocalOptPass.dir/clean

LocalOpt/CMakeFiles/LocalOptPass.dir/depend:
	cd /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/g1ft/Desktop/homework/CMU_745/ass1 /Users/g1ft/Desktop/homework/CMU_745/ass1/LocalOpt /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug/LocalOpt /Users/g1ft/Desktop/homework/CMU_745/ass1/cmake-build-debug/LocalOpt/CMakeFiles/LocalOptPass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : LocalOpt/CMakeFiles/LocalOptPass.dir/depend
