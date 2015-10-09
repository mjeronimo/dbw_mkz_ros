
#ifndef SRC_LOWPASS_H_
#define SRC_LOWPASS_H_

namespace dbw_mkz_twist_controller{

class LowPass{
public:
  LowPass();
  void setParams(double tau, double ts);
  double filt(double val);
private:
//  double tau_;
//  double ts_;
  double a_;
  double b_;
  double last_val_;
};

}


#endif /* SRC_LOWPASS_H_ */
