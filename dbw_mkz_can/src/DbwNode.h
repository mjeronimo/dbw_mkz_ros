#ifndef _DBW_NODE_H_
#define _DBW_NODE_H_

#include <ros/ros.h>

// ROS messages
#include <dataspeed_can_msg_filters/ApproximateTime.h>
#include <dataspeed_can_msgs/CanMessageStamped.h>
#include <dbw_mkz_msgs/BrakeCmd.h>
#include <dbw_mkz_msgs/BrakeReport.h>
#include <dbw_mkz_msgs/ThrottleCmd.h>
#include <dbw_mkz_msgs/ThrottleReport.h>
#include <dbw_mkz_msgs/SteeringCmd.h>
#include <dbw_mkz_msgs/SteeringReport.h>
#include <dbw_mkz_msgs/GearCmd.h>
#include <dbw_mkz_msgs/GearReport.h>
#include <dbw_mkz_msgs/TurnSignalCmd.h>
#include <dbw_mkz_msgs/Misc1Report.h>
#include <dbw_mkz_msgs/WheelSpeedReport.h>
#include <dbw_mkz_msgs/FuelLevelReport.h>
#include <dbw_mkz_msgs/SuspensionReport.h>
#include <dbw_mkz_msgs/TirePressureReport.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>
#include <geometry_msgs/TwistStamped.h>
#include <std_msgs/Bool.h>

namespace dbw_mkz_can
{

class DbwNode
{
public:
  DbwNode(ros::NodeHandle &node, ros::NodeHandle &private_nh);
  ~DbwNode();

private:
  void timerCallback(const ros::TimerEvent& event);
  void recvCAN(const dataspeed_can_msgs::CanMessageStamped::ConstPtr& msg);
  void recvCanImu(const std::vector<dataspeed_can_msgs::CanMessageStamped::ConstPtr> &msgs);
  void recvCanGps(const std::vector<dataspeed_can_msgs::CanMessageStamped::ConstPtr> &msgs);
  void recvBrakeCmd(const dbw_mkz_msgs::BrakeCmd::ConstPtr& msg);
  void recvThrottleCmd(const dbw_mkz_msgs::ThrottleCmd::ConstPtr& msg);
  void recvSteeringCmd(const dbw_mkz_msgs::SteeringCmd::ConstPtr& msg);
  void recvGearCmd(const dbw_mkz_msgs::GearCmd::ConstPtr& msg);
  void recvTurnSignalCmd(const dbw_mkz_msgs::TurnSignalCmd::ConstPtr& msg);

  ros::Timer timer_;
  bool prev_enable_;
  bool enable_;
  bool driver_brake_;
  bool driver_throttle_;
  bool driver_steering_;
  bool driver_gear_;
  inline bool driver() { return driver_brake_ || driver_throttle_ || driver_steering_ || driver_gear_; }
  inline bool clear() { return enable_ && driver(); }
  inline bool enabled() { return enable_ && !driver(); }
  bool publishDbwEnabled();
  void enableSystem();
  void driverCancel();
  void driverBrake(bool driver);
  void driverThrottle(bool driver);
  void driverSteering(bool driver);
  void driverGear(bool driver);

  // Brake lights
  bool boo_status_;
  bool boo_control_;
  double boo_thresh_lo_;
  double boo_thresh_hi_;

  // Subscribed topics
  ros::Subscriber sub_can_;
  ros::Subscriber sub_brake_;
  ros::Subscriber sub_throttle_;
  ros::Subscriber sub_steering_;
  ros::Subscriber sub_gear_;
  ros::Subscriber sub_turn_signal_;

  // Published topics
  ros::Publisher pub_can_;
  ros::Publisher pub_brake_;
  ros::Publisher pub_throttle_;
  ros::Publisher pub_steering_;
  ros::Publisher pub_gear_;
  ros::Publisher pub_misc_1_;
  ros::Publisher pub_wheel_speeds_;
  ros::Publisher pub_suspension_;
  ros::Publisher pub_tire_pressure_;
  ros::Publisher pub_fuel_level_;
  ros::Publisher pub_imu_;
  ros::Publisher pub_gps_fix_;
  ros::Publisher pub_gps_vel_;
  ros::Publisher pub_sys_enable_;

  // Time Synchronization
  dataspeed_can_msg_filters::ApproximateTime sync_imu_;
  dataspeed_can_msg_filters::ApproximateTime sync_gps_;
};

} // namespace dbw_mkz_can

#endif // _DBW_NODE_H_
