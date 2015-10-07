cmake_minimum_required(VERSION 2.8.3)
project(dbw_test)

#catkin_python_setup()

find_package(catkin REQUIRED COMPONENTS
  dbw_mkz_msgs
  roslib
  rospy
  roscpp
)

catkin_package()

add_executable(gear_ratio src/gear_ratio.cpp)
target_link_libraries(gear_ratio
  ${catkin_LIBRARIES}
)