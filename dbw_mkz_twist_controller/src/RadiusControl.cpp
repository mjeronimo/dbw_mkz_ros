#include "RadiusControl.h"

namespace dbw_mkz_twist_controller{

RadiusControl::RadiusControl()
{
  YAML::Node steering_params = YAML::LoadFile(
      ros::package::getPath("dbw_mkz_twist_controller") + "/yaml/steering_params.yaml");
  wheelbase_m_ = dbw_mkz_twist_controller::inchesToMeters(steering_params["wheelbase"].as<double>());
  steering_ratio_ = dbw_mkz_twist_controller::inchesToMeters(steering_params["steering_ratio"].as<double>());
}

double RadiusControl::getSteeringWheel(double radius)
{
  return isnan(radius) ? 0.0 : angles::from_degrees(atan(wheelbase_m_ / radius) / steering_ratio_);
}


}
