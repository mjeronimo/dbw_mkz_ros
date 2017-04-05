cmake_minimum_required(VERSION 2.8.3)
project(dbw_mkz_twist_controller)

set(${PROJECT_NAME}_CATKIN_DEPS
  dbw_mkz_msgs
  dynamic_reconfigure
  geometry_msgs
  roscpp
  sensor_msgs
  std_msgs
)

find_package(catkin REQUIRED COMPONENTS
  ${${PROJECT_NAME}_CATKIN_DEPS}
)

generate_dynamic_reconfigure_options(
  cfg/Controller.cfg
  cfg/TwistTest.cfg
)

catkin_package(CATKIN_DEPENDS ${${PROJECT_NAME}_CATKIN_DEPS})

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(twist_controller src/twist_controller_node.cpp src/TwistControllerNode.cpp)
add_dependencies(twist_controller dbw_mkz_msgs_gencpp)
add_dependencies(twist_controller ${PROJECT_NAME}_gencfg)
target_link_libraries(twist_controller
  ${catkin_LIBRARIES}
)

add_executable(twist_tester src/twist_tester.cpp)
add_dependencies(twist_tester dbw_mkz_msgs_gencpp)
add_dependencies(twist_tester ${PROJECT_NAME}_gencfg)
target_link_libraries(twist_tester
  ${catkin_LIBRARIES}
)

install(TARGETS twist_controller twist_tester
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY launch
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
