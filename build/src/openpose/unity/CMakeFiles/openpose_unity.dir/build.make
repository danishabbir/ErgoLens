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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/whil/Developer/sub/openpose

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/whil/Developer/sub/openpose/build

# Include any dependencies generated for this target.
include src/openpose/unity/CMakeFiles/openpose_unity.dir/depend.make

# Include the progress variables for this target.
include src/openpose/unity/CMakeFiles/openpose_unity.dir/progress.make

# Include the compile flags for this target's objects.
include src/openpose/unity/CMakeFiles/openpose_unity.dir/flags.make

src/openpose/unity/CMakeFiles/openpose_unity.dir/unityBinding.cpp.o: src/openpose/unity/CMakeFiles/openpose_unity.dir/flags.make
src/openpose/unity/CMakeFiles/openpose_unity.dir/unityBinding.cpp.o: ../src/openpose/unity/unityBinding.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/whil/Developer/sub/openpose/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/openpose/unity/CMakeFiles/openpose_unity.dir/unityBinding.cpp.o"
	cd /home/whil/Developer/sub/openpose/build/src/openpose/unity && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/openpose_unity.dir/unityBinding.cpp.o -c /home/whil/Developer/sub/openpose/src/openpose/unity/unityBinding.cpp

src/openpose/unity/CMakeFiles/openpose_unity.dir/unityBinding.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openpose_unity.dir/unityBinding.cpp.i"
	cd /home/whil/Developer/sub/openpose/build/src/openpose/unity && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/whil/Developer/sub/openpose/src/openpose/unity/unityBinding.cpp > CMakeFiles/openpose_unity.dir/unityBinding.cpp.i

src/openpose/unity/CMakeFiles/openpose_unity.dir/unityBinding.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openpose_unity.dir/unityBinding.cpp.s"
	cd /home/whil/Developer/sub/openpose/build/src/openpose/unity && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/whil/Developer/sub/openpose/src/openpose/unity/unityBinding.cpp -o CMakeFiles/openpose_unity.dir/unityBinding.cpp.s

# Object files for target openpose_unity
openpose_unity_OBJECTS = \
"CMakeFiles/openpose_unity.dir/unityBinding.cpp.o"

# External object files for target openpose_unity
openpose_unity_EXTERNAL_OBJECTS =

src/openpose/unity/libopenpose_unity.so: src/openpose/unity/CMakeFiles/openpose_unity.dir/unityBinding.cpp.o
src/openpose/unity/libopenpose_unity.so: src/openpose/unity/CMakeFiles/openpose_unity.dir/build.make
src/openpose/unity/libopenpose_unity.so: src/openpose/pose/libopenpose_pose.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_shape.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_aruco.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_bgsegm.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_bioinspired.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_ccalib.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_datasets.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_dpm.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_face.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_freetype.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_fuzzy.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_hdf.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_line_descriptor.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_optflow.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_plot.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_reg.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_saliency.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_stereo.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_structured_light.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_surface_matching.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_text.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_ximgproc.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_xobjdetect.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_xphoto.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libglog.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libglog.so
src/openpose/unity/libopenpose_unity.so: caffe/lib/libcaffe.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libgflags.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libglut.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libXmu.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libXi.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libGL.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libGLU.so
src/openpose/unity/libopenpose_unity.so: src/openpose/core/libopenpose_core.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libcudart_static.a
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/librt.so
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_video.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_viz.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_phase_unwrapping.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_rgbd.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_videoio.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.3.2.0
src/openpose/unity/libopenpose_unity.so: /usr/lib/x86_64-linux-gnu/libopencv_core.so.3.2.0
src/openpose/unity/libopenpose_unity.so: src/openpose/unity/CMakeFiles/openpose_unity.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/whil/Developer/sub/openpose/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libopenpose_unity.so"
	cd /home/whil/Developer/sub/openpose/build/src/openpose/unity && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/openpose_unity.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/openpose/unity/CMakeFiles/openpose_unity.dir/build: src/openpose/unity/libopenpose_unity.so

.PHONY : src/openpose/unity/CMakeFiles/openpose_unity.dir/build

src/openpose/unity/CMakeFiles/openpose_unity.dir/clean:
	cd /home/whil/Developer/sub/openpose/build/src/openpose/unity && $(CMAKE_COMMAND) -P CMakeFiles/openpose_unity.dir/cmake_clean.cmake
.PHONY : src/openpose/unity/CMakeFiles/openpose_unity.dir/clean

src/openpose/unity/CMakeFiles/openpose_unity.dir/depend:
	cd /home/whil/Developer/sub/openpose/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/whil/Developer/sub/openpose /home/whil/Developer/sub/openpose/src/openpose/unity /home/whil/Developer/sub/openpose/build /home/whil/Developer/sub/openpose/build/src/openpose/unity /home/whil/Developer/sub/openpose/build/src/openpose/unity/CMakeFiles/openpose_unity.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/openpose/unity/CMakeFiles/openpose_unity.dir/depend

