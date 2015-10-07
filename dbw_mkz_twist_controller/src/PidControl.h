#ifndef PIDCONTROL_H
#define PIDCONTROL_H

#include <ros/ros.h>

namespace dbw_mkz_twist_controller{

class PidControl{
public:
    PidControl();
    PidControl(double kp, double ki, double kd, double min, double max);

    double step(double error, double sample_time);
    void setGains(double kp, double ki, double kd, double min, double max);
    void resetIntegrator();

private:
    double last_error_;
    double int_val_;

    double kp_;
    double ki_;
    double kd_;

    double max_;
    double min_;
};

}

#endif // PIDCONTROL_H
