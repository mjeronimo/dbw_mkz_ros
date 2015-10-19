#! /usr/bin/env python
import rospy
import rospkg
import rosbag
import sys
import csv
import glob
from math import floor

bag_topics = []
csv_topics = []
csv_types = []

TWIST_TYPE = 'geometry_msgs/Twist'
TWISTSTAMPED_TYPE = 'geometry_msgs/TwistStamped'
FIX_TYPE = 'sensor_msgs/NavSatFix'
IMU_TYPE = 'sensor_msgs/Imu'
THROTTLE_REPORT_TYPE = 'dbw_mkz_msgs/ThrottleReport'
BRAKE_REPORT_TYPE = 'dbw_mkz_msgs/BrakeReport'
STEERING_REPORT_TYPE = 'dbw_mkz_msgs/SteeringReport'
GEAR_REPORT_TYPE = 'dbw_mkz_msgs/GearReport'
FUEL_LEVEL_REPORT_TYPE = 'dbw_mkz_msgs/FuelLevelReport'
TIRE_PRESSURE_REPORT_TYPE = 'dbw_mkz_msgs/TirePressureReport'
WHEEL_SPEED_REPORT_TYPE = 'dbw_mkz_msgs/TirePressureReport'
MISC1_REPORT_TYPE = 'dbw_mkz_msgs/Misc1Report'

def write_twist_msg(t, topic, msg, csv_writer):        
    csv_writer.writerow([t, topic+'/linear/x', msg.linear.x])
    csv_writer.writerow([t, topic+'/linear/y', msg.linear.y])
    csv_writer.writerow([t, topic+'/linear/z', msg.linear.z])
    csv_writer.writerow([t, topic+'/angular/x', msg.angular.x])
    csv_writer.writerow([t, topic+'/angular/y', msg.angular.y])
    csv_writer.writerow([t, topic+'/angular/z', msg.angular.z])
    
def write_fix_msg(t, topic, msg, csv_writer):        
    csv_writer.writerow([t, topic+'/latitude', msg.latitude])
    csv_writer.writerow([t, topic+'/longitude', msg.longitude])
    csv_writer.writerow([t, topic+'/altitude', msg.altitude])
    csv_writer.writerow([t, topic+'/status', msg.status.status])
    
def write_imu_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/orientation/w', msg.orientation.w])
    csv_writer.writerow([t, topic+'/orientation/x', msg.orientation.x])
    csv_writer.writerow([t, topic+'/orientation/y', msg.orientation.y])
    csv_writer.writerow([t, topic+'/orientation/z', msg.orientation.z])
    csv_writer.writerow([t, topic+'/linear_acceleration/x', msg.linear_acceleration.x])
    csv_writer.writerow([t, topic+'/linear_acceleration/y', msg.linear_acceleration.y])
    csv_writer.writerow([t, topic+'/linear_acceleration/z', msg.linear_acceleration.z])
    csv_writer.writerow([t, topic+'/angular_velocity/x', msg.angular_velocity.x])
    csv_writer.writerow([t, topic+'/angular_velocity/y', msg.angular_velocity.y])
    csv_writer.writerow([t, topic+'/angular_velocity/z', msg.angular_velocity.z])

def write_throttle_report_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/pedal_input', msg.pedal_input])
    csv_writer.writerow([t, topic+'/pedal_cmd', msg.pedal_cmd])
    csv_writer.writerow([t, topic+'/pedal_output', msg.pedal_output])
    csv_writer.writerow([t, topic+'/enabled', int(msg.enabled)])
    csv_writer.writerow([t, topic+'/driver', int(msg.driver)])
    csv_writer.writerow([t, topic+'/fault_ch1', int(msg.fault_ch1)])
    csv_writer.writerow([t, topic+'/fault_ch2', int(msg.fault_ch2)])
    csv_writer.writerow([t, topic+'/fault_connector', int(msg.fault_connector)])
    
