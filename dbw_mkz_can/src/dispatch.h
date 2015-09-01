#ifndef _DISPATCH_H
#define _DISPATCH_H
#include <stdint.h>

typedef struct {
  uint16_t PCMD;
  uint8_t BCMD :1;
  uint8_t :7;
  uint8_t EN :1;
  uint8_t CLEAR :1;
  uint8_t :6;
} MsgBrakeCmd;

typedef struct {
  uint16_t PI;
  uint16_t PC;
  uint16_t PO;
  uint8_t BO :1;
  uint8_t BC :1;
  uint8_t BI :1;
  uint8_t :5;
  uint8_t ENABLED :1;
  uint8_t DRIVER :1;
  uint8_t :2;
  uint8_t FLT1 :1;
  uint8_t FLT2 :1;
  uint8_t FLTB :1;
  uint8_t FLTCON :1;
} MsgBrakeReport;

typedef struct {
  uint16_t PCMD;
  uint8_t :8;
  uint8_t EN :1;
  uint8_t CLEAR :1;
  uint8_t :6;
} MsgThrottleCmd;

typedef struct {
  uint16_t PI;
  uint16_t PC;
  uint16_t PO;
  uint8_t :8;
  uint8_t ENABLED :1;
  uint8_t DRIVER :1;
  uint8_t :2;
  uint8_t FLT1 :1;
  uint8_t FLT2 :1;
  uint8_t :1;
  uint8_t FLTCON :1;
} MsgThrottleReport;

typedef struct {
  int16_t SCMD;
  uint8_t EN :1;
  uint8_t CLEAR :1;
  uint8_t :6;
  uint8_t :8;
} MsgSteeringCmd;

typedef struct {
  int16_t ANGLE;
  int16_t CMD;
  uint16_t SPEED;
  int8_t TORQUE;
  uint8_t ENABLED :1;
  uint8_t DRIVER :1;
  uint8_t :2;
  uint8_t FLTBUS1 :1;
  uint8_t FLTBUS2 :1;
  uint8_t :1;
  uint8_t FLTCON :1;
} MsgSteeringReport;

typedef struct {
  uint8_t GCMD :3;
  uint8_t :4;
  uint8_t CLEAR :1;
} MsgGearCmd;

typedef struct {
  uint8_t STATE :3;
  uint8_t DRIVER :1;
  uint8_t CMD :3;
  uint8_t FLTBUS :1;
} MsgGearReport;

typedef struct {
  uint8_t TRNCMD :2;
  uint8_t :6;
} MsgTurnSignalCmd;

typedef struct {
  uint8_t turn_signal :2;
  uint8_t head_light_hi :2;
  uint8_t wiper_front :4;
  uint8_t light_ambient :3;
  uint8_t :5;
  uint8_t btn_cc_on_off :1;
  uint8_t btn_cc_res_cncl:1;
  uint8_t btn_cc_set_inc :1;
  uint8_t btn_cc_set_dec :1;
  uint8_t btn_cc_gap_inc :1;
  uint8_t btn_cc_gap_dec :1;
  uint8_t btn_la_on_off :1;
  uint8_t FLTBUS :1;
} MsgMiscReport;

typedef struct {
  uint16_t front_left;
  uint16_t front_right;
  uint16_t rear_left;
  uint16_t rear_right;
} MsgReportWheelSpeed;

typedef struct {
  int16_t accel_lat;
  int16_t accel_long;
  int16_t accel_vert;
} MsgReportAccel;

typedef struct {
  int16_t gyro_roll;
  int16_t gyro_yaw;
} MsgReportGyro;

#define BUILD_ASSERT(cond) do { (void) sizeof(char [1 - 2*!(cond)]); } while(0)
static void dispatchAssertSizes() {
  BUILD_ASSERT(4 == sizeof(MsgBrakeCmd));
  BUILD_ASSERT(8 == sizeof(MsgBrakeReport));
  BUILD_ASSERT(4 == sizeof(MsgThrottleCmd));
  BUILD_ASSERT(8 == sizeof(MsgThrottleReport));
  BUILD_ASSERT(4 == sizeof(MsgSteeringCmd));
  BUILD_ASSERT(8 == sizeof(MsgSteeringReport));
  BUILD_ASSERT(1 == sizeof(MsgGearCmd));
  BUILD_ASSERT(1 == sizeof(MsgGearReport));
  BUILD_ASSERT(1 == sizeof(MsgTurnSignalCmd));
  BUILD_ASSERT(3 == sizeof(MsgMiscReport));
  BUILD_ASSERT(8 == sizeof(MsgReportWheelSpeed));
  BUILD_ASSERT(6 == sizeof(MsgReportAccel));
  BUILD_ASSERT(4 == sizeof(MsgReportGyro));
}
#undef BUILD_ASSERT

enum {
  ID_BRAKE_CMD          = 0x060,
  ID_BRAKE_REPORT       = 0x061,
  ID_THROTTLE_CMD       = 0x062,
  ID_THROTTLE_REPORT    = 0x063,
  ID_STEERING_CMD       = 0x064,
  ID_STEERING_REPORT    = 0x065,
  ID_GEAR_CMD           = 0x066,
  ID_GEAR_REPORT        = 0x067,
  ID_MISC_CMD           = 0x068,
  ID_MISC_REPORT        = 0x069,
  ID_REPORT_WHEEL_SPEED = 0x06A,
  ID_REPORT_ACCEL       = 0x06B,
  ID_REPORT_GYRO        = 0x06C,
};

#endif // _DISPATCH_H
