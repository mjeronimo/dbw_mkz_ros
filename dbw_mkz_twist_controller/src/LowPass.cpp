#include "LowPass.h"

namespace dbw_mkz_twist_controller{

LowPass::LowPass(){}

void LowPass::setParams(double tau, double ts)
{
//  tau_ = tau;
//  ts_ = ts;

  a_ = 1 / (tau / ts + 1);
  b_ = tau / ts / (tau / ts + 1);
}

double LowPass::filt(double val)
{
  double new_val = a_ * val + b_ * last_val_;
  last_val_ = new_val;
  return new_val;
}

}
