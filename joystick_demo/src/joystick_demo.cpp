#include <ros/ros.h>
#include "JoystickDemo.h"

joystick_demo::JoystickDemo* node;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "joystick_demo");
  ros::NodeHandle n;

  node = new joystick_demo::JoystickDemo(n);

  ros::spin();
}
