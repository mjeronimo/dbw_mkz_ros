#! /usr/bin/env python
import rospy
import rosbag
import sys

def bag_to_csv():
    global csv_writer
   
    rospy.init_node('bag_to_csv')
   
    if not rospy.has_param('~file'):
        rospy.logerr('Bag filename not set!')
        sys.exit()        
     
    bag_filename = str(rospy.get_param('~file'))
    rospy.loginfo('loading bag file ' + bag_filename)
    
#     csvfile = open(rospkg.RosPack().get_path('dbw_csv_tools') + '/can_csv_data.csv', 'w')
#     csv_writer = csv.writer(csvfile, delimiter=',')
    
if __name__ == '__main__':
    try:
        bag_to_csv()
    except rospy.ROSInterruptException:
        pass
