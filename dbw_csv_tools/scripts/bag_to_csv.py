#! /usr/bin/env python
import rospy
import rospkg
import rosbag
import sys
import csv

bag_topics = []
csv_topics = []
csv_types = []

TWIST_TYPE = 'geometry_msgs/Twist'
TWISTSTAMPED_TYPE = 'geometry_msgs/TwistStamped'
FIX_TYPE = 'sensor_msgs/NavSatFix'
THROTTLE_REPORT_TYPE = 'dbw_mkz_msgs/ThrottleReport'
BRAKE_REPORT_TYPE = 'dbw_mkz_msgs/BrakeReport'
STEERING_REPORT_TYPE = 'dbw_mkz_msgs/SteeringReport'
GEAR_REPORT_TYPE = 'dbw_mkz_msgs/GearReport'
FUEL_LEVEL_REPORT_TYPE = 'dbw_mkz_msgs/FuelLevelReport'
TIRE_PRESSURE_REPORT_TYPE = 'dbw_mkz_msgs/TirePressureReport'
WHEEL_SPEED_REPORT_TYPE = 'dbw_mkz_msgs/TirePressureReport'

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
    csv_writer.writerow([t, topic+'/enabled', msg.enabled])
    csv_writer.writerow([t, topic+'/driver', msg.driver])
    csv_writer.writerow([t, topic+'/fault_bus1', msg.fault_bus1])
    csv_writer.writerow([t, topic+'/fault_bus2', msg.fault_bus2])
    csv_writer.writerow([t, topic+'/fault_connector', msg.fault_connector])
    
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
    
    # Initialize node
    rospy.init_node('bag_to_csv')
    
    # Load relative time param
    use_relative_time = rospy.get_param('~relative_time', True)
   
    # Load bag file
    if not rospy.has_param('~file'):
        rospy.logerr('Bag filename not set!')
        sys.exit()        
    bag_filename = str(rospy.get_param('~file'))
    rospy.loginfo('loading bag file ' + bag_filename)
    bag = rosbag.Bag(bag_filename)
    
    # Load desired topics
    csv_topics = rospy.get_param('~topics')
    bag_types = bag.get_type_and_topic_info(csv_topics)[1].values()
    bag_topics = bag.get_type_and_topic_info(csv_topics)[1].keys()
    start_time = bag.get_start_time()
    
    if not use_relative_time:
        start_time = 0
    
    for i in xrange(0, len(csv_topics)):
        for j in xrange(0, len(bag_topics)):
            if csv_topics[i] == bag_topics[j]:
                csv_types.append(bag_types[j].msg_type)
    
    for i in xrange(0, len(csv_topics)):
        rospy.loginfo('name: ' + csv_topics[i] + ' type: ' + csv_types[i])
    
    
#     for i in xrange(0, len(bag_types)):
#         rospy.loginfo('type: ' + str(bag_types[i].msg_type))
#         rospy.loginfo('name: ' + str(bag_topics[i]))
        
#     rospy.loginfo('Published topics:')
#     for i in xrange(0,len(published_topics)):
#         rospy.loginfo('name: ' + published_topics[i][0] + ' type: ' + published_topics[i][1])        
#     
#     rospy.loginfo('CSV topics:')
#     for i in xrange(0, len(csv_topics)):
#         rospy.loginfo('  ' + csv_topics[i])
    
    csvfile = open(rospkg.RosPack().get_path('dbw_csv_tools') + '/can_csv_data.csv', 'w')
    csv_writer = csv.writer(csvfile, delimiter=',')
    rospy.loginfo('start time: ' + str(start_time))
    
    for topic, msg, t in bag.read_messages(csv_topics, start_time=rospy.Time(bag.get_start_time()), end_time=rospy.Time(bag.get_start_time())+rospy.Duration(10.0)):
        type_idx = get_csv_type(topic)
        if type_idx >= 0:
            # Found desired topic, determine type and write it to CSV
            if csv_types[type_idx] == TWIST_TYPE:
                write_twist_msg(t.to_sec()-start_time, topic, msg, csv_writer)
            elif csv_types[type_idx] == TWISTSTAMPED_TYPE:
                write_twist_msg(t.to_sec()-start_time, topic, msg.twist, csv_writer)
            elif csv_types[type_idx] == FIX_TYPE:
                write_fix_msg(t.to_sec()-start_time, topic, msg, csv_writer)
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
    
    csvfile.close()
    
if __name__ == '__main__':
    try:
        bag_to_csv()
    except rospy.ROSInterruptException:
        pass
