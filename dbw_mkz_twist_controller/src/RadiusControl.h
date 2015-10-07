#ifndef RADIUSCONTROL_H
#define RADIUSCONTROL_H

#include <yaml-cpp/yaml.h>
#include <ros/package.h>
#include <angles/angles.h>
#include "helper_functions.h"

namespace dbw_mkz_twist_controller{

class RadiusControl{
public:
  RadiusControl();
  double getSteeringWheel(double radius);
private:
  double wheelbase_m_;
  double steering_ratio_;
};

}

#endif // RADIUSCONTROL_H
