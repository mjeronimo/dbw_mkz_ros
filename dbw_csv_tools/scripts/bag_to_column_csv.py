#! /usr/bin/env python
import rospy
import rospkg
import rosbag
import sys
import csv
import glob
import csv_fcns
from math import floor
from yaml import load

import bool_msg
import twist_msg
import fix_msg
import imu_msg
import throttle_msg
import brake_msg
import steering_msg
import gear_msg
import misc_msg
import tire_pressure_msg
import fuel_msg
import wheel_speed_msg

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

BOOL_TYPE = 'std_msgs/Bool'
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

bag_topics = []
csv_topics = []
csv_types = []

def get_csv_type(topic_name):
    global csv_topics
    
    for i in xrange(0, len(csv_topics)):
        if csv_topics[i] == topic_name:
            return i
        
    return -1    

def bag_to_column_csv():
    global csv_writer
    global bag_topics
    global csv_topics
    global csv_types
    
    bags = glob.glob('*.bag')
    print 'Bag files found: ' + str(len(bags))
    for fname in bags:
        print '    ' + fname
    
    yaml_file = open('params.yaml', 'r')
    params = load(yaml_file)
    csv_topics = params['topics']
    
    for fname in bags:
        csv_filename = fname.split('.')[0] + '.csv'    
        csvfile = open(csv_filename, 'w')
        csv_writer = csv.writer(csvfile, delimiter=',')
        
        # Load bag file    
        print 'Loading bag file: [' + fname + ']'
        bag = rosbag.Bag(fname)
        bag_types = bag.get_type_and_topic_info(csv_topics)[1].values()
        bag_topics = bag.get_type_and_topic_info(csv_topics)[1].keys()
        
        unsupported_topics = []
        csv_header = ['time']
        row_offsets = []
        num_messages = bag.get_message_count(csv_topics)
        message_count = 0.0
        percent_thres = 10.0
        
        for csv_topic in csv_topics:
            for j in xrange(0, len(bag_topics)):
                if csv_topic == bag_topics[j]:
                    csv_types.append(bag_types[j].msg_type)
                    row_offsets.append(len(csv_header))
                    if bag_types[j].msg_type == BOOL_TYPE:
                        csv_header += bool_msg.get_signal_names(csv_topic)
                    elif bag_types[j].msg_type == TWIST_TYPE:
                        csv_header += twist_msg.get_signal_names(csv_topic)
                    elif bag_types[j].msg_type == TWISTSTAMPED_TYPE:
                        csv_header += twist_msg.get_signal_names(csv_topic)
                    elif bag_types[j].msg_type == FIX_TYPE:
                        csv_header += fix_msg.get_signal_names(csv_topic)
                    elif bag_types[j].msg_type == IMU_TYPE:
                        csv_header += imu_msg.get_signal_names(csv_topic)        
                    elif bag_types[j].msg_type == THROTTLE_REPORT_TYPE:
                        csv_header += throttle_msg.get_signal_names(csv_topic)    
                    elif bag_types[j].msg_type == BRAKE_REPORT_TYPE:
                        csv_header += brake_msg.get_signal_names(csv_topic) 
                    elif bag_types[j].msg_type == STEERING_REPORT_TYPE:
                        csv_header += steering_msg.get_signal_names(csv_topic)   
                    elif bag_types[j].msg_type == GEAR_REPORT_TYPE:
                        csv_header += gear_msg.get_signal_names(csv_topic) 
                    elif bag_types[j].msg_type == FUEL_LEVEL_REPORT_TYPE:
                        csv_header += fuel_msg.get_signal_names(csv_topic)        
                    elif bag_types[j].msg_type == TIRE_PRESSURE_REPORT_TYPE:
                        csv_header += tire_pressure_msg.get_signal_names(csv_topic)  
                    elif bag_types[j].msg_type == WHEEL_SPEED_REPORT_TYPE:
                        csv_header += wheel_speed_msg.get_signal_names(csv_topic)
                    elif bag_types[j].msg_type == MISC1_REPORT_TYPE:
                        csv_header += misc_msg.get_signal_names(csv_topic)
                       
        print 'Topics being converted:'
        for i in xrange(0, len(csv_topics)):
            print '    ' + csv_topics[i] + ' (' + csv_types[i] + ')'
        
        
        
        # Convert
        print 'Converting...'
        csv_writer.writerow(csv_header)
        for topic, msg, t in bag.read_messages(csv_topics):
            type_idx = get_csv_type(topic)
            
            row = ['' for i in range(len(csv_header))]
            row[0] = t.to_sec()
            
            if type_idx >= 0:
                if csv_types[type_idx] == BOOL_TYPE:
                    row = bool_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif csv_types[type_idx] == TWIST_TYPE:
                    row = twist_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif csv_types[type_idx] == TWISTSTAMPED_TYPE:
                    row = twist_msg.modify_csv_row(msg.twist, row, row_offsets[type_idx])
                elif csv_types[type_idx] == FIX_TYPE:
                    row = fix_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif csv_types[type_idx] == IMU_TYPE:                
                    row = imu_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif csv_types[type_idx] == THROTTLE_REPORT_TYPE:
                    row = throttle_msg.modify_csv_row(msg, row, row_offsets[type_idx])                
                elif csv_types[type_idx] == BRAKE_REPORT_TYPE:
                    row = brake_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif csv_types[type_idx] == STEERING_REPORT_TYPE:
                    row = steering_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif csv_types[type_idx] == GEAR_REPORT_TYPE:
                    row = gear_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif csv_types[type_idx] == FUEL_LEVEL_REPORT_TYPE:
                    row = fuel_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif csv_types[type_idx] == TIRE_PRESSURE_REPORT_TYPE:
                    row = tire_pressure_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif csv_types[type_idx] == WHEEL_SPEED_REPORT_TYPE:
                    row = wheel_speed_msg.modify_csv_row(msg, row, row_offsets[type_idx])         
                elif csv_types[type_idx] == MISC1_REPORT_TYPE:
                    row = misc_msg.modify_csv_row(msg, row, row_offsets[type_idx])
                elif type_idx not in unsupported_topics:
                    print bcolors.WARNING + topic + ': Message type not supported by CSV converter (' + csv_types[type_idx] + ')' + bcolors.ENDC
                    unsupported_topics.append(type_idx)                    
                
                csv_writer.writerow(row)    
                
            message_count += 1
            if (100.0 * message_count / num_messages) >= percent_thres:
                percent_thres += 10
                print str(int(100 * message_count / num_messages)) + '% Complete'       
              
        print bcolors.OKGREEN + 'Finished conversion! Saved CSV data to [' + csv_filename + ']' + bcolors.ENDC
        csvfile.close()
    
if __name__ == '__main__':
    try:
        bag_to_column_csv()
    except rospy.ROSInterruptException:
        pass
