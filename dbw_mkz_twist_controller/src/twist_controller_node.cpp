#include <ros/ros.h>
#include "TwistControllerNode.h"

dbw_mkz_twist_controller::TwistControllerNode* node;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "twist_controller");
  ros::NodeHandle n;
  ros::NodeHandle pn("~");

  node = new dbw_mkz_twist_controller::TwistControllerNode(n, pn);

  ros::spin();
}
