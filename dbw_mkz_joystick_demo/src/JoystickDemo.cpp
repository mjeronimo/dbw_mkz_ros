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

#include "JoystickDemo.h"

namespace joystick_demo
{

JoystickDemo::JoystickDemo(ros::NodeHandle n)
{
  last_joy_.axes.resize(8, 0);
  last_joy_.buttons.resize(11, 0);

  sub_joy_ = n.subscribe("/joy", 1, &JoystickDemo::recvJoy, this);
  sub_enable_ = n.subscribe("dbw_enabled", 1, &JoystickDemo::recvEnable, this);

  joy_data_.brake_cmd = 0.0;
  joy_data_.gear_cmd = dbw_mkz_msgs::Gear::NONE;
  joy_data_.steering_joy = 0.0;
  joy_data_.steering_mult = false;
  joy_data_.throttle_joy = 0.0;
  joy_data_.turn_signal_cmd = dbw_mkz_msgs::TurnSignal::NONE;
  joy_data_.joy_throttle_valid = false;
  joy_data_.joy_brake_valid = false;

  pub_throttle_ = n.advertise<dbw_mkz_msgs::ThrottleCmd>("throttle_cmd", 1);
  pub_brake_ = n.advertise<dbw_mkz_msgs::BrakeCmd>("brake_cmd", 1);
  pub_turn_signal_ = n.advertise<dbw_mkz_msgs::TurnSignalCmd>("turn_signal_cmd", 1);
  pub_steering_ = n.advertise<dbw_mkz_msgs::SteeringCmd>("steering_cmd", 1);
  pub_gear_ = n.advertise<dbw_mkz_msgs::GearCmd>("gear_cmd", 1);

  cmd_timer_ = n.createTimer(ros::Duration(0.02), &JoystickDemo::cmdCallback, this);
}

void JoystickDemo::cmdCallback(const ros::TimerEvent& event)
{
  // Throttle
  dbw_mkz_msgs::ThrottleCmd throttle_msg;
  throttle_msg.enable = true;
  throttle_msg.pedal_cmd_type = dbw_mkz_msgs::ThrottleCmd::CMD_PERCENT;
  throttle_msg.pedal_cmd = joy_data_.throttle_joy;
  pub_throttle_.publish(throttle_msg);

  // Brake
  dbw_mkz_msgs::BrakeCmd brake_msg;
  brake_msg.pedal_cmd_type = dbw_mkz_msgs::BrakeCmd::CMD_PERCENT;
  brake_msg.pedal_cmd = joy_data_.brake_cmd;
  brake_msg.enable = true;
  pub_brake_.publish(brake_msg);

  // Gear
  dbw_mkz_msgs::GearCmd gear_msg;
  gear_msg.cmd.gear = joy_data_.gear_cmd;
  pub_gear_.publish(gear_msg);

  // Steering
  dbw_mkz_msgs::SteeringCmd steering_msg;
  steering_msg.enable = true;
  steering_msg.steering_wheel_angle_cmd = joy_data_.steering_joy;
  if (joy_data_.steering_mult) {
    steering_msg.steering_wheel_angle_cmd *= 2.0;
  }
  pub_steering_.publish(steering_msg);

  // Turn signal
  dbw_mkz_msgs::TurnSignalCmd turn_signal_msg;
  turn_signal_msg.cmd.turn_signal = joy_data_.turn_signal_cmd;
  pub_turn_signal_.publish(turn_signal_msg);
}

void JoystickDemo::recvEnable(const std_msgs::Bool::ConstPtr& msg)
{
}

void JoystickDemo::recvJoy(const sensor_msgs::Joy::ConstPtr& msg)
{
  // Handle joystick startup
  if (msg->axes[5] != 0.0) {
    joy_data_.joy_throttle_valid = true;
  }
  if (msg->axes[2] != 0.0) {
    joy_data_.joy_brake_valid = true;
  }

  // Throttle
  if (joy_data_.joy_throttle_valid) {
    joy_data_.throttle_joy = 0.5 - 0.5 * msg->axes[5];
  }

  // Brake
  if (joy_data_.joy_brake_valid) {
    joy_data_.brake_cmd = 0.5 - 0.5 * msg->axes[2];
  }

  // Gear
  if (msg->buttons[3]) {
    joy_data_.gear_cmd = dbw_mkz_msgs::Gear::PARK;
  } else if (msg->buttons[1]) {
    joy_data_.gear_cmd = dbw_mkz_msgs::Gear::REVERSE;
  } else if (msg->buttons[0]) {
    joy_data_.gear_cmd = dbw_mkz_msgs::Gear::DRIVE;
  } else if (msg->buttons[2]) {
    joy_data_.gear_cmd = dbw_mkz_msgs::Gear::NEUTRAL;
  } else {
    joy_data_.gear_cmd = dbw_mkz_msgs::Gear::NONE;
  }

  // Steering
  joy_data_.steering_joy = 235.0 * M_PI / 180.0 * ((fabs(msg->axes[0]) > fabs(msg->axes[3])) ? msg->axes[0] : msg->axes[3]);
  joy_data_.steering_mult = msg->buttons[6] || msg->buttons[7];

  // Turn signal
  if (msg->axes[6] != last_joy_.axes[6]) {
    switch (joy_data_.turn_signal_cmd) {
      case dbw_mkz_msgs::TurnSignal::NONE:
        if (msg->axes[6] < -0.5) {
          joy_data_.turn_signal_cmd = dbw_mkz_msgs::TurnSignal::RIGHT;
        } else if (msg->axes[6] > 0.5) {
          joy_data_.turn_signal_cmd = dbw_mkz_msgs::TurnSignal::LEFT;
        }
        break;
      case dbw_mkz_msgs::TurnSignal::LEFT:
        if (msg->axes[6] < -0.5) {
          joy_data_.turn_signal_cmd = dbw_mkz_msgs::TurnSignal::RIGHT;
        } else if (msg->axes[6] > 0.5) {
          joy_data_.turn_signal_cmd = dbw_mkz_msgs::TurnSignal::NONE;
        }
        break;
      case dbw_mkz_msgs::TurnSignal::RIGHT:
        if (msg->axes[6] < -0.5) {
          joy_data_.turn_signal_cmd = dbw_mkz_msgs::TurnSignal::NONE;
        } else if (msg->axes[6] > 0.5) {
          joy_data_.turn_signal_cmd = dbw_mkz_msgs::TurnSignal::LEFT;
        }
        break;
    }
  }

  last_joy_ = *msg;
}

}
