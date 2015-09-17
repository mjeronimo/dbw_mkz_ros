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
  joy_data_.enable = false;
  joy_data_.gear_cmd = dbw_mkz_msgs::Gear::NONE;
  joy_data_.steering_joy = 0.0;
  joy_data_.steering_mult = false;
  joy_data_.throttle_joy = 0.0;
  joy_data_.turn_signal_cmd = dbw_mkz_msgs::TurnSignal::NONE;

  pub_throttle_ = n.advertise<dbw_mkz_msgs::ThrottleCmd>("throttle_command", 1);
  pub_brake_ = n.advertise<dbw_mkz_msgs::BrakeCmd>("brake_command", 1);
  pub_turn_signal_ = n.advertise<dbw_mkz_msgs::TurnSignalCmd>("turn_signal_command", 1);
  pub_steering_ = n.advertise<dbw_mkz_msgs::SteeringCmd>("steering_command", 1);
  pub_gear_ = n.advertise<dbw_mkz_msgs::GearCmd>("gear_command", 1);

  cmd_timer_ = n.createTimer(ros::Duration(0.02), &JoystickDemo::cmdCallback, this);
}

void JoystickDemo::cmdCallback(const ros::TimerEvent& event)
{
  // Throttle
  dbw_mkz_msgs::ThrottleCmd throttle_msg;
  throttle_msg.enable = joy_data_.enable;
  throttle_msg.pedal_cmd = 0.15 + joy_data_.throttle_joy * (0.8 - 0.15);
  pub_throttle_.publish(throttle_msg);

  // Brake
  dbw_mkz_msgs::BrakeCmd brake_msg;
  brake_msg.pedal_cmd_type = dbw_mkz_msgs::BrakeCmd::CMD_PEDAL;
  brake_msg.pedal_cmd = joy_data_.brake_cmd;
  brake_msg.enable = joy_data_.enable;
  brake_msg.boo_cmd = (brake_msg.pedal_cmd > 0.2);
  pub_brake_.publish(brake_msg);

  // Gear
  dbw_mkz_msgs::GearCmd gear_msg;
  gear_msg.cmd.gear = joy_data_.gear_cmd;
  pub_gear_.publish(gear_msg);

  // Steering
  dbw_mkz_msgs::SteeringCmd steering_msg;
  steering_msg.enable = joy_data_.enable;
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
  joy_data_.enable = msg->data;
}

void JoystickDemo::recvJoy(const sensor_msgs::Joy::ConstPtr& msg)
{
  // Throttle
  joy_data_.throttle_joy = 0.5 - 0.5 * msg->axes[5];

  // Brake
  double brake_joy = 0.5 - 0.5 * msg->axes[2];
  if (brake_joy < 0.5) {
    joy_data_.brake_cmd = 0.15 + brake_joy * (0.27 - 0.15) / 0.5;
  } else {
    joy_data_.brake_cmd = 0.27 + (brake_joy - 0.5) * (0.5 - 0.27) / 0.5;
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
  joy_data_.steering_joy = 235.0 * ((fabs(msg->axes[0]) > fabs(msg->axes[3])) ? msg->axes[0] : msg->axes[3]);
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
