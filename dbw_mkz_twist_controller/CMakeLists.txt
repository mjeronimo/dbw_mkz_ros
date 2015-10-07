cmake_minimum_required(VERSION 2.8.3)
project(dbw_mkz_twist_controller)

find_package(catkin REQUIRED COMPONENTS
  dynamic_reconfigure
  dbw_mkz_msgs
  roscpp
  roslib
  tf
)

generate_dynamic_reconfigure_options(
  cfg/Controller.cfg
  cfg/TwistTest.cfg
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(twist_controller src/twist_controller_node.cpp src/TwistControllerNode.cpp)
add_dependencies(twist_controller ${PROJECT_NAME}_gencfg)
target_link_libraries(twist_controller
  ${catkin_LIBRARIES}
  yaw_control
  pid_control
)

add_executable(twist_tester src/twist_tester.cpp)
add_dependencies(twist_tester ${PROJECT_NAME}_gencfg)
target_link_libraries(twist_tester
  ${catkin_LIBRARIES}
)

add_library(radius_control src/RadiusControl.cpp)
target_link_libraries(radius_control
    ${catkin_LIBRARIES}
    yaml-cpp
)

add_library(yaw_control src/YawControl.cpp)
target_link_libraries(yaw_control
    ${catkin_LIBRARIES}
    radius_control
)

add_library(heading_control src/HeadingControl.cpp)
target_link_libraries(heading_control
    ${catkin_LIBRARIES}
    pid_control
)

add_library(speed_control src/SpeedControl.cpp)
target_link_libraries(speed_control
    ${catkin_LIBRARIES}
    pid_control
    yaml-cpp
)

add_library(pid_control src/PidControl.cpp)
target_link_libraries(pid_control
    ${catkin_LIBRARIES}
)
