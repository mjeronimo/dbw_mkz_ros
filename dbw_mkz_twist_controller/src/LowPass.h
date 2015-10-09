#ifndef SRC_LOWPASS_H_
#define SRC_LOWPASS_H_

namespace dbw_mkz_twist_controller {

class LowPass {
public:
  LowPass() : ready_(false), last_val_(0) { a_ = 1; b_ = 0; }
  LowPass(double tau, double ts) : ready_(false), last_val_(0) { setParams(tau, ts); }
  void setParams(double tau, double ts) {
    a_ = 1 / (tau / ts + 1);
    b_ = tau / ts / (tau / ts + 1);
  }
  double get() { return last_val_; }
  double filt(double val) {
    if (ready_) {
      val = a_ * val + b_ * last_val_;
    } else {
      ready_ = true;
    }
    last_val_ = val;
    return val;
  }
private:
  bool ready_;
  double a_;
  double b_;
  double last_val_;
};

}

#endif /* SRC_LOWPASS_H_ */
