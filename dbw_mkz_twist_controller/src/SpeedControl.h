#ifndef SPEEDCONTROL_H
#define SPEEDCONTROL_H

#include <ros/package.h>
#include "PidControl.h"
//#include <yaml-cpp/yaml.h>

namespace dbw_mkz_twist_controller{

class SpeedControl{
public:
  SpeedControl();
  SpeedControl(double p, double i, double d, double min, double max);
  void setGains(double p, double i, double d, double min, double max);

  void step(double target_speed, double current_speed, double sample_time, bool reset_integrator,
            double &throttle_pedal, double &brake_pedal);
  void setModelParams(double idle_accel, double throttle_factor, double brake_factor);
private:
  PidControl pid_;

  // Model parameters
//  double idling_force_;
//  double max_drive_force_;
//  double max_braking_viscosity_;
//  double rolling_resistance_coeff_;
//  double vehicle_mass_;

  double idle_accel_;
  double throttle_factor_;
  double brake_factor_;
};

}

#endif // SPEEDCONTROL_H
