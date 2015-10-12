#ifndef YAWCONTROL_H
#define YAWCONTROL_H

#include "RadiusControl.h"
#include "helper_functions.h"
#include <math.h>

namespace dbw_mkz_twist_controller {

class YawControl {
public:
  YawControl() : max_lateral_accel_(INFINITY) {}
  YawControl(double max_lateral_accel) : max_lateral_accel_(fabs(max_lateral_accel)) {}
  void setMaxLateralAccel(double max_lateral_accel) { max_lateral_accel_ = fabs(max_lateral_accel); }
  double getSteeringWheel(double cmd_vx, double cmd_wz, double speed) {
    cmd_wz = fabs(cmd_vx) > 0 ? cmd_wz * speed / cmd_vx : 0.0;
    if (fabs(speed) > 0.1) {
      double max_yaw_rate = fabs(max_lateral_accel_ / speed);
      if (cmd_wz > max_yaw_rate) {
        cmd_wz = max_yaw_rate;
      } else if (cmd_wz < -max_yaw_rate) {
        cmd_wz = -max_yaw_rate;
      }
    }
    return radius_control_.getSteeringWheel(std::max(speed, mphToMps(4.0)) / cmd_wz);
  }
private:
  RadiusControl radius_control_;
  double max_lateral_accel_;
};

}

#endif // YAWCONTROL_H
