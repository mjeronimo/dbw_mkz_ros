cmake_minimum_required(VERSION 2.8.3)
project(dbw_mkz_can)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  nodelet
  message_filters
  sensor_msgs
  dataspeed_can_msgs
  dataspeed_can_msg_filters
  dbw_mkz_msgs
)

catkin_package(
  LIBRARIES ${PROJECT_NAME}
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
  src/nodelet.cpp
  src/DbwNode.cpp
)
add_dependencies(${PROJECT_NAME} dataspeed_can_msgs_gencpp)
add_dependencies(${PROJECT_NAME} dbw_mkz_msgs_gencpp)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

add_executable(dbw_node
  src/node.cpp
  src/DbwNode.cpp
)
add_dependencies(dbw_node dataspeed_can_msgs_gencpp)
add_dependencies(dbw_node dbw_mkz_msgs_gencpp)
target_link_libraries(dbw_node
  ${catkin_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} dbw_node
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)
