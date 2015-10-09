#include "RadiusControl.h"

namespace dbw_mkz_twist_controller{

RadiusControl::RadiusControl()
{
  YAML::Node steering_params = YAML::LoadFile(
      ros::package::getPath("dbw_mkz_twist_controller") + "/yaml/steering_params.yaml");
  wheelbase_m_ = inchesToMeters(steering_params["wheelbase"].as<double>());
  track_m_ = inchesToMeters(steering_params["track"].as<double>());
  steering_ratio_ = inchesToMeters(steering_params["steering_ratio"].as<double>());
  max_steering_wheel_angle_ = steering_params["max_steering_wheel_angle"].as<double>();
}

double RadiusControl::getSteeringWheel(double radius)
{
  double ack_radius = radius > 0 ? std::max(radius - 0.5*track_m_, 0.0) : std::min(radius + 0.5*track_m_,0.0);

  ROS_INFO("radius: %f, ack_radius: %f", radius, ack_radius);

  double steering_wheel_angle = isnan(radius) ? 0.0 : angles::from_degrees(atan(wheelbase_m_ / ack_radius) / steering_ratio_);

  if (fabs(steering_wheel_angle) > max_steering_wheel_angle_){
    steering_wheel_angle = steering_wheel_angle >= 0 ? max_steering_wheel_angle_ : -max_steering_wheel_angle_;
  }

  return steering_wheel_angle;
}


}
