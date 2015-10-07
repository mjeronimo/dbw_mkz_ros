#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "math.h"
#include "tf/tf.h"

namespace dbw_mkz_twist_controller{

inline double inchesToMeters(double inches)
{
  return 0.0254 * inches;
}

inline double mphToMps(double mph)
{
  return mph * 0.44704;
}

inline double mpsToMph(double mps)
{
  return mps / 0.44704;
}

inline double subtractAngles(double ang1, double ang2)
{
  double diff = ang1 - ang2;
  if (diff > M_PI) {
    diff -= 2 * M_PI;
  } else if (diff < -M_PI) {
    diff += 2 * M_PI;
  }
  return diff;
}

inline double extractYaw(const tf::Quaternion& q)
{
  return atan2(2 * q.getW() * q.getZ(), 1 - 2 * q.getZ() * q.getZ());
}

inline double extractYaw(const geometry_msgs::Quaternion& q)
{
  return atan2(2 * q.w * q.z, 1 - 2 * q.z * q.z);
}

inline double throttleRelToActual(double relative_position)
{
  return 0.15 + relative_position * (0.8 - 0.15);
}

inline double throttleActualToRel(double actual_pedal)
{
  return (actual_pedal - 0.15) / (0.8 - 0.15);
}

inline double brakeRelToActual(double relative_position)
{
  return 0.15 + relative_position * (0.5 - 0.15);
}

inline double brakeActualToRel(double actual_pedal)
{
  return (actual_pedal - 0.15) / (0.5 - 0.15);
}


}

#endif // HELPER_FUNCTIONS_H
