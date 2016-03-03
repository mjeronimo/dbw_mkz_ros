/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2015-2016, Dataspeed Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Dataspeed Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

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
#include <dbw_mkz_msgs/SurroundReport.h>
#include <dbw_mkz_msgs/BrakeInfoReport.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/PointCloud2.h>
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
  bool fault_brakes_;
  bool fault_throttle_;
  bool fault_steering_cal_;
  inline bool fault() { return fault_brakes_ || fault_throttle_ || fault_steering_cal_; }
  inline bool driver() { return driver_brake_ || driver_throttle_ || driver_steering_ || driver_gear_; }
  inline bool clear() { return enable_ && driver(); }
  inline bool enabled() { return enable_ && !fault() && !driver(); }
  bool publishDbwEnabled();
  void enableSystem();
  void driverCancel();
  void driverBrake(bool driver);
  void driverThrottle(bool driver);
  void driverSteering(bool driver);
  void driverGear(bool driver);
  void faultBrakes(bool fault);
  void faultThrottle(bool fault);
  void faultSteeringCal(bool fault);

  enum {
    JOINT_FL = 0,
    JOINT_FR,
    JOINT_RL,
    JOINT_RR,
    JOINT_SL,
    JOINT_SR,
    JOINT_COUNT,
  };
  sensor_msgs::JointState joint_state_;
  void publishJointStates(const ros::Time &stamp, const dbw_mkz_msgs::WheelSpeedReport *wheels, const dbw_mkz_msgs::SteeringReport *steering);

  // Brake lights
  bool boo_status_;
  bool boo_control_;
  double boo_thresh_lo_;
  double boo_thresh_hi_;

  // Throttle ignore driver override
  bool throttle_ignore_;

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
  ros::Publisher pub_surround_;
  ros::Publisher pub_sonar_cloud_;
  ros::Publisher pub_brake_info_;
  ros::Publisher pub_imu_;
  ros::Publisher pub_gps_fix_;
  ros::Publisher pub_gps_vel_;
  ros::Publisher pub_joint_states_;
  ros::Publisher pub_sys_enable_;

  // Time Synchronization
  dataspeed_can_msg_filters::ApproximateTime sync_imu_;
  dataspeed_can_msg_filters::ApproximateTime sync_gps_;
};

} // namespace dbw_mkz_can

#endif // _DBW_NODE_H_
