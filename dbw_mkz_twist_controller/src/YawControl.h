#ifndef YAWCONTROL_H
#define YAWCONTROL_H

#include "RadiusControl.h"

namespace dbw_mkz_twist_controller{

class YawControl {
public:
  YawControl();
  double getSteeringWheel(double yaw_rate, double speed);
private:
  RadiusControl radius_control_;
};

}

#endif // YAWCONTROL_H
