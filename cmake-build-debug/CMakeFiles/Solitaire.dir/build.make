# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Solitaire.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Solitaire.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Solitaire.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Solitaire.dir/flags.make

CMakeFiles/Solitaire.dir/Main.c.obj: CMakeFiles/Solitaire.dir/flags.make
CMakeFiles/Solitaire.dir/Main.c.obj: C:/Users/lingn/Downloads/Solitaire-main/Solitaire-main/Main.c
CMakeFiles/Solitaire.dir/Main.c.obj: CMakeFiles/Solitaire.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Solitaire.dir/Main.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Solitaire.dir/Main.c.obj -MF CMakeFiles\Solitaire.dir\Main.c.obj.d -o CMakeFiles\Solitaire.dir\Main.c.obj -c C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main\Main.c

CMakeFiles/Solitaire.dir/Main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Solitaire.dir/Main.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main\Main.c > CMakeFiles\Solitaire.dir\Main.c.i

CMakeFiles/Solitaire.dir/Main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Solitaire.dir/Main.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main\Main.c -o CMakeFiles\Solitaire.dir\Main.c.s

# Object files for target Solitaire
Solitaire_OBJECTS = \
"CMakeFiles/Solitaire.dir/Main.c.obj"

# External object files for target Solitaire
Solitaire_EXTERNAL_OBJECTS =

Solitaire.exe: CMakeFiles/Solitaire.dir/Main.c.obj
Solitaire.exe: CMakeFiles/Solitaire.dir/build.make
Solitaire.exe: CMakeFiles/Solitaire.dir/linkLibs.rsp
Solitaire.exe: CMakeFiles/Solitaire.dir/objects1.rsp
Solitaire.exe: CMakeFiles/Solitaire.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Solitaire.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Solitaire.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Solitaire.dir/build: Solitaire.exe
.PHONY : CMakeFiles/Solitaire.dir/build

CMakeFiles/Solitaire.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Solitaire.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Solitaire.dir/clean

CMakeFiles/Solitaire.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main\cmake-build-debug C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main\cmake-build-debug C:\Users\lingn\Downloads\Solitaire-main\Solitaire-main\cmake-build-debug\CMakeFiles\Solitaire.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Solitaire.dir/depend