def write_brake_report_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/pedal_input', msg.pedal_input])
    csv_writer.writerow([t, topic+'/pedal_cmd', msg.pedal_cmd])
    csv_writer.writerow([t, topic+'/pedal_output', msg.pedal_output])
    csv_writer.writerow([t, topic+'/torque_input', msg.torque_input])
    csv_writer.writerow([t, topic+'/torque_cmd', msg.torque_cmd])
    csv_writer.writerow([t, topic+'/torque_output', msg.torque_output])
    csv_writer.writerow([t, topic+'/boo_input', int(msg.boo_input)])
    csv_writer.writerow([t, topic+'/boo_cmd', int(msg.boo_cmd)])
    csv_writer.writerow([t, topic+'/boo_output', int(msg.boo_output)])
    csv_writer.writerow([t, topic+'/enabled', int(msg.enabled)])
    csv_writer.writerow([t, topic+'/driver', int(msg.driver)])
    csv_writer.writerow([t, topic+'/fault_ch1', int(msg.fault_ch1)])
    csv_writer.writerow([t, topic+'/fault_ch2', int(msg.fault_ch2)])
    csv_writer.writerow([t, topic+'/fault_boo', int(msg.fault_boo)])
    csv_writer.writerow([t, topic+'/fault_connector', int(msg.fault_connector)])
   
def write_steering_report_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/steering_wheel_angle', msg.steering_wheel_angle])
    csv_writer.writerow([t, topic+'/steering_wheel_angle_cmd', msg.steering_wheel_angle_cmd])
    csv_writer.writerow([t, topic+'/steering_wheel_torque', msg.steering_wheel_torque])
    csv_writer.writerow([t, topic+'/speed', msg.speed])
    csv_writer.writerow([t, topic+'/enabled', int(msg.enabled)])
    csv_writer.writerow([t, topic+'/driver', int(msg.driver)])
    csv_writer.writerow([t, topic+'/fault_bus1', int(msg.fault_bus1)])
    csv_writer.writerow([t, topic+'/fault_bus2', int(msg.fault_bus2)])
    csv_writer.writerow([t, topic+'/fault_connector', int(msg.fault_connector)])
    
def write_gear_report_msg(t, topic, msg, csv_writer):  
    csv_writer.writerow([t, topic+'/state', msg.state])
    csv_writer.writerow([t, topic+'/cmd', msg.cmd])
    csv_writer.writerow([t, topic+'/driver', int(msg.driver)])
    csv_writer.writerow([t, topic+'/fault_bus', int(msg.fault_bus)])
  
def write_fuel_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/fuel_level', msg.fuel_level])
  
def write_tire_pressure_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/front_left', msg.front_left])
    csv_writer.writerow([t, topic+'/front_right', msg.front_right])
    csv_writer.writerow([t, topic+'/rear_left', msg.rear_left])
    csv_writer.writerow([t, topic+'/rear_right', msg.rear_right])

def write_wheel_speed_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/front_left', msg.front_left])
    csv_writer.writerow([t, topic+'/front_right', msg.front_right])
    csv_writer.writerow([t, topic+'/rear_left', msg.rear_left])
    csv_writer.writerow([t, topic+'/rear_right', msg.rear_right])
    
def write_misc1_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/turn_signal', msg.turn_signal])
    csv_writer.writerow([t, topic+'/high_beam_headlights', int(msg.high_beam_headlights)])
    csv_writer.writerow([t, topic+'/wiper', msg.wiper])
    csv_writer.writerow([t, topic+'/ambient_light', msg.ambient_light])
    csv_writer.writerow([t, topic+'/btn_cc_on_off', int(msg.btn_cc_on_off)])
    csv_writer.writerow([t, topic+'/btn_cc_res_cncl', int(msg.btn_cc_res_cncl)])
    csv_writer.writerow([t, topic+'/btn_cc_set_inc', int(msg.btn_cc_set_inc)])
    csv_writer.writerow([t, topic+'/btn_cc_set_dec', int(msg.btn_cc_set_dec)])
    csv_writer.writerow([t, topic+'/btn_cc_gap_inc', int(msg.btn_cc_gap_inc)])
    csv_writer.writerow([t, topic+'/btn_cc_gap_dec', int(msg.btn_cc_gap_dec)])
    csv_writer.writerow([t, topic+'/btn_la_on_off', int(msg.btn_la_on_off)])
    csv_writer.writerow([t, topic+'/fault_bus', int(msg.fault_bus)])
  
def get_csv_type(topic_name):
    global csv_topics
    
    for i in xrange(0, len(csv_topics)):
        if csv_topics[i] == topic_name:
            return i
        
    return -1    

