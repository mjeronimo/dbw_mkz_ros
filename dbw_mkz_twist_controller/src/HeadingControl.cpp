#include "HeadingControl.h"

namespace dbw_mkz_twist_controller{

HeadingControl::HeadingControl(double p, double i, double d, double min, double max)
{
  setGains(p, i, d, min, max);
}

void HeadingControl::setGains(double p, double i, double d, double min, double max)
{
  pid_.setGains(p, i, d, min, max);
}

double HeadingControl::stepControl(double error, double speed, double sample_time, bool reset_integrator)
{
  if (reset_integrator) {
    pid_.resetIntegrator();
  }
  double yaw_rate_command = pid_.step(error, sample_time);
  return yaw_control_.getSteeringWheel(yaw_rate_command, speed);
}

}
