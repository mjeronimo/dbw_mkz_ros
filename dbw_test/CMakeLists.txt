cmake_minimum_required(VERSION 2.8.3)
project(dbw_test)

catkin_python_setup()

find_package(catkin REQUIRED COMPONENTS
  dbw_mkz_msgs
  rospy
)

catkin_package()

