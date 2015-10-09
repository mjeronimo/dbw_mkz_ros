#ifndef RADIUSCONTROL_H
#define RADIUSCONTROL_H
#include <math.h>
#include <yaml-cpp/yaml.h>
#include <ros/package.h>
#include "helper_functions.h"

namespace dbw_mkz_twist_controller {

class RadiusControl {
public:
  RadiusControl() {
    YAML::Node steering_params = YAML::LoadFile(
        ros::package::getPath("dbw_mkz_twist_controller") + "/yaml/steering_params.yaml");
    wheelbase_m_ = inchesToMeters(steering_params["wheelbase"].as<double>());
    steering_ratio_ = inchesToMeters(steering_params["steering_ratio"].as<double>());
    max_steering_wheel_angle_ = fabs(steering_params["max_steering_wheel_angle"].as<double>());
  }
  void setWheelBase(double val) { wheelbase_m_ = val; }
  void setSteeringRatio(double val) { steering_ratio_ = val; }
  void setMaxSteeringWheelAngle(double val) { max_steering_wheel_angle_ = fabs(val); }
  double getSteeringWheel(double radius) {
    double steering_wheel_angle = isnan(radius) ? 0.0 : atan(wheelbase_m_ / radius) / steering_ratio_;
    if (steering_wheel_angle > max_steering_wheel_angle_) {
      return max_steering_wheel_angle_;
    } else if (steering_wheel_angle < -max_steering_wheel_angle_) {
      return -max_steering_wheel_angle_;
    }
    return steering_wheel_angle;
  }
private:
  double wheelbase_m_;
  double steering_ratio_;
  double max_steering_wheel_angle_;
};

}

#endif // RADIUSCONTROL_H
