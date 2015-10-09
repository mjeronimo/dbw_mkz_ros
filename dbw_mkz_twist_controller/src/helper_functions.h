#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

namespace dbw_mkz_twist_controller {

static inline double inchesToMeters(double inches)
{
  return 0.0254 * inches;
}

static inline double mphToMps(double mph)
{
  return mph * 0.44704;
}

static inline double mpsToMph(double mps)
{
  return mps / 0.44704;
}

}

#endif // HELPER_FUNCTIONS_H
