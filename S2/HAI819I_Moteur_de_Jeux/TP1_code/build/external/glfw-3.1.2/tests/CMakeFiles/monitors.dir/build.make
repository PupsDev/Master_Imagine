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
CMAKE_SOURCE_DIR = /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build

# Include any dependencies generated for this target.
include external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/compiler_depend.make

# Include the progress variables for this target.
include external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/progress.make

# Include the compile flags for this target's objects.
include external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/flags.make

external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/monitors.c.o: external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/flags.make
external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/monitors.c.o: ../external/glfw-3.1.2/tests/monitors.c
external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/monitors.c.o: external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/monitors.c.o"
	cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/monitors.c.o -MF CMakeFiles/monitors.dir/monitors.c.o.d -o CMakeFiles/monitors.dir/monitors.c.o -c /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/external/glfw-3.1.2/tests/monitors.c

external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/monitors.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/monitors.dir/monitors.c.i"
	cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/external/glfw-3.1.2/tests/monitors.c > CMakeFiles/monitors.dir/monitors.c.i

external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/monitors.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/monitors.dir/monitors.c.s"
	cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/external/glfw-3.1.2/tests/monitors.c -o CMakeFiles/monitors.dir/monitors.c.s

external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o: external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/flags.make
external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o: ../external/glfw-3.1.2/deps/getopt.c
external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o: external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o"
	cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o -MF CMakeFiles/monitors.dir/__/deps/getopt.c.o.d -o CMakeFiles/monitors.dir/__/deps/getopt.c.o -c /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/external/glfw-3.1.2/deps/getopt.c

external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/monitors.dir/__/deps/getopt.c.i"
	cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/external/glfw-3.1.2/deps/getopt.c > CMakeFiles/monitors.dir/__/deps/getopt.c.i

external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/monitors.dir/__/deps/getopt.c.s"
	cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/external/glfw-3.1.2/deps/getopt.c -o CMakeFiles/monitors.dir/__/deps/getopt.c.s

# Object files for target monitors
monitors_OBJECTS = \
"CMakeFiles/monitors.dir/monitors.c.o" \
"CMakeFiles/monitors.dir/__/deps/getopt.c.o"

# External object files for target monitors
monitors_EXTERNAL_OBJECTS =

external/glfw-3.1.2/tests/monitors: external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/monitors.c.o
external/glfw-3.1.2/tests/monitors: external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o
external/glfw-3.1.2/tests/monitors: external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/build.make
external/glfw-3.1.2/tests/monitors: external/glfw-3.1.2/src/libglfw3.a
external/glfw-3.1.2/tests/monitors: /usr/lib/x86_64-linux-gnu/librt.so
external/glfw-3.1.2/tests/monitors: /usr/lib/x86_64-linux-gnu/libm.so
external/glfw-3.1.2/tests/monitors: /usr/lib/x86_64-linux-gnu/libX11.so
external/glfw-3.1.2/tests/monitors: /usr/lib/x86_64-linux-gnu/libXrandr.so
external/glfw-3.1.2/tests/monitors: /usr/lib/x86_64-linux-gnu/libXinerama.so
external/glfw-3.1.2/tests/monitors: /usr/lib/x86_64-linux-gnu/libXi.so
external/glfw-3.1.2/tests/monitors: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
external/glfw-3.1.2/tests/monitors: /usr/lib/x86_64-linux-gnu/libXcursor.so
external/glfw-3.1.2/tests/monitors: /usr/lib/x86_64-linux-gnu/libGL.so
external/glfw-3.1.2/tests/monitors: external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable monitors"
	cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/monitors.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/build: external/glfw-3.1.2/tests/monitors
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/build

external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/clean:
	cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -P CMakeFiles/monitors.dir/cmake_clean.cmake
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/clean

external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/depend:
	cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/external/glfw-3.1.2/tests /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/monitors.dir/depend