def bag_to_csv():
    global csv_writer
    global bag_topics
    global csv_topics
    global csv_types
    
    bags = glob.glob('*.bag')
    print 'Found ' + str(len(bags)) + ' bag files:'
    for fname in bags:
        print '    ' + fname
    
    # Initialize node
    rospy.init_node('bag_to_csv')
    
    # Load parameters
    use_relative_time = rospy.get_param('relative_time', True)
   
    # Load desired topics
    csv_topics = rospy.get_param('topics')
    
    for fname in bags:
        csv_filename = fname.split('.')[0] + '.csv'    
        csvfile = open(csv_filename, 'w')
        csv_writer = csv.writer(csvfile, delimiter=',')
        
        # Load bag file    
        print 'Loading bag file: ' + fname
        bag = rosbag.Bag(fname)
        bag_types = bag.get_type_and_topic_info(csv_topics)[1].values()
        bag_topics = bag.get_type_and_topic_info(csv_topics)[1].keys()
        start_time = bag.get_start_time()
        
        if not use_relative_time:
            start_time = 0
        
        for i in xrange(0, len(csv_topics)):
            for j in xrange(0, len(bag_topics)):
                if csv_topics[i] == bag_topics[j]:
                    csv_types.append(bag_types[j].msg_type)
        
        print 'Topics being converted:'
        for i in xrange(0, len(csv_topics)):
            print '    ' + csv_topics[i] + ' (' + csv_types[i] + ')'
        
        
        # Initialize
        unsupported_topics = []
        num_messages = bag.get_message_count(csv_topics)
        message_count = 0.0
        percent_thres = 10.0
        
        # Convert
        print 'Converting...'
        for topic, msg, t in bag.read_messages(csv_topics):
            if rospy.is_shutdown():
                rospy.loginfo('User hit Ctrl-C, shutting down')
                break
            
            type_idx = get_csv_type(topic)
            if type_idx >= 0:
                # Found desired topic, determine type and write it to CSV
                if csv_types[type_idx] == TWIST_TYPE:
                    write_twist_msg(t.to_sec()-start_time, topic, msg, csv_writer)
                elif csv_types[type_idx] == TWISTSTAMPED_TYPE:
                    write_twist_msg(t.to_sec()-start_time, topic, msg.twist, csv_writer)
                elif csv_types[type_idx] == FIX_TYPE:
                    write_fix_msg(t.to_sec()-start_time, topic, msg, csv_writer)
                elif csv_types[type_idx] == IMU_TYPE:                
                    write_imu_msg(t.to_sec()-start_time, topic, msg, csv_writer)
                elif csv_types[type_idx] == THROTTLE_REPORT_TYPE:
                    write_throttle_report_msg(t.to_sec()-start_time, topic, msg, csv_writer)                
                elif csv_types[type_idx] == BRAKE_REPORT_TYPE:
                    write_brake_report_msg(t.to_sec()-start_time, topic, msg, csv_writer)
                elif csv_types[type_idx] == STEERING_REPORT_TYPE:
                    write_steering_report_msg(t.to_sec()-start_time, topic, msg, csv_writer)
                elif csv_types[type_idx] == GEAR_REPORT_TYPE:
                    write_gear_report_msg(t.to_sec()-start_time, topic, msg, csv_writer)
                elif csv_types[type_idx] == FUEL_LEVEL_REPORT_TYPE:
                    write_fuel_msg(t.to_sec()-start_time, topic, msg, csv_writer)
                elif csv_types[type_idx] == TIRE_PRESSURE_REPORT_TYPE:
                    write_tire_pressure_msg(t.to_sec()-start_time, topic, msg, csv_writer)
                elif csv_types[type_idx] == WHEEL_SPEED_REPORT_TYPE:
                    write_wheel_speed_msg(t.to_sec()-start_time, topic, msg, csv_writer)         
                elif csv_types[type_idx] == MISC1_REPORT_TYPE:
                    write_misc1_msg(t.to_sec()-start_time, topic, msg, csv_writer)
                elif type_idx not in unsupported_topics:
                    rospy.logwarn(topic + ': Message type not supported by CSV converter (' + csv_types[type_idx] + ')')
                    unsupported_topics.append(type_idx)      
            message_count += 1
            if (100.0 * message_count / num_messages) >= percent_thres:
                percent_thres += 10
                print str(int(100 * message_count / num_messages)) + '% Complete'       
              
        print 'Finished conversion! Saved CSV data to ' + csv_filename
        csvfile.close()
    
if __name__ == '__main__':
    try:
        bag_to_csv()
    except rospy.ROSInterruptException:
        pass
