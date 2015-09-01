cmake_minimum_required(VERSION 2.8.3)
project(dbw_mkz_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  std_msgs
)

add_message_files(DIRECTORY msg FILES
  AmbientLight.msg
  BrakeCmd.msg
  BrakeReport.msg
  Gear.msg
  GearCmd.msg
  GearReport.msg
  Misc1Report.msg
  SteeringCmd.msg
  SteeringReport.msg
  ThrottleCmd.msg
  ThrottleReport.msg
  TurnSignal.msg
  TurnSignalCmd.msg
  WheelSpeedReport.msg
  Wiper.msg
)

generate_messages(DEPENDENCIES
  std_msgs
)

catkin_package(CATKIN_DEPENDS
  message_runtime
  std_msgs
)
