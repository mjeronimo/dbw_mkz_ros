#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <dynamic_reconfigure/server.h>
#include <dbw_mkz_msgs/TwistCmd.h>
#include <dbw_mkz_twist_controller/TwistTestConfig.h>
#include "helper_functions.h"

dbw_mkz_msgs::TwistCmd cmd;
bool enable = false;
bool limits = false;

void reconfig(dbw_mkz_twist_controller::TwistTestConfig& config, uint32_t level)
{
  cmd.twist.linear.x = dbw_mkz_twist_controller::mphToMps(config.speed);
  cmd.twist.angular.z = config.yaw_rate;
  cmd.accel_limit = config.accel_max;
  cmd.decel_limit = config.decel_max;
  enable = config.go;
  limits = config.use_limits;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "twist_tester");
  ros::NodeHandle n;

  ros::Publisher pub_twist = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
  ros::Publisher pub_twist2 = n.advertise<dbw_mkz_msgs::TwistCmd>("cmd_vel_with_limits", 1);

  dynamic_reconfigure::Server<dbw_mkz_twist_controller::TwistTestConfig> srv;
  srv.setCallback(boost::bind(reconfig, _1, _2));

  ros::Rate r(20);
  while (ros::ok()){
    if (enable){
      if (limits) {
        pub_twist2.publish(cmd);
      } else {
        pub_twist.publish(cmd.twist);
      }
    }
    r.sleep();
    ros::spinOnce();
  }
  return 0;
}
