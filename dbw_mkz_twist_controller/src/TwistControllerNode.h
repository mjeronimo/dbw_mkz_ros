#ifndef TWISTCONTROLLERNODE_H_
#define TWISTCONTROLLERNODE_H_

#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Imu.h>
#include <dbw_mkz_msgs/ThrottleCmd.h>
#include <dbw_mkz_msgs/ThrottleReport.h>
#include <dbw_mkz_msgs/BrakeCmd.h>
#include <dbw_mkz_msgs/BrakeReport.h>
#include <dbw_mkz_msgs/SteeringCmd.h>
#include <dbw_mkz_msgs/SteeringReport.h>
#include <dbw_mkz_msgs/FuelLevelReport.h>

#include <dynamic_reconfigure/server.h>
#include <dbw_mkz_twist_controller/ControllerConfig.h>

#include "YawControl.h"
#include "PidControl.h"
#include "LowPass.h"

#include <std_msgs/Float64.h>

namespace dbw_mkz_twist_controller{

#define GAS_DENSITY             2.858  // kg/gal

class TwistControllerNode{
public:
  TwistControllerNode(ros::NodeHandle n, ros::NodeHandle pn);
private:
  void reconfig(ControllerConfig& config, uint32_t level);
  void loadParams(ros::NodeHandle pn);
  void controlCallback(const ros::TimerEvent& event);
  void recvTwist(const geometry_msgs::Twist::ConstPtr& msg);
  void recvSteeringReport(const dbw_mkz_msgs::SteeringReport::ConstPtr& msg);
  void recvImu(const sensor_msgs::Imu::ConstPtr& msg);
  void recvEnable(const std_msgs::Bool::ConstPtr& msg);
  void recvFuel(const dbw_mkz_msgs::FuelLevelReport::ConstPtr& msg);

  ros::Publisher pub_throttle_;
  ros::Publisher pub_brake_;
  ros::Publisher pub_steering_;
  ros::Publisher pub_accel_;
  ros::Publisher pub_req_accel_;
  ros::Subscriber sub_steering_;
  ros::Subscriber sub_imu_;
  ros::Subscriber sub_enable_;
  ros::Subscriber sub_twist_;
  ros::Subscriber sub_fuel_level_;
  ros::Timer control_timer_;

  geometry_msgs::Twist cmd_vel_;
  geometry_msgs::Twist actual_;
  ros::Time cmd_stamp_;
  dynamic_reconfigure::Server<ControllerConfig> srv_;

  PidControl speed_pid_;
  PidControl accel_pid_;
  YawControl yaw_control_;
  LowPass lpf_accel_;
  LowPass lpf_fuel_;
  ControllerConfig cfg_;
  bool sys_enable_;

  // Parameters
  double control_period_;
};

}

#endif /* TWISTCONTROLLERNODE_H_ */
