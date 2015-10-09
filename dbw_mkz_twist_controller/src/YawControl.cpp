#include "YawControl.h"

namespace dbw_mkz_twist_controller{

YawControl::YawControl()
{
  max_lateral_accel_ = 0.0;
}

void YawControl::setMaxLateralAccel(double max_lateral_accel)
{
  max_lateral_accel_ = max_lateral_accel;
}

double YawControl::getSteeringWheel(double yaw_rate, double speed)
{
//  double r = speed / yaw_rate;
//  double rmin = speed * speed / max_lateral_accel_;
//  printf("rmin: %f\r\n", rmin);
//
//  if (fabs(r) < rmin){
//    r = (r > 0) ? rmin : -rmin;
//  }

  if (fabs(speed) > 0.1){
    double max_yaw_rate = max_lateral_accel_ / fabs(speed);
    if (fabs(yaw_rate) > max_yaw_rate){
      yaw_rate = yaw_rate >= 0 ? max_yaw_rate : -max_yaw_rate;
    }
  }

  double r = std::max(speed, mphToMps(4.0)) / yaw_rate;

  return radius_control_.getSteeringWheel(r);
}

}
