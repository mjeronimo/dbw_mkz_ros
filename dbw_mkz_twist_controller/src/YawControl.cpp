#include "YawControl.h"

namespace dbw_mkz_twist_controller{

YawControl::YawControl(){}

double YawControl::getSteeringWheel(double yaw_rate, double speed)
{
  return radius_control_.getSteeringWheel(speed / yaw_rate);
}

}
