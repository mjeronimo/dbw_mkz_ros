#ifndef HEADINGCONTROL_H
#define HEADINGCONTROL_H

#include <ros/ros.h>
#include "YawControl.h"
#include "PidControl.h"

namespace dbw_mkz_twist_controller{

class HeadingControl{
public:
  HeadingControl(double p, double i, double d, double min, double max);

  void setGains(double p, double i, double d, double min, double max);
  double stepControl(double error, double speed, double sample_time, bool reset_integrator);
private:

  PidControl pid_;
  YawControl yaw_control_;

};

}

#endif // HEADINGCONTROL_H
