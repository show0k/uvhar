# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/build

# Utility rule file for ROSBUILD_gensrv_cpp.

CMakeFiles/ROSBUILD_gensrv_cpp: ../srv_gen/cpp/include/ardrone_driver/Takeoff.h
CMakeFiles/ROSBUILD_gensrv_cpp: ../srv_gen/cpp/include/ardrone_driver/Reset.h
CMakeFiles/ROSBUILD_gensrv_cpp: ../srv_gen/cpp/include/ardrone_driver/Land.h

../srv_gen/cpp/include/ardrone_driver/Takeoff.h: ../srv/Takeoff.srv
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/roscpp/scripts/gensrv_cpp.py
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/roscpp/scripts/genmsg_cpp.py
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/roslib/scripts/gendeps
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: ../manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/genmsg_cpp/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/tools/rospack/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/roslib/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/roslang/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/rospy/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/3rdparty/xmlrpcpp/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/rosconsole/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/roscpp/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/std_msgs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/std_srvs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/tools/rosclean/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/tools/rosgraph/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/rosmaster/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/rosout/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/tools/roslaunch/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/test/rostest/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/tools/topic_tools/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/tools/rosbag/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/tools/rosrecord/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/tools/rosbagmigration/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/stacks/common_msgs/geometry_msgs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/stacks/common_msgs/sensor_msgs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/roslib/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/roscpp/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/core/roscpp/srv_gen/generated
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/std_msgs/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/std_srvs/srv_gen/generated
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/ros/tools/topic_tools/srv_gen/generated
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/stacks/common_msgs/geometry_msgs/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/stacks/common_msgs/sensor_msgs/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Takeoff.h: /opt/ros/cturtle/stacks/common_msgs/sensor_msgs/srv_gen/generated
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../srv_gen/cpp/include/ardrone_driver/Takeoff.h"
	/opt/ros/cturtle/ros/core/roscpp/scripts/gensrv_cpp.py /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/srv/Takeoff.srv

../srv_gen/cpp/include/ardrone_driver/Reset.h: ../srv/Reset.srv
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/roscpp/scripts/gensrv_cpp.py
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/roscpp/scripts/genmsg_cpp.py
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/roslib/scripts/gendeps
../srv_gen/cpp/include/ardrone_driver/Reset.h: ../manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/genmsg_cpp/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/tools/rospack/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/roslib/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/roslang/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/rospy/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/3rdparty/xmlrpcpp/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/rosconsole/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/roscpp/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/std_msgs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/std_srvs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/tools/rosclean/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/tools/rosgraph/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/rosmaster/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/rosout/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/tools/roslaunch/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/test/rostest/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/tools/topic_tools/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/tools/rosbag/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/tools/rosrecord/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/tools/rosbagmigration/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/stacks/common_msgs/geometry_msgs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/stacks/common_msgs/sensor_msgs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/roslib/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/roscpp/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/core/roscpp/srv_gen/generated
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/std_msgs/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/std_srvs/srv_gen/generated
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/ros/tools/topic_tools/srv_gen/generated
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/stacks/common_msgs/geometry_msgs/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/stacks/common_msgs/sensor_msgs/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Reset.h: /opt/ros/cturtle/stacks/common_msgs/sensor_msgs/srv_gen/generated
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../srv_gen/cpp/include/ardrone_driver/Reset.h"
	/opt/ros/cturtle/ros/core/roscpp/scripts/gensrv_cpp.py /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/srv/Reset.srv

../srv_gen/cpp/include/ardrone_driver/Land.h: ../srv/Land.srv
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/roscpp/scripts/gensrv_cpp.py
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/roscpp/scripts/genmsg_cpp.py
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/roslib/scripts/gendeps
../srv_gen/cpp/include/ardrone_driver/Land.h: ../manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/genmsg_cpp/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/tools/rospack/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/roslib/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/roslang/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/rospy/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/3rdparty/xmlrpcpp/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/rosconsole/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/roscpp/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/std_msgs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/std_srvs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/tools/rosclean/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/tools/rosgraph/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/rosmaster/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/rosout/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/tools/roslaunch/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/test/rostest/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/tools/topic_tools/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/tools/rosbag/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/tools/rosrecord/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/tools/rosbagmigration/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/stacks/common_msgs/geometry_msgs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/stacks/common_msgs/sensor_msgs/manifest.xml
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/roslib/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/roscpp/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/core/roscpp/srv_gen/generated
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/std_msgs/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/std_srvs/srv_gen/generated
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/ros/tools/topic_tools/srv_gen/generated
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/stacks/common_msgs/geometry_msgs/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/stacks/common_msgs/sensor_msgs/msg_gen/generated
../srv_gen/cpp/include/ardrone_driver/Land.h: /opt/ros/cturtle/stacks/common_msgs/sensor_msgs/srv_gen/generated
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating ../srv_gen/cpp/include/ardrone_driver/Land.h"
	/opt/ros/cturtle/ros/core/roscpp/scripts/gensrv_cpp.py /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/srv/Land.srv

ROSBUILD_gensrv_cpp: CMakeFiles/ROSBUILD_gensrv_cpp
ROSBUILD_gensrv_cpp: ../srv_gen/cpp/include/ardrone_driver/Takeoff.h
ROSBUILD_gensrv_cpp: ../srv_gen/cpp/include/ardrone_driver/Reset.h
ROSBUILD_gensrv_cpp: ../srv_gen/cpp/include/ardrone_driver/Land.h
ROSBUILD_gensrv_cpp: CMakeFiles/ROSBUILD_gensrv_cpp.dir/build.make
.PHONY : ROSBUILD_gensrv_cpp

# Rule to build all files generated by this target.
CMakeFiles/ROSBUILD_gensrv_cpp.dir/build: ROSBUILD_gensrv_cpp
.PHONY : CMakeFiles/ROSBUILD_gensrv_cpp.dir/build

CMakeFiles/ROSBUILD_gensrv_cpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ROSBUILD_gensrv_cpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ROSBUILD_gensrv_cpp.dir/clean

CMakeFiles/ROSBUILD_gensrv_cpp.dir/depend:
	cd /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/build /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/build /home/maarten/uva/uvhar/ros/siue-cs-siue-ros-pkg-a96b211/ardrone/ardrone_driver/build/CMakeFiles/ROSBUILD_gensrv_cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ROSBUILD_gensrv_cpp.dir/depend

