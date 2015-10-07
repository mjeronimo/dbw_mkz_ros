#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>

double tq_in;
double tq_out;
double speed_in;
double speed_out;
double mtr_pwr;
double eng_pwr;

void recvTqIn(const std_msgs::Int16::ConstPtr& msg)
{
  tq_in = (double)msg->data;
}

void recvTqOut(const std_msgs::Int32::ConstPtr& msg)
{
  tq_out = (double)msg->data;
}

void recvSpeedIn(const std_msgs::Float64::ConstPtr& msg)
{
  speed_in = msg->data / 60.0 * 2 * M_PI;
}

void recvSpeedOut(const std_msgs::Float64::ConstPtr& msg)
{
  speed_out = msg->data;
}

void recvMtrPwr(const std_msgs::Float64::ConstPtr& msg)
{
  mtr_pwr = msg->data / 100.0;
}

void recvEngPwr(const std_msgs::Float64::ConstPtr& msg)
{
  eng_pwr = msg->data / 100.0;
}

void timerCallback(const ros::TimerEvent& event)
{
  ROS_INFO("---");
//  ROS_INFO("Torque: %f   (%f, %f)", tq_out / tq_in, tq_in, tq_out);
//  ROS_INFO("Speed: %f   (%f, %f)", speed_in / speed_out, speed_in, speed_out);
  ROS_INFO("Max electric power: %f kW", tq_out * speed_out / mtr_pwr / 1000.0);

  ROS_INFO("Max eng power: %f kW", speed_in * tq_in / eng_pwr / 1000.0 );




}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "gear_ratio");
  ros::NodeHandle n;

  ros::Subscriber sub_tq_in = n.subscribe("/EngineData_7/EngAout_Tq_Actl", 1, recvTqIn);
  ros::Subscriber sub_tq_out = n.subscribe("/VehicleOperatingModes/PrplWhlTot2_Tq_Actl", 1, recvTqOut);
  ros::Subscriber sub_speed_in = n.subscribe("/EngVehicleSpThrottle/EngAout3_N_Actl", 1, recvSpeedIn);
  ros::Subscriber sub_speed_out = n.subscribe("/HEV_Powertrain_Data5/TrnAout2_W_ActlUnfilt", 1, recvSpeedOut);

  ros::Subscriber sub_mtr_pwr = n.subscribe("/Cluster_HEV_Data5/MtrPwLvl_Pc_Dsply", 1, recvMtrPwr);
  ros::Subscriber sub_eng_pwr = n.subscribe("/Cluster_HEV_Data2/EngPwLvl_Pc_Dsply", 1, recvEngPwr);

  ros::Timer timer = n.createTimer(ros::Duration(0.02), timerCallback);

  ros::spin();
}
