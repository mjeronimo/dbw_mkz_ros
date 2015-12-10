/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2015, Dataspeed Inc.
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
