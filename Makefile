# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zach/Documents/Drone/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zach/Documents/Drone

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/local/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/zach/Documents/Drone/CMakeFiles /home/zach/Documents/Drone/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/zach/Documents/Drone/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Drone

# Build rule for target.
Drone: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 Drone
.PHONY : Drone

# fast build rule for target.
Drone/fast:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/build
.PHONY : Drone/fast

cmdmgr/cmdmgr_buffer.o: cmdmgr/cmdmgr_buffer.c.o

.PHONY : cmdmgr/cmdmgr_buffer.o

# target to build an object file
cmdmgr/cmdmgr_buffer.c.o:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_buffer.c.o
.PHONY : cmdmgr/cmdmgr_buffer.c.o

cmdmgr/cmdmgr_buffer.i: cmdmgr/cmdmgr_buffer.c.i

.PHONY : cmdmgr/cmdmgr_buffer.i

# target to preprocess a source file
cmdmgr/cmdmgr_buffer.c.i:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_buffer.c.i
.PHONY : cmdmgr/cmdmgr_buffer.c.i

cmdmgr/cmdmgr_buffer.s: cmdmgr/cmdmgr_buffer.c.s

.PHONY : cmdmgr/cmdmgr_buffer.s

# target to generate assembly for a file
cmdmgr/cmdmgr_buffer.c.s:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_buffer.c.s
.PHONY : cmdmgr/cmdmgr_buffer.c.s

cmdmgr/cmdmgr_cmd.o: cmdmgr/cmdmgr_cmd.c.o

.PHONY : cmdmgr/cmdmgr_cmd.o

# target to build an object file
cmdmgr/cmdmgr_cmd.c.o:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_cmd.c.o
.PHONY : cmdmgr/cmdmgr_cmd.c.o

cmdmgr/cmdmgr_cmd.i: cmdmgr/cmdmgr_cmd.c.i

.PHONY : cmdmgr/cmdmgr_cmd.i

# target to preprocess a source file
cmdmgr/cmdmgr_cmd.c.i:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_cmd.c.i
.PHONY : cmdmgr/cmdmgr_cmd.c.i

cmdmgr/cmdmgr_cmd.s: cmdmgr/cmdmgr_cmd.c.s

.PHONY : cmdmgr/cmdmgr_cmd.s

# target to generate assembly for a file
cmdmgr/cmdmgr_cmd.c.s:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_cmd.c.s
.PHONY : cmdmgr/cmdmgr_cmd.c.s

cmdmgr/cmdmgr_crc.o: cmdmgr/cmdmgr_crc.c.o

.PHONY : cmdmgr/cmdmgr_crc.o

# target to build an object file
cmdmgr/cmdmgr_crc.c.o:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_crc.c.o
.PHONY : cmdmgr/cmdmgr_crc.c.o

cmdmgr/cmdmgr_crc.i: cmdmgr/cmdmgr_crc.c.i

.PHONY : cmdmgr/cmdmgr_crc.i

# target to preprocess a source file
cmdmgr/cmdmgr_crc.c.i:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_crc.c.i
.PHONY : cmdmgr/cmdmgr_crc.c.i

cmdmgr/cmdmgr_crc.s: cmdmgr/cmdmgr_crc.c.s

.PHONY : cmdmgr/cmdmgr_crc.s

# target to generate assembly for a file
cmdmgr/cmdmgr_crc.c.s:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_crc.c.s
.PHONY : cmdmgr/cmdmgr_crc.c.s

cmdmgr/cmdmgr_handler.o: cmdmgr/cmdmgr_handler.c.o

.PHONY : cmdmgr/cmdmgr_handler.o

# target to build an object file
cmdmgr/cmdmgr_handler.c.o:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_handler.c.o
.PHONY : cmdmgr/cmdmgr_handler.c.o

cmdmgr/cmdmgr_handler.i: cmdmgr/cmdmgr_handler.c.i

.PHONY : cmdmgr/cmdmgr_handler.i

# target to preprocess a source file
cmdmgr/cmdmgr_handler.c.i:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_handler.c.i
.PHONY : cmdmgr/cmdmgr_handler.c.i

cmdmgr/cmdmgr_handler.s: cmdmgr/cmdmgr_handler.c.s

.PHONY : cmdmgr/cmdmgr_handler.s

# target to generate assembly for a file
cmdmgr/cmdmgr_handler.c.s:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/cmdmgr/cmdmgr_handler.c.s
.PHONY : cmdmgr/cmdmgr_handler.c.s

ctrlmgr/ctrlmgr_ctrl.o: ctrlmgr/ctrlmgr_ctrl.c.o

.PHONY : ctrlmgr/ctrlmgr_ctrl.o

# target to build an object file
ctrlmgr/ctrlmgr_ctrl.c.o:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/ctrlmgr/ctrlmgr_ctrl.c.o
.PHONY : ctrlmgr/ctrlmgr_ctrl.c.o

ctrlmgr/ctrlmgr_ctrl.i: ctrlmgr/ctrlmgr_ctrl.c.i

.PHONY : ctrlmgr/ctrlmgr_ctrl.i

# target to preprocess a source file
ctrlmgr/ctrlmgr_ctrl.c.i:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/ctrlmgr/ctrlmgr_ctrl.c.i
.PHONY : ctrlmgr/ctrlmgr_ctrl.c.i

ctrlmgr/ctrlmgr_ctrl.s: ctrlmgr/ctrlmgr_ctrl.c.s

.PHONY : ctrlmgr/ctrlmgr_ctrl.s

# target to generate assembly for a file
ctrlmgr/ctrlmgr_ctrl.c.s:
	$(MAKE) -f CMakeFiles/Drone.dir/build.make CMakeFiles/Drone.dir/ctrlmgr/ctrlmgr_ctrl.c.s
.PHONY : ctrlmgr/ctrlmgr_ctrl.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... Drone"
	@echo "... cmdmgr/cmdmgr_buffer.o"
	@echo "... cmdmgr/cmdmgr_buffer.i"
	@echo "... cmdmgr/cmdmgr_buffer.s"
	@echo "... cmdmgr/cmdmgr_cmd.o"
	@echo "... cmdmgr/cmdmgr_cmd.i"
	@echo "... cmdmgr/cmdmgr_cmd.s"
	@echo "... cmdmgr/cmdmgr_crc.o"
	@echo "... cmdmgr/cmdmgr_crc.i"
	@echo "... cmdmgr/cmdmgr_crc.s"
	@echo "... cmdmgr/cmdmgr_handler.o"
	@echo "... cmdmgr/cmdmgr_handler.i"
	@echo "... cmdmgr/cmdmgr_handler.s"
	@echo "... ctrlmgr/ctrlmgr_ctrl.o"
	@echo "... ctrlmgr/ctrlmgr_ctrl.i"
	@echo "... ctrlmgr/ctrlmgr_ctrl.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

