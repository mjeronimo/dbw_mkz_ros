#include <ros/ros.h>
#include "DbwNode.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "dwb_mkz");
  ros::NodeHandle node;
  ros::NodeHandle priv_nh("~");

  // create DbwNode class
  dbw_mkz_can::DbwNode n(node, priv_nh);

  // handle callbacks until shut down
  ros::spin();

  return 0;
}
