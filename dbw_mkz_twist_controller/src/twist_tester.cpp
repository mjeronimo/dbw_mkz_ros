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

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <dynamic_reconfigure/server.h>
#include <dbw_mkz_msgs/TwistCmd.h>
#include <dbw_mkz_twist_controller/TwistTestConfig.h>

static double mphToMps(double mph) { return mph * 0.44704; }

dbw_mkz_msgs::TwistCmd cmd;
bool enable = false;
bool limits = false;

void reconfig(dbw_mkz_twist_controller::TwistTestConfig& config, uint32_t level)
{
  cmd.twist.linear.x = mphToMps(config.speed);
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
