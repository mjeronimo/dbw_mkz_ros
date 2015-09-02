cmake_minimum_required(VERSION 2.8.3)
project(joystick_demo)

set(${PROJECT_NAME}_CATKIN_DEPS
  dbw_mkz_msgs
  roscpp
)

find_package(catkin REQUIRED COMPONENTS ${${PROJECT_NAME}_CATKIN_DEPS})

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} src/joystick_demo.cpp src/JoystickDemo.cpp)
add_dependencies(${PROJECT_NAME} dbw_mkz_msgs_gencpp)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
