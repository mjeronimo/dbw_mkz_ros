#! /usr/bin/env python
import rospy
from dbw_mkz_msgs.msg import BrakeCmd
from std_msgs.msg import Float64

brake_val = 0.15

def recv_brake_val(data):
    global brake_val
    brake_val = data.data

def brake_pub():
    global brake_val
    
    pub = rospy.Publisher('brake_cmd', BrakeCmd, queue_size=1)
    rospy.Subscriber('brake_val', Float64, recv_brake_val)
    rospy.init_node('brake_pub');
    
    rate = rospy.Rate(50)
    while not rospy.is_shutdown():
        brake_cmd = BrakeCmd()
        if brake_val >= 0.2:
            brake_cmd.boo_cmd = True
        else:
            brake_cmd.boo_cmd = False
        brake_cmd.enable = True
        brake_cmd.pedal_cmd = brake_val
        brake_cmd.pedal_cmd_type = 1
        pub.publish(brake_cmd)                    
        rate.sleep()

if __name__ == '__main__':
    try:
        brake_pub()
    except rospy.ROSInterruptException:
        pass
