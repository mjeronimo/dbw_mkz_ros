#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <dynamic_reconfigure/server.h>
#include <dbw_mkz_twist_controller/TwistTestConfig.h>
#include "helper_functions.h"

geometry_msgs::Twist twist;
bool enable;

void reconfig(dbw_mkz_twist_controller::TwistTestConfig& config, uint32_t level)
{
  twist.linear.x = dbw_mkz_twist_controller::mphToMps(config.speed);
  twist.angular.z = config.yaw_rate;
  enable = config.go;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "twist_tester");
  ros::NodeHandle n;

  ros::Publisher pub_twist = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);

  dynamic_reconfigure::Server<dbw_mkz_twist_controller::TwistTestConfig> srv;
  srv.setCallback(boost::bind(reconfig, _1, _2));

  ros::Rate r(20);
  while (ros::ok()){
    if (enable){
      pub_twist.publish(twist);
    }
    r.sleep();
    ros::spinOnce();
  }
  return 0;
}
