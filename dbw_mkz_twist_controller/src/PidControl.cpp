#include "PidControl.h"


namespace dbw_mkz_twist_controller{

PidControl::PidControl()
{
  PidControl(0, 0, 0, 0, 0);
}

PidControl::PidControl(double kp, double ki, double kd, double min, double max)
{
  last_error_ = 0.0;
  int_val_ = 0.0;

  setGains(kp, ki, kd, min, max);
}

void PidControl::setGains(double kp, double ki, double kd, double min, double max)
{
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
  min_ = min;
  max_ = max;
}

void PidControl::resetIntegrator()
{
  int_val_ = 0.0;
}

double PidControl::step(double error, double sample_time)
{
  double integral = int_val_ + error * sample_time;
  double derivative = (error - last_error_) / sample_time;

  double y = kp_ * error + ki_ * int_val_ + kd_ * derivative;

  double control_val;
  if (y > max_) {
    control_val = max_;
  } else if (y < min_) {
    control_val = min_;
  } else {
    int_val_ = integral;
    control_val = y;
  }

  // Move the chains
  last_error_ = error;

  return control_val;
}

}
