#ifndef PIDCONTROL_H
#define PIDCONTROL_H

#include <math.h>

namespace dbw_mkz_twist_controller {

class PidControl {
public:
    PidControl() {
      last_error_ = 0.0; int_val_ = 0.0; last_int_val_ = 0.0;
      kp_ = 0.0; ki_ = 0.0; kd_ = 0.0; min_ = -INFINITY; max_ = INFINITY;
    }
    PidControl(double kp, double ki, double kd, double min, double max) {
      last_error_ = 0.0; int_val_ = 0.0; last_int_val_ = 0.0;
      kp_ = kp; ki_ = ki; kd_ = kd; min_ = min; max_ = max;
    }

    void setGains(double kp, double ki, double kd) { kp_ = kp; ki_ = ki; kd_ = kd; }
    void setRange(double min, double max) { min_ = min; max_ = max; }
    void setParams(double kp, double ki, double kd, double min, double max) { kp_ = kp; ki_ = ki; kd_ = kd; min_ = min; max_ = max; }
    void resetIntegrator() { int_val_ = 0.0; last_int_val_ = 0.0; }
    void revertIntegrator() { int_val_ = last_int_val_; }

    double step(double error, double sample_time) {
      last_int_val_ = int_val_;

      double integral = int_val_ + error * sample_time;
      double derivative = (error - last_error_) / sample_time;

      double y = kp_ * error + ki_ * int_val_ + kd_ * derivative;
      if (y > max_) {
        y = max_;
      } else if (y < min_) {
        y = min_;
      } else {
        int_val_ = integral;
      }
      last_error_ = error;
      return y;
    }

private:
    double last_error_;
    double int_val_, last_int_val_;
    double kp_, ki_, kd_;
    double max_, min_;
};

}

#endif // PIDCONTROL_H
