#! /usr/bin/env python
import rospy
import sys
import csv
from std_msgs.msg import Float64, UInt16
from dbw_mkz_msgs.msg import BrakeReport
from math import fabs

requested_tq = 0
actl_brake_pos = 0

def recv_brake_tq(data):
    global requested_tq
    requested_tq = data.data

def recv_actl_brake_pos(data):
    actl_brake_pos = data.pedal_output

def brake_sweep():
    # Initialization
    pub = rospy.Publisher('brake_val', Float64, queue_size=1)
    rospy.Subscriber('/ABS_BrkBst_Data_HS1/BrkTot_Tq_RqDrv', UInt16, recv_brake_tq)
    rospy.Subscriber('brake_report', BrakeReport, recv_actl_brake_pos)
    rospy.init_node('brake_sweep')
    
    # Parameters
    start_val = rospy.get_param('~start_val')
    end_val = rospy.get_param('~end_val')
    sweep_res = rospy.get_param('~sweep_res')
    rate = rospy.Rate(1.0 / rospy.get_param('~settle_time'))
    
    # Open CSV file
    csvfile = open('/home/dataspeed/Desktop/brake_sweep_data.csv', 'w')
    csv_writer = csv.writer(csvfile, delimiter=',')
    
    # Perform sweep
    for i in xrange(0, int((end_val - start_val) / sweep_res + 1)):
        # Handle premature shutdowns
        if rospy.is_shutdown():
            pub.publish(0.15)
            csvfile.close()
            sys.exit()
        
        # Publish brake command
        brake_val = start_val + i * sweep_res
        pub.publish(brake_val)
        
        # Wait for requested torque signal to settle
        rate.sleep()
        
        # Make sure pedal command is responding
        pedal_diff = brake_val - actl_brake_pos
        if (fabs(pedal_diff) > 0.02):
            rospy.logwarn('Large disparity between pedal request and actual... not saving data point')
            continue
        
        # Write data to file
        rospy.loginfo('Data point: ' + str(actl_brake_pos) + ', ' + str(requested_tq))
        csv_writer.writerow([str(actl_brake_pos), str(requested_tq)])
        
    csvfile.close()
            
if __name__ == '__main__':
    try:
        brake_sweep()
    except rospy.ROSInterruptException:
        pass