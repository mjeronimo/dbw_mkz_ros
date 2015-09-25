#ifndef JOYSTICKDEMO_H_
#define JOYSTICKDEMO_H_

#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Bool.h>

#include <dbw_mkz_msgs/ThrottleCmd.h>
#include <dbw_mkz_msgs/BrakeCmd.h>
#include <dbw_mkz_msgs/SteeringCmd.h>
#include <dbw_mkz_msgs/GearCmd.h>
#include <dbw_mkz_msgs/ThrottleReport.h>
#include <dbw_mkz_msgs/BrakeReport.h>
#include <dbw_mkz_msgs/SteeringReport.h>
#include <dbw_mkz_msgs/GearReport.h>
#include <dbw_mkz_msgs/Misc1Report.h>
#include <dbw_mkz_msgs/TurnSignalCmd.h>

namespace joystick_demo
{

typedef struct {
  double brake_cmd;
  double throttle_joy;
  double steering_joy;
  bool steering_mult;
  int gear_cmd;
  int turn_signal_cmd;
  bool joy_throttle_valid;
  bool joy_brake_valid;
} JoystickDataStruct;

class JoystickDemo {
public:
  JoystickDemo(ros::NodeHandle n);
private:
  void recvJoy(const sensor_msgs::Joy::ConstPtr& msg);
  void recvEnable(const std_msgs::Bool::ConstPtr& msg);
  void cmdCallback(const ros::TimerEvent& event);

  ros::Subscriber sub_joy_;
  ros::Subscriber sub_enable_;
  ros::Publisher pub_throttle_;
  ros::Publisher pub_brake_;
  ros::Publisher pub_steering_;
  ros::Publisher pub_gear_;
  ros::Publisher pub_turn_signal_;
  ros::Timer cmd_timer_;

  JoystickDataStruct joy_data_;
  sensor_msgs::Joy last_joy_;

};

}

#endif /* JOYSTICKDEMO_H_ */
