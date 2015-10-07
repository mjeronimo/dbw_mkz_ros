#include "TwistControllerNode.h"

namespace dbw_mkz_twist_controller{

TwistControllerNode::TwistControllerNode(ros::NodeHandle n, ros::NodeHandle pn)
{
  // Initialize
  loadParams(pn);
  pid_ = new PidControl;
  fuel_level_ = 100.0;

  // Topics
  sub_twist_ = n.subscribe("cmd_vel", 1, &TwistControllerNode::recvTwist, this);
  sub_steering_ = n.subscribe("steering_report", 1, &TwistControllerNode::recvSteeringReport, this);
  sub_imu_ = n.subscribe("imu/data_raw", 1, &TwistControllerNode::recvImu, this);
  sub_enable_ = n.subscribe("dbw_enabled", 1, &TwistControllerNode::recvEnable, this);
  sub_fuel_level_ = n.subscribe("fuel_level_report", 1, &TwistControllerNode::recvFuel, this);

  pub_throttle_ = n.advertise<dbw_mkz_msgs::ThrottleCmd>("throttle_cmd", 1);
  pub_brake_ = n.advertise<dbw_mkz_msgs::BrakeCmd>("brake_cmd", 1);
  pub_steering_ = n.advertise<dbw_mkz_msgs::SteeringCmd>("steering_cmd", 1);

  // Timers
  control_timer_ = n.createTimer(ros::Duration(control_period_), &TwistControllerNode::controlCallback, this);

  // Dynamic reconfigure
  srv_.setCallback(boost::bind(&TwistControllerNode::reconfig, this, _1, _2));
}

void TwistControllerNode::controlCallback(const ros::TimerEvent& event)
{
  if ((event.current_real - cmd_stamp_).toSec() > (10.0 * control_period_)){
    pid_->resetIntegrator();
    return;
  }

  double vehicle_mass = cfg_.vehicle_mass + fuel_level_ / 100.0 * cfg_.fuel_capacity * GAS_DENSITY;
  double vel_error = cmd_vel_.linear.x - actual_.linear.x;
  double force = pid_->step(vel_error, control_period_) * vehicle_mass;

  if (sys_enable_){
    dbw_mkz_msgs::ThrottleCmd throttle_cmd;
    dbw_mkz_msgs::BrakeCmd brake_cmd;
    dbw_mkz_msgs::SteeringCmd steering_cmd;

    throttle_cmd.enable = true;
//    throttle_cmd.pedal_cmd_type = dbw_mkz_msgs::ThrottleCmd::CMD_TORQUE;
    if (force >= 0){
      throttle_cmd.pedal_cmd = force * cfg_.wheel_radius;
    }else{
      throttle_cmd.pedal_cmd = 0;
    }

    brake_cmd.enable = true;
    brake_cmd.pedal_cmd_type = dbw_mkz_msgs::BrakeCmd::CMD_TORQUE;
    if (force < 0){
      brake_cmd.pedal_cmd = -force * cfg_.wheel_radius;
    }else{
      brake_cmd.pedal_cmd = 0;
    }

    YawControl yaw_control;
    steering_cmd.enable = true;
    steering_cmd.steering_wheel_angle_cmd = yaw_control.getSteeringWheel(cmd_vel_.angular.z, cmd_vel_.linear.x)
        + cfg_.steer_kp * (cmd_vel_.angular.z - actual_.angular.z);

    pub_throttle_.publish(throttle_cmd);
    pub_brake_.publish(brake_cmd);
    pub_steering_.publish(steering_cmd);
  }else{
    pid_->resetIntegrator();
  }
}

void TwistControllerNode::reconfig(ControllerConfig& config, uint32_t level)
{
  cfg_ = config;
  cfg_.vehicle_mass -= cfg_.fuel_capacity * GAS_DENSITY; // Subtract weight of full gas tank
  cfg_.vehicle_mass += 150.0; // Account for some passengers

  pid_->setGains(cfg_.speed_kp, cfg_.speed_ki, cfg_.speed_kd, cfg_.accel_min, cfg_.accel_max);
}

void TwistControllerNode::recvTwist(const geometry_msgs::Twist::ConstPtr& msg)
{
  cmd_vel_ = *msg;
  cmd_stamp_ = ros::Time::now();
}

void TwistControllerNode::recvFuel(const dbw_mkz_msgs::FuelLevelReport::ConstPtr& msg)
{
  fuel_level_ = msg->fuel_level;
}

void TwistControllerNode::loadParams(ros::NodeHandle pn)
{
  double control_rate;
  pn.param("control_rate", control_rate, 50.0);
  control_period_ = 1.0 / control_rate;
}

void TwistControllerNode::recvSteeringReport(const dbw_mkz_msgs::SteeringReport::ConstPtr& msg)
{
  actual_.linear.x = msg->speed;
}

void TwistControllerNode::recvImu(const sensor_msgs::Imu::ConstPtr& msg)
{
  actual_.angular.z = msg->angular_velocity.z;
}

void TwistControllerNode::recvEnable(const std_msgs::Bool::ConstPtr& msg)
{
  sys_enable_ = msg->data;
}

}
