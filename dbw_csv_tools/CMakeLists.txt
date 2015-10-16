cmake_minimum_required(VERSION 2.8.3)
project(dbw_csv_tools)

find_package(catkin REQUIRED COMPONENTS
  dbw_mkz_msgs
  rosbag
  roslib
  rospy
  sensor_msgs
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

