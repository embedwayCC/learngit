# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chencheng/learngit/learn_cmake/cmake_demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chencheng/learngit/learn_cmake/cmake_demo/build

# Include any dependencies generated for this target.
include src/demo/CMakeFiles/demo.dir/depend.make

# Include the progress variables for this target.
include src/demo/CMakeFiles/demo.dir/progress.make

# Include the compile flags for this target's objects.
include src/demo/CMakeFiles/demo.dir/flags.make

src/demo/CMakeFiles/demo.dir/demo_main.c.o: src/demo/CMakeFiles/demo.dir/flags.make
src/demo/CMakeFiles/demo.dir/demo_main.c.o: ../src/demo/demo_main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chencheng/learngit/learn_cmake/cmake_demo/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/demo/CMakeFiles/demo.dir/demo_main.c.o"
	cd /home/chencheng/learngit/learn_cmake/cmake_demo/build/src/demo && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/demo.dir/demo_main.c.o   -c /home/chencheng/learngit/learn_cmake/cmake_demo/src/demo/demo_main.c

src/demo/CMakeFiles/demo.dir/demo_main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/demo.dir/demo_main.c.i"
	cd /home/chencheng/learngit/learn_cmake/cmake_demo/build/src/demo && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/chencheng/learngit/learn_cmake/cmake_demo/src/demo/demo_main.c > CMakeFiles/demo.dir/demo_main.c.i

src/demo/CMakeFiles/demo.dir/demo_main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/demo.dir/demo_main.c.s"
	cd /home/chencheng/learngit/learn_cmake/cmake_demo/build/src/demo && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/chencheng/learngit/learn_cmake/cmake_demo/src/demo/demo_main.c -o CMakeFiles/demo.dir/demo_main.c.s

src/demo/CMakeFiles/demo.dir/demo_main.c.o.requires:
.PHONY : src/demo/CMakeFiles/demo.dir/demo_main.c.o.requires

src/demo/CMakeFiles/demo.dir/demo_main.c.o.provides: src/demo/CMakeFiles/demo.dir/demo_main.c.o.requires
	$(MAKE) -f src/demo/CMakeFiles/demo.dir/build.make src/demo/CMakeFiles/demo.dir/demo_main.c.o.provides.build
.PHONY : src/demo/CMakeFiles/demo.dir/demo_main.c.o.provides

src/demo/CMakeFiles/demo.dir/demo_main.c.o.provides.build: src/demo/CMakeFiles/demo.dir/demo_main.c.o

# Object files for target demo
demo_OBJECTS = \
"CMakeFiles/demo.dir/demo_main.c.o"

# External object files for target demo
demo_EXTERNAL_OBJECTS =

../bin/demo: src/demo/CMakeFiles/demo.dir/demo_main.c.o
../bin/demo: src/demo/CMakeFiles/demo.dir/build.make
../bin/demo: src/demo/CMakeFiles/demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../../../bin/demo"
	cd /home/chencheng/learngit/learn_cmake/cmake_demo/build/src/demo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/demo/CMakeFiles/demo.dir/build: ../bin/demo
.PHONY : src/demo/CMakeFiles/demo.dir/build

src/demo/CMakeFiles/demo.dir/requires: src/demo/CMakeFiles/demo.dir/demo_main.c.o.requires
.PHONY : src/demo/CMakeFiles/demo.dir/requires

src/demo/CMakeFiles/demo.dir/clean:
	cd /home/chencheng/learngit/learn_cmake/cmake_demo/build/src/demo && $(CMAKE_COMMAND) -P CMakeFiles/demo.dir/cmake_clean.cmake
.PHONY : src/demo/CMakeFiles/demo.dir/clean

src/demo/CMakeFiles/demo.dir/depend:
	cd /home/chencheng/learngit/learn_cmake/cmake_demo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chencheng/learngit/learn_cmake/cmake_demo /home/chencheng/learngit/learn_cmake/cmake_demo/src/demo /home/chencheng/learngit/learn_cmake/cmake_demo/build /home/chencheng/learngit/learn_cmake/cmake_demo/build/src/demo /home/chencheng/learngit/learn_cmake/cmake_demo/build/src/demo/CMakeFiles/demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/demo/CMakeFiles/demo.dir/depend

