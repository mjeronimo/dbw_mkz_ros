/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2015, Dataspeed Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Dataspeed Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

#ifndef RADIUSCONTROL_H
#define RADIUSCONTROL_H
#include <math.h>
#include <yaml-cpp/yaml.h>
#include <ros/package.h>
#include "helper_functions.h"

namespace dbw_mkz_twist_controller {

class RadiusControl {
public:
  RadiusControl() {
    YAML::Node steering_params = YAML::LoadFile(
        ros::package::getPath("dbw_mkz_twist_controller") + "/yaml/steering_params.yaml");
    wheelbase_m_ = inchesToMeters(steering_params["wheelbase"].as<double>());
    steering_ratio_ = inchesToMeters(steering_params["steering_ratio"].as<double>());
    max_steering_wheel_angle_ = fabs(steering_params["max_steering_wheel_angle"].as<double>());
  }
  void setWheelBase(double val) { wheelbase_m_ = val; }
  void setSteeringRatio(double val) { steering_ratio_ = val; }
  void setMaxSteeringWheelAngle(double val) { max_steering_wheel_angle_ = fabs(val); }
  double getSteeringWheel(double radius) {
    double steering_wheel_angle = isnan(radius) ? 0.0 : atan(wheelbase_m_ / radius) / steering_ratio_;
    if (steering_wheel_angle > max_steering_wheel_angle_) {
      return max_steering_wheel_angle_;
    } else if (steering_wheel_angle < -max_steering_wheel_angle_) {
      return -max_steering_wheel_angle_;
    }
    return steering_wheel_angle;
  }
private:
  double wheelbase_m_;
  double steering_ratio_;
  double max_steering_wheel_angle_;
};

}

#endif // RADIUSCONTROL_H
