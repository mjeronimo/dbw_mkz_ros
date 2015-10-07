#include "SpeedControl.h"

namespace dbw_mkz_twist_controller{

SpeedControl::SpeedControl()
{
  SpeedControl(0, 0, 0, 0, 0);
}

SpeedControl::SpeedControl(double p, double i, double d, double min, double max)
{
  setGains(p, i, d, min, max);

//  YAML::Node steering_params = YAML::LoadFile(
//      ros::package::getPath("dbw_mkz_twist_controller") + "/yaml/throttle_brake_params.yaml");
//  idling_force_ = steering_params["idling_force"].as<double>();
//  max_drive_force_ = steering_params["max_drive_force"].as<double>();
//  max_braking_viscosity_ = steering_params["max_braking_viscosity"].as<double>();
//  rolling_resistance_coeff_ = steering_params["rolling_resistance_coeff"].as<double>();
//  vehicle_mass_ = steering_params["vehicle_mass"].as<double>();
}

void SpeedControl::setGains(double p, double i, double d, double min, double max)
{
  pid_.setGains(p, i, d, min, max);
}

void SpeedControl::setModelParams(double idle_accel, double throttle_factor, double brake_factor)
{
  idle_accel_ = idle_accel;
  throttle_factor_ = throttle_factor;
  brake_factor_ = brake_factor;
}

void SpeedControl::step(double target_speed, double current_speed, double sample_time, bool reset_integrator,
                        double& throttle_pedal, double& brake_pedal)
{
  if (reset_integrator || (target_speed == 0.0)) {
    pid_.resetIntegrator();
  }

  double error = target_speed - current_speed;
  double accel_command = pid_.step(error, sample_time);

  double net_accel = accel_command - idle_accel_;
  if (net_accel > 0) {
    throttle_pedal = net_accel * throttle_factor_; //vehicle_mass_ / max_drive_force_;
    brake_pedal = 0;
  } else {
    throttle_pedal = 0;
    brake_pedal = -net_accel * brake_factor_; //vehicle_mass_ / max_braking_viscosity_;
  }
}

}
