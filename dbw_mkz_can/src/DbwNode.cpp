#include "DbwNode.h"
#include "dispatch.h"

namespace dbw_mkz_can
{

DbwNode::DbwNode(ros::NodeHandle &node, ros::NodeHandle &priv_nh)
{
  // Initialize enable state machine
  prev_enable_ = true;
  enable_ = false;
  driver_brake_ = false;
  driver_throttle_ = false;
  driver_steering_ = false;
  driver_gear_ = false;

  // Setup brake lights (BOO)
  boo_status_ = false;
  boo_control_ = true;
  boo_thresh_lo_ = 0.20;
  boo_thresh_hi_ = 0.22;
  priv_nh.getParam("boo_control", boo_control_);
  priv_nh.getParam("boo_thresh_lo", boo_thresh_lo_);
  priv_nh.getParam("boo_thresh_hi", boo_thresh_hi_);
  if (boo_thresh_lo_ > boo_thresh_hi_) {
    std::swap(boo_thresh_lo_, boo_thresh_hi_);
  }

  // Set up Publishers
  pub_can_ = node.advertise<dataspeed_can_msgs::CanMessage>("can_tx", 10);
  pub_brake_ = node.advertise<dbw_mkz_msgs::BrakeReport>("brake_report", 2);
  pub_throttle_ = node.advertise<dbw_mkz_msgs::ThrottleReport>("throttle_report", 2);
  pub_steering_ = node.advertise<dbw_mkz_msgs::SteeringReport>("steering_report", 2);
  pub_gear_ = node.advertise<dbw_mkz_msgs::GearReport>("gear_report", 2);
  pub_misc_1_ = node.advertise<dbw_mkz_msgs::Misc1Report>("misc_1_report", 2);
  pub_wheel_speeds_ = node.advertise<dbw_mkz_msgs::WheelSpeedReport>("wheel_speed_report", 2);
  pub_sys_enable_ = node.advertise<std_msgs::Bool>("dbw_enabled", 1, true);
  publishDbwEnabled();

  // Set up Subscribers
  sub_can_ = node.subscribe("can_rx", 100, &DbwNode::recvCAN, this, ros::TransportHints().tcpNoDelay(true));
  sub_brake_ = node.subscribe("brake_cmd", 1, &DbwNode::recvBrakeCmd, this, ros::TransportHints().tcpNoDelay(true));
  sub_throttle_ = node.subscribe("throttle_cmd", 1, &DbwNode::recvThrottleCmd, this, ros::TransportHints().tcpNoDelay(true));
  sub_steering_ = node.subscribe("steering_cmd", 1, &DbwNode::recvSteeringCmd, this, ros::TransportHints().tcpNoDelay(true));
  sub_gear_ = node.subscribe("gear_cmd", 1, &DbwNode::recvGearCmd, this, ros::TransportHints().tcpNoDelay(true));
  sub_turn_signal_ = node.subscribe("turn_signal_cmd", 1, &DbwNode::recvTurnSignalCmd, this, ros::TransportHints().tcpNoDelay(true));

  // Set up Timer
  timer_ = node.createTimer(ros::Duration(1 / 20.0), &DbwNode::timerCallback, this);
}

DbwNode::~DbwNode()
{
}

void DbwNode::recvCAN(const dataspeed_can_msgs::CanMessageStamped::ConstPtr& msg)
{
  if (!msg->msg.extended) {
    switch (msg->msg.id) {
      case ID_BRAKE_REPORT:
        if (msg->msg.dlc >= sizeof(MsgBrakeReport)) {
          const MsgBrakeReport *ptr = (const MsgBrakeReport*)msg->msg.data.elems;
          driverBrake(ptr->DRIVER);
          dbw_mkz_msgs::BrakeReport out;
          out.header.stamp = msg->header.stamp;
          out.pedal_input  = (float)ptr->PI / UINT16_MAX;
          out.pedal_cmd    = (float)ptr->PC / UINT16_MAX;
          out.pedal_output = (float)ptr->PO / UINT16_MAX;
          out.boo_input  = ptr->BI ? true : false;
          out.boo_cmd    = ptr->BC ? true : false;
          out.boo_output = ptr->BO ? true : false;
          out.enabled = ptr->ENABLED ? true : false;
          out.driver = ptr->DRIVER ? true : false;
          out.fault_ch1 = ptr->FLT1 ? true : false;
          out.fault_ch2 = ptr->FLT2 ? true : false;
          out.fault_boo = ptr->FLTB ? true : false;
          out.fault_connector = ptr->FLTCON ? true : false;
          pub_brake_.publish(out);
        }
        break;

      case ID_THROTTLE_REPORT:
        if (msg->msg.dlc >= sizeof(MsgThrottleReport)) {
          const MsgThrottleReport *ptr = (const MsgThrottleReport*)msg->msg.data.elems;
          driverThrottle(ptr->DRIVER);
          dbw_mkz_msgs::ThrottleReport out;
          out.header.stamp = msg->header.stamp;
          out.pedal_input  = (float)ptr->PI / UINT16_MAX;
          out.pedal_cmd    = (float)ptr->PC / UINT16_MAX;
          out.pedal_output = (float)ptr->PO / UINT16_MAX;
          out.enabled = ptr->ENABLED ? true : false;
          out.driver = ptr->DRIVER ? true : false;
          out.fault_ch1 = ptr->FLT1 ? true : false;
          out.fault_ch2 = ptr->FLT2 ? true : false;
          out.fault_connector = ptr->FLTCON ? true : false;
          pub_throttle_.publish(out);
        }
        break;

      case ID_STEERING_REPORT:
        if (msg->msg.dlc >= sizeof(MsgSteeringReport)) {
          const MsgSteeringReport *ptr = (const MsgSteeringReport*)msg->msg.data.elems;
          driverSteering(ptr->DRIVER);
          dbw_mkz_msgs::SteeringReport out;
          out.header.stamp = msg->header.stamp;
          out.steering_wheel_angle     = (float)ptr->ANGLE * (0.1 * M_PI / 180);
          out.steering_wheel_angle_cmd = (float)ptr->CMD   * (0.1 * M_PI / 180);
          out.steering_wheel_torque = (float)ptr->TORQUE * 0.0625;
          out.speed = (float)ptr->SPEED * (0.01 / 3.6);
          out.enabled = ptr->ENABLED ? true : false;
          out.driver = ptr->DRIVER ? true : false;
          out.fault_bus1 = ptr->FLTBUS1 ? true : false;
          out.fault_bus2 = ptr->FLTBUS2 ? true : false;
          out.fault_connector = ptr->FLTCON ? true : false;
          pub_steering_.publish(out);
        }
        break;

      case ID_GEAR_REPORT:
        if (msg->msg.dlc >= sizeof(MsgGearReport)) {
          const MsgGearReport *ptr = (const MsgGearReport*)msg->msg.data.elems;
          driverGear(ptr->DRIVER);
          dbw_mkz_msgs::GearReport out;
          out.header.stamp = msg->header.stamp;
          out.state.gear = ptr->STATE;
          out.cmd.gear = ptr->CMD;
          out.driver = ptr->DRIVER ? true : false;
          out.fault_bus = ptr->FLTBUS ? true : false;
          pub_gear_.publish(out);
        }
        break;

      case ID_MISC_REPORT:
        if (msg->msg.dlc >= sizeof(MsgMiscReport)) {
          const MsgMiscReport *ptr = (const MsgMiscReport*)msg->msg.data.elems;
          if (ptr->btn_cc_gap_inc) {
            driverCancel();
          } else if (ptr->btn_cc_set_dec && ptr->btn_cc_gap_dec) {
            enableSystem();
          }
          dbw_mkz_msgs::Misc1Report out;
          out.header.stamp = msg->header.stamp;
          out.turn_signal.turn_signal = ptr->turn_signal;
          out.high_beam_headlights = ptr->head_light_hi ? true : false;
          out.wiper.wiper = ptr->wiper_front;
          out.ambient_light.ambient_light = ptr->light_ambient;
          out.btn_cc_on_off = ptr->btn_cc_on_off ? true : false;
          out.btn_cc_res_cncl = ptr->btn_cc_res_cncl ? true : false;
          out.btn_cc_set_inc = ptr->btn_cc_set_inc ? true : false;
          out.btn_cc_set_dec = ptr->btn_cc_set_dec ? true : false;
          out.btn_cc_gap_inc = ptr->btn_cc_gap_inc ? true : false;
          out.btn_cc_gap_dec = ptr->btn_cc_gap_dec ? true : false;
          out.btn_la_on_off = ptr->btn_la_on_off ? true : false;
          out.fault_bus = ptr->FLTBUS ? true : false;
          pub_misc_1_.publish(out);
        }
        break;

      case ID_REPORT_WHEEL_SPEED:
        if (msg->msg.dlc >= sizeof(MsgReportWheelSpeed)) {
          const MsgReportWheelSpeed *ptr = (const MsgReportWheelSpeed*)msg->msg.data.elems;
          dbw_mkz_msgs::WheelSpeedReport out;
          out.header.stamp = msg->header.stamp;
          out.front_left  = (float)ptr->front_left  * 0.01;
          out.front_right = (float)ptr->front_right * 0.01;
          out.rear_left   = (float)ptr->rear_left   * 0.01;
          out.rear_right  = (float)ptr->rear_right  * 0.01;
          pub_wheel_speeds_.publish(out);
        }
        break;

#if 0
      case ID_REPORT_ACCEL:
        if (msg->msg.dlc >= sizeof(MsgReportAccel)) {
          const MsgReportAccel *ptr = (const MsgReportAccel*)msg->msg.data.elems;
          (float)ptr->accel_lat  * 0.01;
          (float)ptr->accel_long * 0.01;
          (float)ptr->accel_vert * 0.01;
        }
        break;

      case ID_REPORT_GYRO:
        if (msg->msg.dlc >= sizeof(MsgReportGyro)) {
          const MsgReportGyro *ptr = (const MsgReportGyro*)msg->msg.data.elems;
          (float)ptr->gyro_roll * 0.0002;
          (float)ptr->gyro_yaw  * 0.0002;
        }
        break;
#endif

      case ID_BRAKE_CMD:
        ROS_WARN("DBW system: Another node on the CAN bus is commanding the vehicle!!! Subsystem: Brake. Id: 0x%03X", ID_BRAKE_CMD);
        break;
      case ID_THROTTLE_CMD:
        ROS_WARN("DBW system: Another node on the CAN bus is commanding the vehicle!!! Subsystem: Throttle. Id: 0x%03X", ID_THROTTLE_CMD);
        break;
      case ID_STEERING_CMD:
        ROS_WARN("DBW system: Another node on the CAN bus is commanding the vehicle!!! Subsystem: Steering. Id: 0x%03X", ID_STEERING_CMD);
        break;
      case ID_GEAR_CMD:
        ROS_WARN("DBW system: Another node on the CAN bus is commanding the vehicle!!! Subsystem: Shifting. Id: 0x%03X", ID_GEAR_CMD);
        break;
      case ID_MISC_CMD:
        ROS_WARN("DBW system: Another node on the CAN bus is commanding the vehicle!!! Subsystem: Turn Signals. Id: 0x%03X", ID_MISC_CMD);
        break;
    }
  }
#if 0
  ROS_INFO("ena: %s, clr: %s, brake: %s, throttle: %s, steering: %s, gear: %s",
           enabled() ? "true " : "false",
           clear() ? "true " : "false",
           driver_brake_ ? "true " : "false",
           driver_throttle_ ? "true " : "false",
           driver_steering_ ? "true " : "false",
           driver_gear_ ? "true " : "false"
       );
#endif
}

void DbwNode::recvBrakeCmd(const dbw_mkz_msgs::BrakeCmd::ConstPtr& msg)
{
  dataspeed_can_msgs::CanMessage out;
  out.id = ID_BRAKE_CMD;
  out.extended = false;
  out.dlc = sizeof(MsgBrakeCmd);
  MsgBrakeCmd *ptr = (MsgBrakeCmd*)out.data.elems;
  memset(ptr, 0x00, sizeof(*ptr));
  if (enabled()) {
    ptr->PCMD = std::max((float)0.0, std::min((float)UINT16_MAX, msg->pedal_cmd * UINT16_MAX));
    if (msg->boo_cmd) {
      ptr->BCMD = 1;
    } else if (boo_control_) {
      if (!boo_status_ && (msg->pedal_cmd > boo_thresh_hi_)) {
        ptr->BCMD = 1;
        boo_status_ = true;
      } else if (boo_status_ && (msg->pedal_cmd < boo_thresh_lo_)) {
        boo_status_ = false;
      }
    }
    if (msg->enable) {
      ptr->EN = 1;
    }
  }
  if (clear()) {
    ptr->CLEAR = 1;
  }
  pub_can_.publish(out);
}

void DbwNode::recvThrottleCmd(const dbw_mkz_msgs::ThrottleCmd::ConstPtr& msg)
{
  dataspeed_can_msgs::CanMessage out;
  out.id = ID_THROTTLE_CMD;
  out.extended = false;
  out.dlc = sizeof(MsgThrottleCmd);
  MsgThrottleCmd *ptr = (MsgThrottleCmd*)out.data.elems;
  memset(ptr, 0x00, sizeof(*ptr));
  if (enabled()) {
    ptr->PCMD = std::max((float)0.0, std::min((float)UINT16_MAX, msg->pedal_cmd * UINT16_MAX));
    if (msg->enable) {
      ptr->EN = 1;
    }
  }
  if (clear()) {
    ptr->CLEAR = 1;
  }
  pub_can_.publish(out);
}

void DbwNode::recvSteeringCmd(const dbw_mkz_msgs::SteeringCmd::ConstPtr& msg)
{
  dataspeed_can_msgs::CanMessage out;
  out.id = ID_STEERING_CMD;
  out.extended = false;
  out.dlc = sizeof(MsgSteeringCmd);
  MsgSteeringCmd *ptr = (MsgSteeringCmd*)out.data.elems;
  memset(ptr, 0x00, sizeof(*ptr));
  if (enabled()) {
    ptr->SCMD = std::max((float)-4700, std::min((float)4700, (float)(msg->steering_wheel_angle_cmd * (180 / M_PI * 10))));
    if (msg->enable) {
      ptr->EN = 1;
    }
  }
  if (clear()) {
    ptr->CLEAR = 1;
  }
  pub_can_.publish(out);
}

void DbwNode::recvGearCmd(const dbw_mkz_msgs::GearCmd::ConstPtr& msg)
{
  dataspeed_can_msgs::CanMessage out;
  out.id = ID_GEAR_CMD;
  out.extended = false;
  out.dlc = sizeof(MsgGearCmd);
  MsgGearCmd *ptr = (MsgGearCmd*)out.data.elems;
  memset(ptr, 0x00, sizeof(*ptr));
  if (enabled()) {
    ptr->GCMD = msg->cmd.gear;
  }
  pub_can_.publish(out);
}

void DbwNode::recvTurnSignalCmd(const dbw_mkz_msgs::TurnSignalCmd::ConstPtr& msg)
{
  dataspeed_can_msgs::CanMessage out;
  out.id = ID_MISC_CMD;
  out.extended = false;
  out.dlc = sizeof(MsgTurnSignalCmd);
  MsgTurnSignalCmd *ptr = (MsgTurnSignalCmd*)out.data.elems;
  memset(ptr, 0x00, sizeof(*ptr));
  if (enabled()) {
    ptr->TRNCMD = msg->cmd.turn_signal;
  }
  pub_can_.publish(out);
}

bool DbwNode::publishDbwEnabled()
{
  bool change = false;
  bool en = enabled();
  if (prev_enable_ != en) {
    std_msgs::Bool msg;
    msg.data = en;
    pub_sys_enable_.publish(msg);
    change = true;
  }
  prev_enable_ = en;
  return change;
}

void DbwNode::timerCallback(const ros::TimerEvent& event)
{
  if (clear()) {
    dataspeed_can_msgs::CanMessage out;
    out.extended = false;

    out.id = ID_BRAKE_CMD;
    out.dlc = sizeof(MsgBrakeCmd);
    memset(out.data.elems, 0x00, 8);
    ((MsgBrakeCmd*)out.data.elems)->CLEAR = 1;
    pub_can_.publish(out);

    out.id = ID_THROTTLE_CMD;
    out.dlc = sizeof(MsgThrottleCmd);
    memset(out.data.elems, 0x00, 8);
    ((MsgThrottleCmd*)out.data.elems)->CLEAR = 1;
    pub_can_.publish(out);

    out.id = ID_STEERING_CMD;
    out.dlc = sizeof(MsgSteeringCmd);
    memset(out.data.elems, 0x00, 8);
    ((MsgSteeringCmd*)out.data.elems)->CLEAR = 1;
    pub_can_.publish(out);

    out.id = ID_GEAR_CMD;
    out.dlc = sizeof(MsgGearCmd);
    memset(out.data.elems, 0x00, 8);
    ((MsgGearCmd*)out.data.elems)->CLEAR = 1;
    pub_can_.publish(out);
  }
}

void DbwNode::enableSystem()
{
  enable_ = true;
  if (publishDbwEnabled()) {
    ROS_INFO("DBW system enabled.");
  }
}

void DbwNode::driverCancel()
{
  enable_ = false;
  if (publishDbwEnabled()) {
    ROS_WARN("DBW system disabled. Cancel button pressed.");
  }
}

void DbwNode::driverBrake(bool driver)
{
  if (driver && enabled()) {
    enable_ = false;
  }
  driver_brake_ = driver;
  if (publishDbwEnabled()) {
    ROS_WARN("DBW system disabled. Driver override on brake pedal.");
  }
}

void DbwNode::driverThrottle(bool driver)
{
  if (driver && enabled()) {
    enable_ = false;
  }
  driver_throttle_ = driver;
  if (publishDbwEnabled()) {
    ROS_WARN("DBW system disabled. Driver override on throttle pedal.");
  }
}

void DbwNode::driverSteering(bool driver)
{
  if (driver && enabled()) {
    enable_ = false;
  }
  driver_steering_ = driver;
  if (publishDbwEnabled()) {
    ROS_WARN("DBW system disabled. Driver override on steering wheel.");
  }
}

void DbwNode::driverGear(bool driver)
{
  if (driver && enabled()) {
    enable_ = false;
  }
  driver_gear_ = driver;
  if (publishDbwEnabled()) {
    ROS_WARN("DBW system disabled. Driver override on shifter.");
  }
}

} // namespace dbw_mkz_can
